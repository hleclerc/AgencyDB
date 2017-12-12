import DeclTypesForRW from "../Core/DeclTypesForRW";
import Inode          from "../../System/Inode";
import Variable       from "../Core/Variable";
import Method         from "../Core/Method";
import Rp             from "../Core/Rp";

//
export default
class ObjectId_Inode extends Rp {
    static term_value = true;

    constructor( inode = new Inode ) {
        super( ObjectId_Inode );
        this.inode = inode;
    }

    to_Inode() : Inode {
        return this.inode;
    }

    to_String() : string {
        return this.inode.toString();
    }

    make_ot_wrapper() {
        return new OtWrapperAtomic( new ObjectId( this ), new ObjectId );
    }

    write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
        bw.write_PT( DeclTypesForRW.num.ObjectId_Inode );
        bw.write_Inode( this.inode, cur_dev );
    }

    static read_ObjectId_Inode_from( br: BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : ObjectId {
        return new ObjectId( br.read_Inode( cur_dev ) );
    }

    self_glo_to_loc( cur_dev : DevId, cur_usr : UsrId ) {
        this.inode.self_glo_to_loc( cur_dev );
    }

    self_loc_to_glo( cur_dev : DevId, cur_usr : UsrId ) {
        this.inode.self_loc_to_glo( cur_dev );
    }

    copy() : Variable {
        return new ObjectId( this.inode );
    }

    obj_type() {
        return ObjectId;
    }

    inode: Inode;
}
Rp.make_templ( ObjectId_Inode );

DeclTypesForRW.readers[ DeclTypesForRW.num.ObjectId_Inode ] = ObjectId_Inode.read_ObjectId_Inode_from;

Method.methods["self_set_b"].add_surdef( 2, [ ObjectId_Inode, "to_Inode" ], function( obj, sup ) {
    obj.rp.inode = Method.methods["to_Inode"].call_1( sup );
} );

Method.methods["equ_immediate"].add_surdef( 2, [ "to_Inode", "to_Inode" ], function( a, b ) : boolean {
    return Method.methods["to_Inode"].call_1( a ).equ( Method.methods["to_Inode"].call_1( b ) );
} );

Method.methods["self_ot_merge_b"].add_surdef( 2, [ "to_Inode", "to_Inode" ], function( a, b, as_usr : UsrId ) : boolean {
    if ( Db.instances.length == 0 ) throw new Error( "weird (merge without any db instance)" );
    // if ( Db.instances.length > 1 ) throw new Error( "TODO: several db instances" );
    let db = Db.instances[ 0 ];
    let id_0 = Method.methods["to_Inode"].call_1( a );
    let id_1 = Method.methods["to_Inode"].call_1( b );
    if ( id_0.equ( id_1 ) )
        return false;

    let obj_0 = db.bind_by_object_id<Variable>( id_0, Variable );
    let obj_1 = db.load_by_object_id<Variable>( id_1, Variable );
    //console.log( obj_0.rp, obj_1.rp );

    Method.methods["self_ot_merge_o"].call_2( obj_0, obj_1, as_usr );
    db.table.find( id_1 ).link = db.table.find( id_0 );
    return true;
} );

import BinaryWriter     from "../../System/BinaryWriter";
import BinaryReader     from "../../System/BinaryReader";
import UsrId            from "../../System/UsrId";
import DevId            from "../../System/DevId";
import OtWrapperAtomic  from "../Ot/OtWrapperAtomic";
import ObjectId         from "../ObjectId";
import Db               from "../../Db/Db";
