#pragma once

#include "../../System/BinStream.h"
#include "../../System/TypeInfo.h"
#include "../../System/Scast.h"
#include "../Core/Error.h"
#include "../Number/Bn.h"
#include <tuple>

namespace RiotBase {

///
template<class... T>
class Tuple : public RpTempl<Tuple<T...>> {
public:
    using TT = std::tuple<T...>;
    enum { term_value = true, atomic_value = true, st = std::tuple_size<TT>::value };

    template<class... A>
    Tuple( A&& ...val ) : val( std::forward<A>( val )... ) {}


    static std::string         type_name      () { std::string res; _disp_type( res, N<st>() ); return res + ">"; }
    //    static const Category *static_category() { return Number::category; }
    //    const Category        *category       () const { return Number::category; }
    void                       write_to_stream( std::ostream &os ) const { _disp( os, N<st>() ); }
    //    Rp                    *copy           () const { return Bn::New( val ); }

    template<PT n> void        _disp          ( std::ostream &os, N<n> ) const { _disp( os << ( n < st ? "," : "" ) << std::get<n-1>( val ), N<n-1>() ); }
    void                       _disp          ( std::ostream &os, N<0> ) const {}

    template<PT n> static void _disp_type     ( std::string &os, N<n> ) { os += n < st ? "," : "Tuple<"; _disp_type( os += TypeInfo<typename std::tuple_element<n-1,TT>::type>::name(), N<n-1>() ); }
    static void                _disp_type     ( std::string &os, N<0> ) {}

    template<PT n> Rp *        _extract       ( PT ind, N<n> ) const { if ( ind == n - 1 ) return Bn<typename std::tuple_element<n-1,TT>::type>::New( std::get<n-1>( val ) ); return _extract( ind, N<n-1>() ); }
    Rp *                       _extract       ( PT ind, N<0> ) const { return Error::New( "Bad index" ); }

    static Rp                 *_select        ( const Rp *tup, const Rp *ind ) {
        PT val = 0; RO_CALL( to_PT, ind, &val, 1 );
        const Tuple *obj = scast( tup );
        return obj->_extract( val, N<st>() );
    }

    static void                init_static_attributes() {
        RpTempl<Tuple<T...>>::init_static_attributes();
        RiotBase_REG_FUNC_2( select_bb, 1, tv0->isa<Tuple>() and tv1->has_surdef_for( RF_to_PT() ), _select );
    }

    TT                         val;
};

} // namespace RiotBase
