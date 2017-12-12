Variable = require "./Core/Variable.js"
require "../System/Property.js"

#
module.exports =
class Number extends Variable
    Variable.make_templ this
    @NativeType = ( window ? global ).Number # shortcut for people willing to import this as 'Array'

    constructor: ( val = 0 ) ->
         switch
             when val instanceof Rp          then @rp = val
             when val instanceof Number      then @rp = Method.methods.copy.call_1 val
             when typeof( val ) == "number"  then @rp = Bn_SI32.New val
             when typeof( val ) == "boolean" then @rp = Bn_SI32.New val
             else console.error "TODO Number from", val

    @property 'val',
        get: -> @toNumber()
        set: ( val ) -> @set val

    self_add: ( val ) -> Method.methods.self_add_b.call_2 this, Number.rp_conv val

    add:      ( val ) -> Method.methods.add_bb.call_2 this, Number.rp_conv val
    sub:      ( val ) -> Method.methods.sub_bb.call_2 this, Number.rp_conv val
    mul:      ( val ) -> Method.methods.mul_bb.call_2 this, Number.rp_conv val
    div:      ( val ) -> Method.methods.div_bb.call_2 this, Number.rp_conv val

    is_equ:   ( val ) -> Method.methods.is_equ_bb.call_2 this, Number.rp_conv val
    is_neq:   ( val ) -> @is_equ( val ).not()
    is_inf:   ( val ) -> Method.methods.is_inf_bb.call_2 this, Number.rp_conv val
    is_infeq: ( val ) -> Method.methods.is_infeq_bb.call_2 this, Number.rp_conv val
    is_sup:   ( val ) -> @is_infeq( val ).not()
    is_supeq: ( val ) -> @is_inf( val ).not()



Bn_SI32  = require "./Number/Bn_SI32.js"
Method   = require "./Core/Method.js"
Rp       = require "./Core/Rp.js"
