Method = require "./Method.js"

#
module.exports =
class Variable
    @make_templ = ( cl ) ->
        cl.prototype.set = ( val ) ->
            if ( rc = cl.rp_conv_on val ).o
                Method.methods.self_set_o.call_2 this, rc.v
            else
                console.log rc
                Method.methods.self_set_b.call_2 this, rc.v
            this

        cl.prototype.inf_immediate = ( val ) ->
            Method.methods.inf_immediate.call_1 this, cl.rp_conv val

        cl.prototype.sup_immediate = ( val ) ->
            Method.methods.sup_immediate.call_1 this, cl.rp_conv val

        cl.prototype.equ_immediate = ( val ) ->
            Method.methods.equ_immediate.call_1 this, cl.rp_conv val

        # conversion to base Variable type (e.g. String, ...) if necessary
        cl.rp_conv = ( val ) ->
            cl.rp_conv_on( val ).v

        # conversion to base Variable type (e.g. String, ...) if necessary. Return { v:value, o:true if object can be owned (does not have to be copied) by the target procedure }
        cl.rp_conv_cp = ( val ) ->
            if ( rp = cl.rp_conv_on val ).o then rp.v else rp.v.copy()

        # conversion to base Variable type (e.g. String, ...) if necessary. Return { v:value, o:true if object can be owned (does not have to be copied) by the target procedure }
        cl.rp_conv_on = ( val ) ->
            if val instanceof cl then { v: val, o: false } else { v: new cl( val ), o: true }

    # attempt to make a RiotBase Variable type (like a String, ...)
    @rp_conv = ( val ) ->
        Variable.rp_conv_on( val ).v

    # attempt to make a new RiotBase Variable type (like a String, ...)
    @rp_conv_cp = ( val ) ->
        if ( rp = Variable.rp_conv_on val ).o then rp.v else rp.v.copy()

    # attempt to make a RiotBase Variable type (like a String, ...)
    @rp_conv_on: ( val ) ->
        switch
            when val instanceof Variable    then { v : val, o: false }
            when typeof( val ) == "string"  then T = require "../String.js"; { v: new T( val ), o: true }
            when typeof( val ) == "number"  then T = require "../Number.js"; { v: new T( val ), o: true }
            when typeof( val ) == "boolean" then T = require "../Number.js"; { v: new T( val ), o: true }
            when val instanceof Array       then T = require "../Array.js" ; { v: new T( val ), o: true }

    @num_type_map = []

    constructor: ( rp ) ->
        @rp = rp

    copy: ->
        Method.methods.copy.call_1 this

    toString: ->
        Method.methods.to_String.call_1 this

    toNumber: ->
        Method.methods.to_Number.call_1 this

    toArray: ->
        Method.methods.to_Array.call_1 this

    is_neg: ->
        Method.methods.is_neg_b.call_1 this

    is_pos: ->
        Method.methods.is_pos_b.call_1 this

    is_null: ->
        Method.methods.is_eqz_b.call_1 this

    is_non_null: ->
        @is_null().not()

    not: ->
        Method.methods.not_b.call_1 this

    neg: ->
        Method.methods.neg_b.call_1 this

    self_neg: ->
        Method.methods.self_neg.call_1 this

    onchange: ( callback ) -> # return a callback_id
        Method.methods.self_on_change.call_1 this, callback

    sig_change: ( arg )-> # arg is optionnal
        Method.methods.sig_change.call_1 this, arg

    onchange_par: ( par, arg ) -> #
        Method.methods.self_on_change_par.call_1 this, par, arg

    rm_onchange: ( callback_id ) ->
        Method.methods.self_rm_on_change.call_1 this, callback_id

    pull_ot_changes: ( changes ) ->
        Method.methods.pull_ot_changes.call_1 this, changes

    new_ot_merger: ->
        Method.methods.new_ot_merger.call_1 this

    write_to: ( bw, cur_dev ) ->
        Method.methods.write_to.call_1 this, bw, cur_dev
