#include "Symbol/Symbol.h"
// #include "Number/PatchCreatorNumber.h"
#include "Number/Bn.h"
#include "String.h"

namespace RiotBase {

Number::Number() : Number( Bn<SI32>::New( 0 ) ) { }

Number::Number( Variable::CreatorId, UsrId usr_id ) {
    rp = Bn<SI32>::New( 0 );
    if ( usr_id )
        TODO; // OtWrapper
}

std::string Number::category_name() { return "Number"; }

#define DECL_BT( DT ) Number::Number( DT val ) : Number( Bn<DT>::New( val ) ) {}
#include "Number/DeclBnNumberTypes.h"
#undef DECL_BT

namespace {

// _equ and _neq variants
template<class A,class B> typename EnableIf<std::is_signed<A>::value   ==    std::is_signed<B>::value,bool>::T _equ( A a, B b ) { return a == b; }
template<class A,class B> typename EnableIf<std::is_signed<A>::value and not std::is_signed<B>::value,bool>::T _equ( A a, B b ) { return a >= 0 ? typename UnsignedCp<A>::T( a ) == b : false; }
template<class A,class B> typename EnableIf<not std::is_signed<A>::value and std::is_signed<B>::value,bool>::T _equ( A a, B b ) { return b >= 0 ? a == typename UnsignedCp<B>::T( b ) : false; }

template<class A,class B> bool _neq( A a, B b ) { return not _equ( a, b ); }

template<class A,class B> typename EnableIf<std::is_signed<A>::value   ==    std::is_signed<B>::value,bool>::T _inf( A a, B b ) { return a < b; }
template<class A,class B> typename EnableIf<std::is_signed<A>::value and not std::is_signed<B>::value,bool>::T _inf( A a, B b ) { return a >= 0 ? typename UnsignedCp<A>::T( a ) < b : true ; }
template<class A,class B> typename EnableIf<not std::is_signed<A>::value and std::is_signed<B>::value,bool>::T _inf( A a, B b ) { return b >= 0 ? a < typename UnsignedCp<B>::T( b ) : false; }

template<class A,class B> typename EnableIf<std::is_signed<A>::value   ==    std::is_signed<B>::value,bool>::T _sup( A a, B b ) { return a > b; }
template<class A,class B> typename EnableIf<std::is_signed<A>::value and not std::is_signed<B>::value,bool>::T _sup( A a, B b ) { return a >= 0 ? typename UnsignedCp<A>::T( a ) > b : false; }
template<class A,class B> typename EnableIf<not std::is_signed<A>::value and std::is_signed<B>::value,bool>::T _sup( A a, B b ) { return b >= 0 ? a > typename UnsignedCp<B>::T( b ) : true ; }

template<class A,class B> typename EnableIf<std::is_signed<A>::value   ==    std::is_signed<B>::value,bool>::T _inf_eq( A a, B b ) { return a <= b; }
template<class A,class B> typename EnableIf<std::is_signed<A>::value and not std::is_signed<B>::value,bool>::T _inf_eq( A a, B b ) { return a >= 0 ? typename UnsignedCp<A>::T( a ) <= b : true ; }
template<class A,class B> typename EnableIf<not std::is_signed<A>::value and std::is_signed<B>::value,bool>::T _inf_eq( A a, B b ) { return b >= 0 ? a <= typename UnsignedCp<B>::T( b ) : false; }

template<class A,class B> typename EnableIf<std::is_signed<A>::value   ==    std::is_signed<B>::value,bool>::T _sup_eq( A a, B b ) { return a >= b; }
template<class A,class B> typename EnableIf<std::is_signed<A>::value and not std::is_signed<B>::value,bool>::T _sup_eq( A a, B b ) { return a >= 0 ? typename UnsignedCp<A>::T( a ) >= b : false; }
template<class A,class B> typename EnableIf<not std::is_signed<A>::value and std::is_signed<B>::value,bool>::T _sup_eq( A a, B b ) { return b >= 0 ? a >= typename UnsignedCp<B>::T( b ) : true ; }

// arithmetic operations
template<class... T> std::tuple<T...> _add( A a, B b ) { return a == b; }



#define DT1 Bool
#include "Number/DefBnNumberBinops.h"
#undef DT1

#define DT1 SI32
#include "Number/DefBnNumberBinops.h"
#undef DT1

#define DT1 SI64
#include "Number/DefBnNumberBinops.h"
#undef DT1

#define DT1 PI64
#include "Number/DefBnNumberBinops.h"
#undef DT1

#define DT1 FP64
#include "Number/DefBnNumberBinops.h"
#undef DT1

} // namespace

} // namespace RiotBase
