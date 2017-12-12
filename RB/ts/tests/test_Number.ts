import PatchManager   from "../RiotBase/Variable/Ot/PatchManager";
import Lazyfier       from "../RiotBase/Variable/Symbol/Lazyfier";
import RenderGraphviz from "../RiotBase/System/RenderGraphviz";
import Number         from "../RiotBase/Variable/Number";
import check          from "../RiotBase/System/Check";

class Methods {
    constructor( public a : number ) {}
    is_infeq( b ) { return this.a <= b; }
    is_supeq( b ) { return this.a >= b; }
    is_inf  ( b ) { return this.a <  b; }
    is_sup  ( b ) { return this.a >  b; }
    is_equ  ( b ) { return this.a == b; }
    is_neq  ( b ) { return this.a != b; }
    add     ( b ) { return this.a +  b; }
    sub     ( b ) { return this.a -  b; }
    mul     ( b ) { return this.a *  b; }
    div     ( b ) { return this.a /  b; }
    not     ()    { return  !  this.a ; }
    neg     ()    { return  -  this.a ; }
}

function test_base_op() {
    for( let i of [ "is_inf", "is_infeq", "is_sup", "is_supeq", "is_equ", "is_neq", "add", "sub", "mul", "div" ] )
        for( let [ c0, c1 ] of [ [ 1, 2 ], [ 2, 1 ], [ 1, 1 ] ] )
            check( new Number( c0 )[ i ]( c1 ), new Methods( c0 )[ i ]( c1 ) );
    for( let i of [ "not", "neg" ] )
        for( let c of [ 0, 1 ] )
            check( new Number( c )[ i ](), new Methods( c )[ i ]() );
}

function test_sym() {
    let s = Lazyfier.new_symbol( Number, "s" );
    // check( s.add( 1 ), "add(s,1)" );
    // check( new Number( 1 ).add( s ), "add(1,s)" );

    s.self_mul( 3 );
    s.self_add( 10 );
    check( s, "add(mul(s,3),10)" );

    let n = new Number( 5 );
    n.self_div( s );
    check( n, "div(5,add(mul(s,3),10))" );

    let o = n.copy();
    o.self_add( Lazyfier.new_symbol( Number, "t" ).add( 2 ) );
    check( n, "div(5,add(mul(s,3),10))" );
    check( o, "add(div(5,add(mul(s,3),10)),add(t,2))" );

    //
    RenderGraphviz.disp_graph( [ o ] );
}

function test_onchange( v ) {
    let val = new Number( 1 );
    let cpt = 0;
    val.onchange( () => cpt += 1 );
    val.self_add( v );
    check( val, 1 + v );
    setTimeout( () => check( cpt, v ), 100 );
}

// test_base_op()
test_sym()
// test_onchange( 1 )
// test_onchange( 0 )
