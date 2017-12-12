declare function require( s: string );
var LinkedStateMixin = require( 'react-addons-linked-state-mixin' );
let React            = require( 'react' );

function login( user, pass ) {
    console.log( user, pass );
}

//
export default React.createClass( {
    displayName: "Input",

    // We're using the mixin `LinkStateMixin` to have two-way databinding between our component and the HTML.
    mixins: [ LinkedStateMixin ],

    getInitialState() {
        return {
            user: '',
            password: ""
        };
    },

    // This will be called when the user clicks on the login button
    login( e ) {
        e.preventDefault();
        login( this.state.user, this.state.password );
    },

    render() {
        return <form role="form">
            <div className="form-group">
                <input type="text" valueLink={ this.linkState('user') } placeholder="Username" />
                <input type="password" valueLink={this.linkState('password') } placeholder="Password" />
            </div>
            <button type="submit" onClick={ this.login }>Submit</button>
        </form>
    }
} );
