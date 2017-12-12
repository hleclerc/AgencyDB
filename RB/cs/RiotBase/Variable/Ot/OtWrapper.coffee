BinaryWriter = require "../../System/BinaryWriter.js"
BinaryReader = require "../../System/BinaryReader.js"
UsrId        = require "../../System/UsrId.js"
GrpId        = require "../../System/GrpId.js"
RegCallbacks = require "../Core/RegCallbacks.js"
Method       = require "../Core/Method.js"
Rp           = require "../Core/Rp.js"
OtMerger     = require "./OtMerger.js"


#
module.exports =
class OtWrapper extends Rp
    @op_types =
        op_add_grp_right:
            attrs: "GrpId grp, PI32 val*"
            right: ( flags, grp, val ) ->
                flags & @can_add_rights.flag
            apply_op: ( aod, grp, val ) ->
                if val &= ~( @grp_rights[ grp ] ?= 0 )
                    @grp_rights[ grp ] |= val
                    @sig_change()
            undo: ( aod, grp, val ) ->
                @op_rem_grp_right.apply_op.call this, aod, grp, val
            update_patch_data_l0: ( args ) ->
                args[ 1 ] &= ~ @grp_right[ args[ 0 ] ]
        op_rem_grp_right:
            attrs: "GrpId grp, PI32 val*"
            right: ( flags, grp, val ) ->
                flags & @can_rem_rights.flag
            apply_op: ( aod, grp, val ) ->
                if val &= @grp_rights[ grp ]
                    @grp_rights[ grp ] &= ~val
                    @sig_change()
            undo: ( aod, grp, val ) ->
                @op_add_grp_right.apply_op.call this, aod, grp, val
            update_patch_data_l0: ( args ) ->
                args[ 1 ] &= @grp_right[ args[ 0 ] ]
        op_add_usr_right:
            attrs: "UsrId usr, PI32 val*"
            right: ( flags, usr, val ) ->
                flags & @can_add_rights.flag
            apply_op: ( aod, usr, val ) ->
                if val &= ~( @usr_rights[ usr ] ?= 0 )
                    @usr_rights[ usr ] |= val
                    @sig_change()
            undo: ( aod, usr, val ) ->
                @op_rem_usr_right.apply_op.call this, aod, usr, val
            update_patch_data_l0: ( args ) ->
                args[ 1 ] &= ~ @usr_right[ args[ 0 ] ]
        op_rem_usr_right:
            attrs: "UsrId usr, PI32 val*"
            right: ( flags, usr, val ) ->
                flags & @can_rem_rights.flag
            apply_op: ( aod, usr, val ) ->
                if val &= @usr_rights[ usr ]
                    @usr_rights[ usr ] &= ~val
                    @sig_change()
            undo: ( aod, usr, val ) ->
                @op_add_usr_right.apply_op.call this, aod, usr, val
            update_patch_data_l0: ( args ) ->
                args[ 1 ] &= @usr_right[ args[ 0 ] ]

    @right_types = "can_add_rights can_rem_rights"

    @concattr = ( a, b ) ->
        r = {}
        for n, v of a
            r[ n ] = v
        for n, v of b
            r[ n ] = v
        r

    @make_templ = ( cl ) ->
        Rp.make_templ cl

        # make stuff like @op_add_grp_right, ... with attributes
        # + @right_for_op_insert, @right_for_...,
        num = -1
        cl.prototype.proto_op_types = []
        for name, ot of cl.op_types
            obj =
                attrs   : []        # data as { type: ..., name: ... }
                undo    : ot.undo   #
                right   : ot.right  #
                num     : num += 1  # num in the list
                reg_op  : undefined # function to store operation in  BinaryWriter
                valid   : undefined # function to test if operation is valid (e.g. has to be stored)
                apply_op: ot.apply_op # ( inst, args... ) function to do the operation in inst (which may be a child of OtWrapper)
            cl.prototype.proto_op_types.push obj

            ba = ot.attrs.split( "," ).join( " " ).split( " " ).filter ( a ) -> a.length
            nv = -1
            for i in [ 0 ... ba.length / 2 ]
                if ba[ 2 * i + 1 ].indexOf( "*" ) >= 0
                    ba[ 2 * i + 1 ] = ba[ 2 * i + 1 ].replace "*", ""
                    nv = i
                obj.attrs.push
                    type: ba[ 2 * i + 0 ]
                    name: ba[ 2 * i + 1 ]

            test_valid_for = ( attr ) ->
                switch
                    when not attr?             then "true"
                    when attr.type == "String" then attr.name + ".length"
                    else attr.name

            eval "obj.reg_op = function ( bw,#{ a.name for a in obj.attrs } ) { if ( obj.valid(#{ a.name for a in obj.attrs }) ) { bw.write_PI8( #{num} ); #{( "bw.write_#{a.type}( #{a.name} );" for a in obj.attrs ).join " "} } }"
            eval "obj.read   = function ( bw, func ) { #{( "#{a.name} = bw.read_#{a.type}();" for a in obj.attrs ).join " "} func( this, #{( a.name for a in obj.attrs ).join ", "} ); }"
            eval "obj.valid  = function ( #{ a.name for a in obj.attrs } ) { return #{ test_valid_for obj.attrs[ nv ] }; }"
            obj.update_patch_data_l0 = ot.update_patch_data_l0 ? ( -> )
            cl.prototype[ name ] = obj

            eval "function rt( usr_id, #{ a.name for a in obj.attrs } ) { return this.#{ name }.right.call( this, this.usr_right( usr_id ), #{ a.name for a in obj.attrs } ); }"
            cl.prototype[ "right_for_#{ name }" ] = rt

        # make stuff like @can_add_right, @can_read, ... with attributes
        num = -1
        for name in cl.right_types.split " "
            cl.prototype[ name ] =
                num : num += 1  # num in the list
                flag: 1 << num


    init: ( usr_id = new UsrId ) ->
        @on_change_callbacks = {}
        @on_change_cur_cb_id = 0
        @on_change_par       = []
        @last_ops            = new BinaryWriter
        @usr_rights          = {} # usr_id -> unsigned (right flags)
        @grp_rights          = {} # grp_id -> unsigned (right flags)
        @usr_id              = usr_id # creator (who has all the right at the beginning)

        @usr_rights[ @usr_id.val ] = ( 1 << @constructor.right_types.split(" ").length ) - 1

    self_on_change: ( callback ) ->
        @on_change_callbacks[ @on_change_cur_cb_id += 1 ] = callback
        @on_change_cur_cb_id

    self_rm_on_change: ( callback_id ) ->
        delete @on_change_callbacks[ @on_change_cur_cb_id ]

    self_on_change_par: ( par, arg ) ->
        @on_change_par.push { par: par, arg: arg }

    pull_ot_changes: ( changes ) ->
        @freeze_last_changes()
        changes.write_some @last_ops.to_Uint8Array()
        @last_ops.clear()

    freeze_last_changes: ->

    new_ot_merger: ->
        new OtMerger this

    usr_right: ( usr_id = new UsrId ) ->
        val = @usr_rights[ usr_id.val ]
        if val? then val else 0

    sig_change: ->
        for p in @on_change_par
            p.par.sig_change p.arg
        for id, cb of @on_change_callbacks
            RegCallbacks.add cb

    patch_data_visitor: ( data, func ) ->
        bw = new BinaryReader data
        while bw.size
            num = bw.read_PI8()
            @proto_op_types[ num ].read bw, func

    reverse_patch_data_visitor: ( data, func ) ->
        # forward
        bw = new BinaryReader data
        offsets = []
        while bw.size
            offsets.push bw.cursor
            num = bw.read_PI8()
            @proto_op_types[ num ].read bw, () -> null

        # backward
        for offset in offsets by -1
            bw.cursor = offset
            num = bw.read_PI8()
            @proto_op_types[ num ].read bw, func




# Methods that will work with an OtWrapper
for n in [ "self_on_change", "self_on_change_par", "pull_ot_changes", "new_ot_merger" ]
    do ( n ) ->
        Method.methods[ n ].add_surdef
            test: ( type_0 ) -> Method.methods.make_ot_wrapper.has_surdef_for type_0
            func: ( args... ) ->
                args[ 0 ].rp = Method.methods.make_ot_wrapper.call_1 args[ 0 ]
                Method.methods[ n ].call_1.apply Method.methods[ n ], args
            pert: 0.5
