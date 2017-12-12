
//
export default
class PersistentData {
    constructor( db : Db ) {
        this.db = db;

        let req_op = indexedDB.open( "objects", 3 );

        req_op.onerror = ( event ) => {
            console.log( "Impossible to open indexedDB" );
        };

        req_op.onsuccess = ( event ) => {
            this.inst = ( event.target as any ).result as IDBDatabase;

            // load inodes, and patches when it's done
            let transaction = this.inst.transaction( "inodes", "readonly" );
            let inodes      = transaction.objectStore( "inodes" );
            let new_items   = [] as Array<DbItem>;

            inodes.openCursor().onsuccess = ( event ) => {
                let cursor = ( event.target as any ).result as IDBCursorWithValue;
                if ( cursor ) {
                    let inode = Inode.fromString( cursor.key );

                    let br = new BinaryReader( cursor.value.type ), tv = br.read_PT();
                    let va = PatchTypes.readers[ tv ]( br /*, cur_dev, cur_usr*/ );
                    let it = this.db.table.insert( new DbItem( this.db, inode, va ) );
                    it.patch_manager.vector_clock = VectorClock.fromJSON( cursor.value.vector_clock );
                    new_items.push( it );

                    cursor.continue();
                } else {
                    let transaction = this.inst.transaction( "patches", "readonly" );
                    let patches     = transaction.objectStore( "patches" );

                    patches.openCursor().onsuccess = ( event ) => {
                        let cursor = ( event.target as any ).result as IDBCursorWithValue;
                        if ( cursor ) {
                            // get the patch
                            let d = cursor.value;
                            let p = new PM.Patch;
                            if ( d.merged_pm ) {
                                p.patch_id            = PatchId.fromString( d.patch_id );
                                p.date                = d.date;
                                p.merge               = new PM.Merge;
                                p.merge.patch_manager = PM.PatchManager.read_from( new BinaryReader( d.merged_pm ) );
                                p.merge.inode         = Inode.fromString( d.merged_inode );
                            } else {
                                p.patch_id            = PatchId.fromString( d.patch_id );
                                p.date                = d.date;
                                p.usr_id              = UsrId.fromString( d.usr_id );
                                p.data                = d.data;
                            }

                            // get inode
                            let si = cursor.key.indexOf( ":" )
                            if ( si >= 0 ) {
                                let inode = Inode.fromString( cursor.key.slice( 0, si ) );
                                let item  = this.db.table.find( inode );
                                if ( item ) {
                                    item.patch_manager.patches.push( p );
                                }
                            }

                            cursor.continue();
                        } else {
                            for( let item of new_items ) {
                                let merger = Method.methods["new_ot_merger"].call_1( item.patch_manager.variable ) as OtMerger;
                                console.log( item.patch_manager.variable );
                                for( let p of item.patch_manager.patches ) {
                                    console.log( p.data );
                                    if ( p.merge )
                                        throw new Error( "TODO" );
                                    else
                                        merger.new_patch( new BinaryWriter, p.data, p.usr_id );
                                }

                                console.log( item.inode + " " + item.patch_manager.variable );
                                item.patch_manager.variable.sig_change();
                            }
                        }
                    }
                }
            }

            // store the changes
            this.db._store_stuff();
        };

        req_op.onupgradeneeded = (event) => {
            let inst = ( event.target as any ).result as IDBDatabase;
            for( let name of [ "patches", "inodes" ] )
                if ( ! inst.objectStoreNames.contains( name ) )
                    inst.createObjectStore( name );
        };
    }

    get connected() : boolean {
        return this.inst != null;
    }

    store_item( item ) {
        let transaction = this.inst.transaction( [ "patches", "inodes" ], "readwrite" );
        let patches     = transaction.objectStore( "patches" );
        let inodes      = transaction.objectStore( "inodes" );

        item.patch_manager.get_added_or_changed_patches( item.stored, ( p : PM.Patch ) => {
            let key = item.inode + ":" + p.patch_id.toString();

            if ( p.merge ) {
                let pm = new BinaryWriter;
                p.merge.patch_manager.write_to( pm );

                patches.put( {
                    patch_id    : p.patch_id.toString(),
                    date        : p.date,
                    merged_pm   : pm.to_Uint8Array(),
                    merged_inode: p.merge.inode.toString()
                }, key );
            } else {
                patches.put( {
                    patch_id: p.patch_id.toString(),
                    date    : p.date,
                    usr_id  : p.usr_id.toString(),
                    data    : p.data
                }, key );
            }
        } );

        let bw = new BinaryWriter;
        Method.methods["get_patch_type"].call_1( item.patch_manager.variable, bw );

        inodes.put( {
            vector_clock: item.stored.toJSON(),
            type        : bw.to_Uint8Array()
        }, item.inode.toString() );
    }

    inst: IDBDatabase;
    db  : Db;
}

import PM         = require( "../../Variable/Ot/PatchManager" );
import PatchTypes   from "../../Variable/Ot/PatchTypes";
import OtMerger     from "../../Variable/Ot/OtMerger";
import Method       from "../../Variable/Core/Method";
import BinaryWriter from "../../System/BinaryWriter";
import BinaryReader from "../../System/BinaryReader";
import VectorClock  from "../../System/VectorClock";
import PatchId      from "../../System/PatchId";
import Inode        from "../../System/Inode";
import UsrId        from "../../System/UsrId";
import DbItem       from "./DbItem";
import Db           from "../Db";
