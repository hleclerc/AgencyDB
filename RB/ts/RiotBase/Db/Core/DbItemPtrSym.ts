import Method from "../../Variable/Core/Method";
import Rp     from "../../Variable/Core/Rp";
import DbItem from "./DbItem";

//
export default
class DbItemPtrSym extends Rp {
    static unknown_value = true;

    constructor( db_item : DbItem ) {
        super( DbItemPtrSym );

        this.db_item = db_item
    }

    prox_ptr() {
        return this.db_item.patch_manager.variable;
    }

    prox_ref() {
        return this.db_item.patch_manager.variable;
    }

    db_item: DbItem;
}
Rp.make_templ( DbItemPtrSym );
