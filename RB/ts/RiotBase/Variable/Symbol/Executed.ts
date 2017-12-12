import Variable from "../Core/Variable";
import Rp       from "../Core/Rp";


//
export default
class Executed extends Rp {
    prox_ref() : Variable {
        return this.sym;
    }

    prox_ptr() : Variable {
        return this.sym;
    }

    sym        : Variable;
}
Rp.make_templ( Executed );
