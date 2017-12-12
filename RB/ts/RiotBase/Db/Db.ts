import Variable       from "../Variable/Core/Variable";
import Method         from "../Variable/Core/Method";
import Directory      from "../Variable/Directory";
import ObjectId       from "../Variable/ObjectId";
import String         from "../Variable/String";
import _Array         from "../Variable/Array";
import _Map           from "../Variable/Map";

import KnownInodes    from "../System/KnownInodes";
import SortedArray    from "../System/SortedArray";
import Inode          from "../System/Inode";
import DevId          from "../System/DevId";
import UsrId          from "../System/UsrId";
import Map            from "../System/Map";

import PersistentData from "./Core/PersistentData";
import TrackerClient  from "./Core/TrackerClient";
import DbTable        from "./Core/DbTable";
import DbItem         from "./Core/DbItem";

import Cookie         from "../Dom/Core/Cookie";


// Local database.
// May link to an server (specifying an url)
//
export default
class Db {
    // shortcuts
    static Directory = Directory;
    static String    = String;
    static Map       = _Map;

    //
    static instances = new Array<Db>();

    // sudefinitions:
    //   - url, login, password
    //   - login, password (url coming from window.location)
    //   - url
    constructor() {
        this.timeout_for_items_with_stuff_to_send = null;
        this.persistent_data                      = new PersistentData( this );
        this.table                                = new DbTable;
        this.dev                                  = new DevId;
        this.usr                                  = new UsrId;
        this.locid                                = 0;

        this.rootd = this.table.find( KnownInodes.root_dir );
        if ( ! this.rootd )
            this.rootd = this.table.insert( new DbItem( this, KnownInodes.root_dir, new Directory ) );

        Db.instances.push( this );
    }

    // connect to a tracker
    connect( user, pass, cb_login? : ( Db ) => void, cb_logout? : ( Db ) => void ) {
        let ip = "localhost:8676";

        // tracker for the corresponding ip
        let tracker : TrackerClient;
        for( let t of this.trackers ) {
            if ( t.ip == ip ) {
                tracker = t;
                break;
            }
        }
        if ( ! tracker ) {
            tracker = new TrackerClient( ip, this );
            this.trackers.push( tracker );
        }

        //
        tracker.send_auth( user, pass, ( dev, usr, token ) => {
            this._set_dev_and_usr_id( dev, usr, token );
            if ( cb_login )
                cb_login( this );
        }, () => {
            this._logout( tracker );
            if ( cb_logout )
                cb_logout( this );
        } );
    }

    // connect to a tracker
    create_user( user: string, pass: string, email: string, cb : ( string ) => void ) {
        let ip = "localhost:8676";

        // tracker for the corresponding ip
        let tracker : TrackerClient;
        for( let t of this.trackers ) {
            if ( t.ip == ip ) {
                tracker = t;
                break;
            }
        }
        if ( ! tracker ) {
            tracker = new TrackerClient( ip, this );
            this.trackers.push( tracker );
        }

        //
        tracker.send_create_user( user, pass, email, cb );
    }

    get connected() : boolean {
        return this.dev.ext();
    }

    logout() {
        for( let t of this.trackers )
            t.send_logout();
        Cookie.get( "login_tok" );
    }

    bind<T>( url, type ) : T {
        return Method.methods['bind_by_path'].call_1( String.rp_conv( url ), this, type, false );
    }

    load<T>( url, type ) : T {
        return Method.methods['bind_by_path'].call_1( String.rp_conv( url ), this, type, true );
    }

    /** id can be an Variable (with to_Inode capabilities) or directly an Inode */
    bind_by_object_id<T>( id, type ) : T {
        return Method.methods['bind_by_object_id'].call_1( ObjectId.rp_conv( id ), this, type, false );
    }

    load_by_object_id<T>( id, type ) : T {
        return Method.methods['bind_by_object_id'].call_1( ObjectId.rp_conv( id ), this, type, true );
    }

    tracker_for_inode( inode : Inode ) : TrackerClient {
        return this.trackers[ 0 ];
    }

    tracker_for_dev( dev : DevId ) : TrackerClient {
        return this.trackers[ 0 ];
    }

    _set_dev_and_usr_id( dev : DevId, usr : UsrId, token: string ) {
        if ( dev.ext() ) {
            this.token = token;
            this.dev   = dev;
            this.usr   = usr;

            this._send_stuff();
        }
    }

    _logout( tracker: TrackerClient ) {
        // we will have to restart a session if we want to connect
        let i = this.trackers.indexOf( tracker );
        if ( i >= 0 )
            this.trackers.splice( i, 1 );

        this.token = "";
        this.dev   = new DevId;
        this.usr   = new UsrId;
    }

    _make_new_item( object_id : ObjectId, type ) : DbItem {
        let inode = new Inode( new DevId, this.locid += 1 );
        object_id.set( inode );
        return this.table.insert( new DbItem( this, inode, new type ) );
    }

    _make_item_from_inode_request( inode : Inode, load_only_once = false ) : DbItem {
        let sym = new Variable( new Lazyfier( new Variable( new DbItemLazySource( this, inode, load_only_once ) ) ) );
        return this.table.insert( new DbItem( this, inode, sym, load_only_once ? DbItem.FLAG_not_bound : 0 ) );
    }

    _sig_changed_stuff( db_item : DbItem ) {
        if ( ! ( db_item.flags & DbItem.FLAG_in_db_stuff_to_store ) ) {
            db_item.flags |= DbItem.FLAG_in_db_stuff_to_store;
            this.items_with_stuff_to_store.push( db_item );

            // timer
            if ( ! this.timeout_for_items_with_stuff_to_store && this.persistent_data.connected ) {
                this.timeout_for_items_with_stuff_to_store = setTimeout( () => {
                    delete this.timeout_for_items_with_stuff_to_store;
                    this._store_stuff();
                }, 1 );
            }
        }

        if ( ! ( db_item.flags & DbItem.FLAG_in_db_stuff_to_send ) ) {
            db_item.flags |= DbItem.FLAG_in_db_stuff_to_send;
            this.items_with_stuff_to_send.push( db_item );

            // timer
            if ( ! this.timeout_for_items_with_stuff_to_send && this.dev.ext() ) {
                this.timeout_for_items_with_stuff_to_send = setTimeout( () => {
                    delete this.timeout_for_items_with_stuff_to_send;
                    this._send_stuff();
                }, 1 );
            }
        }
    }

    _store_stuff() {
        let cp = this.items_with_stuff_to_store;
        this.items_with_stuff_to_store = [];
        for( let item of cp ) {
            item.flags &= ~ DbItem.FLAG_in_db_stuff_to_store;
            item.store_stuff();
        }
    }

    _send_stuff() {
        let cp = this.items_with_stuff_to_send;
        this.items_with_stuff_to_send = [];
        for( let item of cp ) {
            item.flags &= ~ DbItem.FLAG_in_db_stuff_to_send;
            item.send_stuff();
        }
    }

    timeout_for_items_with_stuff_to_store: number;                     // ref of timer launched to send stuff
    timeout_for_items_with_stuff_to_send : number;                     // ref of timer launched to send stuff
    items_with_stuff_to_store            = [] as Array<DbItem>;        // items that have been modified
    items_with_stuff_to_send             = [] as Array<DbItem>;        // items that have been modified
    trackers                             = [] as Array<TrackerClient>; // connection to known trackers
    table                                : DbTable;                    // inode -> DbItem
    token                                : string;                     //
    dev                                  : DevId;                      // assigned device id
    usr                                  : UsrId;                      // who am I
    rootd                                : DbItem;                     // root directory
    locid                                : number;                     // used for num of Inodes
    persistent_data                      : PersistentData;             // such as the local storage behaves ~ like a peer connection
}


Method.methods['bind_by_path'].add_surdef( 2, [ "to_String" ], function( url : Variable, db : Db, type, load_only_once : boolean ) {
    let srl = url.toString();
    while ( (<any>srl).startsWith( '/' ) )
        srl = srl.substr( 1 );
    while ( (<any>srl).endsWith( '/' ) )
        srl = srl.substr( 0, srl.length - 1 );

    // root dir ?
    if ( srl.length == 0 )
        return db.table.find( KnownInodes.root_dir ).make_ptr();

    // else, look for parent directory and item in it
    let base_dir;
    let isl = srl.lastIndexOf( '/' );
    if ( isl >= 0 ) {
        let dir = srl.substr( 0, isl );
        srl = srl.substr( isl + 1 );
        base_dir = db.load( dir, Directory );
    } else {
        base_dir = db.table.find( KnownInodes.root_dir ).make_ptr();
    }

    let object_id = base_dir.get( srl );
    return load_only_once ? db.load_by_object_id( object_id, type ) : db.bind_by_object_id( object_id, type );
} );

Method.methods['bind_by_object_id'].add_surdef( 2, [ "to_Inode" ], function( object_id : ObjectId, db : Db, type, load_only_once : boolean ) {
    let inode = Method.methods['to_Inode'].call_1( object_id );
    if ( ! inode.num )
        return db._make_new_item( object_id, type ).make_ptr();
    let item = db.table.find( inode );
    if ( ! item ) {
        item = db._make_item_from_inode_request( inode, load_only_once );
        return item.patch_manager.variable; // we return the variable, because the answer will make a DbItemPtr
    }
    return item.make_ptr();
} );

import Lazyfier         from "../Variable/Symbol/Lazyfier";
import VectorClock      from "../System/VectorClock";
import DbItemLazySource from "./DbItemLazySource";
