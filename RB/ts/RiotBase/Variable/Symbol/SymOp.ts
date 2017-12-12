import Variable from "../Core/Variable";
import Method   from "../Core/Method";
import Sym      from "./Sym";
import Lazyfier from "./Lazyfier";

//
export default
class SymOp extends Sym {
    constructor( name : string, self_for_arg : boolean, ...args ) {
        super( SymOp );
        this.self_for_arg = self_for_arg;
        this.args         = args.slice( 0, args.length - 1 );
        this.attrs        = args[ args.length - 1 ];
        this.name         = name;

        for( let a of this.args ) {
            let csd = a.common_symbolic_data();
            if ( csd )
                csd.parents.push( this );
        }
    }

    to_String() : string {
        function disp_attr( attr ) {
            if ( typeof attr == "function" ) return "cb";
            return attr.toString();
        }
        return `${this.name}(${ this.args.map( a => a.toString() ).concat( this.attrs.map( disp_attr ) ) })`;
    }

    rp_structure() : string {
        function disp_attr( attr ) {
            if ( typeof attr == "function" ) return "cb";
            return attr.toString();
        }
        return `${this.name}(${ this.args.map( a => a.rp_structure() ).concat( this.attrs.map( disp_attr ) ) })`;
    }

    /** no request to make: data will come from the children */
    ww_launch() {
    }

    graphviz_repr() : string {
        switch ( this.name ) {
            case "add": return this.self_for_arg ? "+=" : "+";
            case "sub": return this.self_for_arg ? "-=" : "-";
            case "mul": return this.self_for_arg ? "*=" : "*";
            case "div": return this.self_for_arg ? "/=" : "/";
            default: return this.name + ( this.self_for_arg ? "_s" : "" );
        }
    }

    obj_type() {
        return this.args[ 0 ].obj_type(); // IMPORTANT_TODO
    }

    symbolic_children() {
        return this.args;
    }

    get_graph_data( graph_data, children, id ) {
        graph_data.push( {
            id      : id,
            name    : this.name,
            children: children,
            attrs   : this.attrs
        } );
    }

    exec() : Variable {
        switch ( this.args.length ) {
            case 1: {
                let c_0 = this.args[ 0 ].common_symbolic_data();
                let v_0 = c_0 ? c_0.ww_val : this.args[ 0 ];
                let o_0 = c_0 && c_0.parents.length > 1 ? "b" : "o";
                Method.methods[ this.self_for_arg ? "self_" + this.name : this.name + "_" + o_0  ].call_1( v_0, ...this.attrs );
                if ( c_0 ) ListUtil.remove( c_0.parents, this );
                return v_0;
            }
            case 2: {
                let c_0 = this.args[ 0 ].common_symbolic_data();
                let c_1 = this.args[ 1 ].common_symbolic_data();
                let v_0 = c_0 ? c_0.ww_val : this.args[ 0 ];
                let v_1 = c_1 ? c_1.ww_val : this.args[ 1 ];
                // console.log( "exec_with " + v_0.rp_structure() + " " + v_1.rp_structure() );
                let o_0 = c_0 && c_0.parents.length > 1 ? "b" : "o";
                let o_1 = c_1 && c_1.parents.length > 1 ? "b" : "o";
                // console.log( "v0: " + ( v_0.rp as any ).db_item.patch_manager.variable + " -> " + ( v_0.rp as any ).db_item.patch_manager.summary() );
                // console.log( "v1: " + ( v_1.rp as any ).db_item.patch_manager.variable + " -> " + ( v_1.rp as any ).db_item.patch_manager.summary() );
                Method.methods[ this.self_for_arg ? "self_" + this.name + "_" + o_1 : this.name + "_" + o_0 + o_1 ].call_2( v_0, v_1, ...this.attrs );
                if ( c_0 ) ListUtil.remove( c_0.parents, this );
                if ( c_1 ) ListUtil.remove( c_1.parents, this );
                return v_0;
            }
            case 3: {
                let c_0 = this.args[ 0 ].common_symbolic_data();
                let c_1 = this.args[ 1 ].common_symbolic_data();
                let c_2 = this.args[ 2 ].common_symbolic_data();
                let v_0 = c_0 ? c_0.ww_val : this.args[ 0 ];
                let v_1 = c_1 ? c_1.ww_val : this.args[ 1 ];
                let v_2 = c_2 ? c_2.ww_val : this.args[ 2 ];
                let o_0 = c_0 && c_0.parents.length > 1 ? "b" : "o";
                let o_1 = c_1 && c_1.parents.length > 1 ? "b" : "o";
                let o_2 = c_2 && c_2.parents.length > 1 ? "b" : "o";
                Method.methods[ this.self_for_arg ? "self_" + this.name + "_" + o_1 + o_2 : this.name + "_" + o_0 + o_1 + o_2 ].call_3( v_0, v_1, v_2, ...this.attrs );
                if ( c_0 ) ListUtil.remove( c_0.parents, this );
                if ( c_1 ) ListUtil.remove( c_1.parents, this );
                if ( c_2 ) ListUtil.remove( c_2.parents, this );
                return v_0;
            }
            default:
                throw new Error( "TODO" );
        }
    }


    self_for_arg : boolean;
    args         : Array<Variable>;
    attrs        : Array<any>;
    name         : string;
}
Sym.make_templ( SymOp );

for( let m of Method.std_mts ) {
    switch ( m.nb_rp_args ) {
        case 1:
            Method.methods[ m.name + "_b" ].add_surdef( 3, ( type_0 ) => type_0.unknown_value, ( v_0, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.copy_rp_without_lazifier( v_0 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_o" ].add_surdef( 3, ( type_0 ) => type_0.unknown_value, ( v_0, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.ref_rp_without_lazifier( v_0 ), args ).as_a_Variable() ).as_a_Variable()
            );
            break;
        case 2:
            Method.methods[ m.name + "_bb" ].add_surdef( 3, ( type_0, type_1 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_ob" ].add_surdef( 3, ( type_0, type_1 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_bo" ].add_surdef( 3, ( type_0, type_1 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_oo" ].add_surdef( 3, ( type_0, type_1 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), args ).as_a_Variable() ).as_a_Variable()
            );
            break;
        case 3:
            Method.methods[ m.name + "_bbb" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), Lazyfier.copy_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_obb" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), Lazyfier.copy_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_bob" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), Lazyfier.copy_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_oob" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), Lazyfier.copy_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_bbo" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), Lazyfier.ref_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_obo" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), Lazyfier.ref_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_boo" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), Lazyfier.ref_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            Method.methods[ m.name + "_ooo" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new Lazyfier( new SymOp( m.name, false, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), Lazyfier.ref_rp_without_lazifier( v_2 ), args ).as_a_Variable() ).as_a_Variable()
            );
            break;
        default:
            throw new Error( "TODO" );
    }
}

for( let m of Method.slf_mts ) {
    switch ( m.nb_rp_args ) {
        case 1:
            Method.methods[ "self_" + m.name ].add_surdef( 2, ( type_0 ) => type_0.unknown_value, function( v_0, ...args ) : boolean {
                v_0.rp = new Lazyfier( Method.methods[ m.name + "_s" ].call_1( new Variable( v_0.rp ), ...args ) );
                return false;
            } );
            Method.methods[ "self_" + m.name ].add_surdef( 3, ( type_0 ) => type_0 == Lazyfier, function( v_0 : { rp: Lazyfier }, ...args ) : boolean {
                let csd = <CommonSymbolicData>Method.methods[ "common_symbolic_data" ].call_1( v_0.rp.sym );
                if ( csd && csd.lazyfier_parents.indexOf( v_0.rp ) >= 0 )
                    csd.lazyfier_parents.splice( csd.lazyfier_parents.indexOf( v_0.rp ), 1 );

                v_0.rp.sym = Method.methods[ m.name + "_s" ].call_1( v_0.rp.sym, ...args );

                let cse = <CommonSymbolicData>Method.methods[ "common_symbolic_data" ].call_1( v_0.rp.sym );
                if ( cse )
                    cse.lazyfier_parents.push( v_0.rp );
                return false;
            } );
            Method.methods[ m.name + "_s" ].add_surdef( 3, ( type_0 ) => type_0.unknown_value, ( v_0, ...args ) =>
                new SymOp( m.name, true, Lazyfier.ref_rp_without_lazifier( v_0 ), args ).as_a_Variable()
            );
            break;
        case 2:
            for( let term of [ "b", "o" ] ) {
                Method.methods[ "self_" + m.name + "_" + term ].add_surdef( 2, ( type_0, type_1 ) => type_0.unknown_value || type_1.unknown_value, function( v_0, v_1, ...args ) : boolean {
                    v_0.rp = new Lazyfier( Method.methods[ m.name + "_s" + term ].call_2( new Variable( v_0.rp ), v_1, ...args ) );
                    return false;
                } );
                Method.methods[ "self_" + m.name + "_" + term ].add_surdef( 3, ( type_0, type_1 ) => type_0 == Lazyfier, function( v_0 : { rp: Lazyfier }, v_1, ...args ) : boolean {
                    let csd = <CommonSymbolicData>Method.methods[ "common_symbolic_data" ].call_1( v_0.rp.sym );
                    if ( csd && csd.lazyfier_parents.indexOf( v_0.rp ) >= 0 )
                        csd.lazyfier_parents.splice( csd.lazyfier_parents.indexOf( v_0.rp ), 1 );

                    v_0.rp.sym = Method.methods[ m.name + "_s" + term ].call_2( v_0.rp.sym, v_1, ...args );

                    let cse = <CommonSymbolicData>Method.methods[ "common_symbolic_data" ].call_1( v_0.rp.sym );
                    if ( cse )
                        cse.lazyfier_parents.push( v_0.rp );
                    return false;
                } );
            }
            Method.methods[ m.name + "_sb" ].add_surdef( 3, ( type_0, type_1 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, ...args ) =>
                new SymOp( m.name, true, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), args ).as_a_Variable()
            );
            Method.methods[ m.name + "_so" ].add_surdef( 3, ( type_0, type_1 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, ...args ) =>
                new SymOp( m.name, true, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), args ).as_a_Variable()
            );
            break;
        case 3:
            for( let term of [ "bb", "bo", "ob", "oo" ] ) {
                Method.methods[ "self_" + m.name + "_" + term ].add_surdef( 2, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value || type_2.unknown_value, function( v_0, v_1, v_2, ...args ) : boolean {
                    v_0.rp = new Lazyfier( Method.methods[ m.name + "_s" + term ].call_3( new Variable( v_0.rp ), v_1, v_2, ...args ) );
                    return false;
                } );
                Method.methods[ "self_" + m.name + "_" + term ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0 == Lazyfier, function( v_0 : { rp: Lazyfier }, v_1, v_2, ...args ) : boolean {
                    let csd = <CommonSymbolicData>Method.methods[ "common_symbolic_data" ].call_1( v_0.rp.sym );
                    if ( csd && csd.lazyfier_parents.indexOf( v_0.rp ) >= 0 )
                        csd.lazyfier_parents.splice( csd.lazyfier_parents.indexOf( v_0.rp ), 1 );

                    v_0.rp.sym = Method.methods[ m.name + "_s" + term ].call_3( v_0.rp.sym, v_1, v_2, ...args );

                    let cse = <CommonSymbolicData>Method.methods[ "common_symbolic_data" ].call_1( v_0.rp.sym );
                    if ( cse )
                        cse.lazyfier_parents.push( v_0.rp );
                    return false;
                } );
            }
            Method.methods[ m.name + "_sbb" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new SymOp( m.name, true, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), Lazyfier.copy_rp_without_lazifier( v_2 ), args ).as_a_Variable()
            );
            Method.methods[ m.name + "_sbo" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new SymOp( m.name, true, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), Lazyfier.ref_rp_without_lazifier( v_2 ), args ).as_a_Variable()
            );
            Method.methods[ m.name + "_sob" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new SymOp( m.name, true, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), Lazyfier.copy_rp_without_lazifier( v_2 ), args ).as_a_Variable()
            );
            Method.methods[ m.name + "_soo" ].add_surdef( 3, ( type_0, type_1, type_2 ) => type_0.unknown_value || type_1.unknown_value, ( v_0, v_1, v_2, ...args ) =>
                new SymOp( m.name, true, Lazyfier.ref_rp_without_lazifier( v_0 ), Lazyfier.ref_rp_without_lazifier( v_1 ), Lazyfier.ref_rp_without_lazifier( v_2 ), args ).as_a_Variable()
            );
            break;
        default:
            throw new Error( "TODO" );
    }
}

import ListUtil           from "../../System/ListUtil";
import CommonSymbolicData from "./CommonSymbolicData";
