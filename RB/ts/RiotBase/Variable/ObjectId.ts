import Variable from "./Core/Variable"

//
export default
class ObjectId extends Variable {
    constructor( val? ) {
        if      ( val instanceof Rp         ) super( val );
        else if ( val instanceof ObjectId   ) super( this.copy().rp );
        else if ( val instanceof Inode      ) super( new ObjectId_Inode( val ) );
        else if ( val == undefined          ) super( new ObjectId_Inode( new Inode ) );
        else throw new Error( "String from a " + val.constructor.name );
    }

    get() : Inode { // to  native type
         return this.toInode();
    }

    copy() : ObjectId {
        return Method.methods["copy"].call_1( this );
    }

    toInode() : Inode {
        return Method.methods["to_Inode"].call_1( this );
    }
}

import ObjectId_Inode from "./ObjectId/ObjectId_Inode";
import Inode          from "../System/Inode";
import Method         from "./Core/Method";
import Rp             from "./Core/Rp";
