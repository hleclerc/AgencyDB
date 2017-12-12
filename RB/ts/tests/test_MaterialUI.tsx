import TextField     from '../RiotBase/MaterialUI/TextField'
import Slider        from '../RiotBase/MaterialUI/Slider'
import String        from '../RiotBase/Variable/String'
import Number        from '../RiotBase/Variable/Number'
import check         from '../RiotBase/System/Check'
import Input         from '../RiotBase/Dom/Input'
import * as ReactDOM from "react-dom";
import * as React    from "react";

function test_input() {
    let str = new String;
    let num = new Number;
    // str.set( "abc\ndef\nghi" );

    //
    // <TextField hintText="s0" type="text" obj={str}/>
    //
    // <TextField hintText="s0" type="text" multiLine={true} obj={str}/>
    // <TextField hintText="s0" type="text" obj={str}/>
    // <TextField hintText="s0" type="text" obj={str}/>
    ReactDOM.render( <div>
        <TextField hintText="s0" type="text" multiLine={true} obj={str}/>
        <TextField hintText="s0" type="text" multiLine={true} obj={str}/>
    </div>, document.getElementById( 'test_div' ) );
    // <Input type="text" obj={str}/>
    // <TextField hintText="s0" type="text" multiLine={true} obj={str}/>
    // <Slider obj={num}/>
    // <Slider obj={num}/>
}

// test_input_db = ->
//     Db = require '../../js/RiotBase/Db/Db.js'
//
//     d0 = new Db "root", "my_pass"
//     d1 = new Db "root", "my_pass"
//     s0 = d0.bind "/test", String
//     s1 = d1.bind "/test", String
//
//     ReactDOM.render <div>
//         <TextField hintText="s0" type="text" obj={s0}/>
//         <TextField hintText="s1" type="text" obj={s1}/>
//     </div>, document.getElementById 'test_div'


test_input()
// test_input_db()
