Method = require "../Core/Method.js"
Rp     = require "../Core/Rp.js"

#
module.exports =
class Bn_SI32 extends Rp
    @term_value = true
    Rp.make_templ this

    init: ( val ) ->
        @val = val

    to_String: ->
        '' + @val

    to_Number: ->
        @val

    copy: ->
        new Number @val

    write_to: ( bw, cur_dev ) ->
        bw.write_PT Bn_SI32.num_for_RW # IMPORTANT_TODO
        if @val < 0 or Math.floor( @val ) != @val
            console.error "TODO: signed and float number"
        bw.write_PT @val

    obj_type: ->
        Number

    make_ot_wrapper: ->
        OtWrapperNumber.New this

    @read_from = ( br, usr_id ) ->
        new Number br.read_PT() # IMPORTANT_TODO: may be signed, etc...


Method.methods.self_set_b.add_surdef
    test: ( type_0, type_1 ) -> type_0 == Bn_SI32 and Method.methods.to_Number.has_surdef_for type_1
    func: ( obj, val ) -> obj.rp.val = Method.methods.to_Number.call_1 val
    pert: 2


num_methods =
    is_eqz:   ( obj ) -> new Number( Method.methods.to_Number.call_1( obj ) == 0 )
    is_pos:   ( obj ) -> new Number( Method.methods.to_Number.call_1( obj ) >  0 )
    not:      ( obj ) -> new Number( not Method.methods.to_Number.call_1( obj ) )
    neg:      ( obj ) -> new Number(  -  Method.methods.to_Number.call_1( obj ) )
    is_equ:   ( o_0, o_1 ) -> new Number( Method.methods.to_Number.call_1( o_0 ) == Method.methods.to_Number.call_1( o_1 ) )
    is_inf:   ( o_0, o_1 ) -> new Number( Method.methods.to_Number.call_1( o_0 ) <  Method.methods.to_Number.call_1( o_1 ) )
    is_infeq: ( o_0, o_1 ) -> new Number( Method.methods.to_Number.call_1( o_0 ) <= Method.methods.to_Number.call_1( o_1 ) )
    add:      ( o_0, o_1 ) -> new Number( Method.methods.to_Number.call_1( o_0 ) +  Method.methods.to_Number.call_1( o_1 ) )
    sub:      ( o_0, o_1 ) -> new Number( Method.methods.to_Number.call_1( o_0 ) -  Method.methods.to_Number.call_1( o_1 ) )
    mul:      ( o_0, o_1 ) -> new Number( Method.methods.to_Number.call_1( o_0 ) *  Method.methods.to_Number.call_1( o_1 ) )
    div:      ( o_0, o_1 ) -> new Number( Method.methods.to_Number.call_1( o_0 ) /  Method.methods.to_Number.call_1( o_1 ) )

# unary
for n in [ "is_eqz", "is_pos", "not", "neg" ]
    Method.methods[ n + "_b" ].add_surdef
        test: ( type_0 ) -> Method.methods.to_Number.has_surdef_for type_0
        func: num_methods[ n ]
        pert: 1

# binary
for n in [ "is_equ", "is_inf", "is_infeq", "add", "sub", "mul", "div" ]
    Method.methods[ n + "_bb" ].add_surdef
        test: ( type_0, type_1 ) -> Method.methods.to_Number.has_surdef_for( type_0 ) and Method.methods.to_Number.has_surdef_for( type_1 )
        func: num_methods[ n ]
        pert: 1


OtWrapperNumber = require "./OtWrapperNumber.js"
Number          = require "../Number.js"
