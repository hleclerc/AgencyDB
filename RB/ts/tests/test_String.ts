import OtWrapperString from "../RiotBase/Variable/String/OtWrapperString";
import PatchManager    from "../RiotBase/Variable/Ot/PatchManager";
import Lazyfier        from "../RiotBase/Variable/Symbol/Lazyfier";
import Caret           from "../RiotBase/Variable/String/Caret";
import BinaryWriter    from "../RiotBase/System/BinaryWriter";
import BinaryReader    from "../RiotBase/System/BinaryReader";
import String          from "../RiotBase/Variable/String";
import Number          from "../RiotBase/Variable/Number";
import DevId           from "../RiotBase/System/DevId";
import UsrId           from "../RiotBase/System/UsrId";
import check           from "../RiotBase/System/Check";
import tpc             from "./tpc";


function test_base_op() {
    var str = new String( "abcd" );
    check( str, "abcd" );
    check( str.length, 4 );

    var cp = str.copy();
    check( cp, "abcd" );

    check( str.charAt( 2 )            , "c"   );
    check( str.equ_immediate( "abcd" ), true  );
    check( str.inf_immediate( "abcd" ), false );
    check( str.sup_immediate( "abcd" ), false );

    check( str.insert( 1, "Z" ), "aZbcd" );
    check( str.append( "efg" ), "aZbcdefg" );
    check( str.remove( 2, 1 ), "aZcdefg" );
    check( cp, "abcd" );

    check( str.get_possible_rights(), "add_rights,rem_rights,read,insert,remove,append" );
}

function test_cursor() {
    var str = new String( "abcd" );
    var cid = str.new_cursor( [ Caret.New( 0, 1 ), Caret.New( 2, 3 ) ] );
    str.insert( 1, "1" );
    str.insert( 3, "3" );
    check( str.get_cursor( cid ), "{beg:0,end:2,foc:1},{beg:4,end:5,foc:1}" );
    str.remove( 1, 1 );
    str.remove( 3, 1 );
    check( str.get_cursor( cid ), "{beg:0,end:1,foc:1},{beg:3,end:3,foc:1}" );
}

function test_sym() {
    let sym = Lazyfier.new_symbol( String, "s" );
    check( sym, "s" );
    check( sym.charAt( 2 ), "select(s,2)" );
}

function test_on_change() {
    let a = new String( "a" );
    let cpt = 0
    let cid = a.onchange( () => cpt += 1 );

    a.append( "b" );

    setTimeout( () => {
        check( cpt, 1 );
        a.rm_onchange( cid );
        a.append( "c" );
        setTimeout( () => {
            check( a, "abc" );
            check( cpt, 1 );
        }, 1 );
    }, 1 );
}

function test_patch() {
    let a = new String( "ab" );
    let changes = new BinaryWriter();
    a.pull_ot_changes( changes );
    check( changes, "4,0,2,97,98" );
    // basic rights
    check( (<OtWrapperString>a.rp).usr_right( new UsrId() ), 63 );
    check( (<OtWrapperString>a.rp).usr_right( new UsrId( new DevId( "O5HeHcHU1Io~" ), 17 ) ), 0 );
    // basic insertion
    check( a.insert( 1, "0" ), "a0b" );
    // cursors
    let ic0 = a.new_cursor( [ Caret.New( 1, 1 ) ] );
    let ic1 = a.new_cursor( [ Caret.New( 1, 2 ) ] );
    check( a.get_cursor( ic0 ), "{beg:1,end:1,foc:1}" );

    a.set_cursor( ic0, [ Caret.New( 0, 1 ) ] );
    check( a.get_cursor( ic0 ), "{beg:0,end:1,foc:1}" );
    check( a.get_cursor( ic1 ), "{beg:1,end:2,foc:1}" );

    check( a.insert( 1, "1" ), "a10b" );
    check( a.get_cursor( ic0 ), "{beg:0,end:2,foc:1}" );
    check( a.get_cursor( ic1 ), "{beg:2,end:3,foc:1}" );

    // changes.clear()
    // a.pull_ot_changes( changes );
    // check( changes, "4,1,1,48,7,0,1,0,7,0,2,0,9,0,2,2,0,4,1,1,49" );
}


function test_patch_manager() {
    let d0 = new DevId( "gMWDnGjZfxZ~" );
    let d1 = new DevId( "O5HeHcHU1Io~" );
    let us = new UsrId( d0, 17 );

    let s0 = new String( "ab" );
    let p0 = new PatchManager( s0 );
    let c0 = p0.get_changes( d1, d0, us );

    let s1 = new String();
    let p1 = new PatchManager( s1 );
    p1.app_changes( new BinaryReader( c0 ), d0, [ d1 ], d1, us, ()=>{} );
    check( s1.toString(), "ab" );

    s1.insert( 1, "0" );
    let c1 = p1.get_changes( d0, d1, us );
    p0.app_changes( new BinaryReader( c1 ), d1, [ d0 ], d0, us, ()=>{} );
    check( s0.toString(), "a0b" );
}

function test_parallel_changes() {
    // simple inserts and removals
    tpc( String, function( s ) {
        s.m[ 0 ].d.append( "ab" );
    }, "", "ab" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 1 );
    }, "abc", "ac" );

    // parallel insert/... operations
    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 1, "ab" );
        s.m[ 1 ].d.insert( 3, "cd" );
    }, "0123", "0ab12cd3" );

    tpc( String, function( s ) {
        s.m[ 1 ].d.insert( 3, "cd" );
        s.m[ 2 ].d.insert( 1, "ab" );
    }, "0123", "0ab12cd3" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 3, "ab" );
        s.m[ 1 ].d.remove( 1, 1 );
    }, "012345", "02ab345" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 3, "ab" );
        s.m[ 1 ].d.remove( 1, 4 );
    }, "012345", "0ab5" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 3, "ab" );
        s.m[ 1 ].d.remove( 4, 1 );
    }, "012345", "012ab35" );


    // parallel remove/... operations
    tpc( String, function( s ) {
        s.m[ 1 ].d.remove( 1, 1 );
        s.m[ 0 ].d.insert( 3, "ab" );
    }, "012345", "02ab345" );

    tpc( String, function( s ) {
        s.m[ 1 ].d.remove( 1, 4 );
        s.m[ 0 ].d.insert( 3, "ab" );
    }, "012345", "0ab5" );

    tpc( String, function( s ) {
        s.m[ 1 ].d.remove( 4, 1 );
        s.m[ 0 ].d.insert( 3, "ab" );
    }, "012345", "012ab35" );


    tpc( String, function( s ) {
        s.m[ 1 ].d.remove( 1, 2 );
        s.m[ 0 ].d.remove( 7, 2 );
    }, "0123456789", "034569" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 4 );
        s.m[ 1 ].d.remove( 3, 4 );
    }, "0123456789", "0789" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 8 );
        s.m[ 1 ].d.remove( 3, 4 );
    }, "0123456789", "09" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 7, 2 );
        s.m[ 1 ].d.remove( 1, 2 );
    }, "0123456789", "034569" );

    tpc( String, function( s ) {
        s.m[ 1 ].d.remove( 3, 4 );
        s.m[ 0 ].d.remove( 1, 4 );
    }, "0123456789", "0789" );

    tpc( String, function( s ) {
        s.m[ 1 ].d.remove( 3, 4 );
        s.m[ 0 ].d.remove( 1, 8 );
    }, "0123456789", "09" );

    // cursor
    tpc( String, function( s ) {
        s.m[ 1 ].d.new_cursor( [ Caret.New( 2 ) ] );
    }, "0123", "0123 C({beg:2,end:2,foc:1})" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 1, "a" );
        s.m[ 1 ].d.new_cursor( [ Caret.New( 2 ) ] );
    }, "0123", "0a123 C({beg:3,end:3,foc:1})" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.new_cursor( [ Caret.New( 2 ) ] );
        s.m[ 1 ].d.insert( 1, "a" );
    }, "0123", "0a123 C({beg:3,end:3,foc:1})" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 2 );
        s.m[ 1 ].d.new_cursor( [ Caret.New( 2 ) ] );
    }, "0123", "03 C({beg:1,end:1,foc:1})" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.new_cursor( [ Caret.New( 2 ) ] );
        s.m[ 1 ].d.remove( 1, 2 );
    }, "0123", "03 C({beg:1,end:1,foc:1})" );

    // rights
    let ext_usr = new UsrId( new DevId( "h8u858_RnDF~" ), 18 );

    tpc( String, function( s ) { // transmission
        s.m[ 0 ].d.add_usr_right( ext_usr, "read" );
    }, "0", "0 read" );


    tpc( String, function( s ) { // transmission with rem
        s.m[ 0 ].d.add_usr_right( ext_usr, "read"   );
        s.m[ 0 ].d.add_usr_right( ext_usr, "insert" );
        s.sync();
        s.m[ 1 ].d.rem_usr_right( ext_usr, "insert" );
    }, "0", "0 read" );

    tpc( String, function( s ) { // right to do things
        s.m[ 0 ].d.rem_usr_right( new UsrId, "insert" );
        s.sync();
        s.m[ 1 ].d.insert( 1, "a" );
    }, "01", "01" );

    // with std patches
    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 1, "c" );  // [c] [] []
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] ); // we want date of "c" < date of "b"

        s.m[ 1 ].d.insert( 2, "b" );  // [c] [b] []
        s.sync( [ s.m[ 0 ] ] ); // [c] [b] [b]
        s.sync( [ s.m[ 2 ] ] ); // [cb] [cb] [b]
        // -> [cb] [cb] [cb]
    }, "012", "0c1b2" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 2, 2 );   // 01234 -> 014
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.insert( 1, "b" ); // 0b1234 -> 0b1234
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234", "0b14" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 2, 2 );        // 014 01234 01234
        s.sync( [ s.m[ 0 ] ] );           // 014 01234 01234
        s.m[ 1 ].d.insert( 1, "b" );      // 014 0b1234 01234
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] ); // 014 0b1234 01234
        s.sync( [ s.m[ 0 ], s.m[ 1 ] ] ); // 014 0b1234 01234
    }, "01234", "0b14" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 2 );   // 01234 -> 034
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.insert( 4, "b" ); // 01234 -> 0123b4
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234", "03b4" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 4 );   // 012345 -> 05
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.insert( 3, "new" ); // 012345 -> 012new345
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "012345", "0new5" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 1, "b" ); // 01234 -> 0b1234
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.remove( 2, 2 );   // 01234 -> 0b14
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234", "0b14" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 4, "b" ); // 01234 -> 0123b4
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.remove( 1, 2 );   // 01234 -> 034
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234", "03b4" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.insert( 3, "b" ); // 01234 -> 012b34
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.remove( 1, 3 );   // 01234 -> 04
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234", "0b4" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 2 ); // 01234567 -> 034567
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.remove( 5, 2 ); // 01234567 -> 012347
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234567", "0347" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 5, 2 ); // 01234567 -> 034567
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.remove( 1, 2 ); // 01234567 -> 012347
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234567", "0347" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 1, 5 ); // 01234567 -> 067
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.remove( 2, 5 ); // 01234567 -> 017
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234567", "07" );

    tpc( String, function( s ) {
        s.m[ 0 ].d.remove( 2, 5 ); // 01234567 -> 067
        s.sync( [ s.m[ 1 ], s.m[ 2 ] ] );
        s.m[ 1 ].d.remove( 1, 5 ); // 01234567 -> 017
        s.sync( [ s.m[ 0 ] ] );
        s.sync( [ s.m[ 2 ] ] );
    }, "01234567", "07" );
}

function test_RW() {
    // basic string
    let st = new String( "abcd" );

    let bw = new BinaryWriter;
    bw.write_Variable( st );

    let va = new BinaryReader( bw.to_Uint8Array() ).read_Variable();
    check( va, st );

    // with patches
    let cid = st.new_cursor( [ Caret.New( 3, 2, 1 ) ] );

    let bx = new BinaryWriter;
    bx.write_Variable( st );
    bx.write_Variable( st );
    bx.write_Variable( st );

    let br = new BinaryReader( bx.to_Uint8Array() );
    for( let i = 2; i--; ) {
        let vb = <String>br.read_Variable();
        check( JSON.stringify( vb.get_cursor_ids() ), JSON.stringify( st.get_cursor_ids() ) );
        check( vb.get_cursor( cid ), st.get_cursor( cid ) );
        check( vb, st );

        if ( i == 1 )
            br.skip_Variable();
    }
}

// test_base_op()
// test_cursor()
// test_sym()
// test_patch();

// test_patch_manager()
test_parallel_changes()
// test_on_change()
// test_RW()
