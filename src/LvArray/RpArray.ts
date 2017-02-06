import { VarAnc }      from "../Core/Variable"
import methods         from "../Core/methods"
import RpList          from "../Core/RpList"
import { BN_PT }       from "../LvNumber/Bn"
import Rp              from "../Core/Rp"

/**
 * Already an OtWrapper
 * 
 * Contains a list of Node with { key, val, id }, sorted by key (which can't be achieved using Map because cmp surdefinition is not possible)
 * and a list a Node, sorted by id (for patch operations)
 */
export default 
class RpArray extends Rp {
    constructor( val_type: { new( rp?: Rp ): VarAnc; } ) {
        super( RpArray );
        this.val_type = val_type;
    }

    // variable_type__b() {
    //     return LvMap;
    // }

    to_String__b(): string {
        return `[${ this.data.map( item => item.toString() ).join( "," ) }]`;
    }

    // copy__b(): Rp {
    //     let res = new RpArray( this.key_type, this.val_type );
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

    val_type__b() {
        return this.val_type;
    }

    _push( val: Rp ) {
        this.data.push( new VarAnc( val ) );
    }

    val_type: { new( rp?: Rp ): VarAnc; };
    data    = new Array<VarAnc>();
} 
Rp.make_templ( RpArray );

methods["push__so"].add_surdef( 1, [ RpArray, type => ! type.symbolic_value ], function( lst: RpArray, val: Rp ) {
    lst._push( val );
    return lst;
} );

// methods["select__bb"].add_surdef( 3, [ RpArray, RpList ], function( map: RpArray, keys: RpList ) {
//     const node = map.kmap.get( keys.cur );
//     return node ? methods["select__bb"].call_2( node.val.rp, keys.nxt ) : null; // TODO: wrapped null
// } );
// methods["select__bb"].add_surdef( 2, [ RpArray, () => true ], function( map: RpArray, key: Rp ) {
//     const node = map.kmap.get( key );
//     return node ? node.val.rp : null; // TODO: wrapped null
// } );

// methods["set__sbo"].add_surdef( 3, [ RpArray, RpList, type => ! type.symbolic_value ], function( map: RpArray, keys: RpList, val: Rp ) {
//     let node = map.kmap.get( keys.cur, function() {
//         if ( map.can_add_key() )
//             return map._create_node( new map.key_type( methods["copy__b"].call_1( keys.cur ) ), new map.val_type(), false );
//         return null;
//     } );
//     if ( node )
//         node.val.rp = methods["set__sbo"].call_3s( node.val, keys.nxt, val );
//     return map;
// } );
// methods["set__sbo"].add_surdef( 2, [ RpArray, type => ! type.symbolic_value, type => ! type.symbolic_value ], function( map: RpArray, key: Rp, val: Rp ) {
//     let need_set = true, node = map.kmap.get( key, function() {
//         if ( map.can_add_key() ) {
//             need_set = false;
//             return map._create_node( new map.key_type( methods["copy__b"].call_1( key ) ), new map.val_type( val ), false );
//         }
//         return null;
//     } );
//     if ( need_set && node )
//         node.val.rp = methods["set__so"].call_2s( node.val, val );
//     return map;
// } );

