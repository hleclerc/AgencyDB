import Variable, { toLv_array } from "../Core/Variable"
import methods                  from "../Core/methods"
import Rp                       from "../Core/Rp"

import RpSymbol                 from "../Symbol/RpSymbol"

let corr = new Map<any,any>();

interface ExAtomic<T> {
    js_repr( prec: number ): string;
    to_Boolean(): boolean;
    copy(): T;
}

export default
function LvAtomic<T extends ExAtomic<T>>( type: { new(): T; } ) {
    class RpAtomic extends Rp {
        constructor( data: T ) {
            super( RpAtomic );
            this.data = data;
        }

        variable_type__b() {
            return LvClass;
        }

        to_String__b(): string {
            return this.data.toString();
        }

        js_repr__b( prec: number ): string {
            return this.data.js_repr( prec );
        }

        copy__b(): Rp {
            return new RpAtomic( this.data.copy() );
        }

        to_Boolean__b(): boolean {
            return this.data.to_Boolean();
        }

        data: T;
    } 
    Rp.make_templ( RpAtomic );

    methods[ "set__sb" ].add_surdef( 10, [ RpAtomic, RpAtomic ], ( a: RpAtomic, b: RpAtomic ) => { a.data = b.data.copy(); return a; } );

    class LvClass extends Variable<LvClass> {
        static NativeType = type;

        constructor( val = null as Rp | LvClass | T ) {
            if      ( val instanceof Rp      ) super( val );
            else if ( val instanceof LvClass ) super( methods[ "copy__b" ].call_1( val.rp ) );
            else                               super( LvClass.makeRp( val as T ) );
        }

        static makeRp( val: T ) {
            return new RpAtomic( val );
        }

        static symbol( name: string ): LvClass {
            return new LvClass( new RpSymbol( LvClass, name ) );
        }

        copy() : LvClass {
            return new LvClass( methods["copy__b"].call_1( this.rp ) );
        }

        get(): T {
            return methods["to_native_type__b"].call_1( this.rp );
        }

        set( nv: T | LvClass ) {
            this.rp = nv instanceof type ? methods[ "set__so" ].call_2s( this, new RpAtomic( nv ) ) : methods[ "set__sb" ].call_2s( this, nv.rp );
            return this;
        }

        get val() : T {
            return this.get();
        }

        set val( n : T ) {
            this.set( n );
        }
    }
    toLv_array.push( val =>
        val instanceof type ? new LvClass( val ) : null
    );
    return LvClass;
} 

