//// nsmake html_content <div id='app'/>
import BinaryWriter     from "../src/ts/System/BinaryWriter"
import MuiThemeProvider from 'material-ui/styles/MuiThemeProvider';
import ReactDOM         from 'react-dom';
import React            from 'react';

const App = () => (
  <MuiThemeProvider>
    <div>proute</div>
  </MuiThemeProvider>
);

ReactDOM.render(
  <App />,
  document.getElementById('app')
);

//
var connection = new WebSocket( 'wss://localhost:7540/ws' );

connection.onopen = function () {
  console.log( 'Open' );
  
  let bw = new BinaryWriter;
  bw.write_PI8( 0 );
  bw.write_String( "robc" );
  bw.write_String( "crob" );

  bw.write_PI8( 0 );
  bw.write_String( "rosc" );
  bw.write_String( "cdob" );

  connection.send( bw.to_Uint8Array() );
};

// Log errors
connection.onerror = function (error) {
  console.log('WebSocket Error ' + error);
};

// Log messages from the server
connection.onmessage = function (e) {
  console.log('Server: ' + e.data);
};
