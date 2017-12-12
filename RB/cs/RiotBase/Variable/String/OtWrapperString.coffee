OtWrapper    = require "../Ot/OtWrapper.js"
Method       = require "../Core/Method.js"

#
module.exports =
class OtWrapperString extends OtWrapper
    # patch types
    @op_types = OtWrapper.concattr OtWrapper.op_types,
        op_insert:
            attrs: "PT pos, String str*"
            right: ( flags, pos, sup ) -> ( flags & @can_insert.flag ) or ( pos == @val.length.val and ( flags & @can_append.flag ) )
            apply_op: ( aod, pos, sup ) ->
                for c in @cursors
                    nl = c.pos.val
                    for i in [ 0 ... nl.length ]
                        for j in [ 0 ... 2 ]
                            if nl[ i ][ j ] >= pos
                                nl[ i ][ j ] += sup.length
                    c.pos.val = nl
                Method.methods.self_insert_bb.call_3 @val, new Number( pos ), new String( sup )
                @sig_change()
            undo: ( aod, pos, sup ) ->
                console.error "TODO"
        op_remove:
            attrs: "PT pos, PT len*"
            right: ( flags, pos, len ) -> flags & @can_remove.flag
            apply_op: ( aod, pos, len ) ->
                for c in @cursors
                    nl = c.pos.val
                    for i in [ 0 ... nl.length ]
                        for j in [ 0 ... 2 ]
                            if nl[ i ][ j ] >= pos + len
                                nl[ i ][ j ] -= len
                            else if nl[ i ][ j ] > pos
                                nl[ i ][ j ] = pos
                    c.pos.val = nl
                Method.methods.self_remove_bb.call_3 @val, new Number( pos ), new Number( len )
                @sig_change()
            undo: ( aod, pos, len ) ->
                console.error "TODO"
        op_remove_und:
            attrs: "PT pos, String str*"
            right: ( flags, pos, str ) -> flags & @can_remove.flag
            apply_op: ( aod, pos, str ) ->
                for c in @cursors
                    nl = c.pos.val
                    for i in [ 0 ... nl.length ]
                        for j in [ 0 ... 2 ]
                            if nl[ i ][ j ] >= pos + str.length
                                nl[ i ][ j ] -= str.length
                            else if nl[ i ][ j ] > pos
                                nl[ i ][ j ] = pos
                    c.pos.val = nl
                Method.methods.self_remove_bb.call_3 @val, new Number( pos ), new Number( str.length )
                @sig_change()
            undo: ( aod, pos, str ) ->
                console.error "TODO"
        op_new_cursor:
            attrs: "DevId dev_id, PT loc_id*, Variable pos"
            right: ( flags, dev_id, loc_id, pos ) -> console.error "TODO"
            apply_op: ( aod, dev_id, loc_id, pos ) ->
                @_new_cursor_wo_cr dev_id, loc_id, pos
            undo: ( aod, dev_id, loc_id, pos ) ->
                @op_rem_cursor.apply_op.call this, aod, dev_id, loc_id
        op_set_cursor:
            attrs: "DevId dev_id, PT loc_id*, Variable pos"
            right: ( flags, dev_id, loc_id, pos ) -> console.error "TODO"
            apply_op: ( aod, dev_id, loc_id, pos ) ->
                console.error "TODO"
            undo: ( aod, dev_id, loc_id, pos ) ->
                console.error "TODO"
        op_set_cursor_und:
            attrs: "DevId dev_id, PT loc_id*, Variable pos, Variable old_pos"
            right: ( flags, dev_id, loc_id, pos, old_pos ) -> console.error "TODO"
            apply_op: ( aod, dev_id, loc_id, pos, old_pos ) ->
                console.error "TODO"
            undo: ( aod, dev_id, loc_id, pos, old_pos ) ->
                console.error "TODO"
        op_rem_cursor:
            attrs: "DevId dev_id, PT loc_id*"
            right: ( flags, dev_id, loc_id ) -> console.error "TODO"
            apply_op: ( aod, dev_id, loc_id ) ->
                @_rem_cursor_wo_cr dev_id, loc_id
            undo: ( aod, dev_id, loc_id ) ->
                console.error "TODO"
        op_rem_cursor_und:
            attrs: "DevId dev_id, PT loc_id*, Variable old_pos, UsrId usr_id"
            right: ( flags, dev_id, old_pos, usr_id ) -> console.error "TODO"
            apply_op: ( aod, dev_id, old_pos, usr_id ) ->
                console.error "TODO"
            undo: ( aod, dev_id, old_pos, usr_id ) ->
                console.error "TODO"

    # right types
    @right_types = OtWrapper.right_types.concat " can_read can_insert can_append can_remove"

    #
    class Cursor
        constructor: ( pos )->
            @pos    = pos # position in the text (list of [start,end]). Type = Array
            @loc_id = 0 # id of the cursor (to be used with dev_id)
            @dev_id = new DevId # from which dev
            @usr_id = new UsrId # from which usr


    #
    OtWrapper.make_templ this

    init: ( rp ) ->
        super()
        @val             = new String rp
        @cursors         = []
        @cur_loc_id      = 0
        @changed_cursors = []

        # first patch
        @op_insert.reg_op @last_ops, 0, @val.toString()

    prox_ref: ->
        @val

    new_cursor: ( pos = new Array )->
        if @usr_right() & @can_read.flag
            # find loc_id
            loc_id = 0
            for c in @cursors when c.dev_id.loc()
                loc_id = Math.max loc_id, c.loc_id
            loc_id += 1

            # make a new cursor
            vpos = Array.rp_conv pos
            @op_new_cursor.reg_op @last_ops, new DevId, loc_id, vpos
            @_new_cursor_wo_cr new DevId, loc_id, vpos

            loc_id

    rem_cursor: ( dev_id, loc_id ) ->
        if @usr_right() & @can_read.flag
            for c in @cursors
                if c.dev_id.val == dev_id.val and c.loc_id == loc_id
                    @op_rem_cursor_und.reg_op @last_ops, dev_id, loc_id, c.pos, c.usr_id
                    @_rem_cursor_wo_cr dev_id, loc_id
                    break

    get_cursor: ( dev_id, loc_id ) ->
        if @usr_right() & @can_read.flag
            for c in @cursors
                if c.dev_id.val == dev_id.val and c.loc_id == loc_id
                    return c.pos
        return undefined

    get_cursor_ids: ->
        if @usr_right() & @can_read.flag
            for c in @cursors
                dev_id: c.dev_id
                loc_id: c.loc_id

    freeze_last_changes: ->
        if @changed_cursors.length
            console.error "TODO"
            @changed_cursors = []

    sig_change: ( item ) ->
        super()
        if item instanceof Cursor and item not in @changed_cursors
            @changed_cursors.push item

    _new_cursor_wo_cr: ( dev_id, loc_id, pos ) ->
        cursor        = new Cursor
        cursor.dev_id = dev_id
        cursor.loc_id = loc_id
        cursor.pos    = pos
        pos.onchange_par new Variable( this ), cursor

        @cursors.push cursor
        @sig_change()

    _rem_cursor_wo_cr: ( dev_id, loc_id ) ->
        for i in [ 0 ... @cursors.length ]
            if @cursors[ i ].dev_id.val == dev_id.val and @cursors[ i ].loc_id == loc_id
                @cursors.splice i, 1
                @sig_change()
                break

    _self_insert: ( pos, sup, usr_id = @usr_id ) ->
        if @right_for_op_insert usr_id, pos, sup
            @op_insert.reg_op @last_ops, pos, sup
            @op_insert.apply_op.call this, { usr: usr_id, type: 0 }, pos, sup

    _self_remove: ( pos, len, usr_id = @usr_id ) ->
        if @right_for_op_remove usr_id, pos, len
            str = @val.val.substr pos, len
            @op_remove_und.reg_op @last_ops, pos, str
            @op_remove_und.apply_op.call this, { usr: usr_id, type: 0 }, pos, str

    _set_cursor: ( cur, pos ) ->
        if ( @usr_right() & @can_read.flag ) and cur.pos != pos
            @op_set_cursor_und.reg_op @last_ops, cur.dev_id, cur.loc_id, pos, cur.pos
            @_set_cursor_wo_cr cur, pos

    _set_cursor_wo_cr: ( cur, pos ) ->
        if cur.pos != pos
            cur.pos = pos
            @sig_change()

Method.methods.self_set_o.add_surdef
    test: ( type_0, type_1 ) -> type_0 == OtWrapperString and Method.methods.to_String.has_surdef_for type_1
    func: ( str, sup ) ->
        if not str.equ_immediate( sup )
            str.clear()
            str.insert 0, sup.toString()
    pert: 2

Method.methods.self_add_b.add_surdef
    test: ( type_0, type_1 ) -> type_0 == OtWrapperString
    func: ( str, sup ) ->
        pos = Method.methods.get_size.call_1 str
        Method.methods.self_insert_bb.call_3 str, pos, sup
    pert: 2

Method.methods.self_insert_bb.add_surdef
    test: ( type_0, type_1, type_2 ) -> type_0 == OtWrapperString and Method.methods.to_Number.has_surdef_for( type_1 ) and Method.methods.to_String.has_surdef_for( type_2 )
    func: ( str, pos, sup ) ->
        npos = Method.methods.to_Number.call_1 pos
        nsup = Method.methods.to_String.call_1 sup
        str.rp._self_insert npos, nsup
    pert: 2

Method.methods.self_remove_bb.add_surdef
    test: ( type_0, type_1, type_2 ) -> type_0 == OtWrapperString and Method.methods.to_Number.has_surdef_for( type_1 ) and Method.methods.to_Number.has_surdef_for( type_2 )
    func: ( str, pos, len ) ->
        npos = Method.methods.to_Number.call_1 pos
        nlen = Method.methods.to_String.call_1 len
        str.rp._self_remove npos, nlen
    pert: 2

# OtWrapperStringCursor = require "./OtWrapperStringCursor.js"
DevId                 = require "../../System/DevId.js"
UsrId                 = require "../../System/UsrId.js"
Variable              = require "../Core/Variable.js"
Array                 = require "../Array.js"
String                = require "../String.js"
Number                = require "../Number.js"
