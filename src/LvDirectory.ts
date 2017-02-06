import Variable, { toLv_array } from "./Core/Variable"
import methods                  from "./Core/methods"
import Rp                       from "./Core/Rp"
            
import RpDirectory              from "./LvDirectory/RpDirectory"
import RpSymbol                from "./Symbol/RpSymbol"
import LvNumber                 from "./LvNumber"

//
export default
class LvDirectory extends Variable<LvDirectory> {
    constructor( val = null as Rp | LvDirectory ) {
        if      ( val instanceof Rp          ) super( val );
        else if ( val instanceof LvDirectory ) super( methods[ "copy__b" ].call_1( val.rp ) );
        else                                   super( LvDirectory.makeRp( val ) );
    }

    static makeRp( val = null ) {
        return new RpDirectory();
    }

    static symbol( name: string ): LvDirectory {
        return new LvDirectory( new RpSymbol( LvDirectory, name ) );
    }

    copy(): LvDirectory {
        return new LvDirectory( methods["copy__b"].call_1( this.rp ) );
    }
}
