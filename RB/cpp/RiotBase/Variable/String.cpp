#include "String/OtWrapperString.h"
#include "String/GenString.h"
#include "Symbol/Symbol.h"
#include "Core/Surdef.h"
#include "String.h"

namespace RiotBase {

String::String( CreatorId, UsrId usr_id ) {
    rp = GenString::New( nullptr, 0, GenString::prereservation_size );
    if ( usr_id )
        rp = OtWrapperString::New( rp, usr_id );
}

String::String( const char *a, PT len ) : String( GenString::New( a, len, len ) ) {
}

String::String( const std::string &a ) : String( &a.front(), a.size() ) {
}

String::String( const char *a ) : String( a, strlen( a ) ) {
}

String::String( DaSi a ) : String( a.data, a.size ) {
}

String::String( char a ) : String( &a, 1 ) {
}

String::String() : String( GenString::New( (const char *)nullptr, 0, 0 ) ) {
}

std::string String::category_name() {
    return "String";
}

String &String::insert( const Number &pos, const String &str ) {
    RO_CALL( self_insert_bb, rp, pos.rp, str.rp );
    return *this;
}

String &String::remove( const Number &pos, const Number &len ) {
    RO_CALL( self_remove_bb, rp, pos.rp, len.rp );
    return *this;
}

String as_a_Variable( const char        *v ) { return v; }
String as_a_Variable( const std::string &v ) { return v; }
String as_a_Variable( char v               ) { return v; }

} // namespace RiotBase

RiotBase::String operator+ ( const RiotBase::String &a, const RiotBase::String &b ) { return RO_CALL( add_bb, a.rp, b.rp ); }
RiotBase::String operator+ ( const RiotBase::String &a, const char             *b ) { return a + RiotBase::String( b ); }
RiotBase::String operator||( const RiotBase::String &a, const RiotBase::String &b ) { return RO_CALL( bor_bb, a.rp, b.rp ); }
RiotBase::String operator||( const RiotBase::String &a, const char             *b ) { return a || RiotBase::String( b ); }
