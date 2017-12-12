import Method from "../../Variable/Core/Method";
import Rp     from "../../Variable/Core/Rp";
import UsrId  from "../../System/UsrId";
import DbItem from "./DbItem";

//
export default
class DbItemPtr extends Rp {
    constructor( db_item : DbItem ) {
        super( DbItemPtr );

        this.db_item = db_item
    }

    prox_ptr() {
        return this.db_item.final_item.patch_manager.variable;
    }

    prox_ref() {
        return this.db_item.final_item.patch_manager.variable;
    }

    db_item: DbItem;
}
Rp.make_templ( DbItemPtr );

Method.methods["self_ot_merge_o"].add_surdef( 4, [ DbItemPtr, DbItemPtr ], function( a: { rp: DbItemPtr }, b: { rp: DbItemPtr }, as_usr : UsrId ) : boolean {
    return a.rp.db_item.patch_manager.merge( b.rp.db_item.patch_manager, b.rp.db_item.inode, as_usr );
} );
