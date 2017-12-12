BinaryWriter = require "../../System/BinaryWriter.js"
VectorClock  = require "../../System/VectorClock.js"
PatchId      = require "../../System/PatchId.js"
DevId        = require "../../System/DevId.js"
UsrId        = require "../../System/UsrId.js"
Method       = require "../Core/Method.js"

#
class Patch
    constructor: ( patch_id ) ->
        @patch_id = new PatchId
        # @usr_id   ///< who has made the changes
        # @date    ///< date of this change (absolute local date)
        # @data    ///< content (operations)

#
class ExtState
    constructor: ->
        @known = new VectorClock
        @sent  = new VectorClock

#
module.exports =
class PatchManager
    constructor: ( variable ) ->
        @variable     = variable
        @ext_states   = new Map # DevId -> ExtState
        @vector_clock = new VectorClock
        @patches      = []

    get_changes: ( dst_dev, cur_dev, cur_usr ) ->
        @freeze_last_mod()

        # ext_state for dst_dev
        if not ( dst_ext_state = @ext_states.get dst_dev.val )?
            dst_ext_state = new ExtState
            @ext_states.set dst_dev.val, dst_ext_state

        # list of indices of patches to be sent
        to_be_sent = []
        @vector_clock.map.forEach ( num, dev ) =>
            knum = dst_ext_state.sent.val val: dev
            for snum in [ knum + 1 .. num ] by 1
                to_be_sent.push @find_patch_index dev, snum

        @get_msg_for_patch_list cur_dev, cur_usr, dst_ext_state.sent, to_be_sent

    # to_be_sent -> list of indices in patch list. known is updated in the procedure
    get_msg_for_patch_list: ( cur_dev, cur_usr, known, to_be_sent ) ->
        if not to_be_sent.length
            return new Uint8Array
        to_be_sent.sort()

        # first part of the message: on which patches the sent ones are based
        bw = new BinaryWriter
        bw.write_PT known.map.size
        known.map.forEach ( num, dev ) =>
            bw.write_DevId new DevId( dev ), cur_dev
            bw.write_PT num

        # second part: the patches
        for ind_patch in to_be_sent
            patch = @patches[ ind_patch ]
            bw.write_PatchId ( if ind_patch then @patches[ ind_patch - 1 ].patch_id else new PatchId ), cur_dev # coordinates of the previous patch
            bw.write_PatchId patch.patch_id, cur_dev                                                            # coordinates of the current patch
            bw.write_UsrId patch.usr_id, cur_dev                                                                # user who has created this patch
            bw.write_PT if ind_patch then patch.date - @patches[ ind_patch - 1 ].date else patch.date           # (delta) creation date
            bw.write_Uint8Array patch.data # IMPORTANT_TODO: global patch data

            # say that the patch has been sent
            known.update patch.patch_id.dev, patch.patch_id.num

        bw.to_Uint8Array()

    app_changes: ( ans, bp, src_dev, dst_devs, cur_dev, cur_usr ) -> # cur_dev should be included in dst_devs
        if not bp.size
            return
        @freeze_last_mod()

        # read id of patches which serve as base for the new ones
        assumed_to_be_known = new VectorClock
        nb_items = bp.read_PT()
        for i in [ 0 ... nb_items ]
            dev = bp.read_DevId cur_dev
            num = bp.read_PT()
            assumed_to_be_known.map.set dev, num

        # there are patch assumed_to_be_known but not present ??
        assumed_to_be_known.map.forEach ( num, dev ) =>
            if @vector_clock.val( dev ) < num
                console.log "TODO"

        # unk
        unk_patches = []
        @vector_clock.map.forEach ( num, dev ) =>
            for i in [ assumed_to_be_known.val( dev ) + 1 .. num ] by 1
                unk_patches.push @patches[ @find_patch_index dev, num ]

        # read the patches
        merger = Method.methods.new_ot_merger.call_1 @variable
        index_first_insertion = 1 << 30
        new_patches = []
        while bp.size
            # prev and position of the patch
            prev_patch_id = bp.read_PatchId cur_dev
            index_ins_patch = if prev_patch_id.num then @find_patch_index( prev_patch_id ) + 1 else 0

            # patch header
            n = new Patch
            n.patch_id = bp.read_PatchId cur_dev
            n.usr_id   = bp.read_UsrId cur_dev
            n.date     = if index_ins_patch then @patches[ index_ins_patch - 1 ] + bp.read_PT() else bp.read_PT()

            # update ext_states (what is known and can be assumed to be sent to the other machines)
            if not @ext_states.has( src_dev.val )
                @ext_states.set src_dev.val, new ExtState
            @ext_states.get( src_dev.val ).known.update n.patch_id
            @ext_states.get( src_dev.val ).sent.update n.patch_id
            for d in dst_devs
                if d.val != cur_dev.val
                    @ext_states.get( src_dev.val ).sent.update n.patch_id

            # we already have this patch ?
            if @vector_clock.contains n.patch_id
                bp.skip_Uint8Array()
                continue

            # else, register the patch and read the content
            data = new BinaryWriter
            if not merger.glo_to_loc data, bp.read_Uint8Array(), cur_dev, cur_usr
                break
            n.data = data.to_Uint8Array()
            @vector_clock.update n.patch_id
            new_patches.push n

            # find where to actually place to patch
            glo_dev = ( p ) -> p.patch_id.dev.to_glo( cur_dev ).val

            if index_ins_patch # IMPORTANT_TODO: check that code
                while index_ins_patch < @patches.length and ( @patches[ index_ins_patch ].date < n.date or ( @patches[ index_ins_patch ].date == n.date and glo_dev( @patches[ index_ins_patch ] ) < glo_dev( n ) ) )
                    index_ins_patch += 1
            else if @patches.length
                n.date = @patches[ index_ins_patch ].date
                while index_ins_patch < @patches.length and @patches[ index_ins_patch ].date == n.date and glo_dev( @patches[ index_ins_patch ] ) < glo_dev( n )
                    index_ins_patch += 1

            # insertion of the patch
            index_first_insertion = Math.min index_first_insertion, index_ins_patch
            @patches.splice index_ins_patch, 0, n

        # update patch content
        if new_patches.length
            # undo
            for index in [ @patches.length - 1 .. index_first_insertion ] by -1
                if @patches[ index ] not in new_patches
                    merger.undo @patches[ index ].data, @patches[ index ].usr_id

            # merge the data
            nb_new = new_patches.length
            nb_unk = 0
            nb_std = 0
            for index in [ index_first_insertion ... @patches.length ]
                # if ( not f->prev )
                #     merger->set_creator( f->usr_id );

                f = @patches[ index ]
                pd = new BinaryWriter
                if f in new_patches
                    console.log f.data
                    merger.new_patch pd, f.data, f.usr_id, nb_unk, nb_std
                    --nb_new
                else if f in unk_patches
                    merger.unk_patch pd, f.data, f.usr_id, nb_new, nb_std
                    --nb_unk
                else
                    merger.std_patch pd, f.data, f.usr_id, nb_unk, nb_new
                    --nb_std

            merger.finalize()

        # if ( new_patches.size() ) {
        #     // find the first patch in new patches
        #     Patch *f = new_patches[ 0 ];
        #     for( PT i = 1; i < new_patches.size(); ++i )
        #         if ( f->num > new_patches[ i ]->num )
        #             f = new_patches[ i ];
        #     // and in unknown patches
        #     for( const auto &pa : unkn.data )
        #         for( PI64 pn : pa.second )
        #             if ( Patch *p = find_patch( pn, pa.first ) )
        #                 if ( f->num > p->num )
        #                     f = p;
        #
        #     // undo stuff made in unk and std patches
        #     PT nb_std = 0;
        #     for( Patch *o = last_patch.ptr(); o; o = o->prev.ptr() ) {
        #         if ( not new_patches.contains( o ) )
        #             merger->undo( o->data, o->usr_id );
        #         ++nb_std;
        #         if ( o == f )
        #             break;
        #     }
        #


    freeze_last_mod: ->
        operations = new BinaryWriter
        @variable.pull_ot_changes operations
        if operations.size
            p              = new Patch
            p.data         = operations.to_Uint8Array()
            p.patch_id.num = @vector_clock.inc new DevId
            p.date         = Date.now()
            p.usr_id       = new UsrId

            @patches.push p

    find_patch_index: ( dev, num ) ->
        for n in [ @patches.length - 1 .. 0 ]
            if @patches[ n ].patch_id.num == num and @patches[ n ].patch_id.dev.val == dev
                return n
        -1
