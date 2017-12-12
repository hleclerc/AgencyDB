/// This file is generated (by bin/Method_template.py). Do not modify it directly.
#pragma once

#include "../Core/Category.h"
#include "../Core/Methods.h"

namespace RiotBase {

struct Op_equ {
    enum { nb_args = 2 };
    static const char *name() { return "equ"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( equ_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_neq {
    enum { nb_args = 2 };
    static const char *name() { return "neq"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( neq_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_inf {
    enum { nb_args = 2 };
    static const char *name() { return "inf"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( inf_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_sup {
    enum { nb_args = 2 };
    static const char *name() { return "sup"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( sup_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_inf_eq {
    enum { nb_args = 2 };
    static const char *name() { return "inf_eq"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( inf_eq_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_sup_eq {
    enum { nb_args = 2 };
    static const char *name() { return "sup_eq"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( sup_eq_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_bor {
    enum { nb_args = 2 };
    static const char *name() { return "bor"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( bor_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_select {
    enum { nb_args = 2 };
    static const char *name() { return "select"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( select_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_neg {
    enum { nb_args = 1 };
    static const char *name() { return "neg"; } 
    Rp *exec_b( const Rp *v0 ) const { return RO_CALL( neg_b, v0 ); }
    static const Category *category( const Rp *v0 ) { return RO_CALL( category, v0 ); }
};
struct Op_split {
    enum { nb_args = 2 };
    static const char *name() { return "split"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( split_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_join {
    enum { nb_args = 2 };
    static const char *name() { return "join"; } 
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( join_bb, v0, v1 ); }
    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_bind_by_kn_object_id {
    enum { nb_args = 1 };
    static const char *name() { return "bind_by_kn_object_id"; } 
    Rp *exec_b( const Rp *v0 ) const { return RO_CALL( bind_by_kn_object_id_b, v0, cat ); }
    static const Category *category( const Rp *v0 ) { return RO_CALL( category, v0 ); }
    const Category *cat;
};

struct Op_set {
    enum { nb_args = 2 };
    static const char *name() { return "set"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_set_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( set_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_set_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_add {
    enum { nb_args = 2 };
    static const char *name() { return "add"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_add_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( add_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_add_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_sub {
    enum { nb_args = 2 };
    static const char *name() { return "sub"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_sub_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( sub_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_sub_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_mul {
    enum { nb_args = 2 };
    static const char *name() { return "mul"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_mul_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( mul_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_mul_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_div {
    enum { nb_args = 2 };
    static const char *name() { return "div"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_div_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( div_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_div_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_mod {
    enum { nb_args = 2 };
    static const char *name() { return "mod"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_mod_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( mod_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_mod_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_add_key {
    enum { nb_args = 2 };
    static const char *name() { return "add_key"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_add_key_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( add_key_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_add_key_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_rem_key {
    enum { nb_args = 2 };
    static const char *name() { return "rem_key"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_rem_key_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( rem_key_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_rem_key_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_push_back {
    enum { nb_args = 2 };
    static const char *name() { return "push_back"; } 
    Rp *exec_sb( Rp *v0, const Rp *v1 ) const { RO_CALL( self_push_back_b, v0, v1 ); return v0; }
    Rp *exec_bb( const Rp *v0, const Rp *v1 ) const { return RO_CALL( push_back_bb, v0, v1 ); }
    Rp *exec_sb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 2 + i ] ) ); RO_CALL( self_push_back_b, sel, ch[ 1 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1 ) { return RO_CALL( category, v0 ); }
};
struct Op_insert {
    enum { nb_args = 3 };
    static const char *name() { return "insert"; } 
    Rp *exec_sbb( Rp *v0, const Rp *v1, const Rp *v2 ) const { RO_CALL( self_insert_bb, v0, v1, v2 ); return v0; }
    Rp *exec_bbb( const Rp *v0, const Rp *v1, const Rp *v2 ) const { return RO_CALL( insert_bbb, v0, v1, v2 ); }
    Rp *exec_sbb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 3 + i ] ) ); RO_CALL( self_insert_bb, sel, ch[ 1 ], ch[ 2 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1, const Rp *v2 ) { return RO_CALL( category, v0 ); }
};
struct Op_remove {
    enum { nb_args = 3 };
    static const char *name() { return "remove"; } 
    Rp *exec_sbb( Rp *v0, const Rp *v1, const Rp *v2 ) const { RO_CALL( self_remove_bb, v0, v1, v2 ); return v0; }
    Rp *exec_bbb( const Rp *v0, const Rp *v1, const Rp *v2 ) const { return RO_CALL( remove_bbb, v0, v1, v2 ); }
    Rp *exec_sbb_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 3 + i ] ) ); RO_CALL( self_remove_bb, sel, ch[ 1 ], ch[ 2 ] ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0, const Rp *v1, const Rp *v2 ) { return RO_CALL( category, v0 ); }
};
struct Op_on_change {
    enum { nb_args = 1 };
    static const char *name() { return "on_change"; } 
    Rp *exec_s( Rp *v0 ) const { RO_CALL( self_on_change, v0, c ); return v0; }
    Rp *exec_b( const Rp *v0 ) const { return RO_CALL( on_change_b, v0, c ); }
    Rp *exec_s_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 1 + i ] ) ); RO_CALL( self_on_change, sel, c ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0 ) { return RO_CALL( category, v0 ); }
    Callback *c;
};
struct Op_then {
    enum { nb_args = 1 };
    static const char *name() { return "then"; } 
    Rp *exec_s( Rp *v0 ) const { RO_CALL( self_then, v0, c ); return v0; }
    Rp *exec_b( const Rp *v0 ) const { return RO_CALL( then_b, v0, c ); }
    Rp *exec_s_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 1 + i ] ) ); RO_CALL( self_then, sel, c ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0 ) { return RO_CALL( category, v0 ); }
    Callback *c;
    bool done = false;
};
struct Op_make_inode_if_z {
    enum { nb_args = 1 };
    static const char *name() { return "make_inode_if_z"; } 
    Rp *exec_s( Rp *v0 ) const { RO_CALL( self_make_inode_if_z, v0, cat ); return v0; }
    Rp *exec_b( const Rp *v0 ) const { return RO_CALL( make_inode_if_z_b, v0, cat ); }
    Rp *exec_s_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 1 + i ] ) ); RO_CALL( self_make_inode_if_z, sel, cat ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0 ) { return RO_CALL( category, v0 ); }
    const Category *cat;
};
struct Op_special_func {
    enum { nb_args = 1 };
    static const char *name() { return "special_func"; } 
    Rp *exec_s( Rp *v0 ) const { RO_CALL( self_special_func, v0, func, parm, nb_parms ); return v0; }
    Rp *exec_b( const Rp *v0 ) const { return RO_CALL( special_func_b, v0, func, parm, nb_parms ); }
    Rp *exec_s_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ 1 + i ] ) ); RO_CALL( self_special_func, sel, func, parm, nb_parms ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( const Rp *v0 ) { return RO_CALL( category, v0 ); }
    const char *func;
    Rp **parm;
    int nb_parms;
};

} // namespace RiotBase
