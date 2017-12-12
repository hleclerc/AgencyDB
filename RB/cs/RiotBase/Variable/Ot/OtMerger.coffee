BinaryWriter = require "../../System/BinaryWriter.js"
UsrId        = require "../../System/UsrId.js"

#
module.exports =
class OtMerger
    @type_new = 0
    @type_unk = 1
    @type_std = 2
    @type_und = 3

    constructor: ( ot_wrapper ) ->
        @ot_wrapper = ot_wrapper
        @cq_new     = new BinaryWriter
        @cq_unk     = new BinaryWriter

    glo_to_loc: ( data, input, cur_dev, cur_usr ) ->
        # return pc->patch_data_visitor( BinStream<CbStringPtr>( &data ), [&]( auto op, auto&& ...args ) {
        #     apply_va( [&]( auto &arg ) { RiotBase::glo_to_loc( arg, cur_dev, cur_usr ); }, args... );
        #     pc->reg_op( loc, op, std::forward<decltype(args)>( args )... );
        # } );
        data.write_some input
        true

    loc_to_glo: ( data, input, cur_dev, cur_usr ) ->
        # return pc->patch_data_visitor( BinStream<CbStringPtr>( &data ), [&]( auto op, auto&& ...args ) {
        #     apply_va( [&]( auto &arg ) { RiotBase::loc_to_glo( arg, cur_dev, cur_usr ); }, args... );
        #     pc->transmission( op, glo, args... );
        # } );
        data.write_some input
        true

    # res = update of data (according to unk patches). msg should be a Uint8Array
    new_patch: ( res, msg, as_usr = new UsrId, nb_rem_unk = 0, nb_rem_std = 0 ) ->
        tmp_res = new BinaryWriter # res may contain data
        @ot_wrapper.patch_data_visitor msg, ( op_new, args_new... ) =>
            op_new.update_patch_data_l0.call @ot_wrapper, args_new

            # 1) update data according to cq_unk; 2) make cq_unk'
            updated_cq_unk = new BinaryWriter
            @ot_wrapper.patch_data_visitor @cq_unk, ( op_unk, args_unk... ) =>
                @ot_wrapper.unk_new_or_new_unk op_new, op_unk, as_usr, tmp_res, updated_cq_unk, args_new, args_unk
                if op_unk.valid.apply @ot_wrapper, args_unk
                    op_unk.reg_op.apply @ot_wrapper, [ updated_cq_unk, op_unk ].concat args_unk

            # check validity and rights to register and apply the operation
            if op_new.valid.apply( @ot_wrapper, args_new ) and op_new.right.apply( @ot_wrapper, [ @ot_wrapper.usr_right as_usr ].concat args_new )
                # register operations
                op_new.reg_op.apply @ot_wrapper, [ tmp_res ].concat args_new
                @cq_unk = updated_cq_unk

                # apply operation
                op_new.apply_op.apply @ot_wrapper, [ usr: as_usr, type: OtMerger.type_new ].concat args_new

        if nb_rem_unk or nb_rem_std # if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
            @cq_new.write_some tmp_res
        res.write_some tmp_res

    # res = update of data (according to new patches). msg should be an Uint8Array
    unk_patch: ( res, msg, as_usr = new UsrId, nb_rem_new = 0, nb_rem_std = 0 ) ->
        tmp_res = new BinaryWriter # res may contain data
        @ot_wrapper.patch_data_visitor msg, ( op_unk, args_unk... ) =>
            op_unk.update_patch_data_l0.call @ot_wrapper, args_unk

            # 1) update data according to cq_new; 2) make cq_new'
            updated_cq_new = new BinaryWriter
            @ot_wrapper.patch_data_visitor @cq_new, ( op_new, args_new... ) =>
                @ot_wrapper.unk_new_or_new_unk op_unk, op_new, as_usr, tmp_res, updated_cq_new, args_unk, args_new
                if op_new.valid.apply @ot_wrapper, args_new
                    op_new.reg_op.apply @ot_wrapper, [ updated_cq_new, op_new ].concat args_new

            # check validity and rights to register and apply the operation
            if op_unk.valid.apply( @ot_wrapper, args_unk ) and op_unk.right.apply( @ot_wrapper, [ @ot_wrapper.usr_right as_usr ].concat args_unk )
                # register operations
                op_unk.reg_op.apply @ot_wrapper, [ tmp_res ].concat args_unk
                @cq_new = updated_cq_new

                # apply operation
                op_unk.apply_op.apply @ot_wrapper, [ usr: as_usr, type: OtMerger.type_unk ].concat args_unk

        if nb_rem_new or nb_rem_std # if there are prout or std patches, we need a representation of the preceding unk stuff to modify the corresponding data
            @cq_unk.write_some tmp_res
        res.write_some tmp_res

    # pd = update of data (according to unk patches). data should be an array
    std_patch: ( res, data, as_usr = new UsrId, nb_rem_unk = 0, nb_rem_std = 0 ) ->
        # goal:
        # - update std data according to new stuff
        # - make new' (new after std)
        # - make the operations
        tmp_res = new BinaryWriter # res may contain data
        @ot_wrapper.patch_data_visitor data, ( op_std, args_std... ) =>
            op_std.update_patch_data_l0.call @ot_wrapper, args_std

            # modifications
            updated_cq_new = new BinaryWriter # res may contain data
            @ot_wrapper.patch_data_visitor @cq_new, ( op_new, args_new... ) =>
                @ot_wrapper.unk_new_or_new_unk op_std, op_new, as_usr, tmp_res, updated_cq_new, args_std, args_new
                if op_new.valid.apply @ot_wrapper, args_new
                    op_new.reg_op.apply @ot_wrapper, [ updated_cq_new ].concat args_new

            # check validity + rights
            if op_std.valid.apply( @ot_wrapper, args_std ) and op_std.right.apply( @ot_wrapper, [ @ot_wrapper.usr_right as_usr ].concat args_std )
                # register operations (in cq_unk)
                op_std.reg_op.apply @ot_wrapper, [ tmp_res ].concat args_std
                @cq_new = updated_cq_new

                # apply operation
                op_std.apply_op.apply @ot_wrapper, [ usr: as_usr, type: OtMerger.type_std ].concat args_std

        # goal:
        # - make unk' (unk after std)
        data = new BinaryReader tmp_res.to_Uint8Array()
        @ot_wrapper.patch_data_visitor data, ( op_std, args_std... ) =>
            CbQueue updated_cq_unk;
            @ot_wrapper.reverse_patch_data_visitor @cq_unk, ( op_unk, args_unk... ) =>
                @ot_wrapper.unk_std_comb op_std, op_unk, as_usr, updated_cq_unk, args_std, args_unk
                op_unk.reg_op.apply @ot_wrapper, [ updated_cq_unk ].concat args_unk

            cq_unk.clear()
            @ot_wrapper.reverse_patch_data_visitor updated_cq_unk, ( op_unk, args_unk... ) =>
                op_unk.reg_op.apply @ot_wrapper, [ cq_unk ].concat args_unk

        res.write_some tmp_res

    # pd = update of data (according to unk patches). data should be an array
    undo: ( data, as_usr = new UsrId ) ->
        @ot_wrapper.reverse_patch_data_visitor data, ( type, args... ) =>
            type.undo.apply @ot_wrapper, [ { usr: as_usr, type: 0 } ].concat args

    finalize: ->
        @ot_wrapper.sig_change()
