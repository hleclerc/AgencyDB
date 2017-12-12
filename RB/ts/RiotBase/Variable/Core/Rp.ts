import Method from "./Method"

let nb_crea_types = 1;
let nb_inst_types = 1;

//
export default
class Rp {
    static make_templ( cl, name = cl.name ) {
        // type_num, type_off
        Rp.type_num_map[ nb_crea_types ] = cl
        cl.type_num = nb_crea_types;
        cl.type_off = 0;

        nb_crea_types += 1;

        // type carac
        cl.unknown_term_value = cl.term_value && cl.unknown_value;
        cl.known_term_value   = cl.term_value && ! cl.unknown_value;

        // static methods
        cl.potentially_updated_type_off = function() : number {
            if ( ! cl.type_off ) {
                cl.type_off = nb_inst_types;
                nb_inst_types += 1;

                for( let n in Method.methods )
                    Method.methods[ n ].resize( nb_inst_types );
            }

            return cl.type_off;
        }
    }

    static unknown_term_value = false;
    static known_term_value   = false;
    static unknown_value      = false;
    static term_value         = false;
    static data_loss_if_prox  = 0;     ///< a coefficient to say how much data will be lost if use of prox_ref, prox_ptr or prox_val
    static type_num_map       = [];    ///< type_num => constructor for corresponding Rp ()
    static type_off           = 0;     ///< will be redefined (declared here for typescript)
    static type_num           = 0;

    constructor( cl ) {
        this.type_off = cl.type_off;
        this.type_num = cl.type_num;
    }

    as_a_Variable() : Variable {
        let T = ( <any>this ).obj_type();
        return new T( this );
    }

    exec() : Variable {
        throw new Error( "TODO: " + (<any>this).constructor.name );
    }

    type_off: number;
    type_num: number;
}

import Variable from "./Variable";
