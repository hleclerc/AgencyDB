//// nsmake ifndef NO_OPERATIONAL_TRANSFORM
//// nsmake global alias ./RpMapOperations.ts ./(GenRpMapOperations.ts).ts
import OtWrapperWithRightFlags from "../Core/OtWrapperWithRightFlags"
import RpMapOperations        from "./RpMapOperations"
//// nsmake endif // NO_OPERATIONAL_TRANSFORM

//// nsmake uncomment_ifdef NO_OPERATIONAL_TRANSFORM
// import OtWrapperWithRightFlags from "../Core/Rp"
//// nsmake endif // NO_OPERATIONAL_TRANSFORM

import BinaryWriter            from "../System/BinaryWriter"
import BinaryReader            from "../System/BinaryReader"
import CmpInterface            from "../System/CmpInterface"
import SortedArray             from "../System/SortedArray"
import PatchId                 from "../System/PatchId"
import UsrId                   from "../System/UsrId"
import DevId                   from "../System/DevId"
import { VarAnc }              from "../Core/Variable"
import methods                 from "../Core/methods"
import RpList                  from "../Core/RpList"
import Rp                      from "../Core/Rp"
import { BN_PT }               from "../LvNumber/Bn"

//
class Node implements CmpInterface {
    constructor( key: VarAnc, val: VarAnc, id: PatchId ) { this.key = key; this.val = val; this.id  = id; }
    inf( key: Rp ): boolean { return methods[ "inf_immediate__bb" ].call_2( this.key.rp, key ); }
    sup( key: Rp ): boolean { return methods[ "sup_immediate__bb" ].call_2( this.key.rp, key ); }

    key: VarAnc;
    val: VarAnc;
    id : PatchId;
}

class IdCmpNode implements CmpInterface {
    constructor( node: Node ) { this.node = node; }
    inf( id: PatchId ): boolean { return this.node.id.inf( id ); }
    sup( id: PatchId ) : boolean { return this.node.id.sup( id ); }

    node: Node;
}

/**
 * Already an OtWrapper
 * 
 * Contains a list of Node with { key, val, id }, sorted by key (which can't be achieved using Map because cmp surdefinition is not possible)
 * and a list a Node, sorted by id (for patch operations)
 */
export default 
class RpMap extends OtWrapperWithRightFlags {
    constructor( key_type: { new( rp?: Rp ): VarAnc; }, val_type: { new( rp?: Rp ): VarAnc; } ) {
        //// nsmake ifndef NO_OPERATIONAL_TRANSFORM
        super( RpMap, new UsrId );
        //// nsmake endif
        //// nsmake uncomment_ifdef NO_OPERATIONAL_TRANSFORM
        // super( RpMap );
        //// nsmake endif
        this.key_type = key_type;
        this.val_type = val_type;
    }

    // variable_type__b() {
    //     return LvMap;
    // }

    to_String__b(): string {
        return `{${ this.kmap.items.map( node => node.key.toString() + ":" + node.val.toString() ).join( "," ) }}`;
    }

    // copy__b(): Rp {
    //     let res = new RpMap( this.key_type, this.val_type );
    //     res.loc_id_num = this.loc_id_num;
    //     res.kmap       = this.kmap.copy();
    //     res.imap       = this.imap.copy();
    //     return res;
    // }

    // js_repr__b( prec: number ): string {
    //     return `Buffer.from("${ this.data.toString() })")`;
    // }

    // copy__b(): Rp {
    //     return new Map( Buffer.from( this.data ) );
    // }

    // to_Boolean__b(): boolean {
    //     return Boolean( this.data );
    // }

    // get_size__b(): LvNumber {
    //     return new LvNumber( this.data.byteLength );
    // }

    new_val() {
        return new this.val_type;
    }

    key_type__b() {
        return this.key_type;
    }

    val_type__b() {
        return this.val_type;
    }

    //// nsmake ifndef NO_OPERATIONAL_TRANSFORM
    new_patch( res: BinaryWriter, msg: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
        RpMapOperations.new_patch( this, res, msg, as_usr, cq_unk, null, src_dev, src_usr, cur_dev, cur_usr );
    }

    undo_patch( data: BinaryReader, as_usr: UsrId ) {
        RpMapOperations.undo_patch( this, data, as_usr );
    }

    get_possible_rights__b(): Array<string> {
        return RpMapOperations.get_possible_rights__b();
    }

    add_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr = new UsrId ) {
        this.sig_change( RpMapOperations.tar.AddUsrRight( this, as_usr, usr, this._right_flags_from_right_type( right_types ) ) );
        return this;
    }

    rem_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr = new UsrId ) {
        this.sig_change( RpMapOperations.tar.RemUsrRight( this, as_usr, usr, this._right_flags_from_right_type( right_types ) ) );
        return this;
    }
    //// nsmake endif

    _create_node( key: VarAnc, val: VarAnc, insert_in_kmap = true, insert_in_imap = true ) : Node {
        let id   = new PatchId( new DevId, this.loc_id_num += 1 );
        let node = new Node( key, val, id );
        // this.new_nodes.push( node );

        // let va = new Variable( this );
        // key.onchange_par( va, node );
        // val.onchange_par( va, node );

        if ( insert_in_imap ) this.imap.set( id , new IdCmpNode( node ) );
        if ( insert_in_kmap ) this.kmap.set( key, node );
        return node;
    }

    can_add_key(): boolean {
        return true;
    }

    key_type   : { new( rp?: Rp ): VarAnc; };
    val_type   : { new( rp?: Rp ): VarAnc; };
    loc_id_num = 0;
    kmap       = new SortedArray<Node>();      // key -> node
    imap       = new SortedArray<IdCmpNode>(); // id -> node
} 
Rp.make_templ( RpMap );

methods["select__bb"].add_surdef( 3, [ RpMap, RpList ], function( map: RpMap, keys: RpList ) {
    const node = map.kmap.get( keys.cur );
    return node ? methods["select__bb"].call_2( node.val.rp, keys.nxt ) : null; // TODO: wrapped null
} );
methods["select__bb"].add_surdef( 2, [ RpMap, () => true ], function( map: RpMap, key: Rp ) {
    const node = map.kmap.get( key );
    return node ? node.val.rp : null; // TODO: wrapped null
} );

methods["set__sbo"].add_surdef( 3, [ RpMap, RpList, type => ! type.symbolic_value ], function( map: RpMap, keys: RpList, val: Rp ) {
    let node = map.kmap.get( keys.cur, function() {
        if ( map.can_add_key() )
            return map._create_node( new map.key_type( methods["copy__b"].call_1( keys.cur ) ), new map.val_type(), false );
        return null;
    } );
    if ( node )
        node.val.rp = methods["set__sbo"].call_3s( node.val, keys.nxt, val );
    return map;
} );
methods["set__sbo"].add_surdef( 2, [ RpMap, type => ! type.symbolic_value, type => ! type.symbolic_value ], function( map: RpMap, key: Rp, val: Rp ) {
    let need_set = true, node = map.kmap.get( key, function() {
        if ( map.can_add_key() ) {
            need_set = false;
            return map._create_node( new map.key_type( methods["copy__b"].call_1( key ) ), new map.val_type( val ), false );
        }
        return null;
    } );
    if ( need_set && node )
        node.val.rp = methods["set__so"].call_2s( node.val, val );
    return map;
} );

