import WithComparison = require( "../../System/WithComparison" );
import VectorClock      from "../../System/VectorClock";

//
export default
class DbItem implements WithComparison.CmpInterface {
    static FLAG_in_db_stuff_to_store  =  1;
    static FLAG_in_db_stuff_to_send   =  2;
    static FLAG_snd_signal_to_tracker =  4;
    static FLAG_ack_signal_to_tracker =  8;
    static FLAG_not_bound             = 16; // not automatically updated

    constructor( db : Db, inode : Inode, variable : Variable, flags = 0 ) {
        this.patch_manager = new PatchManager( variable );
        this.inode         = inode;
        this.db            = db;
        this.flags         = flags;

        db._sig_changed_stuff( this );

        variable.onchange( () => {
            db._sig_changed_stuff( this );
        } );
    }

    inf( inode ) : boolean {
        return this.inode.inf( inode );
    }

    sup( inode ) : boolean {
        return this.inode.sup( inode );
    }

    equ( inode ) : boolean {
        return this.inode.equ( inode );
    }

    make_ptr() : Variable {
        let cl = this.patch_manager.variable.constructor as any;
        return this.patch_manager.variable.is_symbolic() ? this.patch_manager.variable : new cl( new DbItemPtr( this ) );
    }

    send_stuff() {
        if ( this.flags & DbItem.FLAG_not_bound )
            return;

        // say that we are playing with this object (once db is connected)
        let tracker = this.db.tracker_for_inode( this.inode );
        if ( ! ( this.flags & DbItem.FLAG_snd_signal_to_tracker ) && this.db.dev.ext() ) {
            this.flags |= DbItem.FLAG_snd_signal_to_tracker;
            tracker.send_bind_obj( this.inode, this.patch_manager.vector_clock );
        }

        // send changes
        for( let dev of this.patch_manager.connected_devices() ) {
            let ch = this.patch_manager.get_changes( dev, this.db.dev, this.db.usr );
            if ( ch.byteLength ) {
                // console.log( "sending stuff ", this.link );
                tracker.peer_connection_to( dev ).send_changes( this.inode, ch );
            }
        }
    }

    store_stuff() {
        if ( this.flags & DbItem.FLAG_not_bound )
            return;

        // store changes
        this.db.persistent_data.store_item( this );
    }

    get final_item() : DbItem {
        var res = this as DbItem;
        while ( res.link )
            res = res.link;
        return res;
    }

    patch_manager     : PatchManager;                   //
    inode             : Inode;                          //
    db                : Db;                             //
    flags             : number;                         //
    when_loaded       = new Array<( DevId )=> void >(); /** list of functions to be called when loaded */
    link              : DbItem;                         //
    stored            = new VectorClock;                // which patches are stored in the db.persistent_data
}

import PatchManager from "../../Variable/Ot/PatchManager";
import Variable     from "../../Variable/Core/Variable";
import Inode        from "../../System/Inode";
import DbItemPtr    from "./DbItemPtr";
import Db           from "../Db";
