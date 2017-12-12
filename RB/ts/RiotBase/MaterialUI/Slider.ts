declare function require( path: string ) : any;
var MUI_Slider = require( 'material-ui/lib/slider' );
var ReactDOM   = require( "react-dom" );
var React      = require( "react" );

//
export default
React.createClass( {
    getInitialState() {
        this._init_inst();
        return { value: this.props.obj.val };
    },

    render() {
        return React.createElement( MUI_Slider, this._input_props() );
        // return < defaultValue={ 0.5 } />
    },

    _init_inst() {
        //
        this.onchange_id = this.props.obj.onchange( () => {
            this.setState( { value: this.props.obj.val } )
        } );

        // attribute for input item
        this.aprops = {
            onChange: ( e, val ) => {
                this.props.obj.val = val;
            },
        }
    },

    _input_props() {
        return (<any>Object).assign( {}, this.aprops, this.props, { value: this.state.value } );
    }

    //   <div className="commentBox">
    //     <h1>Comments</h1>
    //     <CommentList data={this.state.data} />
    //     <CommentForm onCommentSubmit={this.handleCommentSubmit} />
    //   </div>
} );
