#pragma once

#include "Array.h"

namespace RiotBase {

///
class String : public VariableTempl<String> {
public:
    String( CreatorId, UsrId usr_id );                             ///<
    String( const std::string &a );                                ///< encoding is assumed to be utf8
    String( const char *a, PT len );                               ///< encoding is assumed to be utf8
    String( const String &a ) : String( RO_CALL( copy, a.rp ) ) {} ///<
    String( const char *a );                                       ///< encoding is assumed to be utf8
    String( DaSi a );                                              ///< encoding is assumed to be utf8
    String( char a );                                              ///< encoding is assumed to be utf8
    String( Rp *rp ) : VariableTempl<String>( rp ) {}              ///<
    String();

    operator          std::string   () const { std::string res; RO_CALL( to_std_string, rp, &res, 1 ); return res; }

    String            &operator=    ( const String &o ) { RO_CALL( self_set_b, rp, o.rp ); return *this; }
    String            &operator<<   ( const String &o ) { RO_CALL( self_add_b, rp, o.rp ); return *this; }
    String            &operator+=   ( const String &o ) { RO_CALL( self_add_b, rp, o.rp ); return *this; }
    String            &insert       ( const Number &pos, const String &str );
    String            &remove       ( const Number &pos, const Number &len );

    Number             size         () const { return RO_CALL( get_size, rp ); }
    Number             has_char     ( const String &char_list ) const { return RO_CALL( has_item_in, rp, char_list.rp ); }

    Array<String>      split        ( const String &sep = ' ' ) const { return RO_CALL( split_bb, rp, sep.rp ); }

    static std::string category_name();
};

template<class T>
String to_String( const T &val ) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

} // namespace RiotBase

RiotBase::String operator+ ( const RiotBase::String &a, const RiotBase::String &b );
RiotBase::String operator+ ( const RiotBase::String &a, const char             *b );
RiotBase::String operator||( const RiotBase::String &a, const RiotBase::String &b );
RiotBase::String operator||( const RiotBase::String &a, const char             *b );
