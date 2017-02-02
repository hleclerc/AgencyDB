import Variable, { VarAnc } from "./Core/Variable"
import methods              from "./Core/methods"
import Rp                   from "./Core/Rp"

//
export default
class LvAny extends Variable<LvAny> {
    constructor( val: Rp | VarAnc ) {
        super( val instanceof Rp ? val : val.rp );
    }

    static makeRp( val: string ): Rp {
        return null;
    }

    // equ( usr: LvAny ): LvNumber {
    //     return new LvNumber( methods[ "is_equ__bb" ].call_2( this.rp, usr.rp ) );
    // }

    // copy() : LvAny {
    //     return new LvAny( methods["copy__b"].call_1( this.rp ) );
    // }

    // get(): UsrId {
    //     return methods["to_native_type__b"].call_1( this.rp );
    // }

    // set( nv: UsrId | LvAny ) {
    //     this.rp = nv instanceof UsrId ? methods[ "set__so" ].call_2s( this, new GenUsrId( nv ) ) : methods[ "set__sb" ].call_2s( this, nv.rp );
    //     return this;
    // }

    // get val() : UsrId {
    //     return this.get();
    // }

    // set val( n : UsrId ) {
    //     this.set( n );
    // }
}

