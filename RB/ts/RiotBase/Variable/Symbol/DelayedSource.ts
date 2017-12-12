import Variable   from "../Core/Variable";
import LazySource from "./LazySource";

//
export default
class DelayedSource extends LazySource {
    constructor( variable : Variable ) {
        super( DelayedSource );

        this.variable = variable;
    }

    to_String() : string {
        return "Delayed[" + this.variable.toString() + "]";
    }

    obj_type() {
        return this.variable.constructor;
    }

    get_graph_data( graph_data, children, id ) {
        this.variable.get_graph_data( graph_data, children, id );
    }

    exec() {
        return this.variable;
    }

    variable: Variable;
}
LazySource.make_templ( DelayedSource );

import BinaryWriter from "../../System/BinaryWriter";
