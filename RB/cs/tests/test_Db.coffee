String = require "../../js/RiotBase/Variable/String.js"
Number = require "../../js/RiotBase/Variable/Number.js"
check  = require "../../js/RiotBase/System/Check.js"
Db     = require "../../js/RiotBase/Db/Db.js"
Slider = require '/js/RiotBase/MaterialUI/Slider.js'

React    = require "react"
ReactDOM = require "react-dom"
# RaisedButton = require 'material-ui/lib/raised-button'
# DatePicker   = require 'material-ui/lib/date-picker/date-picker';

#
test_table = ->
    db = new Db

    check db.bind( "/", Db.Directory ), "{}"

    s0 = db.bind "/test", String
    check db.bind( "/", Db.Directory ), "{test:1@-----------~}"
    s1 = db.bind "/test", String
    s0.append "a"
    check s0, "a"
    check s1, "a"

#
test_conn = ->
    d0 = new Db
    d0.connect "root", "my_pass"
    s0 = d0.bind "/test", String

    d1 = new Db
    d1.connect "root", "my_pass"
    s1 = d1.bind "/test", String

    ReactDOM.render <div><Slider obj={s0}/><Slider obj={s1}/></div>, document.getElementById( 'test_div' )
    s0.append "a"

    s0.onchange -> console.log "change 0"
    s1.onchange -> console.log "change 1"

test_conn_num = ->
    d0 = new Db
    d0.connect "root", "my_pass"
    s0 = d0.bind "/test", Number

    d1 = new Db
    d1.connect "root", "my_pass"
    s1 = d1.bind "/test", String

    ReactDOM.render <div><Slider obj={s0}/><Slider obj={s1}/></div>, document.getElementById( 'test_div' )
    s0.append "a"

    s0.onchange -> console.log "change 0"
    s1.onchange -> console.log "change 1"

# éléments graphiques (React, ...)
#
#


# test_table()
# test_conn()
