import { Button, 
         Form,
         FormGroup,
         Col,
         ControlLabel,
         FormControl,
         Checkbox,
         Modal,
         Jumbotron }  from 'react-bootstrap'
import * as React from 'react'

export
interface FormLoginProps {
    onSubmit: ( email: string, password: string, remember_me: boolean, cb: ( err: string ) => void ) => void;
}

export
class FormLoginState {
    showModal   = true;
    email       = "";
    password    = "";
    remember_me = false;
    waiting     = false;
    error       = "";
    info        = "";
}

/**
 * 
 */
export default
class FormLogin extends React.Component<FormLoginProps,FormLoginState> {
    constructor( props: FormLoginProps ){
        super( props );
        this.state = new FormLoginState;
    }

    close() {
        this.setState({ showModal: false });
    }

    open() {
        this.setState({ showModal: true });
    }

    handleInputChange( event ) {
        const target = event.target;
        this.setState( { [target.name]: target.type === 'checkbox' ? target.checked : target.value } );
    }

    onSubmit( event ) {
        event.preventDefault();
        this.setState({ waiting: true, info: "Waiting for credentials..." });

        this.props.onSubmit( this.state.email, this.state.password, this.state.remember_me, error => {
            this.setState({ waiting: false, info: "" });
            console.log( error );
            if ( error )
                return this.setState({ error });
            this.close();
        } );
    }
    
    render() {
        return <Modal show={ this.state.showModal } onHide={ this.close }>
            <Modal.Header closeButton>
                    <Modal.Title>Login</Modal.Title>
            </Modal.Header>
          <Modal.Body>
                <Form horizontal onSubmit={ e => this.onSubmit( e ) }>
                    <FormGroup controlId="formHorizontalEmail">
                        <Col componentClass={ControlLabel} sm={2}> Email </Col>
                        <Col sm={10}> <FormControl value={ this.state.email } name="email" placeholder="Email" onChange={ e => this.handleInputChange( e ) }/> </Col>
                    </FormGroup>

                    <FormGroup controlId="formHorizontalPassword">
                        <Col componentClass={ControlLabel} sm={2}> Password </Col>
                        <Col sm={10}> <FormControl value={ this.state.password } name="password" type="password" placeholder="Password" onChange={ e => this.handleInputChange( e ) }/> </Col>
                    </FormGroup>

                    <FormGroup>
                        <Col smOffset={2} sm={10}> <Checkbox checked={ this.state.remember_me } name="remember_me" onChange={ e => this.handleInputChange( e ) }>Remember me</Checkbox> </Col>
                    </FormGroup>

                    <Col smOffset={2} sm={10}>
                        { this.state.error && <p className="error-message">{ this.state.error }</p>}
                        { this.state.info  && <p>{ this.state.info }</p>}
                    </Col>

                    <FormGroup>
                        <Col smOffset={2} sm={10}>
                            <Button bsStyle="primary" type="submit"> Sign in </Button>
                        </Col>
                    </FormGroup>
                </Form>​
          </Modal.Body>
        </Modal>;
        /*return <Modal show={ this.state.showModal } onHide={ this.close }>
          <Modal.Header closeButton>
                <Modal.Title>Login</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <Form onSubmit={ e => this.onSubmit( e ) }>
                <FormGroup controlId="formHorizontalEmail">
                    <Col componentClass={ControlLabel} sm={2}> Email </Col>
                    <Col sm={10}> <FormControl value={ this.state.email } name="email" placeholder="Email" onChange={ e => this.handleInputChange( e ) }/> </Col>
                </FormGroup>

                <FormGroup controlId="formHorizontalPassword">
                    <Col componentClass={ControlLabel} sm={2}> Password </Col>
                    <Col sm={10}> <FormControl value={ this.state.password } name="password" type="password" placeholder="Password" onChange={ e => this.handleInputChange( e ) }/> </Col>
                </FormGroup>

                <FormGroup>
                    <Col smOffset={2} sm={10}> <Checkbox checked={ this.state.remember_me } name="remember_me" onChange={ e => this.handleInputChange( e ) }>Remember me</Checkbox> </Col>
                </FormGroup>

                <Col smOffset={2} sm={10}>
                    { this.state.error && <p className="error-message">{ this.state.error }</p>}
                    { this.state.info  && <p>{ this.state.info }</p>}
                </Col>

                <FormGroup>
                    <Col smOffset={2} sm={10}>
                        <Button bsStyle="primary" type="submit"> Sign in </Button>
                    </Col>
                </FormGroup>
            </Form>​
            poue
            <p/>
          </Modal.Body>
          <Modal.Footer></Modal.Footer>
        </Modal>;*/
    }
}
// <Modal.Footer> <Button onClick={this.close}>Close</Button> </Modal.Footer>
