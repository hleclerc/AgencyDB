import ModalUserLogin from '../RiotBase/MaterialUI/ModalUserLogin';
import TextField      from '../RiotBase/MaterialUI/TextField'
import Input          from '../RiotBase/Dom/Input';
import Div            from '../RiotBase/Dom/Div'

import FlatButton     from 'material-ui/lib/flat-button';
import AppBar         from 'material-ui/lib/app-bar';

import Directory      from "../RiotBase/Variable/Directory";
import String         from "../RiotBase/Variable/String";
import Number         from "../RiotBase/Variable/Number";

import check          from "../RiotBase/System/Check";

import DbItemPtr      from "../RiotBase/Db/Core/DbItemPtr";
import Db             from "../RiotBase/Db/Db";
// import Slider      from "/js/RiotBase/MaterialUI/Slider";

declare function require( s: string );
let React    = require( "react" );
let ReactDOM = require( "react-dom" );

//
function test_table() {
    let db = new Db;
    db.connect( "root", "my_pass" );

    check( db.bind( "/", Db.Directory ), "{}" );

    let s0 = db.bind<String>( "/test", String );
    check( db.bind( "/", Db.Directory ), "{test:1@-----------~}" );
    let s1 = db.bind<String>( "/test", String );
    s0.append( "a" );
    check( s0, "a" );
    check( s1, "a" );
}


//
function test_conn() {
    let db = new Db;

    setTimeout( function() {
        function alet() {
            console.log( "val: " + st );
            // console.log( ( st.rp as DbItemPtr ).db_item.final_item.patch_manager.summary( db.dev ) );
            // console.log( db.persistent_data.inst );
        }

        let di = db.bind<Directory>( "/", Directory );
        let st = db.bind<String>( "/test", String );
        // st.append( window.location.search ? window.location.search.slice( 1 ) : 'pouet' );

        // setTimeout( function() {
        //     let inode = di.get( "test" ).toInode();
        //     console.log( "" + inode );
        //     let val = db.bind_by_object_id<String>( inode, String );
        //     console.log( "" + val );
        // }, 5000 );
        //
        // <Input type="text" obj={st}/>

        ReactDOM.render( <div>
            <AppBar
                title={<span style={{cursor:"pointer"}}>Title</span>}
                // onTitleTouchTap={handleTouchTap}
                // iconElementLeft={<IconButton><NavigationClose /></IconButton>}
                iconElementRight={<FlatButton onMouseDown={ () => db.logout() } label="Logout" />}
            />
            <TextField multiLine={ true } hintText="s0" type="text" obj={ st }/>
            <Div obj={ di } onClick={ alet }/>
            <br/>
            <ModalUserLogin db={ db }/>
        </div>, document.getElementById( 'test_div' ) );
        // multiLine={ true }
    }, 1000 );
}

//
// function test_conn_num() {
//     let d0 = new Db
//     d0.connect( "root", "my_pass" );
//     let s0 = d0.bind( "/test", Number );
//
//     let d1 = new Db
//     d1.connect( "root", "my_pass" );
//     let s1 = d1.bind( "/test", String );
//
//     ReactDOM.render( <div><Slider obj={s0}/><Slider obj={s1}/></div>, document.getElementById( 'test_div' ) );
//     s0.append( "a" );
//
//     s0.onchange( () => console.log( "change 0" ) );
//     s1.onchange( () => console.log( "change 1" ) );
// }

// test_table()
test_conn()
