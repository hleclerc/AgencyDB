#define DECL_BT( DT0 ) \
    RiotBase_DEF_FUNC_2( equ_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _equ   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( neq_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _neq   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( inf_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _inf   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( sup_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _sup   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( inf_eq_bb    , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _inf_eq( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( sup_eq_bb    , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _sup_eq( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    \
    RiotBase_DEF_FUNC_2( add_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _add   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( sub_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _sub   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( mul_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _mul   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( div_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _div   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    RiotBase_DEF_FUNC_2( bor_bb       , 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return mBn( _bor   ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ) ); } \
    \
    RiotBase_DEF_FUNC_2( equ_immediate, 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return _equ( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ); } \
    RiotBase_DEF_FUNC_2( inf_immediate, 1, tv0->isa<Bn<DT0>>() and tv1->isa<Bn<DT1>>() ) { return _inf( static_cast<const Bn<DT0> *>( v0 )->val, static_cast<const Bn<DT1> *>( v1 )->val ); } \

#include "DeclBnNumberTypes.h"

#undef DECL_BT
