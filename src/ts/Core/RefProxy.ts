import methods from './methods';
import Rp      from './Rp';

export default
class RefProxy extends Rp {
    constructor( rp: Rp ) {
        super( RefProxy );
        this.rp = rp;
    }

    prox_ref__b(): Rp {
        return this.rp;
    }

    rp: Rp;
}
Rp.make_templ( RefProxy );

