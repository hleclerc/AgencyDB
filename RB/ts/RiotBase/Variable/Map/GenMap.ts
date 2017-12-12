import WithComparison = require( "../../System/WithComparison" );

import BinaryWriter from "../../System/BinaryWriter";
import BinaryReader from "../../System/BinaryReader";
import SortedArray  from "../../System/SortedArray";
import PatchId      from "../../System/PatchId";
import DevId        from "../../System/DevId";
import Variable     from "../Core/Variable";
import PatchTypes   from "../Ot/PatchTypes";
import OtWrapper    from "../Ot/OtWrapper";
import OtMerger     from "../Ot/OtMerger";
import Method       from "../Core/Method";


//
class Node implements WithComparison.CmpInterface {
    constructor( key : Variable, val : Variable, id : PatchId ) { this.key = key; this.val = val; this.id  = id; }
    inf( key : Variable ) : boolean { return this.key.inf_immediate( key ); }
    sup( key : Variable ) : boolean { return this.key.sup_immediate( key ); }

    key: Variable;
    val: Variable;
    id : PatchId;
}

class IdCmpNode implements WithComparison.CmpInterface {
    constructor( node : Node ) { this.node = node; }
    inf( id : PatchId ) : boolean { return this.node.id.inf( id ); }
    sup( id : PatchId ) : boolean { return this.node.id.sup( id ); }

    node: Node;
}

//
export default
class GenMap extends OtWrapper {
    // patch types
    static op_new_key = {
        attrs: [ "PatchId", "Variable", "Variable" ],
        right( rp: GenMap, flags : number, key_id : PatchId, key : Variable, val : Variable ) : boolean {
            return GenMap.can_add_key.ok( flags );
        },
        valid( rp: GenMap, key_id : PatchId, key : Variable, val : Variable ) : boolean {
            return key_id.num != 0;
        },
        apply_op( rp: GenMap, aod, key_id : PatchId, key : Variable, val : Variable ) : void {
            let old = rp.kmap.get( key );
            if ( old != undefined ) {
                Method.methods["self_ot_merge_b"].call_2( old.val, val, aod.usr );
            } else {
                let node = new Node( key, val, key_id );

                let va = new Variable( rp );
                key.onchange_par( va, node );
                val.onchange_par( va, node );

                let changes = new BinaryWriter;
                key.pull_ot_changes( changes );
                val.pull_ot_changes( changes );

                rp.kmap.set( key   , node );
                rp.imap.set( key_id, new IdCmpNode( node ) );
            }
            rp.sig_change();
        },
        undo( rp: GenMap, aod, key_id : PatchId, key_data : Variable, val_data : Variable ) : void {
            GenMap.op_rem_key.apply_op( rp, aod, key_id );
        },
        update_patch_data_l0( rp: GenMap, args ) : void {
        }
    };

    static op_rem_key = {
        attrs: [ "PatchId" ],
        right( rp: GenMap, flags : number, key_id : PatchId ) : boolean {
            return GenMap.can_rem_key.ok( flags );
        },
        valid( rp: GenMap, key_id : PatchId ) : boolean {
            return key_id.num != 0;
        },
        apply_op( rp: GenMap, aod, key_id : PatchId ) : void {
            let ic = rp.imap.get( key_id );
            if ( ic != null )
                rp._rem_node( ic.node, false );
        },
        undo( rp: GenMap, aod, key_id : PatchId ) : void {
            throw new Error( "TODO" );
        },
        update_patch_data_l0( rp: GenMap, args ) : void {
        }
    };

    static op_rem_key_und = {
        attrs: [ "PatchId", "Variable", "Variable" ],
        right( rp: GenMap, flags : number, key_id : PatchId, key : Variable, val : Variable ) : boolean {
            return GenMap.op_rem_key.right( rp, flags, key_id );
        },
        valid( rp: GenMap, key_id : PatchId, key : Variable, val : Variable ) : boolean {
            return key_id.num != 0;
        },
        apply_op( rp: GenMap, aod, key_id : PatchId, key : Variable, val : Variable ) : void {
            GenMap.op_rem_key.apply_op( rp, aod, key_id );
        },
        undo( rp: GenMap, aod, key_id : PatchId, key : Variable, val : Variable ) : void {
            throw new Error( "TODO" );
        },
        update_patch_data_l0( rp: GenMap, args ) : void {
        }
    };

    static op_mod_key = {
        attrs: [ "PatchId", "Buffer" ],
        right( rp: GenMap, flags : number, key_id : PatchId, msg : Uint8Array ) : boolean {
            return GenMap.can_mod_key.ok( flags );
        },
        valid( rp: GenMap, key_id : PatchId, msg : Uint8Array ) : boolean {
            return key_id.num != 0;
        },
        apply_op( rp: GenMap, aod, key_id : PatchId, msg : Uint8Array ) : void {
            let id_cmp = rp.imap.get( key_id );
            if ( id_cmp != null ){
                let pm = id_cmp.node.key.new_ot_merger();
                let res = new BinaryWriter;
                switch ( aod.type ) {
                    case OtMerger.MergingType.NEW:  pm.new_patch( res, msg, aod.usr ); break;
                    case OtMerger.MergingType.UNK:  pm.unk_patch( res, msg, aod.usr ); break;
                    case OtMerger.MergingType.STD:  pm.std_patch( res, msg, aod.usr ); break;
                    case OtMerger.MergingType.UNDO: pm.undo( msg, aod.usr ); break;
                    default: throw new Error( "TODO" );
                }
            }
        },
        undo( rp: GenMap, aod, key_id : PatchId, msg : Uint8Array ) : void {
            let id_cmp = rp.imap.get( key_id );
            if ( id_cmp != null ) {
                let pm = id_cmp.node.key.new_ot_merger();
                pm.undo( msg, aod.usr );
            }
        },
        update_patch_data_l0( rp: GenMap, args ) : void {
        }
    };

    static op_mod_val = {
        attrs: [ "PatchId", "Buffer" ],
        right( rp: GenMap, flags : number, key_id : PatchId, msg : Uint8Array ) : boolean {
            return GenMap.can_mod_val.ok( flags );
        },
        valid( rp: GenMap, key_id : PatchId, msg : Uint8Array ) : boolean {
            return key_id.num != 0;
        },
        apply_op( rp: GenMap, aod, key_id : PatchId, msg : Uint8Array ) : void {
            let id_cmp = rp.imap.get( key_id );
            if ( id_cmp != null ) {
                let pm = id_cmp.node.val.new_ot_merger();
                let res = new BinaryWriter;
                switch ( aod.type ) {
                    case OtMerger.MergingType.NEW:  pm.new_patch( res, msg, aod.usr ); break;
                    case OtMerger.MergingType.UNK:  pm.unk_patch( res, msg, aod.usr ); break;
                    case OtMerger.MergingType.STD:  pm.std_patch( res, msg, aod.usr ); break;
                    case OtMerger.MergingType.UNDO: pm.undo( msg, aod.usr ); break;
                    default: throw new Error( "TODO" );
                }
            }
        },
        undo( rp: GenMap, aod, key_id : PatchId, msg : Uint8Array ) : void {
            let id_cmp = rp.imap.get( key_id );
            if ( id_cmp != null ) {
                let pm = id_cmp.node.val.new_ot_merger();
                pm.undo( msg, aod.usr );
            }
        },
        update_patch_data_l0( rp: GenMap, args ) : void {
        }
    };

    static op_types = OtWrapper.op_types.concat( [
        GenMap.op_new_key,
        GenMap.op_rem_key,
        GenMap.op_rem_key_und,
        GenMap.op_mod_key,
        GenMap.op_mod_val
    ] );


    // right types
    static can_read = {
        flags: 4,
        name: "read",
        ok( flags : number ) : boolean { return ( flags & 4 ) != 0; }
    }
    static can_add_key = {
        flags: 8,
        name: "add_key",
        ok( flags : number ) : boolean { return ( flags & 8 ) != 0; }
    }
    static can_rem_key = {
        flags: 16,
        name: "rem_key",
        ok( flags : number ) : boolean { return ( flags & 16 ) != 0; }
    }
    static can_mod_key = {
        flags: 32,
        name: "mod_key",
        ok( flags : number ) : boolean { return ( flags & 32 ) != 0; }
    }
    static can_mod_val = {
        flags: 64,
        name: "mod_val",
        ok( flags : number ) : boolean { return ( flags & 64 ) != 0; }
    }

    static right_types = OtWrapper.right_types.concat( [
        GenMap.can_read,
        GenMap.can_add_key,
        GenMap.can_rem_key,
        GenMap.can_mod_key,
        GenMap.can_mod_val
    ] );

    constructor( obj, new_val : () => Variable ) {
        super( GenMap );

        this.new_val    = new_val;
        this.loc_id_num = 0;
        this.new_nodes  = [];
        this.mod_nodes  = [];
        this.rem_nodes  = [];
        this.kmap       = new SortedArray<Node     >(); // key -> node
        this.imap       = new SortedArray<IdCmpNode>(); // id -> node

        for( let k in obj )
            this._create_node( Variable.rp_conv_cp( k ), Variable.rp_conv_cp( obj[ k ] ) );
    }

    to_String() : string {
        let res = [];
        for( let node of this.kmap.items )
            res.push( node.key + ":" + node.val );
        return `{${ res }}`;
    }

    sig_change( node? ) {
        OtWrapper.prototype.sig_change.call( this, node );
        if ( node != null && this.mod_nodes.indexOf( node ) < 0 && this.new_nodes.indexOf( node ) < 0 )
            this.mod_nodes.push( node );
    }

    get_patch_type( bw : BinaryWriter ) {
        bw.write_PT( PatchTypes.num.GenMap );
    }

    cmp_patch_type( br : BinaryReader ) : boolean {
        return br.read_PT() == PatchTypes.num.GenMap;
    }

    freeze_last_changes() {
        // new nodes
        for( let n of this.new_nodes ) {
            let changes = new BinaryWriter;
            n.key.pull_ot_changes( changes );
            n.val.pull_ot_changes( changes );
            GenMap.reg_op( this.last_ops, GenMap.op_new_key, n.id, n.key, n.val );
        }
        this.new_nodes = [];

        // mod keys and vals
        for( let n of this.mod_nodes ) {
            let key_data = new BinaryWriter;
            let val_data = new BinaryWriter;
            n.key.pull_ot_changes( key_data );
            n.val.pull_ot_changes( val_data );
            if ( key_data.size ) GenMap.reg_op( this.last_ops, GenMap.op_mod_key, n.id, key_data.to_Uint8Array() );
            if ( val_data.size ) GenMap.reg_op( this.last_ops, GenMap.op_mod_val, n.id, val_data.to_Uint8Array() );
        }
        this.mod_nodes = [];

        // rem_nodes
        for( let n of this.rem_nodes )
            GenMap.reg_op( this.last_ops, GenMap.op_rem_key_und, n.id, n.key, n.val );
        this.rem_nodes = [];
    }

    _rem_item( key : Variable ) {
        let n = this.kmap.get( key );
        if ( n != null )
            this._rem_node( n );
    }

    _create_node( key, val, insert_in_kmap = true, insert_in_imap = true ) : Node {
        let id   = new PatchId( new DevId, this.loc_id_num += 1 );
        let node = new Node( key, val, id );
        this.new_nodes.push( node );

        let va = new Variable( this );
        key.onchange_par( va, node );
        val.onchange_par( va, node );

        if ( insert_in_imap ) this.imap.set( id , new IdCmpNode( node ) );
        if ( insert_in_kmap ) this.kmap.set( key, node );
        return node;
    }

    _rem_node( node, reg = true ) : void {
        this.kmap.del( node.key );
        this.imap.del( node.id  );
        this.sig_change();

        if ( reg ) {
            // remove node from new_nodes and mod_nodes
            let i = this.new_nodes.indexOf( node );
            let j = this.mod_nodes.indexOf( node );
            if ( i >= 0 ) this.new_nodes.splice( i, 1 );
            if ( j >= 0 ) this.mod_nodes.splice( j, 1 );

            // store it in rem_nodes
            this.rem_nodes.push( node );
        }
    }

    new_val    : () => Variable;
    loc_id_num : number;
    new_nodes  : Array<Node>;
    mod_nodes  : Array<Node>;
    rem_nodes  : Array<Node>;
    kmap       : SortedArray<Node>;      // key -> node
    imap       : SortedArray<IdCmpNode>; // id -> node
}
OtWrapper.make_templ( GenMap );

PatchTypes.readers[ PatchTypes.num.GenMap ] = function( br : BinaryReader ) : _Map<_String,_ObjectId> {
    return new _Map<_String,_ObjectId>( new GenMap( {}, () => new _ObjectId ) );
}

Method.methods["mod_select"].add_surdef( 2, [ GenMap, ( type ) => type.known_term_value ], function( map : { rp: GenMap }, key ) : Variable {
    let node = map.rp.kmap.get( key, function() {
        if ( GenMap.can_add_key.ok( map.rp.usr_right() ) ){
            let vnkey = Method.methods["copy"].call_1( key );
            let vnval = map.rp.new_val();
            return map.rp._create_node( vnkey, vnval, false );
        }
        return null;
    } );
    return node != null ? node.val : null;
} );

Method.methods["rem_item"].add_surdef( 2, [ GenMap, ( type ) => type.known_term_value ], function( map : { rp: GenMap }, key ) {
    map.rp._rem_item( key );
} );

import _ObjectId  from "../ObjectId";
import _String    from "../String";
import _Map       from "../Map";
