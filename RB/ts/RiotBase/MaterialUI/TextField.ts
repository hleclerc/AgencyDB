import InputMixin    from '../Dom/Core/InputMixin';
import String        from "../Variable/String";

import _TextField    from 'material-ui/lib/text-field';
import * as ReactDOM from 'react-dom';
import * as React    from 'react';

interface TextFieldProps extends React.Props<TextField> {
    className?: string;
    defaultValue?: string | number;
    disabled?: boolean;
    errorStyle?: React.CSSProperties;
    errorText?: React.ReactNode;
    floatingLabelStyle?: React.CSSProperties;
    floatingLabelText?: React.ReactNode;
    fullWidth?: boolean;
    hintStyle?: React.CSSProperties;
    hintText?: React.ReactNode;
    id?: string;
    inputStyle?: React.CSSProperties;
    multiLine?: boolean;
    onBlur?: React.FocusEventHandler;
    onChange?: React.FormEventHandler;
    onEnterKeyDown?: React.KeyboardEventHandler;
    onFocus?: React.FocusEventHandler;
    onKeyDown?: React.KeyboardEventHandler;
    rows?: number,
    rowsMax?: number,
    style?: React.CSSProperties;
    type?: string;
    underlineDisabledStyle?: React.CSSProperties;
    underlineFocusStyle?: React.CSSProperties;
    underlineShow?: boolean;
    underlineStyle?: React.CSSProperties;
    value?: string | number;
}

interface Props extends TextFieldProps {
    obj: String;
}

export default class TextField extends InputMixin<Props> {
    constructor( props : Props ) {
        super( props );
    }

    render() {
        return React.createElement( _TextField, this._input_props() );
    }

    _input_element() : HTMLTextAreaElement | HTMLInputElement {
        let e = ReactDOM.findDOMNode( this );
        if ( this.props.multiLine )
            return e.firstElementChild.nextElementSibling.firstElementChild.nextElementSibling as HTMLTextAreaElement;
        return e.getElementsByTagName( 'input' )[ 0 ];
    }
}
