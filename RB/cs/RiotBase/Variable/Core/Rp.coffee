Method = require "./Method.js"

# global variable (only accessible from this file)
nb_crea_types = 1
nb_inst_types = 1

#
module.exports =
class Rp
    @names_for_rw = [
        ###~g++ -E -I`pwd`
        #define DECL_TUPLE_TYPE( NAME ) #NAME,
        #include <cpp/RiotBase/Variable/Core/TupleTypesDecl.h>
        ~###
    ]

    @make_templ: ( cl, args = {} ) ->
        # type_num, type_off
        cl.type_num = nb_crea_types
        cl.type_off = 0
        nb_crea_types += 1

        # type carac
        for name, val of args
            cl[ name ] = val
        cl.unknown_term_value = cl.term_value and cl.unknown_value
        cl.known_term_value   = cl.term_value and not cl.unknown_value

        # num in types for R/W
        cl.num_for_RW = Rp.names_for_rw.indexOf cl.name
        Rp.num_type_map[ cl.num_for_RW ] = cl

        # static methods
        cl.New = ( args... ) ->
            # res = Reflect.construct cl, args
            res = new cl
            res.type_num = cl.type_num
            res.type_off = cl.type_off
            res.init.apply res, args
            res

        cl.potentially_updated_type_off = ->
            if not cl.type_off
                cl.type_off = nb_inst_types
                nb_inst_types += 1

                for n, m of Method.methods
                    m.resize nb_inst_types

            cl.type_off


    @num_type_map = [] # num type => constructor for corresponding Rp ()

    constructor: ->
        @type_off = 0
        @type_num = 0

    as_a_Variable: ->
        ct = Method.methods.obj_type.call_1 rp: this
        new ct this
