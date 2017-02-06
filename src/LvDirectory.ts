import Variable, { toLv_array } from "./Core/Variable"
import methods                  from "./Core/methods"
import Rp                       from "./Core/Rp"            
import RpDirectory              from "./LvDirectory/RpDirectory"
import RpSymbol                 from "./Symbol/RpSymbol"
import Inode                    from "./System/Inode"
import LvString                 from "./LvString"
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

    /** return a proxy on the value */
    get( name: string | LvString ): LvInode {
        return new LvInode( typeof name == "string" ? 
            methods["select_ref__oo"].call_2( this.rp, LvString.makeRp( name ), this ) :
            methods["select_ref__ob"].call_2( this.rp, name.rp, this )
        );
    }

    set( name: string | LvString, inode: Inode | LvInode ) {
        let sup = this.get( name );
        sup.rp = inode instanceof LvInode ?
            methods["set__sb"].call_2s( sup, inode.rp           ) :
            methods["set__so"].call_2s( sup, LvInode.makeRp( inode ) );
        return this;
    }
}
