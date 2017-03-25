import BinaryWriter from "./System/BinaryWriter"
import BinaryReader from "./System/BinaryReader"
import Inode        from "./System/Inode"
import DevId        from "./System/DevId"
import UsrId        from "./System/UsrId"
import methods      from "./Core/methods"
import DbConnToDb   from "./Db/DbConnToDb"
import DbConn       from "./Db/DbConn"
import DbItem       from "./Db/DbItem"

/**
 * 
 */
export default
class Db {
    static CMD = {
        change_item: 0,
    };

    bind<type>( type, id: Inode ): type {
        let res = new type(), db_item = new DbItem( this, res, id );
        res.rp = methods[ "add_patch_manager__s" ].call_1s( res, db_item );
        this.items.set( id.toString(), db_item );
        return res;
    }

    connect( dst_db: Db ) {
        this.connections.push( new DbConnToDb( dst_db, this ) );
    }

    new_Inode(): Inode {
        return new Inode();
    }

    send_changes() {
        if ( this.modified_items.length == 0 )
            return;

        // for each connection, prepare a message and send it
        for( let conn of this.connections ) {
            let bw = new BinaryWriter;
            for( let item of this.modified_items ) {
                const data = item.get_changes( conn.dst_dev_id() );
                if ( data && data.byteLength ) {
                    bw.write_PI8       ( Db.CMD.change_item );
                    bw.write_obj       ( item.id ); //
                    bw.write_Uint8Array( data );
                }
            }
            conn.send_changes( bw.to_Uint8Array() );
        }
        this.modified_items.length = 0;
    }

    recv_changes( msg: Uint8Array, src_dev: DevId, src_usr: UsrId ) {
        let br = new BinaryReader( msg );
        while ( br.size ) {
            const cmd = br.read_PI8();
            switch ( cmd ) {
                case Db.CMD.change_item:
                    const item_id = Inode.read_from( br, src_dev, this.dev_id );
                    const data = br.read_Uint8Array();

                    let item = this.items.get( item_id.toString() );
                    if ( item )
                        item.app_changes( new BinaryReader( data ), src_dev, src_usr );
                    break;
                default: 
                    console.error( "Uknown cmd type" );
                    return;
            }
        }
    }

    add_modified_items( item: DbItem ) {
        this.modified_items.push( item );
    }

    modified_items = new Array<DbItem>();
    connections    = new Array<DbConn>();
    items          = new Map<string,DbItem>();
    dev_id         = null as DevId;
    usr_id         = null as UsrId;
}

 
