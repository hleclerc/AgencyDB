OtWrapper    = require "../Ot/OtWrapper.js"
Method       = require "../Core/Method.js"

#
module.exports =
class GenArray extends OtWrapper
    # patch types
    @op_types = OtWrapper.concattr OtWrapper.op_types,
        op_insert:
            attrs: "PT pos, Variable val*"
            right: ( flags, pos, val ) -> ( flags & @can_insert.flag ) or ( pos == @data.length and ( flags & @can_append.flag ) )
            apply_op: ( aod, pos, val ) ->
                val.onchange_par new Variable( this ), val
                @data.splice pos, 0, val
                @sig_change()
            undo: ( aod, pos, val ) ->
                console.error "TODO"
        op_remove:
            attrs: "PT pos, PT len*"
            right: ( flags, pos, len ) -> flags & @can_remove.flag
            apply_op: ( aod, pos, len ) ->
                @data.splice pos, 1
                @sig_change()
            undo: ( aod, pos, len ) ->
                console.error "TODO"
        op_remove_und:
            attrs: "PT pos, Variable item*"
            right: ( flags, pos, item ) -> flags & @can_remove.flag
            apply_op: ( aod, pos, item ) ->
                @data.splice pos, 1
                @sig_change()
            undo: ( aod, pos, item ) ->
                console.error "TODO"

    # right types
    @right_types = OtWrapper.right_types.concat " can_read can_insert can_remove can_append"

    #
    OtWrapper.make_templ this

    init: ( array ) ->
        super()

        @data = for v in array
            Variable.rp_conv_cp v

        va = new Variable this
        for d in @data
            d.onchange_par va, d

        @mod_nodes = []

    to_String: ->
        "[#{ @data }]"

    to_Array: ->
        for v in @data
            v.val

    get_size: ->
         new Number @data.length

    copy: ->
         new Array @data

    write_to: ( bw, cur_dev ) ->
        bw.write_PT GenArray.num_for_RW
        bw.write_PT @data.length
        for d in @data
            d.write_to bw

    @read_from = ( br, usr_id ) ->
        d = for i in [ 0 ... br.read_PT() ]
            br.read_Variable usr_id
        new Array d

    obj_type: ->
        Array

    sig_change_val: ( rp ) ->
        if rp not in @mod_nodes
            @mod_nodes.push rp
        super()

    freeze_last_changes: ->
        if @mod_nodes.length
            console.error "TODO"
            @mod_nodes = []

    # beware: item is owned by this
    _self_insert: ( pos, item, usr_id = @usr_id ) ->
        if @right_for_op_insert usr_id, pos, item
            @op_insert.reg_op @last_ops, pos, item
            @op_insert.apply_op.call this, { usr: usr_id, type: 0 }, pos, item
            true

    _self_remove: ( pos, len, usr_id = @usr_id ) ->
        if len and @right_for_op_remove( usr_id, pos, len )
            for i in [ 0 ... len ]
                if pos >= @data.length
                    break
                val = @data[ pos ]
                @op_remove_und.reg_op @last_ops, pos, val
                @op_remove_und.apply_op.call this, { usr: usr_id, type: 0 }, pos, val
            true

#
# Method.methods.mod_select.add_surdef
#     pert: 1
#     test: ( type_0, type_1 ) -> type_0 == OtWrapperMap and type_1.known_term_value
#     func: ( map, key ) ->
#         OtWrapperMapSelect.New map.rp, map.rp.kmap.get key, ->
#             if not ( map.rp.usr_right() & map.rp.can_add_key.flag )
#                 return undefined # Error.New "No right to add key"
#             vnkey = new map.rp.category_key Method.methods.copy.call_1 key
#             vnval = new map.rp.category_val
#             map.rp.create_node_4k vnkey, vnval
#
# Method.methods.val_type.add_surdef
#     pert: 1
#     test: ( type_0 ) -> type_0 == OtWrapperMap
#     func: ( map ) -> map.rp.category_val

Method.methods.self_set_o.add_surdef
    test: ( type_0, type_1 ) -> type_0 == GenArray
    func: ( arr, val ) ->
        ol = arr.length.val
        nl = val.length.val
        if ol > nl
            arr._self_remove nl, ol - nl
        for i in [ 0 ... Math.min( ol, nl ) ] by 1
            Method.methods.self_set_o.call_2 arr.at( i ), val.at( i )
        for i in [ ol ... nl ] by 1
            arr._self_insert i, val.at( i )
    pert: 2

Method.methods.self_insert_bo.add_surdef
    test: ( type_0, type_1, type_2 ) -> type_0 == GenArray and Method.methods.to_Number.has_surdef_for type_1
    func: ( str, pos, item ) ->
        npos = Method.methods.to_Number.call_1 pos
        str.rp._self_insert npos, item
    pert: 1

Method.methods.self_remove_bb.add_surdef
    test: ( type_0, type_1, type_2 ) -> type_0 == GenArray and Method.methods.to_Number.has_surdef_for( type_1 ) and Method.methods.to_Number.has_surdef_for( type_2 )
    func: ( str, pos, len ) ->
        npos = Method.methods.to_Number.call_1 pos
        nlen = Method.methods.to_String.call_1 len
        str.rp._self_remove npos, nlen
    pert: 1


Method.methods.mod_select.add_surdef
    pert: 1
    test: ( type_0, type_1 ) -> type_0 == GenArray and Method.methods.to_Number.has_surdef_for type_1
    func: ( arr, pos ) -> arr.rp.data[ Method.methods.to_Number.call_1 pos ]


# GenArraySelect = require "./GenArraySelect.js"
BinaryWriter         = require "../../System/BinaryWriter.js"
PatchId              = require "../../System/PatchId.js"
DevId                = require "../../System/DevId.js"
Bn_SI32              = require "../Number/Bn_SI32.js"
Variable             = require "../Core/Variable.js"
Number               = require "../Number.js"
Array                = require "../Array.js"
