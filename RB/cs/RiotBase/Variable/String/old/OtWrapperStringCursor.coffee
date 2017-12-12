Method       = require "../Core/Method.js"
Rp           = require "../Core/Rp.js"

#
module.exports =
class OtWrapperStringCursor extends Rp
    Rp.make_templ this

    init: ( ot_wrapper_str, cursor ) ->
        @ot_wrapper_str = ot_wrapper_str
        @cursor         = cursor

    prox_ref: ->
        @cursor.pos

    prox_ptr: ->
        if @cursor not in @ot_wrapper_str.changed_cursors
            @ot_wrapper_str.changed_cursors.push @cursor
        @ot_wrapper_str.sig_change()
        @cursor.pos

# from a number
# Method.methods.self_set_b.add_surdef
#     test: ( type_0, type_1 ) -> type_0 == OtWrapperStringCursor and Method.methods.to_Number.has_surdef_for( type_1 )
#     func: ( cur, pos ) ->
#         console.log pos
#         npos = Method.methods.to_Number.call_1 pos
#         cur.rp.ot_wrapper_str._set_cursor cur.rp.cursor, new Array [ [ npos, npos ] ]
#     pert: 1
