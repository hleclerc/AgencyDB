//// nsmake html_content <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/latest/css/bootstrap.min.css">
//// nsmake html_content <div id='app'/>

//// nsmake ext_lib react           https://cdnjs.cloudflare.com/ajax/libs/react/15.4.2/react.min.js                     React
//// nsmake ext_lib react-dom       https://cdnjs.cloudflare.com/ajax/libs/react/15.4.2/react-dom.min.js                 ReactDOM
//// nsmake ext_lib react-bootstrap https://cdnjs.cloudflare.com/ajax/libs/react-bootstrap/0.30.8/react-bootstrap.min.js ReactBootstrap

import TrackerConn   from './TrackerConn'
import FormLogin     from './FormLogin'
import * as ReactDOM from 'react-dom'
import * as React    from 'react'

let tracker_conn = new TrackerConn( "localhost:7540" );

ReactDOM.render(
    <FormLogin onSubmit={ tracker_conn.onSubmit.bind( tracker_conn ) }/>,
    document.getElementById('app'),
    () => {}
);

