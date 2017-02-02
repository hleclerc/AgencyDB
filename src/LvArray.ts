import Variable, { VarAnc } from "./Core/Variable"
import methods              from "./Core/methods"
import Rp                   from "./Core/Rp"
           
import GenSymbol            from "./Symbol/GenSymbol"
import GenArray             from "./LvArray/GenArray"
import { BN_PT }            from "./LvNumber/Bn"
import LvNumber             from "./LvNumber"


/** nasty stuff for typescipt: the actual content is defined inside LvArray_fact, which is inaccessible from here */
export
interface LvArray<V> extends Variable<LvArray<V>> {
    copy(): LvArray<V>;
    get( key: number | LvNumber ): V;                  /** return a proxy on the value */
    set( key: number | LvNumber, val: V ): LvArray<V>; /**  */
    push( val ): LvArray<V>;                           /** append an item (at the end) */
    length: LvNumber;
    size: LvNumber;  /** same thing than length (size is here to support generic code) */
}

/** */
interface LvArray_type<V> {
    new( val?: Rp ): LvArray<V>;
    makeRp( any ): Rp;
    symbol( name: string ): LvArray<V>;
}

/** v => _LvArray<v> */
const LvArray_map = new Map<any,any>();

/** */
export default function LvArray_func<V extends VarAnc,U>( v: { new(): V; makeRp: ( val: any ) => Rp; } ): LvArray_type<V> {
    let m = LvArray_map.get( v );
    if ( ! m ) 
        LvArray_map.set( v, m = LvArray_fact( v ) );
    return m;
}

//
function LvArray_fact<V extends VarAnc>( v: { new( rp?: Rp ): V; makeRp: ( any ) => Rp; } ) {
    class _LvArray extends Variable<_LvArray> implements LvArray<V> {
        static use_get_for_select = true; /** used for code generation. TODO: something that depends on Rp */
        static NativeType = Array;
        static val_type = v;

        constructor( val: Rp ) { //  | LvArray | Map | string | Array<number>
            if ( val instanceof Rp ) super( val );
            else                     super( new GenArray( v ) );
        }

        static symbol( name: string ): _LvArray {
            return new _LvArray( new GenSymbol( _LvArray, name ) );
        }

        copy(): _LvArray {
            return new _LvArray( methods[ "copy__b" ].call_1( this.rp ) );
        }

        get length(): LvNumber {
            return new LvNumber( methods[ "get_size__b" ].call_1( this.rp ) );
        }

        get size(): LvNumber {
            return new LvNumber( methods[ "get_size__b" ].call_1( this.rp ) );
        }

        get( key: number | LvNumber ): V {
            return new v( typeof key == "number" ? 
                methods["select_ref__oo"].call_2( this.rp, LvNumber.makeRp( key ), this ) :
                methods["select_ref__ob"].call_2( this.rp, key.rp, this )
            );
        }
       
        set( key: number | LvNumber, val ): _LvArray {
            let sup = this.get( key );
            sup.rp = val instanceof VarAnc ?
                methods["set__sb"].call_2s( sup, val.rp           ) :
                methods["set__so"].call_2s( sup, v.makeRp( val ) );
            return this;
        }

        push( val ): _LvArray {
            this.rp = val instanceof VarAnc ?
                methods["push__sb"].call_2s( this, val.rp           ) :
                methods["push__so"].call_2s( this, v.makeRp( val ) );
            return this;
        }
    }
    return _LvArray;
}
