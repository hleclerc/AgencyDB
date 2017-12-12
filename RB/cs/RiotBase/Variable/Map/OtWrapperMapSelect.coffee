Method = require "../Core/Method.js"
Rp     = require "../Core/Rp.js"

#
module.exports =
class OtWrapperMapSelect extends Rp
    Rp.make_templ this

    init: ( map, node ) ->
        @map  = map
        @node = node

    prox_ptr: ->
        @map.sig_change_val @node
        @node.val

    prox_ref: ->
        @node.val

# Method.methods.self_set_b.add_surdef
#     test: ( type_0, type_1 ) -> type_0 == OtWrapperMapSelect and type_1.known_term_value
#     func: ( obj, val ) ->
#         obj.rp.map.sig_change_val obj.rp.node
#         Method.methods.self_set_b.call_1 obj.rp.node.val, val
#     pert: 2
