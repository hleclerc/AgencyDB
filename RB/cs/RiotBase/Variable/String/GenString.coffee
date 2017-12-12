Method       = require "../Core/Method.js"
Rp           = require "../Core/Rp.js"

#
module.exports =
class GenString extends Rp
    @term_value = true
    Rp.make_templ this

    init: ( data ) ->
        @data = data

    to_String: ->
        @data

    make_ot_wrapper: ->
        OtWrapperString.New this

    get_size: ->
         new Number @data.length

    copy: ->
         new String @data

    write_to: ( bw, cur_dev ) ->
        # IMPORTANT_TODO
        console.error "TODO"

    obj_type: ->
        String

Method.methods.select_bb.add_surdef
    test: ( type_0, type_1 ) -> type_0 == GenString and Method.methods.to_Number.has_surdef_for( type_1 )
    func: ( str, ind ) -> new String str.rp.data[ Method.methods.to_Number.call_1 ind ]
    pert: 1

Method.methods.self_add_b.add_surdef
    test: ( type_0, type_1 ) -> type_0 == GenString and Method.methods.to_String.has_surdef_for( type_1 )
    func: ( str, sup ) -> str.rp.data += Method.methods.to_String.call_1 sup
    pert: 1

Method.methods.self_insert_bb.add_surdef
    test: ( type_0, type_1, type_2 ) -> type_0 == GenString and Method.methods.to_Number.has_surdef_for( type_1 ) and Method.methods.to_String.has_surdef_for( type_2 )
    func: ( str, pos, sup ) ->
        npos = Method.methods.to_Number.call_1 pos
        nsup = Method.methods.to_String.call_1 sup
        str.rp.data = str.rp.data.slice( 0, npos ) + nsup + str.rp.data.slice( npos )
    pert: 1

Method.methods.self_remove_bb.add_surdef
    test: ( type_0, type_1, type_2 ) -> type_0 == GenString and Method.methods.to_Number.has_surdef_for( type_1 ) and Method.methods.to_Number.has_surdef_for( type_2 )
    func: ( str, pos, len ) ->
        npos = Method.methods.to_Number.call_1 pos
        nlen = Method.methods.to_Number.call_1 len
        str.rp.data = str.rp.data.slice( 0, npos ) + str.rp.data.slice( npos + nlen )
    pert: 1

Method.methods.new_cursor.add_surdef
    test: ( type_0 ) -> type_0 == GenString
    func: ( obj, dev_id, loc_id ) ->
        obj.rp = Method.methods.make_ot_wrapper.call_1 obj
        Method.methods.new_cursor.call_1 obj, dev_id, loc_id
    pert: 1

Method.methods.rem_cursor.add_surdef
    test: ( type_0 ) -> type_0 == GenString
    func: ( obj, dev_id, loc_id ) ->
    pert: 1

Method.methods.get_cursor.add_surdef
    test: ( type_0 ) -> type_0 == GenString
    func: ( obj, dev_id, loc_id ) ->
    pert: 1

Method.methods.self_set_b.add_surdef
    test: ( type_0, type_1 ) -> type_0 == GenString and type_1 == GenString
    func: ( obj, val ) -> obj.rp.data = val.toString()
    pert: 2

Method.methods.equ_immediate.add_surdef
    test: ( type_0, type_1 ) -> type_0 == GenString or type_1 == GenString
    func: ( obj, val ) -> obj.toString() == val.toString()
    pert: 2

Method.methods.inf_immediate.add_surdef
    test: ( type_0, type_1 ) -> type_0 == GenString or type_1 == GenString
    func: ( obj, val ) -> obj.toString() < val.toString()
    pert: 2

Method.methods.sup_immediate.add_surdef
    test: ( type_0, type_1 ) -> type_0 == GenString or type_1 == GenString
    func: ( obj, val ) -> obj.toString() > val.toString()
    pert: 2

OtWrapperString = require "./OtWrapperString.js"
Bn_SI32         = require "../Number/Bn_SI32.js"
String          = require "../String.js"
Number          = require "../Number.js"
