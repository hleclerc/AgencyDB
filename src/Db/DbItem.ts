import BinaryWriter from "../System/BinaryWriter";
import BinaryReader from "../System/BinaryReader";
import VectorClock  from "../System/VectorClock";
import ListUtil     from "../System/ListUtil";
import MapUtil      from "../System/MapUtil";
import PatchId      from "../System/PatchId";
import ItemId       from "../System/ItemId";
import UsrId        from "../System/UsrId";
import DevId        from "../System/DevId";
import PatchManager from "../Core/PatchManager";
import { VarAnc }   from "../Core/Variable";
import OtMerger     from "../Core/OtMerger";
import methods      from "../Core/methods";
import Db           from "../Db";

//
// export
// class Merge {
//     patch_manager: PatchManager;
//     inode        : Inode;
// }

/**  */
export
class Patch {
    patch_id  : PatchId;      /** 0 means that it is a merging patch. in this case, we use inode to retrieve it */
    causal_num: number;       /** number that enable causality ordering */
    date      : number;       /** date of this change (using the clock from the corresponding device) */
    usr_id    : UsrId;        /** who has made the changes */
    data      : Uint8Array;   /** content (operations) */
    merge     = false;        /** Merge; */

    sup( that: Patch, cur_dev: DevId ): boolean {
        if ( this.causal_num != that.causal_num )
            return this.causal_num > that.causal_num;
        if ( this.date != that.date )
            return this.date > that.date;
        return this.patch_id.dev.to_glo( cur_dev ).val > that.patch_id.dev.to_glo( cur_dev ).val;
    }
}

/** Store known and sent patches for other db instances */
class ExtState {
    known = new VectorClock;
    sent  = new VectorClock;
}

/**
 * 
 */
export default
class DbItem extends PatchManager {
    constructor( db: Db, variable: VarAnc, id: ItemId ) {
        super();

        this.variable = variable;
        this.id       = id;
        this.db       = db;
    }

    /** called by OtWrappers to register a modification */
    new_modification( mod: Uint8Array ) {
        if ( mod.byteLength ) {
            if ( this.modifications.size == 0 )
                this.db.add_modified_items( this );
            this.modifications.write_some( mod );
        }
    }

    /** return a patch list */
    get_changes( dev_id: DevId ): Uint8Array {
        this.freeze_last_mod();

        // find the patches to be sent, register them in dst_ext_state.sent 
        let dst_ext_state = MapUtil.get_or_set( this.ext_states, dev_id.val, () => new ExtState ), to_be_sent = new Array<number>();
        this.vector_clock.map.forEach( ( num, dev ) => {
            let knum = dst_ext_state.sent.val( dev );
            for( let snum = knum + 1; snum <= num; ++snum )
                to_be_sent.push( this.find_patch_index( dev, snum ) );
        } );
        // this.vector_clock.merges.forEach( ( inode : Inode ) => {
        //     if ( ! dst_ext_state.sent.contains_merge( inode ) )
        //         to_be_sent.push( this.find_merge_index( inode ) );
        // } );
        if ( to_be_sent.length == 0 )
            return null;
            

        // header
        let bw = new BinaryWriter;
        methods[ "get_patch_type__b" ].call_1( this.variable.rp, bw ); // patch type
        bw.write_obj( dst_ext_state.sent );                            // on which state the patches are based

        // the patches
        // let merger = Method.methods["new_ot_merger"].call_1( this.variable ) as OtMerger;
        for( let ind_patch of to_be_sent ) {
            let patch = this.patches[ ind_patch ];

            bw.write_obj       ( patch.patch_id   ); // id of the current patch
            bw.write_PT        ( patch.causal_num );
            bw.write_Date      ( patch.date       );    // creation date
            bw.write_obj       ( patch.usr_id     );    // user who has created this patch
            bw.write_Uint8Array( patch.data       );
        }

        // we assume that the result will actually be sent (we do not assume that it will be received yet)
        dst_ext_state.sent = this.vector_clock.copy();

        return bw.to_Uint8Array();
    }

    /** add new patches from the external world */
    app_changes( br: BinaryReader, src_dev: DevId, src_usr: UsrId, dst_devs = new Array<DevId>() ) {
        if ( ! br.size )
            return;
        // if ( this.variable.is_symbolic() ) {
        //     let wc      = new WaitingChange;
        //     wc.bp       = br.slice( br.size );
        //     wc.prereq   = br.read_VectorClock( cur_dev );
        //     wc.src_dev  = src_dev;
        //     wc.dst_devs = dst_devs;
        //     wc.cur_dev  = cur_dev;
        //     wc.cur_usr  = cur_usr;
        //     wc.when_unk = when_unk;
        //     this.waiting_changes.push( wc );
        //     return;
        // }

        // we work on patches... we need known states
        this.freeze_last_mod();

        //
        if ( ! methods[ "cmp_patch_type__b" ].call_1( this.variable.rp, br ) )
            throw new Error( "TODO: != patch_type" );

        // read id of patches which serve as base for the new ones
        let assumed_to_be_known = VectorClock.read_from( br, src_dev, this.db.dev_id );

        // there are patch or merges assumed_to_be_known but not present ??
        assumed_to_be_known.map.forEach( ( num: number, dev: string ) => {
            if ( this.vector_clock.val( dev ) < num )
                throw new Error( 'TODO assumed_to_be_known that actually are not known: store the data and send a request to get the corresponding stuff' );
            }
        );
        // assumed_to_be_known.merges.forEach( ( item_id: ItemId ) => {
        //     if ( ! this.vector_clock.contains_merge( item_id ) )
        //         throw new Error( 'TODO assumed_to_be_known that actually are not known: store the data and send a request to get the corresponding stuff' );
        // } );

        // unk patches or merges (i.e. stuff not known by the sender)
        let unk_patches = new Array<Patch>();
        let unk_merges = new Array<Patch>();
        let index_first_insertion = 1 << 30;
        this.vector_clock.map.forEach( ( num: number, dev: string ) => {
            for( let i = assumed_to_be_known.val( dev ) + 1; i <= num; ++i ) {
                let index = this.find_patch_index( dev, i );
                if ( index_first_insertion > index )
                    index_first_insertion = index;
                unk_patches.push( this.patches[ index ] );
            }
        } );
        // this.vector_clock.merges.forEach( ( item_id: ItemId ) => {
        //     if ( ! assumed_to_be_known.contains_merge( item_id ) ) {
        //         let index = this.find_merge_index( item_id );
        //         //console.log( index, ""+inode, " ass:"+assumed_to_be_known );
        //         if ( index_first_insertion > index )
        //             index_first_insertion = index;
        //         unk_merges.push( this.patches[ index ] );
        //     }
        // } );

        // read the patches, store a copy of the references in new_patches
        let merger = methods[ "new_ot_merger__b" ].call_1( this.variable.rp, this ) as OtMerger;
        let new_patches = new Array<Patch>();
        let new_merges = new Array<Patch>();
        while( br.size ) {
            // prev && position of the patch
            // let index_ins_patch = 0;
            // switch ( br.read_PT() ) {
            //     case 0: break; // no prev
            //     case 1: index_ins_patch = this.find_patch_index_PatchId( bp.read_PatchId( cur_dev ) ) + 1; break; // prev is a Patch (not Merge)
            //     case 2: index_ins_patch = this.find_merge_index( bp.read_Inode( cur_dev ) ) + 1; break; // prev is a merge
            // }
            // patch header
            let n = new Patch;
            n.patch_id   = PatchId.read_from( br, src_dev, this.db.dev_id );
            n.causal_num = br.read_PT();
            n.date       = br.read_Date();
            n.usr_id     = UsrId.read_from( br, src_dev, src_usr, this.db.dev_id, this.db.usr_id );

            if ( n.patch_id.num ) {
                // update ext_states (what is known && can be assumed to be sent to the other machines)
                MapUtil.get_or_set( this.ext_states, src_dev.val, () => new ExtState ).sent.update_patch( n.patch_id );
                for( let dst_dev of dst_devs ) {
                    if ( dst_dev.val != this.db.dev_id.val )
                        MapUtil.get_or_set( this.ext_states, dst_dev.val, () => new ExtState ).sent.update_patch( n.patch_id );
                }

                // we actually already have this patch ?
                if ( this.vector_clock.contains_patch( n.patch_id ) ) {
                    ListUtil.remove( unk_patches, this.patches[ this.find_patch_index_PatchId( n.patch_id ) ] ); // put the patch in std_patches
                    br.skip_Uint8Array(); // no need to read the patch data
                    continue;
                }

                // else, read the content
                let orig = br.read_Uint8Array();
                let data = new BinaryWriter;
                if ( ! merger.glo_to_loc( data, orig, src_dev, src_usr, this.db.dev_id, this.db.usr_id ) ) {
                    console.error( "invalid patch. TODO: send a message for that." );
                    break;
                }
                n.data = data.to_Uint8Array();

                // register the patch
                this.vector_clock.update_patch( n.patch_id );
                new_patches.push( n );
            } else {
                throw new Error( "TODO: merge" );
                // n.merge       = new Merge;
                // n.merge.inode = bp.read_Inode( cur_dev );

                // let mtv = bp.read_PT();
                // if ( ! PatchTypes.readers[ mtv ] )
                //     throw new Error( "TODO: make a reader for patch_type " + PatchTypes.list[ mtv ] );
                // n.merge.patch_manager = PatchManager.read_from( bp, mtv, src_dev, cur_dev, cur_usr );

                // // we actually already have this merge ?
                // if ( this.vector_clock.contains_merge( n.merge.inode ) ) {
                //     ListUtil.remove( unk_merges, this.patches[ this.find_merge_index( n.merge.inode ) ] ); // put the patch in std_patches
                //     continue;
                // }

                // // reg the merge
                // this.vector_clock.update_merge( n.merge.inode );
                // new_merges.push( n );
            }

            // find where to actually place to patch
            let index_ins_patch = this.patches.length;
            while ( index_ins_patch && this.patches[ index_ins_patch - 1 ].sup( n, this.db.dev_id ) )
                --index_ins_patch;
            this.patches.splice( index_ins_patch, 0, n );

            // 
            if ( index_first_insertion > index_ins_patch )
                index_first_insertion = index_ins_patch;
        }

        // update patch content
        if ( new_patches.length || new_merges.length ) {
            // std patches at the beginning are not interesting -> we can skip them
            let in_std = ( index : number ) => this.patches[ index ].merge ?
                    new_merges .indexOf( this.patches[ index ] ) < 0 && unk_merges .indexOf( this.patches[ index ] ) < 0 :
                    new_patches.indexOf( this.patches[ index ] ) < 0 && unk_patches.indexOf( this.patches[ index ] ) < 0;
            while( index_first_insertion < this.patches.length && in_std( index_first_insertion ) )
                ++index_first_insertion;

            // undo std and unk patches and merges
            for( let index = this.patches.length - 1; index >= index_first_insertion; --index ) {
                if ( this.patches[ index ].merge ) {
                    if ( new_merges.indexOf( this.patches[ index ] ) < 0 )
                        throw new Error( "TODO: undo merge" );
                } else {
                    if ( new_patches.indexOf( this.patches[ index ] ) < 0 )
                        merger.undo_patch( this.patches[ index ].data, this.patches[ index ].usr_id );
                }
            }

            // merge the data
            let nb_new = new_patches.length + new_merges.length;
            let nb_unk = unk_patches.length + unk_patches.length;
            let nb_std = this.patches.length - index_first_insertion - nb_unk - nb_new;
            for( let index = index_first_insertion; index < this.patches.length; ++index ) {
                // console.log( `Receving: ${ index }`, new_patches.length + new_merges.length );
                // if ( ! f->prev )
                //     merger->set_creator( f->usr_id );

                let f = this.patches[ index ];
                if ( f.merge ) {
                    throw new Error( "TODO: merge" );
                    // if ( new_merges.indexOf( f ) >= 0 ) {
                    //     merger.new_merge( f.merge.patch_manager, f.usr_id, nb_unk, nb_std );
                    //     // console.log( "new merge", cur_dev.val, 'from', src_dev.val, "-> " + this.variable );
                    //     --nb_new;
                    // } else if ( unk_merges.indexOf( f ) >= 0 ) {
                    //     merger.unk_merge( f.merge.patch_manager, f.usr_id, nb_new, nb_std );
                    //     // console.log( "unk", cur_dev.val, 'from', src_dev.val, f.patch_id.toString(), f.data, "" + this.variable );
                    //     --nb_unk;
                    // } else {
                    //     merger.std_merge( f.merge.patch_manager, f.usr_id, nb_unk, nb_new );
                    //     // console.log( "std", cur_dev.val, 'from', src_dev.val, f.patch_id.toString(), f.data, "" + this.variable );
                    //     --nb_std;
                    // }
                } else {
                    let pd = new BinaryWriter;
                    if ( new_patches.indexOf( f ) >= 0 ) {
                        merger.new_patch( pd, f.data, f.usr_id, nb_unk, nb_std );
                        // console.log( "new", cur_dev.val, 'from', src_dev.val, f.patch_id.toString(), f.data, "" + this.variable, " usr:" + f.usr_id );
                        --nb_new;
                    } else if ( unk_patches.indexOf( f ) >= 0 ) {
                        merger.unk_patch( pd, f.data, f.usr_id, nb_new, nb_std );
                        // console.log( "unk", cur_dev.val, 'from', src_dev.val, f.patch_id.toString(), f.data, "" + this.variable );
                        --nb_unk;
                    } else {
                        merger.std_patch( pd, f.data, f.usr_id, nb_unk, nb_new );
                        // console.log( "std", cur_dev.val, 'from', src_dev.val, f.patch_id.toString(), f.data, "" + this.variable );
                        --nb_std;
                    }

                    // stores the updated patch data
                    f.data = pd.to_Uint8Array();
                }
            }

            merger.finalize();
        }

        // //
        // if ( unk_patches.length && when_unk ) {
        //     when_unk( this.get_changes( src_dev, cur_dev, cur_usr ) );
        // }
    }

    freeze_last_mod() {
        if ( this.modifications.size ) {
            let p        = new Patch;
            p.causal_num = this.patches.length ? this.patches[ this.patches.length - 1 ].causal_num + 1 : 0;
            p.data       = this.modifications.to_Uint8Array();
            p.patch_id   = new PatchId( new DevId( DevId.null_DevId_str ), this.vector_clock.self_inc( DevId.null_DevId_str ) );
            p.date       = Date.now();
            p.usr_id     = new UsrId;

            this.patches.push( p );
            this.modifications.clear();
        }
        
    }

    find_patch_index( dev: string, num: number ): number {
        for( let n = this.patches.length; n--; )
            if ( this.patches[ n ].patch_id && this.patches[ n ].patch_id.num == num && this.patches[ n ].patch_id.dev.val == dev )
                return n;
        return -2;
    }

    find_patch_index_PatchId( pid: PatchId ): number {
        return this.find_patch_index( pid.dev.val, pid.num );
    }

    modifications = new BinaryWriter;
    vector_clock  = new VectorClock;            /** coordinates of known patches */
    ext_states    = new Map<String,ExtState>(); /** What is known/sent for each external device */
    patches       = new Array<Patch>();
    variable      : VarAnc;
    id            : ItemId;
    db            : Db;
}
 
