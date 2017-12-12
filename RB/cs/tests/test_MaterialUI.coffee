TextField = require '../../js/RiotBase/MaterialUI/TextField.js';
String    = require '../../js/RiotBase/Variable/String.js'
check     = require '../../js/RiotBase/System/Check.js'
Input     = require '../../js/RiotBase/Dom/Input.js'

MextField = require "material-ui/lib/text-field"
ReactDOM  = require "react-dom"
React     = require "react"

test_input = ->
    str = new String

    ReactDOM.render <div>
        <TextField hintText="s0" type="text" obj={str}/>
        <Input type="text" obj={str}/>
    </div>, document.getElementById 'test_div'

test_input_db = ->
    Db = require '../../js/RiotBase/Db/Db.js'

    d0 = new Db "root", "my_pass"
    d1 = new Db "root", "my_pass"
    s0 = d0.bind "/test", String
    s1 = d1.bind "/test", String

    ReactDOM.render <div>
        <TextField hintText="s0" type="text" obj={s0}/>
        <TextField hintText="s1" type="text" obj={s1}/>
    </div>, document.getElementById 'test_div'


# test_input()
test_input_db()
