import methods from "./methods"
import Method  from "./Method"

let nb_crea_types = 1;
let nb_inst_types = 1;

//
export default
class Rp {
    static make_templ( cl, name = cl.name ) {
        // type_num, type_off
        Rp.type_num_map[ nb_crea_types ] = cl;
        cl.type_num = nb_crea_types;
        cl.type_off = 0;

        nb_crea_types += 1;

        // static methods
        cl.potentially_updated_type_off = function(): number {
            if ( ! cl.type_off ) {
                cl.type_off = nb_inst_types;
                nb_inst_types += 1;

                for( let n in methods )
                    methods[ n ].resize( nb_inst_types );
            }

            return cl.type_off;
        }
    }

    static term_value         = true;
    static watch_self_op      = false; /** false if ok to say that xxx__sx is the same thing than xxx__ox (self op are like a normal op with first arg that can be owned by the procedure) */
    static data_loss_if_prox  = 0;     /** a coefficient to say how much data will be lost if use of prox_ref, prox_ptr or prox_val */
    static methods            = [];    /** List of method to be exported */
    static type_num_map       = [];    /** type_num => constructor for corresponding Rp () */
    static type_off           = 0;     /** will be redefined (declared here for typescript) */
    static type_num           = 0;

    static __Rp_type__        = true;

    constructor( cl ) {
        this.type_off = cl.type_off;
        this.type_num = cl.type_num;
    }

    // as_a_Variable() : Variable {
    //     let T = ( <any>this ).obj_type();
    //     return new T( this );
    // }

    // exec() : Variable {
    //     throw new Error( "TODO: " + (<any>this).constructor.name );
    // }

    type_off: number;
    type_num: number;
}

// export function method( target, name, descriptor ) {
//     methods[ name ].add_surdef( 1, [ target ], function( a, ...args ) { return descriptor.value.call( a, ...args ); } );
// }

