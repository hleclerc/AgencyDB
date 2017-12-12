OtWrapper    = require "../Ot/OtWrapper.js"
Method       = require "../Core/Method.js"

#
module.exports =
class OtWrapperNumber extends OtWrapper
    # patch types
    @op_types = OtWrapper.concattr OtWrapper.op_types,
        op_set:
            attrs: "Number nv"
            right: ( flags, nv ) -> if nv.sup_immediate( @val ) then flags & @can_increase.flag else nv.equ_immediate( @val ) or ( flags & @can_decrease.flag )
            apply_op: ( aod, nv ) ->
                Method.methods.self_set_b.call_2 @val, nv
                @sig_change()
            undo: ( aod, nv ) ->
                console.error "TODO"
        op_set_und:
            attrs: "Number nv, Number ov"
            right: ( flags, nv, ov ) -> if nv.sup_immediate( @val ) then flags & @can_increase.flag else nv.equ_immediate( @val ) or ( flags & @can_decrease.flag )
            apply_op: ( aod, nv, ov ) ->
                Method.methods.self_set_b.call_2 @val, nv
                @sig_change()
            undo: ( aod, nv, ov ) ->
                console.error "TODO"
        op_add:
            attrs: "Number nv*"
            right: ( flags, nv ) -> if nv.is_pos().toNumber() then flags & @can_increase.flag else nv.is_null().toNumber() or ( flags & @can_decrease.flag )
            apply_op: ( aod, nv ) ->
                Method.methods.self_add_b.call_2 @val, nv
                @sig_change()
            undo: ( aod, nv ) ->
                console.error "TODO"

    # right types
    @right_types = OtWrapper.right_types.concat " can_read can_increase can_decrease"

    #
    OtWrapper.make_templ this

    init: ( rp ) ->
        super()
        @val = new Number rp

        # first patch
        if @val.is_non_null().toNumber()
            @op_set.reg_op @last_ops, @val

    prox_ref: ->
        @val

    _self_set: ( nv, usr_id = @usr_id ) ->
        if not @val.equ_immediate( nv ) and @right_for_op_set_und usr_id, nv
            @op_set_und.reg_op @last_ops, nv, @val
            @op_set_und.apply_op.call this, { usr: usr_id, type: 0 }, nv, @val
            true

    _self_add: ( nv, usr_id = @usr_id ) ->
        if nv.is_non_null().toNumber() and @right_for_op_add usr_id, nv
            @op_add.reg_op @last_ops, nv
            @op_add.apply_op.call this, { usr: usr_id, type: 0 }, nv
            true


Method.methods.self_set_o.add_surdef
    test: ( type_0, type_1 ) -> type_0 == OtWrapperNumber
    func: ( num, sup ) -> num.rp._self_set sup
    pert: 2

Method.methods.self_add_b.add_surdef
    test: ( type_0, type_1 ) -> type_0 == OtWrapperNumber
    func: ( num, sup ) -> num.rp._self_add sup
    pert: 2

Method.methods.self_sub_b.add_surdef
    test: ( type_0, type_1 ) -> type_0 == OtWrapperNumber
    func: ( num, sup ) -> num.rp._self_add sup.neg()
    pert: 2

Variable = require "../Core/Variable.js"
Number   = require "../Number.js"
