//// nsmake html_content <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/latest/css/bootstrap.min.css">
//// nsmake html_content <div id='app'/>

//// nsmake ext_lib react           https://cdnjs.cloudflare.com/ajax/libs/react/15.4.2/react.min.js                     React
//// nsmake ext_lib react-dom       https://cdnjs.cloudflare.com/ajax/libs/react/15.4.2/react-dom.min.js                 ReactDOM
//// nsmake ext_lib react-bootstrap https://cdnjs.cloudflare.com/ajax/libs/react-bootstrap/0.30.8/react-bootstrap.min.js ReactBootstrap

declare var hmr: any, module: any;
module.onDispose = ( keep ) => { console.log( "od:", keep ); };


import * as React    from 'react'
import * as ReactDOM from 'react-dom'
import { Button, Form, FormGroup, Col, ControlLabel, FormControl, Checkbox, Modal } from 'react-bootstrap'

//  remove tap delay, essential for MaterialUI to work properly
// injectTapEventPlugin();

/*const onSubmit = () => {};
const onChange = ( e ) => { console.log( e ); };

const App = () => (
    <MuiThemeProvider muiTheme={ getMuiTheme() }>
        <FormLogin onSubmit={ onSubmit } onChange={ onChange } errors='' user=''/>
    </MuiThemeProvider>
);*/
        //   validationState={this.getValidationState()}

const App = React.createClass({
    getInitialState() {
        return {
            value: '',
            showModal: true 
        };
    },

    close() {
        this.setState({ showModal: false });
    },

    open() {
        this.setState({ showModal: true });
    },

    handleChange( e ) {
        this.setState({ value: e.target.value });
    },

    render() {
        return <Modal show={this.state.showModal} onHide={this.close}>
          <Modal.Header closeButton>
            <Modal.Title>Modal heading</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <h4>Text in a modal</h4>
            <p>Duis mollis, est non commodo luctus, nisi erat porttitigula.</p>

            <Form>
                <FormGroup controlId="formHorizontalEmail">
                    <Col componentClass={ControlLabel} sm={2}>
                        Email
                    </Col>
                    <Col sm={10}>
                        <FormControl type="email" placeholder="Email" />
                    </Col>
                </FormGroup>

                <FormGroup controlId="formHorizontalPassword">
                    <Col componentClass={ControlLabel} sm={2}>
                        Password
                    </Col>
                    <Col sm={10}>
                        <FormControl type="password" placeholder="Password" />
                    </Col>
                </FormGroup>

                <FormGroup>
                    <Col smOffset={2} sm={10}>
                        <Checkbox>Remember me</Checkbox>
                    </Col>
                </FormGroup>

                <FormGroup>
                    <Col smOffset={2} sm={10}>
                        <Button type="submit">
                        Sign in
                        </Button>
                    </Col>
                </FormGroup>
            </Form>​
          </Modal.Body>
          <Modal.Footer>
            <Button onClick={this.close}>Close</Button>
          </Modal.Footer>
        </Modal>;
    }
});

ReactDOM.render(
    <App />,
    document.getElementById('app'),
    () => {}
);

// //
// var connection = new WebSocket( 'wss://localhost:7540/ws' );

// connection.onopen = function () {
//   console.log( 'Open' );

//   let bw = new BinaryWriter;
//   bw.write_PI8( 0 );
//   bw.write_String( "robc" );
//   bw.write_String( "crob" );

//   bw.write_PI8( 0 );
//   bw.write_String( "rosc" );
//   bw.write_String( "cdob" );

//   connection.send( bw.to_Uint8Array() );
// };

// // Log errors
// connection.onerror = function (error) {
//   console.log('WebSocket Error ' + error);
// };

// // Log messages from the server
// connection.onmessage = function (e) {
//   console.log('Server: ' + e.data);
// };
