import FlatButton   from 'material-ui/lib/flat-button';
import TextField    from 'material-ui/lib/text-field';
import Checkbox     from 'material-ui/lib/checkbox';
import Dialog       from 'material-ui/lib/dialog';
import Cookie       from '../Dom/Core/Cookie';
import Db           from '../Db/Db';
import * as React   from 'react';


interface Props {
    db: Db;
};

class State {
    open          = true;
    user_creation = false;
    error_message = "";
    info_message  = "";
    waiting       = false;
};

function no_msg( s : State ) {
    s.error_message = "";
    s.info_message  = "";
    s.waiting       = false;
}

/*
 Prop pour la connection:
   Db contient une liste de callbacks à appeler quand connecté
*/
export default class ModalUserLogin extends React.Component<Props,State> {
    constructor( props ) {
        super( props );

        this.state = new State;

        let tok = Cookie.get( "login_tok" );
        if ( tok.length ) {
            this.state.open         = false;
            this.state.waiting      = true;
            this.state.info_message = "Trying token authentication";

            // try to connect with the token
            this.props.db.connect( tok, "", ( db : Db ) => {
                if ( db.connected ) {
                    Cookie.set( "login_tok", db.token, 1.0 );
                    this.setState( ( s ) => { s.open = false; no_msg( s ); return s; } );
                } else {
                    // token was invalid
                    Cookie.del( "login_tok" );
                    this.setState( ( s ) => { s.open = true; no_msg( s ); return s; } );
                }
            }, () => {
                this.setState( ( s ) => { s.open = true; no_msg( s ); return s; } );
            } );
        }
    }

    handleClose() {
        this.setState( ( s ) => { s.open = false; return s; } );
    }

    ansLogin( db : Db ) {
        if ( db.connected ) {
            Cookie.set( "login_tok", db.token, 1.0 );
            this.setState( ( s ) => { s.open = false; return s; } );
        } else {
            this.setState( ( s ) => { s.error_message = "Wrong username/password combination"; s.waiting = false; return s; } );
        }
    }

    ansLogout() {
        this.setState( ( s ) => { s.open = true; no_msg( s ); return s; } );
    }

    handleEnter() {
        if ( this.state.user_creation ) {
            let email = ( this.refs["email"] as TextField ).getValue();
            if ( this.user && this.pass && email ) {
                this.setState( ( s ) => { s.waiting = true; s.info_message = "Waiting for user creation"; s.error_message = ""; return s; } );
                this.props.db.create_user( this.user, this.pass, email, ( msg : string ) => {
                    this.setState( ( s ) => {
                        s.user_creation = false;
                        s.waiting = false;
                        s.info_message = `You should have received a message (at ${email})`;
                        s.error_message = msg;
                        return s;
                    } );
                } );
            } else {
                this.setState( ( s ) => { s.error_message = "You have to provide an username, a password and an email"; return s; } );
            }
        } else {
            if ( this.user && this.pass ) {
                this.setState( ( s ) => { s.waiting = true; s.info_message = "Waiting for login"; s.error_message = ""; return s; } );
                this.props.db.connect( this.user, this.pass, this.ansLogin.bind( this ), this.ansLogout.bind( this ) );
            } else {
                this.setState( ( s ) => { s.error_message = "You have to provide an username and a password"; return s; } );
            }
        }
    }

    handleSetSignUpMode( e : MouseEvent ) {
        this.setState( ( s ) => { s.user_creation = true; return s; } )
        e.preventDefault();
    }

    handleSetSignInMode( e : MouseEvent ) {
        this.setState( ( s ) => { s.user_creation = false; return s; } )
        e.preventDefault();
    }

    componentDidMount() {
        if ( this.state.open )
            ( this.refs["username"] as TextField ).focus();
    }

    render() {
        const actions = [
            <FlatButton
                label="Cancel"
                secondary={ true }
                onMouseDown={ this.handleClose.bind( this ) }
            />,
            <FlatButton
                label={ this.state.user_creation ? "Sign up" : "Sign in" }
                primary={ true }
                keyboardFocused={ true }
                onMouseDown={ this.handleEnter.bind( this ) }
            />
        ];

        const inorup = [
            <div style={{paddingTop:"7px"}}>
                <a href='#' onMouseDown={ this.handleSetSignUpMode.bind( this ) }>Create a new user</a>
            </div>,
            <div>
                <TextField ref="email" hintText="E-mail" onEnterKeyDown={ this.handleEnter.bind( this ) } type="email"/><br/>
                <div style={{paddingTop:"7px"}}>
                    <a href='#' onMouseDown={ this.handleSetSignInMode.bind( this ) }>Return to sign in mode</a>
                </div>

            </div>
        ];

        return <div>
            <Dialog title={ this.state.user_creation ? "Sign up" : "Sign in" } actions={ actions } modal={ false } open={ this.state.open } onRequestClose={ this.handleClose.bind( this ) } >
                <TextField ref="username" hintText="Username" onEnterKeyDown={ this.handleEnter.bind( this ) } onChange={ ( e ) => this.user = ( e.target as any ).value } type="text"/><br/>
                <TextField hintText="password" onEnterKeyDown={ this.handleEnter.bind( this ) } onChange={ ( e ) => this.pass = ( e.target as any ).value } type="password"/>
                { inorup[ Number( this.state.user_creation ) ] }
                <br/>
                <Checkbox defaultChecked={ true } ref="remember_me" label="Remember me"/>
                <div style={{ paddingTop: "7px" }}>{ this.state.info_message }</div>
                <div style={{ color: "red", paddingTop: "7px" }}>{ this.state.error_message }</div>
            </Dialog>
        </div>;
        // <div>{ this.state.waiting ? "Waiting for authentication" : null }</div>
    }


    user       : string;
    pass       : string;
}
