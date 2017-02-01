import Variable, { VarAnc } from "./Core/Variable"
import methods              from "./Core/methods"
import Rp                   from "./Core/Rp"
           
import GenSymbol            from "./Symbol/GenSymbol"
import GenMap               from "./LvMap/GenMap"
import { BN_PT }            from "./LvNumber/Bn"
import LvNumber             from "./LvNumber"


/** nasty stuff for typescipt: the actual content is defined inside LvMap_fact, which is inaccessible from here */
interface LvMap<K,V> extends Variable<LvMap<K,V>> {
    copy(): LvMap<K,V>;
    get( key ): V;
    set( key, val ): LvMap<K,V>;
    size: LvNumber;
}

/** */
interface LvMap_type<K,V> {
    new( val?: Rp ): LvMap<K,V>;
    make_Rp( any ): Rp;
    symbol( name: string ): LvMap<K,V>;
}

/** k, v => _LvMap<k,v> */
const LvMap_map = new Map<any,Map<any,any>>();

/** */
export default function LvMap_func<K extends VarAnc,V extends VarAnc,U>( k: { new(): K; make_Rp: ( any ) => Rp; }, v: { new(): V; make_Rp: ( any ) => Rp; } ): LvMap_type<K,V> {
    let m = LvMap_map.get( k );
    if ( ! m ) 
        LvMap_map.set( k, m = new Map<any,any>() );
    let n = m.get( v );
    if ( ! n )
        m.set( v, n = LvMap_fact( k, v ) );
    return n;
}

/** helper for typescript to make new LvMaps without repeating K and V */
// export function new_LvMap<K extends VarAnc,V extends VarAnc>( k: { new(): K; }, v: { new(): V; } ): LvMap<K,V> {
//     return new LvMap<K,V>( k, v );
// }

//
function LvMap_fact<K extends VarAnc,V extends VarAnc>( k: { new(): K; make_Rp: ( any ) => Rp; }, v: { new( rp?: Rp ): V; make_Rp: ( any ) => Rp; } ) {
    class _LvMap extends Variable<_LvMap> implements LvMap<K,V> {
        static use_get_for_select = true; /** used for code generation. TODO: something that depends on Rp */
        static NativeType = Map;
        static key_type = k;
        static val_type = v;

        constructor( val: Rp ) { //  | LvMap | Map | string | Array<number>
            if ( val instanceof Rp ) super( val );
            else                     super( new GenMap( k, v ) );
        }

        static symbol( name: string ): _LvMap {
            return new _LvMap( new GenSymbol( _LvMap, name ) );
        }

        copy(): _LvMap {
            return new _LvMap( methods[ "copy__b" ].call_1( this.rp ) );
        }

        get size(): LvNumber {
            return new LvNumber( methods[ "get_size__b" ].call_1( this.rp ) );
        }

        /** return a proxy on the value */
        get( key ): V {
            return new v( key instanceof VarAnc ? 
                methods["select_ref__ob"].call_2( this.rp, key.rp, this ) :
                methods["select_ref__oo"].call_2( this.rp, k.make_Rp( key ), this )
            );
        }

        /**  */
        set( key, val ): _LvMap {
            let sup = this.get( key );
            sup.rp = val instanceof v ?
                methods["set__sb"].call_2s( sup, val.rp           ) :
                methods["set__so"].call_2s( sup, v.make_Rp( val ) );
            // this.rp = key instanceof VarAnc ? (
            //     val instanceof VarAnc ?
            //         methods["set_val__sbb"].call_3s( this, key.rp                                                 , val.rp ) :
            //         methods["set_val__sbo"].call_3s( this, key.rp                                                 , v.make_Rp( val ) )
            // ) : (
            //     val instanceof VarAnc ?
            //         methods["set_val__sob"].call_3s( this, methods["key_type__b"].call_1( this.rp ).make_Rp( key ), val.rp                                                  ) :
            //         methods["set_val__soo"].call_3s( this, methods["key_type__b"].call_1( this.rp ).make_Rp( key ), v.make_Rp( val ) )
            // );
            return this;
        }

        // delete( key ) { // remove item with key `key`
        //     methods["rem_item__so"].call_2( this, Variable.rp_conv_cp( key ) );
        // }

        // clear() {
        //     this.remove( 0, this.length.toNumber() );
        //     return this;
        // }
    }
    return _LvMap;
}