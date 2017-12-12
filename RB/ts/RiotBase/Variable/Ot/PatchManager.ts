import BinaryWriter from "../../System/BinaryWriter";
import BinaryReader from "../../System/BinaryReader";
import VectorClock  from "../../System/VectorClock";
import ListUtil     from "../../System/ListUtil";
import MapUtil      from "../../System/MapUtil";
import PatchId      from "../../System/PatchId";
import Inode        from "../../System/Inode";
import DevId        from "../../System/DevId";
import UsrId        from "../../System/UsrId";
import Map          from "../../System/Map";
import Lazyfier     from "../Symbol/Lazyfier";
import Variable     from "../Core/Variable";
import Method       from "../Core/Method";
import PatchTypes   from "./PatchTypes";

//
export
class Merge {
    patch_manager: PatchManager;
    inode        : Inode;
}

//
export
class Patch {
    patch_id : PatchId;      ///< 0 means that it is a merging patch. in this case, we use inode to retrieve it
    date     : number;       ///< date of this change (absolute local date)
    usr_id   : UsrId;        ///< who has made the changes
    data     : Uint8Array;   ///< content (operations)
    merge    : Merge;
}

//
class ExtState {
    known = new VectorClock;
    sent  = new VectorClock;
}

//
class WaitingChange {
    bp      : BinaryReader;
    prereq  : VectorClock;
    src_dev : DevId;
    dst_devs: DevId[];
    cur_dev : DevId;
    cur_usr : UsrId;
    when_unk: ( Uint8Array ) => void;
    patches : Patch[];
}

//
class WaitingMerge {
    siht  : PatchManager;
    b_id  : Inode;
    as_usr: UsrId;
}

//
export
class PatchManager {
    static Patch = Patch;

    constructor( variable : Variable ) {
        this.variable = variable;
    }

    /** write all the patch data for dst_dev in bw */
    write_to( bw : BinaryWriter, dst_dev? : DevId, cur_dev? : DevId, cur_usr? : UsrId ) {
        this.freeze_last_mod();

        // patch type
        Method.methods["get_patch_type"].call_1( this.variable, bw );

        // second part: the patches
        bw.write_PT( this.patches.length );
        let merger = Method.methods["new_ot_merger"].call_1( this.variable ) as OtMerger;
        for( let ind_patch = 0; ind_patch < this.patches.length; ++ind_patch ) {
            let patch = this.patches[ ind_patch ];

            if ( patch.merge ) {
                bw.write_PT   ( 0 );
                bw.write_Date ( ind_patch ? patch.date - this.patches[ ind_patch - 1 ].date : patch.date );  // (delta) creation date
                bw.write_UsrId     ( patch.usr_id, cur_dev, cur_usr );                                            // user who has created this patch
                bw.write_Inode( patch.merge.inode, cur_dev );
                patch.merge.patch_manager.write_to( bw, dst_dev, cur_dev, cur_usr );
            } else {
                // get data with correct dev_id and usr_id
                let dw = new BinaryWriter;
                merger.loc_to_glo( dw, patch.data, cur_dev, cur_usr );

                // content
                bw.write_PatchId   ( patch.patch_id, cur_dev );                                                   // coordinates of the current patch
                bw.write_Date      ( ind_patch ? patch.date - this.patches[ ind_patch - 1 ].date : patch.date );  // (delta) creation date
                bw.write_UsrId     ( patch.usr_id, cur_dev, cur_usr );                                            // user who has created this patch
                bw.write_Uint8Array( dw.to_Uint8Array() );
            }
        }

        // say that the patch has been sent
        if ( dst_dev ) {
            let dst_ext_state = MapUtil.get_or_set( this.ext_states, dst_dev.val, () => new ExtState );
            dst_ext_state.sent = this.vector_clock.copy();
        }
    }

    /** make a new PatchManager with data coming from write_to */
    static read_from( br : BinaryReader, tv? : number, src_dev? : DevId, cur_dev? : DevId, cur_usr? : UsrId ) : PatchManager {
        if ( ! tv ) tv = br.read_PT();
        let variable = PatchTypes.readers[ tv ]( br, cur_dev, cur_usr );
        let res = new PatchManager( variable );

        let dst_ext_state = src_dev ? MapUtil.get_or_set( res.ext_states, src_dev.val, () => new ExtState ) : null;
        let merger = Method.methods["new_ot_merger"].call_1( res.variable ) as OtMerger;
        for( let num_patch = 0, n = br.read_PT(); num_patch < n; ++num_patch ) {
            // read the patch
            let p      = new Patch;
            p.patch_id = br.read_PatchId( cur_dev );
            p.date     = num_patch ? res.patches[ num_patch - 1 ].date + br.read_Date() : br.read_Date();
            p.usr_id   = br.read_UsrId( cur_dev, cur_usr );

            if ( p.patch_id.num ) {
                let data = new BinaryWriter;
                if ( ! merger.glo_to_loc( data, br.read_Uint8Array(), cur_dev, cur_usr ) ) {
                    console.error( "invalid patch. TODO: send a message for that." );
                    break;
                }
                p.data = data.to_Uint8Array();

                // update the vector_clocks
                res.vector_clock.update_patch( p.patch_id );
                if ( dst_ext_state ) dst_ext_state.sent.update_patch( p.patch_id );

                // exec
                merger.new_patch( new BinaryWriter, p.data, p.usr_id, 0, 0 );
            } else {
                p.merge       = new Merge;
                p.merge.inode = br.read_Inode( cur_dev );

                let mtv = br.read_PT();
                if ( ! PatchTypes.readers[ mtv ] )
                    throw new Error( "TODO: make a reader for patch_type " + PatchTypes.list[ mtv ] );
                p.merge.patch_manager = PatchManager.read_from( br, mtv, src_dev, cur_dev, cur_usr );

                // update the vector_clocks
                res.vector_clock.update_merge( p.merge.inode );
                if ( dst_ext_state ) dst_ext_state.sent.update_merge( p.merge.inode );

                // exec
                merger.new_merge( p.merge.patch_manager, p.usr_id, 0, 0 );
            }

            res.patches.push( p );
        }

        return res;
    }

    /** message to be sent to `dst_dev` for update of its data */
    get_changes( dst_dev : DevId, cur_dev? : DevId, cur_usr? : UsrId ) : Uint8Array {
        this.freeze_last_mod();

        // ext_state for dst_dev
        let dst_ext_state = MapUtil.get_or_set( this.ext_states, dst_dev.val, () => new ExtState );

        // list of indices of patches to be sent
        let to_be_sent = [];
        this.vector_clock.map.forEach( ( num : number, dev : string ) => {
            let knum = dst_ext_state.sent.val( dev );
            for( let snum = knum + 1; snum <= num; snum += 1 )
                to_be_sent.push( this.find_patch_index( dev, snum ) );
        } );
        this.vector_clock.merges.forEach( ( inode : Inode ) => {
            if ( ! dst_ext_state.sent.contains_merge( inode ) )
                to_be_sent.push( this.find_merge_index( inode ) );
        } );

        // console.log( cur_dev + " -> " + dst_dev + " : " + dst_ext_state.sent + " vc:" + this.vector_clock, this.patches, to_be_sent );

        if ( ! to_be_sent.length )
            return new Uint8Array( [] );
        to_be_sent.sort(); // ok because we work on indices

        // let's make the message
        let bw = new BinaryWriter;

        //
        Method.methods["get_patch_type"].call_1( this.variable, bw );

        // on which patches the sent ones are based
        bw.write_VectorClock( dst_ext_state.sent, cur_dev );

        // second part: the patches
        let merger = Method.methods["new_ot_merger"].call_1( this.variable ) as OtMerger;
        for( let ind_patch of to_be_sent ) {
            let patch = this.patches[ ind_patch ];

            // coordinates of the previous patch ( -> type of the previous patch + coordinates )
            if ( ind_patch ) {
                let prev = this.patches[ ind_patch - 1 ];
                if ( prev.merge ) {
                    bw.write_PT( 2 );
                    bw.write_Inode( prev.merge.inode, cur_dev ); // coordinates of the previous patch
                } else {
                    bw.write_PT( 1 );
                    bw.write_PatchId( prev.patch_id, cur_dev ); // coordinates of the previous patch
                }
            } else {
                bw.write_PT( 0 );
            }

            if ( patch.merge ) {
                bw.write_PT   ( 0 );
                bw.write_Date ( ind_patch ? patch.date - this.patches[ ind_patch - 1 ].date : patch.date );  // (delta) creation date
                bw.write_UsrId( patch.usr_id, cur_dev, cur_usr );                                            // user who has created this merge
                bw.write_Inode( patch.merge.inode, cur_dev );
                patch.merge.patch_manager.write_to( bw, dst_dev, cur_dev, cur_usr );

                // say that the patch has been sent
                dst_ext_state.sent.update_merge( patch.merge.inode );
            } else {
                // get data with correct dev_id and usr_id
                let dw = new BinaryWriter;
                merger.loc_to_glo( dw, patch.data, cur_dev, cur_usr );

                // content
                bw.write_PatchId   ( patch.patch_id, cur_dev );                                                   // coordinates of the current patch
                bw.write_Date      ( ind_patch ? patch.date - this.patches[ ind_patch - 1 ].date : patch.date );  // (delta) creation date
                bw.write_UsrId     ( patch.usr_id, cur_dev, cur_usr );                                            // user who has created this patch
                bw.write_Uint8Array( dw.to_Uint8Array() );

                // say that the patch has been sent
                dst_ext_state.sent.update_patch( patch.patch_id );
            }
        }

        return bw.to_Uint8Array();
    }

    /** say that `known` is known by dev `src_dev` */
    update_known_patches_for( src_dev : DevId, known : VectorClock ) {
        // console.error( `update ${ Date.now() - 1458834611723 }` );
        let dst_ext_state = MapUtil.get_or_set( this.ext_states, src_dev.val, () => new ExtState );
        dst_ext_state.sent.update_VectorClock( known );
    }

    /** when_unk is called when some patches are not known by src_dev (with a message containing the patches) */
    app_changes( bp : BinaryReader, src_dev : DevId, dst_devs : DevId[], cur_dev : DevId, cur_usr : UsrId, when_unk : ( Uint8Array ) => void ) : void { // cur_dev should be included in dst_devs
        if ( this.variable.is_symbolic() ) {
            let wc      = new WaitingChange;
            wc.bp       = bp.slice( bp.size );
            wc.prereq   = bp.read_VectorClock( cur_dev );
            wc.src_dev  = src_dev;
            wc.dst_devs = dst_devs;
            wc.cur_dev  = cur_dev;
            wc.cur_usr  = cur_usr;
            wc.when_unk = when_unk;
            this.waiting_changes.push( wc );
            return;
        }

        // console.error( `app ${ Date.now() - 1458834611723 }` );
        if ( ! bp.size )
            return;

        // we work on patches...
        this.freeze_last_mod();

        //
        if ( ! Method.methods["cmp_patch_type"].call_1( this.variable, bp ) )
            throw new Error( "TODO: != patch_type" );

        // read id of patches which serve as base for the new ones
        let assumed_to_be_known = bp.read_VectorClock( cur_dev );

        // there are patch or merges assumed_to_be_known but not present ??
        assumed_to_be_known.map.forEach( ( num : number, dev : string ) => {
            if ( this.vector_clock.val( dev ) < num )
                throw new Error( 'TODO assumed_to_be_known that actually are not known: store the data and send a request to get the corresponding stuff' );
            }
        );
        assumed_to_be_known.merges.forEach( ( inode : Inode ) => {
            if ( ! this.vector_clock.contains_merge( inode ) )
                throw new Error( 'TODO assumed_to_be_known that actually are not known: store the data and send a request to get the corresponding stuff' );
        } );

        // unk patches or merges (i.e. stuff not known by the sender)
        let unk_patches = new Array<Patch>();
        let unk_merges = new Array<Patch>();
        let index_first_insertion = 1 << 30;
        this.vector_clock.map.forEach( ( num : number, dev : string ) => {
            for( let i = assumed_to_be_known.val( dev ) + 1; i <= num; ++i ) {
                let index = this.find_patch_index( dev, i );
                if ( index_first_insertion > index )
                    index_first_insertion = index;
                unk_patches.push( this.patches[ index ] );
            }
        } );
        this.vector_clock.merges.forEach( ( inode : Inode ) => {
            if ( ! assumed_to_be_known.contains_merge( inode ) ) {
                let index = this.find_merge_index( inode );
                //console.log( index, ""+inode, " ass:"+assumed_to_be_known );
                if ( index_first_insertion > index )
                    index_first_insertion = index;
                unk_merges.push( this.patches[ index ] );
            }
        } );

        // read the patches, store a copy of the references in new_patches
        let merger = Method.methods["new_ot_merger"].call_1( this.variable ) as OtMerger;
        let new_patches = new Array<Patch>();
        let new_merges = new Array<Patch>();
        while( bp.size ) {
            // prev && position of the patch
            let index_ins_patch = 0;
            switch ( bp.read_PT() ) {
                case 0: break; // no prev
                case 1: index_ins_patch = this.find_patch_index_PatchId( bp.read_PatchId( cur_dev ) ) + 1; break; // prev is a Patch (not Merge)
                case 2: index_ins_patch = this.find_merge_index( bp.read_Inode( cur_dev ) ) + 1; break; // prev is a merge
            }
            if ( index_ins_patch < 0 )
                throw new Error( "TODO" );

            // patch header
            let n = new Patch;
            n.patch_id = bp.read_PatchId( cur_dev );
            n.date     = index_ins_patch ? this.patches[ index_ins_patch - 1 ].date + bp.read_Date() : bp.read_Date();
            n.usr_id   = bp.read_UsrId( cur_dev, cur_usr );

            // console.log( `Receving: ${ n.patch_id } in ${ cur_dev }` );

            if ( n.patch_id.num ) {
                // update ext_states (what is known && can be assumed to be sent to the other machines)
                MapUtil.get_or_set( this.ext_states, src_dev.val, () => new ExtState ).sent.update_patch( n.patch_id );
                for( let dst_dev of dst_devs ) {
                    if ( dst_dev.val != cur_dev.val )
                        MapUtil.get_or_set( this.ext_states, dst_dev.val, () => new ExtState ).sent.update_patch( n.patch_id );
                }

                // we actually already have this patch ?
                if ( this.vector_clock.contains_patch( n.patch_id ) ) {
                    ListUtil.remove( unk_patches, this.patches[ this.find_patch_index_PatchId( n.patch_id ) ] ); // put the patch in std_patches
                    bp.skip_Uint8Array(); // no need to read the patch data
                    continue;
                }

                // else, read the content
                let orig = bp.read_Uint8Array();
                let data = new BinaryWriter;
                if ( ! merger.glo_to_loc( data, orig, cur_dev, cur_usr ) ) {
                    console.error( "invalid patch. TODO: send a message for that." );
                    break;
                }
                n.data = data.to_Uint8Array();

                // register the patch
                this.vector_clock.update_patch( n.patch_id );
                new_patches.push( n );
            } else {
                n.merge       = new Merge;
                n.merge.inode = bp.read_Inode( cur_dev );

                let mtv = bp.read_PT();
                if ( ! PatchTypes.readers[ mtv ] )
                    throw new Error( "TODO: make a reader for patch_type " + PatchTypes.list[ mtv ] );
                n.merge.patch_manager = PatchManager.read_from( bp, mtv, src_dev, cur_dev, cur_usr );

                // we actually already have this merge ?
                if ( this.vector_clock.contains_merge( n.merge.inode ) ) {
                    ListUtil.remove( unk_merges, this.patches[ this.find_merge_index( n.merge.inode ) ] ); // put the patch in std_patches
                    continue;
                }

                // reg the merge
                this.vector_clock.update_merge( n.merge.inode );
                new_merges.push( n );
            }

            // find where to actually place to patch
            let glo_dev = function( p : Patch ) : string { return ( p.merge ? p.merge.inode.dev : p.patch_id.dev ).to_glo( cur_dev ).val; }

            if ( index_ins_patch ) { // IMPORTANT_TODO: check that code
                while ( index_ins_patch < this.patches.length && (
                     this.patches[ index_ins_patch ].date < n.date ||
                     ( this.patches[ index_ins_patch ].date == n.date && glo_dev( this.patches[ index_ins_patch ] ) < glo_dev( n ) )
                ) )
                    index_ins_patch += 1;
            } else if ( this.patches.length ){
                n.date = this.patches[ index_ins_patch ].date;
                while ( index_ins_patch < this.patches.length && this.patches[ index_ins_patch ].date == n.date && glo_dev( this.patches[ index_ins_patch ] ) < glo_dev( n ) )
                    index_ins_patch += 1;
            }

            // insertion of the patch
            if ( index_first_insertion > index_ins_patch )
                index_first_insertion = index_ins_patch;
            this.patches.splice( index_ins_patch, 0, n );
        }

        // update patch content
        if ( new_patches.length + new_merges.length ) {
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
                    if ( new_merges.indexOf( f ) >= 0 ) {
                        merger.new_merge( f.merge.patch_manager, f.usr_id, nb_unk, nb_std );
                        // console.log( "new merge", cur_dev.val, 'from', src_dev.val, "-> " + this.variable );
                        --nb_new;
                    } else if ( unk_merges.indexOf( f ) >= 0 ) {
                        merger.unk_merge( f.merge.patch_manager, f.usr_id, nb_new, nb_std );
                        // console.log( "unk", cur_dev.val, 'from', src_dev.val, f.patch_id.toString(), f.data, "" + this.variable );
                        --nb_unk;
                    } else {
                        merger.std_merge( f.merge.patch_manager, f.usr_id, nb_unk, nb_new );
                        // console.log( "std", cur_dev.val, 'from', src_dev.val, f.patch_id.toString(), f.data, "" + this.variable );
                        --nb_std;
                    }
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

        //
        if ( unk_patches.length && when_unk ) {
            when_unk( this.get_changes( src_dev, cur_dev, cur_usr ) );
        }
    }

    get_added_or_changed_patches( known : VectorClock, f : ( Patch ) => void ) {
        this.freeze_last_mod();

        // list of indices of patches to be sent
        let first_ind = this.patches.length;
        this.vector_clock.map.forEach( ( num : number, dev : string ) => {
            let knum = known.val( dev );
            if ( knum < num )
                first_ind = Math.min( first_ind, this.find_patch_index( dev, knum + 1 ) );
        } );
        this.vector_clock.merges.forEach( ( inode : Inode ) => {
            if ( ! known.contains_merge( inode ) )
                first_ind = Math.min( first_ind, this.find_merge_index( inode ) );
        } );

        //
        for( let ind_patch = first_ind; ind_patch < this.patches.length; ++ind_patch ) {
            let patch = this.patches[ ind_patch ];

            f( patch );

            if ( patch.merge )
                known.update_merge( patch.merge.inode );
            else
                known.update_patch( patch.patch_id );
        }
    }

    freeze_last_mod() {
        let operations = new BinaryWriter;
        this.variable.pull_ot_changes( operations );
        if ( operations.size ) {
            let p      = new Patch;
            p.data     = operations.to_Uint8Array();
            p.patch_id = new PatchId( new DevId( DevId.null_DevId_str ), this.vector_clock.inc( DevId.null_DevId_str ) );
            p.date     = this.patches.length ? Math.max( Date.now(), this.patches[ this.patches.length - 1 ].date + 1 ) : Date.now();
            p.usr_id   = new UsrId;

            this.patches.push( p );
            this.vector_clock.update_patch( p.patch_id );
        }
    }

    find_patch_index( dev : string, num : number ) : number {
        for( let n = this.patches.length - 1; n >= 0; n -= 1 )
            if ( this.patches[ n ].patch_id && this.patches[ n ].patch_id.num == num && this.patches[ n ].patch_id.dev.val == dev )
                return n;
        return -2;
    }

    find_patch_index_PatchId( pid : PatchId ) : number {
        return this.find_patch_index( pid.dev.val, pid.num );
    }

    find_merge_index( inode : Inode ) {
        for( let n = this.patches.length - 1; n >= 0; --n )
            if ( this.patches[ n ].merge && this.patches[ n ].merge.inode.equ( inode ) )
                return n;
        return -2;
    }

    connected_devices() : Array<DevId> {
        let res = [];
        this.ext_states.forEach( ( s : ExtState, dev : string ) => {
            res.push( DevId.fromString( dev ) );
        } );
        return res;
    }

    summary( cur_dev = new DevId, sp = "" ) : string {
        let res = "";
        for( let p of this.patches ) {
            if ( p.merge ) {
                res += sp + "m " + p.merge.inode.toString( cur_dev ) + "\n";
                res += p.merge.patch_manager.summary( cur_dev, sp + "  " );
            } else {
                let ts = ( x ) => {
                    if ( x instanceof DevId || x instanceof Inode )
                        return x.toString( cur_dev );
                    if ( x instanceof Array )
                        return "[" + x.toString( cur_dev ) + "]";
                    return x.toString();
                };
                res += sp + "p " + p.patch_id.toString( cur_dev ) + ":";
                (<OtWrapper>this.variable.rp).patch_data_visitor( p.data, ( op, ...args ) => {
                    res += " " + op.name + '(' + args.map( ts ).join( ',' ) + ')';
                } );
            }
            res += "\n";
        }
        return res;
    }

    merge( siht : PatchManager, b_id : Inode, as_usr : UsrId ) {
        if ( this.variable.is_symbolic() ) {
            let wm    = new WaitingMerge;
            wm.siht   = siht;
            wm.b_id   = b_id;
            wm.as_usr = as_usr;
            this.waiting_merges.push( wm );
            return true;
        }

        this.freeze_last_mod();
        siht.freeze_last_mod();
        ( this.variable.rp as OtWrapper ).get_ext_patch_merge_effect( siht.variable.rp as OtWrapper );

        //
        let bw = new BinaryWriter;
        Method.methods["get_patch_type"].call_1( siht.variable, bw );
        if ( ! Method.methods["cmp_patch_type"].call_1( this.variable, new BinaryReader( bw.to_Uint8Array() ) ) )
            throw new Error( "TODO: merge with != patch_type" );

        // look if data is already merged
        for( let i = this.patches.length; i--; ) {
            if ( this.patches[ i ].merge && this.patches[ i ].merge.inode.equ( b_id ) ) {
                // console.error( "I had it already", this.patches, b_id, ""+this.variable );
                if ( this.patches[ i ].merge.patch_manager.vector_clock.equ( siht.vector_clock ) ) {
                    if ( ! this.variable.equ_immediate( siht.variable ) )
                        this.variable.sig_change();
                    return true;
                }
                throw new Error( "TODO: change content of merging patches" );
            }
        }

        // register the data
        let p                 = new Patch;
        p.date                = this.patches.length ? Math.max( Date.now(), this.patches[ this.patches.length - 1 ].date + 1 ) : Date.now();
        p.usr_id              = as_usr;
        p.merge               = new Merge;
        p.merge.patch_manager = siht;
        p.merge.inode         = b_id;

        this.patches.push( p );
        this.vector_clock.update_merge( p.merge.inode );

        // execute
        let merger = Method.methods["new_ot_merger"].call_1( this.variable ) as OtMerger;
        merger.new_merge( p.merge.patch_manager, as_usr, 0, 0 );
        return true;
    }


    variable        : Variable;
    ext_states      = new Map<String,ExtState>(); // DevId -> ExtState
    vector_clock    = new VectorClock;            // coordinates of known patches
    patches         = [] as Array<Patch>;
    waiting_merges  = [] as Array<WaitingMerge>;
    waiting_changes = [] as Array<WaitingChange>;
}

export default PatchManager;

import OtWrapper from "./OtWrapper";
import OtMerger  from "./OtMerger";
