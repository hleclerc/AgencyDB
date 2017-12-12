import BinaryWriter from "../../System/BinaryWriter";
import BinaryReader from "../../System/BinaryReader";
import UsrId        from "../../System/UsrId";
import DevId        from "../../System/DevId";

enum MergingType {
    NEW, UNK, STD, UNDO
}

//
export default
class OtMerger {
    static MergingType = MergingType;

    constructor( ot_wrapper ) {
        this.ot_wrapper = ot_wrapper
        this.cq_new     = new BinaryWriter
        this.cq_unk     = new BinaryWriter
    }

    glo_to_loc( data : BinaryWriter, input : Uint8Array, cur_dev : DevId, cur_usr : UsrId ) : boolean {
        this.ot_wrapper.patch_data_visitor( input, ( op, ...args ) => {
            OtWrapper.glo_to_loc( op, args, cur_dev, cur_usr );
            (<any>this).ot_wrapper.constructor.reg_op( data, op, ...args );
        } );
        return true;
    }

    loc_to_glo( data : BinaryWriter, input : Uint8Array, cur_dev : DevId, cur_usr : UsrId ) : boolean {
        this.ot_wrapper.patch_data_visitor( input, ( op, ...args ) => {
            OtWrapper.loc_to_glo( op, args, cur_dev, cur_usr );
            if ( typeof op.transmission != "undefined" )
                op.transmission( this.ot_wrapper, data, ...args );
            else
                (<any>this).ot_wrapper.constructor.reg_op( data, op, ...args );
        } );
        return true;
    }

    apply_op( aod, msg : Uint8Array ) : void {
        let res = new BinaryWriter;
        switch ( aod.type ) {
            case OtMerger.MergingType.NEW:  this.new_patch( res, msg, aod.usr ); break;
            case OtMerger.MergingType.UNK:  this.unk_patch( res, msg, aod.usr ); break;
            case OtMerger.MergingType.STD:  this.std_patch( res, msg, aod.usr ); break;
            case OtMerger.MergingType.UNDO: this.undo_patch( msg, aod.usr ); break;
            default: throw new Error( "TODO" );
        }
    }

    /** res = update of data (according to unk patches) */
    new_patch( res : BinaryWriter, msg : Uint8Array, as_usr = new UsrId, nb_rem_unk = 0, nb_rem_std = 0 ) {
        let tmp_res = new BinaryWriter; // res may contain data
        this.ot_wrapper.patch_data_visitor( msg, ( op_new, ...args_new ) => {
            op_new.update_patch_data_l0( this.ot_wrapper, args_new );

            // 1) update data according to cq_unk; 2) make cq_unk'
            let updated_cq_unk = new BinaryWriter;
            this.ot_wrapper.patch_data_visitor( this.cq_unk.to_Uint8Array(), ( op_unk, ...args_unk ) => {
                this.ot_wrapper.unk_new_or_new_unk( op_new, op_unk, as_usr, tmp_res, updated_cq_unk, args_new, args_unk );
                if ( op_unk.valid( this.ot_wrapper, ...args_unk ) )
                    (<any>this).ot_wrapper.constructor.reg_op( updated_cq_unk, op_unk, ...args_unk );
            } );

            // check validity && rights to register && apply the operation
            if ( op_new.valid( this.ot_wrapper, ...args_new ) && op_new.right( this.ot_wrapper, this.ot_wrapper.usr_right( as_usr ), ...args_new ) ) {
                // register operations
                (<any>this).ot_wrapper.constructor.reg_op( tmp_res, op_new, ...args_new );
                this.cq_unk = updated_cq_unk;

                // apply operation
                op_new.apply_op( this.ot_wrapper, { usr: as_usr, type: OtMerger.MergingType.NEW }, ...args_new );
            }
        } );

        if ( nb_rem_unk || nb_rem_std ) // if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
            this.cq_new.write_some( tmp_res.to_Uint8Array() );
        res.write_some( tmp_res.to_Uint8Array() );
    }

    /** res = update of data (according to new patches) */
    unk_patch( res : BinaryWriter, msg : Uint8Array, as_usr = new UsrId, nb_rem_new = 0, nb_rem_std = 0 ) {
        let tmp_res = new BinaryWriter; // res may contain data
        this.ot_wrapper.patch_data_visitor( msg, ( op_unk, ...args_unk ) => {
            op_unk.update_patch_data_l0( this.ot_wrapper, args_unk );

            // 1) update data according to cq_new; 2) make cq_new'
            let updated_cq_new = new BinaryWriter;
            this.ot_wrapper.patch_data_visitor( this.cq_new.to_Uint8Array(), ( op_new, ...args_new ) => {
                this.ot_wrapper.unk_new_or_new_unk( op_unk, op_new, as_usr, tmp_res, updated_cq_new, args_unk, args_new );
                if ( op_new.valid( this.ot_wrapper, ...args_new ) )
                    (<any>this).ot_wrapper.constructor.reg_op( updated_cq_new, op_new, ...args_new );
            } );
            // check validity && rights to register && apply the operation
            if ( op_unk.valid( this.ot_wrapper, ...args_unk ) && op_unk.right( this.ot_wrapper, this.ot_wrapper.usr_right( as_usr ), args_unk ) ) {
                // register operations
                (<any>this).ot_wrapper.constructor.reg_op( tmp_res, op_unk, ...args_unk );
                this.cq_new = updated_cq_new;

                // apply operation
                op_unk.apply_op( this.ot_wrapper, { usr: as_usr, type: OtMerger.MergingType.UNK  }, ...args_unk );
            }
        } );

        if ( nb_rem_new || nb_rem_std ) // if there are prout or std patches, we need a representation of the preceding unk stuff to modify the corresponding data
            this.cq_unk.write_some( tmp_res.to_Uint8Array() );
        res.write_some( tmp_res.to_Uint8Array() );
    }

    /** pd = update of data (according to unk patches) */
    std_patch( res : BinaryWriter, data : Uint8Array, as_usr = new UsrId, nb_rem_unk = 0, nb_rem_std = 0 ) {
        // goal:
        // - update std data according to new stuff
        // - make new' (new after std)
        // - make the operations
        let tmp_res = new BinaryWriter; // res may contain data
        this.ot_wrapper.patch_data_visitor( data, ( op_std, ...args_std ) => {
            op_std.update_patch_data_l0( this.ot_wrapper, args_std );

            // modifications
            let updated_cq_new = new BinaryWriter; // res may contain data
            this.ot_wrapper.patch_data_visitor( this.cq_new.to_Uint8Array(), ( op_new, ...args_new ) => {
                this.ot_wrapper.unk_new_or_new_unk( op_std, op_new, as_usr, tmp_res, updated_cq_new, args_std, args_new );
                if ( op_new.valid( this.ot_wrapper, ...args_new ) )
                    (<any>this).ot_wrapper.constructor.reg_op( updated_cq_new, op_new, ...args_new );
            } );
            // check validity + rights
            if ( op_std.valid( this.ot_wrapper, ...args_std ) && op_std.right( this.ot_wrapper, this.ot_wrapper.usr_right( as_usr ), args_std ) ) {
                // register operations (in cq_unk)
                (<any>this).ot_wrapper.constructor.reg_op( tmp_res, op_std, ...args_std );
                this.cq_new = updated_cq_new;

                // apply operation
                op_std.apply_op( this.ot_wrapper, { usr: as_usr, type: OtMerger.MergingType.STD }, ...args_std );
            }
        } );

        // goal:
        // - make unk' (unk after std)
        this.ot_wrapper.patch_data_visitor( tmp_res.to_Uint8Array(), ( op_std, ...args_std ) => {
            let updated_cq_unk = new BinaryWriter;
            this.ot_wrapper.reverse_patch_data_visitor( this.cq_unk.to_Uint8Array(), ( op_unk, ...args_unk ) => {
                this.ot_wrapper.unk_std_comb( op_std, op_unk, as_usr, updated_cq_unk, args_std, args_unk );
                op_unk.reg_op.apply( this.ot_wrapper, [ updated_cq_unk ].concat( args_unk ) );
            } );

            this.cq_unk = new BinaryWriter;
            this.ot_wrapper.reverse_patch_data_visitor( updated_cq_unk.to_Uint8Array(), ( op_unk, ...args_unk ) => {
                (<any>this).ot_wrapper.constructor.reg_op( this.cq_unk, op_unk, ...args_unk );
            } );
        } );

        res.write_some( tmp_res.to_Uint8Array() );
    }

    /** pd = update of data (according to unk patches). data should be an array */
    undo_patch( data : Uint8Array, as_usr = new UsrId ) {
        this.ot_wrapper.reverse_patch_data_visitor( data, ( type, ...args ) => {
            type.undo( this.ot_wrapper, { usr: as_usr, type: 0 }, ...args );
        } );
    }

    /** */
    new_merge( pm : PatchManager, as_usr : UsrId, nb_rem_unk = 0, nb_rem_std = 0 ) {
        let bw = new BinaryWriter;
        this.ot_wrapper.get_merge_effect( bw, pm.variable.rp, as_usr );
        this.new_patch( new BinaryWriter, bw.to_Uint8Array(), as_usr, nb_rem_unk, nb_rem_std );
    }

    /** */
    unk_merge( pm : PatchManager, as_usr : UsrId, nb_rem_unk = 0, nb_rem_std = 0 ) {
        throw new Error( "TODO" );
    }

    /** */
    std_merge( pm : PatchManager, as_usr : UsrId, nb_rem_unk = 0, nb_rem_std = 0 ) {
        throw new Error( "TODO" );
    }

    finalize() {
        this.ot_wrapper.sig_change();
    }

    ot_wrapper: OtWrapper;
    cq_new    : BinaryWriter;
    cq_unk    : BinaryWriter;
}

import OtWrapper    from "./OtWrapper";
import PatchManager from "./PatchManager";
