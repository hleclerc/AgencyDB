#pragma once

#include "Core/VariableTempl.h"

namespace RiotBase {
class String;

///
class Number : public VariableTempl<Number> {
public:
    Number( Variable::CreatorId, UsrId usr_id );
    Number( const Number &n ) : Number( RO_CALL( copy, n.rp ) ) {}
    Number( Rp *rp ) : VariableTempl<Number>( rp ) {}
    Number();

    #define DECL_BT( DT ) Number( DT val );
    #include "Number/DeclBnNumberTypes.h"
    #undef DECL_BT

    explicit operator bool() const { bool res = 0; RO_CALL( to_Bool, rp, &res, 1 ); return res; }
    explicit operator SI32() const { SI32 res = 0; RO_CALL( to_SI32, rp, &res, 1 ); return res; }
    explicit operator PT  () const { PT   res = 0; RO_CALL( to_PT  , rp, &res, 1 ); return res; }

    Number   operator-    () const { return RO_CALL( neg_b, rp ); }

    Number  &operator=    ( const Number &o ) { RO_CALL( self_set_b, rp, o.rp ); return *this; }
    Number  &operator+=   ( const Number &o ) { RO_CALL( self_add_b, rp, o.rp ); return *this; }
    Number  &operator-=   ( const Number &o ) { RO_CALL( self_sub_b, rp, o.rp ); return *this; }
    Number  &operator*=   ( const Number &o ) { RO_CALL( self_mul_b, rp, o.rp ); return *this; }
    Number  &operator/=   ( const Number &o ) { RO_CALL( self_div_b, rp, o.rp ); return *this; }

    static std::string category_name();
};

inline Number operator+ ( const Number &v0, const Number &v1 ) { return RO_CALL( add_bb   , v0.rp, v1.rp ); }
inline Number operator- ( const Number &v0, const Number &v1 ) { return RO_CALL( sub_bb   , v0.rp, v1.rp ); }
inline Number operator* ( const Number &v0, const Number &v1 ) { return RO_CALL( mul_bb   , v0.rp, v1.rp ); }
inline Number operator/ ( const Number &v0, const Number &v1 ) { return RO_CALL( div_bb   , v0.rp, v1.rp ); }
inline Number operator||( const Number &v0, const Number &v1 ) { return RO_CALL( bor_bb   , v0.rp, v1.rp ); }
                                                                                          
inline Number operator!=( const Number &v0, const Number &v1 ) { return RO_CALL( neq_bb   , v0.rp, v1.rp ); }
inline Number operator==( const Number &v0, const Number &v1 ) { return RO_CALL( equ_bb   , v0.rp, v1.rp ); }
inline Number operator< ( const Number &v0, const Number &v1 ) { return RO_CALL( inf_bb   , v0.rp, v1.rp ); }
inline Number operator> ( const Number &v0, const Number &v1 ) { return RO_CALL( sup_bb   , v0.rp, v1.rp ); }
inline Number operator<=( const Number &v0, const Number &v1 ) { return RO_CALL( inf_eq_bb, v0.rp, v1.rp ); }
inline Number operator>=( const Number &v0, const Number &v1 ) { return RO_CALL( sup_eq_bb, v0.rp, v1.rp ); }

inline Number as_a_Variable( Bool v ) { return v; }

inline Number as_a_Variable( PI8  v ) { return v; }
inline Number as_a_Variable( PI16 v ) { return v; }
inline Number as_a_Variable( PI32 v ) { return v; }
inline Number as_a_Variable( PI64 v ) { return v; }

inline Number as_a_Variable( SI8  v ) { return v; }
inline Number as_a_Variable( SI16 v ) { return v; }
inline Number as_a_Variable( SI32 v ) { return v; }
inline Number as_a_Variable( SI64 v ) { return v; }

inline Number as_a_Variable( FP32 v ) { return v; }
inline Number as_a_Variable( FP64 v ) { return v; }

String as_a_Variable( const char        *v );
String as_a_Variable( const std::string &v );
String as_a_Variable( char v               );

} // namespace RiotBase
