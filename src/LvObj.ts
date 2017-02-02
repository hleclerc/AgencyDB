import Variable    from "./Core/Variable"
import methods     from "./Core/methods"
import Rp          from "./Core/Rp"

import GenSymbol   from "./Symbol/GenSymbol"
import GenObj      from "./LvObj/GenObj"

/**
 * To store generic "objects": a kind of json that can store lists, maps, UsrId... with pointers, versionning, ...
 * 
 */
export default
class LvObj extends Variable<LvObj> {
    static NativeType = Number;

    constructor( val = null as Rp ) {
        if ( val instanceof Rp ) super( val );
        else                     super( LvObj.makeRp( val ) );
    }

    static makeRp( val ) {
        return new GenObj;
    }

    static symbol( name: string ): LvObj {
        return new LvObj( new GenSymbol( LvObj, name ) );
    }

    copy() : LvObj {
        return new LvObj( methods["copy__b"].call_1( this.rp ) );
    }

    // get(): number {
    //     return this.toNumber();
    // }

    // set( nv: number | LvNumber ) {
    //     this.rp = typeof nv == 'number' ? methods[ "set__so" ].call_2s( this, new BN_FP64( nv ) ) : methods[ "set__sb" ].call_2s( this, ( nv as LvNumber ).rp );
    //     return this;
    // }

    // get val() : number {
    //     return this.get();
    // }

    // set val( n : number ) {
    //     this.set( n );
    // }
}

