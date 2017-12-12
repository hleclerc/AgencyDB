import LazySource  from "../Variable/Symbol/LazySource";
import Lazyfier    from "../Variable/Symbol/Lazyfier";
import Sym         from "../Variable/Symbol/Sym";
import Variable    from "../Variable/Core/Variable";
import VectorClock from "../System/VectorClock";
import DevId       from "../System/DevId";
import Inode       from "../System/Inode";
import DbItemPtr   from "./Core/DbItemPtr";
import Db          from "./Db";

class DbItemLazyRequest extends Sym {
    constructor( ls : DbItemLazySource ) {
        super( DbItemLazyRequest );
        this.ls = ls;
    }

    ww_launch() {
        if ( Db.instances.length == 0 ) throw new Error( "TODO" );
        let db = Db.instances[ 0 ];

        // what to do when loaded
        let db_item = db.table.find( this.ls.inode );
        db_item.when_loaded.push( ( src : DevId ) => {
            // console.log( "beg load", db_item.patch_manager.variable.rp_structure() );
            this.ww_set_lazy_value( new Variable( new DbItemPtr( db_item ) ) );
            // console.log( "end load" );
        } );

        // query
        if ( this.ls.only_load )
            db.tracker_for_inode( this.ls.inode ).send_load_obj( this.ls.inode );
        else
            db.tracker_for_inode( this.ls.inode ).send_load_and_bind_obj( this.ls.inode );
    }

    to_String() : string {
        return `DbItemLazyRequest(${this.ls.to_String()})`
    }

    obj_type() {
        return Variable;
    }

    ls : DbItemLazySource;
}
Sym.make_templ( DbItemLazyRequest );

//
export default
class DbItemLazySource extends LazySource {
    constructor( db : Db, inode : Inode, only_load : boolean ) {
        super( DbItemLazySource );
        this.db         = db;
        this.inode      = inode;
        this.only_load  = only_load;
    }

    exec() : Variable {
        return new Variable( new Lazyfier( new Variable( new DbItemLazyRequest( this ) ) ) );
    }

    obj_type() {
        return Variable;
    }

    to_String() : string {
        return `DbItemLazySource(${this.inode},${this.only_load})`
    }

    db       : Db;
    inode    : Inode;
    only_load: boolean;
}
LazySource.make_templ( DbItemLazySource );
