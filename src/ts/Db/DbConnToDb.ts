import DevId        from "../System/DevId"
import UsrId        from "../System/UsrId"
import DbConn       from "./DbConn"
import Db           from "../Db"

export default
class DbConnToDb extends DbConn {
    constructor( dst_db: Db, src_db: Db ) {
        super();
        this.dst_db = dst_db;
        this.src_db = src_db;
    }

    send_changes( changes: Uint8Array ): void {
        this.dst_db.recv_changes( changes, this.src_db.dev_id, this.src_db.usr_id );
    }

    dst_dev_id(): DevId {
        return this.dst_db.dev_id;
    }

    dst_db: Db;
    src_db: Db;
} 
 
