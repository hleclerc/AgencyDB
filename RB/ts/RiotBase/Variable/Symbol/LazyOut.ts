import Sym from "./Sym";

//
export default
class LazyOut extends Sym {
    constructor( sched_id ) {
        super( LazyOut );
        this.csd.sched_id = sched_id;
    }
}
Sym.make_templ( LazyOut );
