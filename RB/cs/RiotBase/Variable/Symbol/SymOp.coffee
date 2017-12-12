Method   = require "../Core/Method.js"
Rp       = require "../Core/Rp.js"
Lazyfier = require "./Lazyfier.js"

#
module.exports =
class SymOp extends Rp
    Rp.make_templ this

    init: ( name, args... ) ->
        @rp_args    = args.slice 0, args.length - 1
        @args       = args[ args.length - 1 ]
        @name       = name

    to_String: ->
        "#{@name}(#{( Method.methods.to_String.call_1( rp: a ) for a in @rp_args ).concat( @args )})"

    copy: ->
        ot = Method.methods.obj_type.call_1 rp: a
        new ot this


for n, m of Method.std_mts
    do ( n ) ->
        switch m.nb_rp_args
            when 1
                Method.methods[ n + "_b" ].add_surdef
                    test: ( type_0 ) -> type_0.unknown_term_value
                    func: ( v_0, args... ) -> Lazyfier.New SymOp.New n, Lazyfier.copy_rp_without_lazifier( v_0 ), args
                    pert: 1
            when 2
                Method.methods[ n + "_bb" ].add_surdef
                    test: ( type_0, type_1 ) -> type_0.unknown_term_value or type_1.unknown_term_value
                    func: ( v_0, v_1, args... ) -> Lazyfier.New SymOp.New n, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ), args
                    pert: 1
            when 3
                Method.methods[ n + "_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> type_0.unknown_term_value or type_1.unknown_term_value or type_2.unknown_term_value
                    func: ( v_0, v_1, v_2, args... ) -> Lazyfier.New SymOp.New n, Lazyfier.copy_rp_without_lazifier( v_0 ), Lazyfier.copy_rp_without_lazifier( v_1 ),  Lazyfier.copy_rp_without_lazifier( v_2 ), args
                    pert: 1
            else
                console.error "TODO"

# RiotBase_DEF_FUNC_2( push_back_bb, 0, FunctionTable_2<RF_self_push_back_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_push_back_b, res, v1 ); return res; }
# RiotBase_DEF_FUNC_2( push_back_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_push_back>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
# RiotBase_DEF_FUNC_2( push_back_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_push_back,true>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }
