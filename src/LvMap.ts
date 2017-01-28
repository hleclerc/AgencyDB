import Variable, { VarAnc } from "./Core/Variable"
import methods              from "./Core/methods"
import Select               from "./Core/Select"
import Rp                   from "./Core/Rp"
           
import GenSymbol            from "./Symbol/GenSymbol"
import GenSymbolLvMap       from "./Map/GenSymbolLvMap"
import GenMap               from "./Map/GenMap"
import { BN_PT }            from "./Number/Bn"
import LvNumber             from "./LvNumber"

/** helper for typescript to make new LvMaps without repeating K and V */
export function new_LvMap<K extends VarAnc,V extends VarAnc>( k: { new(): K; }, v: { new(): V; } ): LvMap<K,V> {
    return new LvMap<K,V>( k, v );
}

//
export default
class LvMap<K extends VarAnc,V extends VarAnc> extends Variable<LvMap<K,V>> {
    static NativeType = Map;

    constructor( k: { new(): K; }, v: { new(): V; } );
    constructor( v: Rp );

    constructor( k: Rp | { new(): K; }, v?: { new(): K; } ) { //  | LvMap | Map | string | Array<number>
        if      ( k instanceof Rp ) super( k );
        else                        super( new GenMap( k, v ) );
    }

    static symbol<K extends VarAnc,V extends VarAnc>( k: { new(): K; }, v: { new(): V; }, name: string ): LvMap<K,V> {
        return new LvMap<K,V>( new GenSymbolLvMap( k, v, name ) );;
    }

    copy(): LvMap<K,V> {
        return new LvMap<K,V>( methods[ "copy__b" ].call_1( this.rp ) );
    }

    get size(): LvNumber {
        return new LvNumber( methods[ "get_size__bo" ].call_1( this.rp ) );
    }

    // set( key: K, val: V ): LvMap<K,V> {
    //     methods["set__sb"].call_2s( this.get( key ), val.rp );
    //     // typeof from == 'number' ? (
    //     //     typeof length == 'number' ?
    //     //         methods["slice__boo"].call_3s( this, new BN_PT( from ), new BN_PT( from + length )                                ) :
    //     //         methods["slice__bob"].call_3s( this, new BN_PT( from ), methods["add__ob"].call_2( new BN_PT( from ), length.rp ) )
    //     // ) : (
    //     //     typeof length == 'number' ?
    //     //         methods["slice__boo"].call_3s( this, from.rp          , methods["add__ob"].call_2( from.rp, new BN_PT( length ) ) ) :
    //     //         methods["slice__bob"].call_3s( this, from.rp          , methods["add__ob"].call_2( from.rp, length.rp )           )
    //     // );

    //     return this;
    //     // if ( val == null )
    //     //     return Variable.prototype.set.call( this, key );
    //     // let rp = Variable.rp_conv_on( val );
    //     // return rp.o ? Method.methods["self_set_o"].call_2( this.get( key ), rp.v ) : Method.methods["self_set_b"].call_2( this.get( key ), rp.v );
    // }

    /** return a proxy on the value */
    get( key ): V {
        const val_type = methods["val_type__b"].call_1( this.rp );
        return new val_type( new Select( this, key instanceof VarAnc ? 
            methods["copy__b"].call_1( key.rp ) : 
            methods["key_type__b"].call_1( this.rp ).make_Rp( key )
        ) );
    }

    /**  */
    set( key, val ): LvMap<K,V> {
        let sup = this.get( key );
        sup.rp = val instanceof VarAnc ?
            methods["set__sb"].call_2s( sup, val.rp ) :
            methods["set__so"].call_2s( sup, methods["val_type__b"].call_1( this.rp ).make_Rp( val ) )
        // this.rp = key instanceof VarAnc ? (
        //     val instanceof VarAnc ?
        //         methods["set_val__sbb"].call_3s( this, key.rp                                                 , val.rp ) :
        //         methods["set_val__sbo"].call_3s( this, key.rp                                                 , methods["val_type__b"].call_1( this.rp ).make_Rp( val ) )
        // ) : (
        //     val instanceof VarAnc ?
        //         methods["set_val__sob"].call_3s( this, methods["key_type__b"].call_1( this.rp ).make_Rp( key ), val.rp                                                  ) :
        //         methods["set_val__soo"].call_3s( this, methods["key_type__b"].call_1( this.rp ).make_Rp( key ), methods["val_type__b"].call_1( this.rp ).make_Rp( val ) )
        // );
        return this;
    }

    // delete( key ) { // remove item with key `key`
    //     methods["rem_item__so"].call_2( this, Variable.rp_conv_cp( key ) );
    // }

    // /** return value at position `pos` (not to be modified) */
    // at( pos: number | LvNumber ): LvNumber {
    //     return new LvNumber( typeof pos == 'number' ? methods[ "select__bo" ].call_2( this.rp, new BN_FP64( pos ) ) : methods[ "select__bb" ].call_2( this.rp, ( pos as LvNumber ).rp ) );
    // }

    // /** return a proxy that gives charAt if read, but it also enable writes */
    // select( index: number | LvNumber ): LvNumber {
    //     return new LvNumber( new Select( this, typeof index == 'number' ? new BN_FP64( index ) : methods[ "copy__b" ].call_1( ( index as LvNumber ).rp ) ) );
    // }

    // clear() {
    //     this.remove( 0, this.length.toNumber() );
    //     return this;
    // }
}
