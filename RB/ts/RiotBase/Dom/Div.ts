import Variable   from '../Variable/Core/Variable';
import PatchId    from '../System/PatchId';
import * as React from 'react';

interface Props extends React.HTMLProps<HTMLElement> {
    obj: Variable;
}
interface State {
    value: string;
}

//
export default class Div extends React.Component<Props,State> {
    constructor( props ) {
        super( props );

        this.onchange_id = this.props.obj.onchange( () => {
            this.setState( { value: this.props.obj.toString() } );
        } );

        this.state = {
            value: this.props.obj.toString()
        };
    }

    render() {
        return React.createElement( "div", this.props, this.state.value, this.props.children );
    }

    onchange_id: PatchId;
}
