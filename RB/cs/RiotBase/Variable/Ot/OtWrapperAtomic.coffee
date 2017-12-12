Method       = require "../Core/Method.js"
OtWrapper    = require "./OtWrapper.js"

#
module.exports =
class OtWrapperAtomic extends OtWrapper
    # patch types
    @op_types = OtWrapper.concattr OtWrapper.op_types,
        op_set:
            attrs: "Variable val"
            right: ( flags, val ) -> flags & @can_write.flag
            apply_op: ( aod, val ) ->
                @val = val
                @sig_change()
            undo: ( aod, val ) ->
                console.error "TODO"
        op_set_und:
            attrs: "Variable val, Variable old"
            right: ( flags, val, old ) -> flags & @can_write.flag
            apply_op: ( aod, val, old ) ->
                @op_set aod, val
            undo: ( aod, old ) ->
                console.error "TODO"

    # right types
    @right_types = OtWrapper.right_types.concat " can_read can_write"

    #
    OtWrapper.make_templ this

    init: ( val, usr_id = new UsrId ) ->
        if val not instanceof Variable then error()
        super usr_id
        @val = val
        @old = undefined

    prox_ref: ->
        @val

    freeze_last_changes: ->
        if not @old? or not @val.equ_immediate( @old )
            @op_set.reg_op @last_ops, @val
            @old = @val.copy()

Method.methods.self_set_o.add_surdef
    test: ( type_0, type_1 ) -> type_0 == OtWrapperAtomic and type_1.term_value
    func: ( str, sup ) ->
        if not str.equ_immediate( sup )
            str.rp.val = sup.copy()
            str.rp.sig_change()
    pert: 2

UsrId    = require "../../System/UsrId.js"
Variable = require "../Core/Variable.js"
