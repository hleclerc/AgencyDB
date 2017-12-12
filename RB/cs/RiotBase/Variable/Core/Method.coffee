module.exports =
class Method
    @methods = {}
    @slf_mts = {}
    @std_mts = {}

    @decl = ( name, constness ) ->
        Method.methods[ name ] = new Method name, constness

    @dstd = ( name, nb_rp_args ) ->
        Method.std_mts[ name ] =
            nb_rp_args: nb_rp_args

        for v in Method.comb_bo nb_rp_args
            constness = ( ( if c == "o" then "c" else "r" ) for c in v ).join ""
            Method.decl name + "_" + v, constness

    @dslf = ( name, nb_rp_args ) ->
        Method.slf_mts[ name ] =
            nb_rp_args: nb_rp_args

        Method.dstd name, nb_rp_args

        for v in Method.comb_bo nb_rp_args - 1
            constness = ( ( if c == "o" then "c" else "r" ) for c in v ).join ""
            Method.decl "self_" + name + ( if v.length then "_" + v else "" ), "r" + constness

    @comb_bo = ( nb_rp_args ) ->
        switch nb_rp_args
            when 0 then [""]
            when 1 then "bo"
            when 2 then ["bb","bo","ob","oo"]
            when 3 then ["bbb","bob","obb","oob","bbo","boo","obo","ooo"]
            else console.error "TODO"

    constructor: ( name, constness ) ->
        @function_list = if constness.length >= 3 then {} else []
        @nb_tabu_types = 0
        @nb_rp_args    = constness.length
        @constness     = constness
        @surdefs       = []
        @name          = name

        @resize 8

    add_surdef: ( surdef ) ->
        surdef.stack = ( new Error ).stack
        @surdefs.push surdef

    call_1: ( args... ) ->
        @function_list[ args[ 0 ].rp.type_off ].apply null, args

    call_2: ( args... ) ->
        @function_list[ args[ 0 ].rp.type_off * @nb_tabu_types + args[ 1 ].rp.type_off ].apply null, args

    call_3: ( args... ) ->
        s = String [ args[ 0 ].rp.type_off, args[ 1 ].rp.type_off, args[ 2 ].rp.type_off ]
        if not @function_list[ s ]?
            @function_list[ s ] = @function_list[ String [ 0, 0, 0 ] ]
        @function_list[ s ].apply null, args

    resize: ( new_nb_tabu_types ) ->
        if @nb_tabu_types < new_nb_tabu_types
            switch @nb_rp_args
                when 1 # 1 rp arg
                    unk = ( args... ) =>
                        type_0 = args[ 0 ].rp.constructor
                        args[ 0 ].rp.type_off = type_0.potentially_updated_type_off()
                        @function_list[ args[ 0 ].rp.type_off ] = @find_best_func_1 type_0
                        @function_list[ args[ 0 ].rp.type_off ].apply null, args

                    while @function_list.length < new_nb_tabu_types
                        @function_list.push unk
                when 2
                    unk = ( args... ) =>
                        type_0 = args[ 0 ].rp.constructor
                        type_1 = args[ 1 ].rp.constructor
                        args[ 0 ].rp.type_off = type_0.potentially_updated_type_off()
                        args[ 1 ].rp.type_off = type_1.potentially_updated_type_off()
                        @function_list[ args[ 0 ].rp.type_off * @nb_tabu_types + args[ 1 ].rp.type_off ] = @find_best_func_2 type_0, type_1
                        @function_list[ args[ 0 ].rp.type_off * @nb_tabu_types + args[ 1 ].rp.type_off ].apply null, args

                    new_function_list = ( unk for i in [ 0 ... new_nb_tabu_types * new_nb_tabu_types ] )
                    for i in [ 0 ... @nb_tabu_types ]
                        for j in [ 0 ... @nb_tabu_types ]
                            new_function_list[ i * new_nb_tabu_types + j ] = @function_list[ i * @nb_tabu_types + j ]
                    @function_list = new_function_list
                when 3
                    unk = ( args... ) =>
                        type_0 = args[ 0 ].rp.constructor
                        type_1 = args[ 1 ].rp.constructor
                        type_2 = args[ 2 ].rp.constructor
                        args[ 0 ].rp.type_off = type_0.potentially_updated_type_off()
                        args[ 1 ].rp.type_off = type_1.potentially_updated_type_off()
                        args[ 2 ].rp.type_off = type_2.potentially_updated_type_off()
                        s = String [ args[ 0 ].rp.type_off, args[ 1 ].rp.type_off, args[ 2 ].rp.type_off ]
                        @function_list[ s ] = @find_best_func_3 type_0, type_1, type_2
                        @function_list[ s ].apply null, args

                    if not @nb_tabu_types
                        @function_list[ String [ 0, 0, 0 ] ] = unk
                else
                    console.error "TODO"
            @nb_tabu_types = new_nb_tabu_types

    pert_surdef_for: ( args... ) ->
        switch @nb_rp_args
            when 1 then @find_best_func_1 args[ 0 ], false, true
            when 2 then @find_best_func_2 args[ 0 ], args[ 1 ], false, true
            when 3 then @find_best_func_3 args[ 0 ], args[ 1 ], args[ 2 ], false, true
            else console.error "TODO"

    has_surdef_for: ( args... ) ->
        undefined != switch @nb_rp_args
            when 1 then @find_best_func_1 args[ 0 ], false
            when 2 then @find_best_func_2 args[ 0 ], args[ 1 ], false
            when 3 then @find_best_func_3 args[ 0 ], args[ 1 ], args[ 2 ], false
            else console.error "TODO"


    find_best_func_1: ( type_0, disp = true, want_pert = false ) ->
        best_surdefs = []

        test_pf = ( surdef ) ->
            if best_surdefs.length == 0 or best_surdefs[ 0 ].pert <= surdef.pert
                if best_surdefs.length and best_surdefs[ 0 ].pert < surdef.pert
                    best_surdefs = []
                best_surdefs.push surdef

        # ext surdefs
        for s in @surdefs
            if s.test type_0
                test_pf s

        # method in type_0 ?
        if type_0.prototype[ @name ]?
            test_pf
                stack: "in type"
                pert : 2
                func : ( val_0, args... ) =>
                    type_0.prototype[ @name ].apply val_0.rp, args

        # proxy ?
        if @name not in ["write_structure","prox_ptr","prox_ref","prox_val"]
            if @constness[ 0 ] == "c"
                if Method.methods.prox_ref.has_surdef_for type_0
                    test_pf stack: "prox_ref", pert: 1, func: ( val_0, args... ) => @call_1.apply this, [].concat( [ Method.methods.prox_ref.call_1 val_0 ], args )
                else if Method.methods.prox_val.has_surdef_for type_0
                    test_pf stack: "prox_ref", pert: 1, func: ( val_0, args... ) => @call_1.apply this, [].concat( [ Method.methods.prox_val.call_1 val_0 ], args )
                else if Method.methods.prox_ptr.has_surdef_for type_0
                    test_pf stack: "prox_ref", pert: 1, func: ( val_0, args... ) => @call_1.apply this, [].concat( [ Method.methods.prox_ptr.call_1 val_0 ], args )
            else if Method.methods.prox_ptr.has_surdef_for type_0 # if we are going to modify the value, (currently) only prox_ptr is allowed
                test_pf stack: "prox_ptr", pert: 1, func: ( val_0, args... ) => @call_1.apply this, [].concat( [ Method.methods.prox_ptr.call_1 val_0 ], args )

        # summary
        if disp
            if best_surdefs.length == 0
                return console.error "impossible to find surdef for #{@name} (with type #{type_0.name})"
            if best_surdefs.length >= 2
                disp_pos = ( s ) ->
                    p = s.stack.split( "\n" )
                    "\n->" + if p.length > 2 then p[ 2 ] else s.stack
                console.error "Ambiguous overload for #{@name} with type=#{type_0.name} and pert=#{best_surdefs[ 0 ].pert}, possibilities are #{best_surdefs.map( disp_pos ).join( "" )}"
        if want_pert
            if best_surdefs.length
                best_surdefs[ 0 ].pert
            else
                -1e300
        else if best_surdefs.length
            best_surdefs[ 0 ].func

    find_best_func_2: ( type_0, type_1, disp = true, want_pert = false ) ->
        # surdefs
        best_surdefs = []

        test_pf = ( surdef ) ->
            if best_surdefs.length == 0 or best_surdefs[ 0 ].pert <= surdef.pert
                if best_surdefs.length and best_surdefs[ 0 ].pert < surdef.pert
                    best_surdefs = []
                best_surdefs.push surdef


        # ext surdefs
        for s in @surdefs
            if s.test type_0, type_1
                test_pf s

        # proxy ?
        if @constness[ 0 ] == "c"
            if Method.methods.prox_ref.has_surdef_for type_0
                test_pf stack: "prox_ref", pert: 1, func: ( val_0, args... ) => @call_2.apply this, [].concat( [ Method.methods.prox_ref.call_1 val_0 ], args )
            else if Method.methods.prox_val.has_surdef_for type_0
                test_pf stack: "prox_val", pert: 1, func: ( val_0, args... ) => @call_2.apply this, [].concat( [ Method.methods.prox_val.call_1 val_0 ], args )
            else if Method.methods.prox_ptr.has_surdef_for type_0
                test_pf stack: "prox_ptr", pert: 1, func: ( val_0, args... ) => @call_2.apply this, [].concat( [ Method.methods.prox_ptr.call_1 val_0 ], args )
        else if Method.methods.prox_ptr.has_surdef_for type_0 # if we are going to modify the value, (currently) only prox_ptr is allowed
            test_pf stack: "prox_ptr", pert: 1, func: ( val_0, args... ) => @call_2.apply this, [].concat( [ Method.methods.prox_ptr.call_1 val_0 ], args )
        else if @constness[ 1 ] == "c"
            if Method.methods.prox_ref.has_surdef_for type_1
                test_pf stack: "prox_ref", pert: 1, func: ( val_0, val_1, args... ) => @call_2.apply this, [].concat( [ val_0, Method.methods.prox_ref.call_1 val_1 ], args )
            else if Method.methods.prox_val.has_surdef_for type_1
                test_pf stack: "prox_val", pert: 1, func: ( val_0, val_1, args... ) => @call_2.apply this, [].concat( [ val_0, Method.methods.prox_val.call_1 val_1 ], args )
            else if Method.methods.prox_ptr.has_surdef_for type_1
                test_pf stack: "prox_ptr", pert: 1, func: ( val_0, val_1, args... ) => @call_2.apply this, [].concat( [ val_0, Method.methods.prox_ptr.call_1 val_1 ], args )
        else if Method.methods.prox_ptr.has_surdef_for type_1 # if we are going to modify the value, (currently) only prox_ptr is allowed
            test_pf stack: "prox_ptr", pert: 1, func: ( val_0, val_1, args... ) => @call_2.apply this, [].concat( [ val_0, Method.methods.prox_ptr.call_1 val_1 ], args )

        # summary
        if disp
            if best_surdefs.length == 0
                console.error "Impossible to find surdef for #{@name} with types [#{type_0.name},#{type_1.name}]"
            if best_surdefs.length >= 2
                disp_pos = ( s ) ->
                    p = s.stack.split( "\n" )
                    "\n->" + if p.length > 2 then p[ 2 ] else s.stack
                console.error "Ambiguous overload for #{@name} with types [#{type_0.name},#{type_1.name}] and pert=#{best_surdefs[ 0 ].pert}, possibilities are #{best_surdefs.map( disp_pos ).join( "" )}"
        if want_pert
            if best_surdefs.length
                best_surdefs[ 0 ].pert
            else
                -1e300
        else if best_surdefs.length
            best_surdefs[ 0 ].func

    find_best_func_3: ( type_0, type_1, type_2, disp = true, want_pert = false ) ->
        # surdefs
        best_pert = -1
        best_func = []

        test_pf = ( pert, func ) ->
            if best_pert <= pert
                if best_pert < pert
                    best_pert = pert
                    best_func = []
                best_func.push func

        for s in @surdefs
            if s.test type_0, type_1, type_2
                test_pf s.pert, s.func

        # proxy ?
        if @constness[ 0 ] == "c"
            if Method.methods.prox_ref.has_surdef_for type_0
                test_pf 1, ( val_0, args... ) => @call_3.apply this, [].concat( [ Method.methods.prox_ref.call_1 val_0 ], args )
            else if Method.methods.prox_val.has_surdef_for type_0
                test_pf 1, ( val_0, args... ) => @call_1.apply this, [].concat( [ Method.methods.prox_val.call_1 val_0 ], args )
            else if Method.methods.prox_ptr.has_surdef_for type_0
                test_pf 1, ( val_0, args... ) => @call_3.apply this, [].concat( [ Method.methods.prox_ptr.call_1 val_0 ], args )
        else if Method.methods.prox_ptr.has_surdef_for type_0
            test_pf 1, ( val_0, args... ) => @call_3.apply this, [].concat( [ Method.methods.prox_ptr.call_1 val_0 ], args )

        if @constness[ 1 ] == "c"
            if Method.methods.prox_ref.has_surdef_for type_1
                test_pf 1, ( val_0, val_1, args... ) => @call_3.apply this, [].concat( [ val_0, Method.methods.prox_ref.call_1 val_1 ], args )
            else if Method.methods.prox_val.has_surdef_for type_1
                test_pf 1, ( val_0, val_1, args... ) => @call_3.apply this, [].concat( [ val_0, Method.methods.prox_val.call_1 val_1 ], args )
            else if Method.methods.prox_ptr.has_surdef_for type_1
                test_pf 1, ( val_0, val_1, args... ) => @call_3.apply this, [].concat( [ val_0, Method.methods.prox_ptr.call_1 val_1 ], args )
        else if Method.methods.prox_ptr.has_surdef_for type_1
            test_pf 1, ( val_0, val_1, args... ) => @call_3.apply this, [].concat( [ val_0, Method.methods.prox_ptr.call_1 val_1 ], args )

        if @constness[ 2 ] == "c"
            if Method.methods.prox_ref.has_surdef_for type_2
                test_pf 1, ( val_0, val_1, val_2, args... ) => @call_3.apply this, [].concat( [ val_0, val_1, Method.methods.prox_ref.call_1 val_2 ], args )
            else if Method.methods.prox_val.has_surdef_for type_2
                test_pf 1, ( val_0, val_1, val_2, args... ) => @call_3.apply this, [].concat( [ val_0, val_1, Method.methods.prox_val.call_1 val_2 ], args )
            else if Method.methods.prox_ptr.has_surdef_for type_2
                test_pf 1, ( val_0, val_1, val_2, args... ) => @call_3.apply this, [].concat( [ val_0, val_1, Method.methods.prox_ptr.call_1 val_2 ], args )
        else if Method.methods.prox_ptr.has_surdef_for type_2
            test_pf 1, ( val_0, val_1, val_2, args... ) => @call_3.apply this, [].concat( [ val_0, val_1, Method.methods.prox_ptr.call_1 val_2 ], args )

        # summary
        if disp
            if best_func.length == 0
                return console.error "impossible to find surdef for #{@name} (with types #{type_0.name}, #{type_1.name} and #{type_2.name})"
            if best_func.length >= 2
                console.error "Ambiguous overload for #{@name} (with types #{type_0.name}, #{type_1.name} and #{type_2.name})"
        if want_pert
            best_pert
        else
            best_func[ 0 ]


# declaration of methods
Method.decl "to_String"        , "c" # returns a native String
Method.decl "to_Number"        , "c" # returns a native Number
Method.decl "to_Array"         , "c" # returns a native Array
Method.decl "to_Inode"         , "c" # returns an Inode object
Method.decl "prox_ptr"         , "r" # returns a Variable, which can be modified
Method.decl "prox_ref"         , "c" # returns a Variable, which should not be modified
Method.decl "prox_val"         , "c" # returns a computed Variable
Method.decl "copy"             , "c" # returns a Variable
Method.decl "make_ot_wrapper"  , "c" # returns an OtWrapper
Method.decl "new_ot_merger"    , "c" # returns an OtMerger
Method.decl "pull_ot_changes"  , "r" # takes a BinaryWriter as arg
Method.decl "write_to"         , "c" # takes a BinaryWriter as arg
Method.decl "new_cursor"       , "r" # return a loc_id (PI64))
Method.decl "get_cursor"       , "c" # + DevID + loc_id (PI64)
Method.decl "rem_cursor"       , "r" # + DevID + loc_id (PI64)
Method.decl "get_cursor_ids"   , "c"
Method.decl "mod_select"       , "rc"
Method.decl "rem_item"         , "rc" # remove item (e.g. using a key)
Method.decl "obj_type"         , "c" # variable type (ex: GenString -> String)
Method.decl "key_type"         , "c"
Method.decl "val_type"         , "c"

Method.decl "inf_immediate"    , "cc"
Method.decl "sup_immediate"    , "cc"
Method.decl "equ_immediate"    , "cc"

Method.decl "get_size"         , "c" # return a Variable (probably a Number)

Method.dstd "select"           , 2

Method.dslf "not"              , 1
Method.dslf "neg"              , 1

Method.dslf "set"              , 2
Method.dslf "add"              , 2
Method.dslf "sub"              , 2
Method.dslf "mul"              , 2
Method.dslf "div"              , 2

Method.dstd "is_pos"           , 1 # true if strictly positive
Method.dstd "is_neg"           , 1 # true if strictly negative
Method.dstd "is_eqz"           , 1 # true if null
Method.dstd "is_equ"           , 2 # true if equal
Method.dstd "is_inf"           , 2
Method.dstd "is_infeq"         , 2

Method.dslf "remove"           , 3
Method.dslf "insert"           , 3

Method.dslf "on_change"        , 1 # return an id, that can be used to remove the callback
Method.decl "sig_change"       , "c" #
Method.dslf "rm_on_change"     , 1 # takes the id that has been used for `on_change`
Method.dslf "on_change_par"    , 1 # add a parent object for ob_change (if this is changed, sig_change will be called on this parent). Optionnal argument: something that will be sent to parent sig_change

Method.decl "bind_by_path"     , "c" # path , db, category
Method.decl "bind_by_object_id", "c" # inode, db, category

# for n, m of Method.methods
    # console.log m.name
require "./DefaultMethods"
