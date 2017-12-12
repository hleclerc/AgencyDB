OtWrapperMap = require "./Map/OtWrapperMap.js"
Variable     = require "./Core/Variable.js"
Method       = require "./Core/Method.js"
Rp           = require "./Core/Rp.js"

#
module.exports =
class Map extends Variable
    Variable.make_templ this
    @NativeType = ( window ? global ).Map # shortcut for people willing to import this as 'Array'

    constructor: ( key_type, val_type ) ->
        switch
            when val_type?              then @rp = OtWrapperMap.New key_type, val_type
            when key_type instanceof Rp then @rp = key_type
            else console.error "TODO", key_type, val_type

    set: ( key, val ) ->
        val_type = Method.methods.val_type.call_1 this
        if ( rp = val_type.rp_conv_on val ).o
            Method.methods.self_set_o.call_2 @get( key ), rp.v
        else
            Method.methods.self_set_b.call_2 @get( key ), rp.v
        this

    get: ( key ) -> # if key is undefined, return a native object. Else, return the value for the key, or undefined
        key_type = Method.methods.key_type.call_1 this
        Method.methods.mod_select.call_2 this, key_type.rp_conv_cp key

    del: ( key ) -> # remove item with key `key`
        key_type = Method.methods.key_type.call_1 this
        Method.methods.rem_item.call_2 this, key_type.rp_conv_cp key
