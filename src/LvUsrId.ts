import Variable    from "./Core/Variable"
import methods     from "./Core/methods"
import Rp          from "./Core/Rp"

import GenSymbol   from "./Symbol/GenSymbol"
import GenUsrId    from "./UsrId/GenUsrId"
import UsrId       from "./System/UsrId"

//
export default
class LvUsrId extends Variable<LvUsrId> {
    static NativeType = UsrId;

    constructor( val = new UsrId() as Rp | LvUsrId | UsrId ) {
        if      ( val instanceof Rp            ) super( val );
        else if ( val instanceof LvUsrId       ) super( methods[ "copy__b" ].call_1( val.rp ) );
        else                                     super( new GenUsrId( val ) );
    }

    static symbol( name: string ): LvUsrId {
        return new LvUsrId( new GenSymbol( LvUsrId, name ) );
    }

    copy() : LvUsrId {
        return new LvUsrId( methods["copy__b"].call_1( this.rp ) );
    }

    get(): UsrId {
        return methods["to_native_type__b"].call_1( this.rp );
    }

    set( nv: UsrId | LvUsrId ) {
        this.rp = nv instanceof UsrId ? methods[ "set__so" ].call_2s( this, new GenUsrId( nv ) ) : methods[ "set__sb" ].call_2s( this, nv.rp );
        return this;
    }

    get val() : UsrId {
        return this.get();
    }

    set val( n : UsrId ) {
        this.set( n );
    }
}

