import InputMixin    from '../Dom/Core/InputMixin';
import String        from '../Variable/String';
import * as ReactDOM from "react-dom";
import * as React    from "react";

interface Props extends React.HTMLProps<HTMLInputElement> {
    obj: String;
}

//
export default class Input extends InputMixin<Props> {
    constructor( props: Props ) {
        super( props );
    }

    render() {
        return React.createElement( "div", { style: { display: "inline-block", position: "relative", padding: "0px", margin: "0px" } }, React.createElement( "input", this._input_props() ) );
    }

    _input_element() {
        return ReactDOM.findDOMNode( this ).getElementsByTagName( 'input' )[ 0 ];
    }
}
