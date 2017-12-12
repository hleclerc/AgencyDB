import SortedArray from "../../System/SortedArray";
import DbItem      from "./DbItem";

//
export default
class DbTable {
    constructor() {
        this.data = new SortedArray<DbItem>(); // Inode -> DbItem. SortedArray is certainly not a perfect choice
    }

    insert( db_item ) : DbItem {
        this.data.set( db_item.inode, db_item );
        return db_item;
    }

    find( inode ) : DbItem {
        return this.data.get( inode );
    }

    data: SortedArray<DbItem> // Inode -> DbItem. SortedArray is certainly not a perfect choice
}
