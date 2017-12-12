import ObjectId from "./ObjectId";
import String   from "./String";
import Map      from "./Map";

function new_val_Directory() {
    return new ObjectId;
}

//
export default
class Directory extends Map<String,ObjectId> {
    constructor( a = {} ) {
        super( a, new_val_Directory );
    }
}
