import CmpInterface from "../System/CmpInterface"
import SortedArray  from "../System/SortedArray"
import PatchId      from "../System/PatchId"
import DevId        from "../System/DevId"
import { VarAnc }   from "../Core/Variable"
import methods      from "../Core/methods"
import Select       from "../Core/Select"
import { BN_PT }    from "../Number/Bn"
import Rp           from "../Core/Rp"
import SelectGenMap from "./SelectGenMap"

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
class GenMap extends Rp {
    constructor( key_type: { new( rp?: Rp ): VarAnc; }, val_type: { new( rp?: Rp ): VarAnc; } ) {
        super( GenMap );
        this.key_type = key_type;
        this.val_type = val_type;
    }

    // variable_type__b() {
    //     return LvMap;
    // }

    to_String__b(): string {
        return `{${ this.kmap.items.map( node => node.key.toString() + ":" + node.val.toString() ).join( "," ) }}`;
    }

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

    key_type   : { new( rp?: Rp ): VarAnc; };
    val_type   : { new( rp?: Rp ): VarAnc; };
    loc_id_num = 0;
    kmap       = new SortedArray<Node>();      // key -> node
    imap       = new SortedArray<IdCmpNode>(); // id -> node
} 
Rp.make_templ( GenMap );

// methods[ "add__bb"    ].add_surdef( 10, [ Map, Map ], ( a: Map, b: Map ) => new Map( a.data + b.data ) );
// methods[ "set__sb"    ].add_surdef( 10, [ Map, Map ], ( a: Map, b: Map ) => { a.data = b.data; return a; } );

// methods[ "set__sbb"   ].add_surdef( 10, [ Map, "to_Number__b", "to_Number__b" ], ( a: Map, b: Rp, c: Rp ) => { a.data[ methods[ "to_Number__b" ].call_1( b ) ] = methods[ "to_Number__b" ].call_1( c ); return a; } );

// methods[ "select__bb" ].add_surdef( 10, [ Map, "to_Number__b" ], ( a: Map, b: Rp ) => new BN_FP64( a.data[ methods[ "to_Number__b" ].call_1( b ) ] ) );

// methods["mod_select__bo"].add_surdef( 2, [ GenMap, ( type ) => ! type.symbolic_value ], function( map: GenMap, key: Rp, variable: VarAnc ) {
//     return new map.val_type( new SelectGenMap( variable, key ) );
// } );

methods["select__bb"].add_surdef( 2, [ GenMap, ( type ) => ! type.symbolic_value ], function( map: GenMap, key: Rp ) {
    let node = map.kmap.get( key );
    return node ? node.val.rp : null;
} );

methods["set__sbb"].add_surdef( 2, [ GenMap, ( type ) => ! type.symbolic_value, ( type ) => ! type.symbolic_value ], function( map: GenMap, key: Rp, val: Rp ) {
    let need_set = true, node = map.kmap.get( key, function() {
        //if ( GenMap.can_add_key.ok( map.rp.usr_right() ) ){
            need_set = false;
            return map._create_node( new map.key_type( methods["copy__b"].call_1( key ) ), new map.val_type( methods["copy__b"].call_1( val ) ), false );
        // }
        // return null;
    } );
    if ( need_set && node )
        node.val = methods["set__sb"].call_2s( node.val, val );
    return map;
} );

methods["set__sbo"].add_surdef( 2, [ GenMap, ( type ) => ! type.symbolic_value, ( type ) => ! type.symbolic_value ], function( map: GenMap, key: Rp, val: Rp ) {
    let need_set = true, node = map.kmap.get( key, function() {
        //if ( GenMap.can_add_key.ok( map.rp.usr_right() ) ){
            need_set = false;
            return map._create_node( new map.key_type( methods["copy__b"].call_1( key ) ), new map.val_type( val ), false );
        // }
        // return null;
    } );
    if ( need_set && node )
        node.val = methods["set__so"].call_2s( node.val, val );
    return map;
} );

