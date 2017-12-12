#pragma once

#include "../../System/BinStream.h"
#include "../../System/TypeInfo.h"
#include "../../System/Convert.h"
#include "../../System/IsSame.h"
#include "../Core/TupleTypes.h"
#include "../Core/RpAndUse.h"
#include "../Core/RpTempl.h"
#include "OtWrapperNumber.h"
#include "../Number.h"
#include <tuple>

namespace RiotBase {
// Rp *PatchCreatorNumber_New( Rp *rp );

///
template<class... T>
class Bn : public RpTempl<Bn<T...>> {
public:
    enum { term_value = true, atomic_value = true };

    Bn( T... val ) : val( val... ) {}

    template<class Cq>
    Bn( BinStream<Cq> bq ) : val( bq.read() ) {}

    static std::string     type_name      () { return "Bn<" + TypeInfo<T...>::name() + ">"; }
    static const Category *static_category() { return Number::category; }
    const Category        *category       () const { return Number::category; }
    void                   write_to_stream( std::ostream &os ) const { os << val; }
    Rp                    *copy           () const { return Bn::New( val ); }
<<<<<<< HEAD:src/RiotBase/Variable/Number/Bn.h
    bool                   to_Bool        ( Bool *res, PT nb_items ) const { return nb_items == 1 and convert( *res, std::get<0>( val ) ); }
    bool                   to_SI32        ( SI32 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, std::get<0>( val ) ); }
    bool                   to_PI32        ( PI32 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, std::get<0>( val ) ); }
    bool                   to_SI64        ( SI64 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, std::get<0>( val ) ); }
    bool                   to_PI64        ( PI64 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, std::get<0>( val ) ); }
    bool                   to_PT          ( PT   *res, PT nb_items ) const { return nb_items == 1 and convert( *res, std::get<0>( val ) ); }
    void                   get_raw_data   ( CbQueue &cq, DevId cur_dev, bool od ) const;
    void                   set_raw_data   ( const CbString &cm, DevId cur_dev );
    // bool                 default_cat_val_b() const { return IsSame<T,SI32>::v and not val;  }

    std::tuple<T...>       val;

};

template<class... T>
void Bn<T...>::get_raw_data( CbQueue &cq, DevId cur_dev, bool od ) const {
    if ( od and val == 0 ) // IsSame<T,SI32>::v and
        return;
    BinStream<CbQueue> bq( &cq );
    bq << TypeInfo<T...>::cat_id();
    bq.write_some( &val, sizeof( val ) );
}

template<class... T>
void Bn<T...>::set_raw_data( const CbString &cm, DevId cur_dev ) {
    CbString cp = cm;
    BinStream<CbString> bq( &cp );
    unsigned cat_id = bq.read_unsigned();
    if ( bq.error() )
        return;
    if ( cat_id != TypeInfo<T...>::cat_id() )
        IMPORTANT_TODO;
    bq.read_some( &val, sizeof( val ) );
}
=======
    Rp                    *neg            () const;
    bool                   to_Bool        ( Bool *res, PT nb_items ) const { return nb_items == 1 and convert( *res, val ); }
    bool                   to_SI32        ( SI32 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, val ); }
    bool                   to_PI32        ( PI32 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, val ); }
    bool                   to_SI64        ( SI64 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, val ); }
    bool                   to_PI64        ( PI64 *res, PT nb_items ) const { return nb_items == 1 and convert( *res, val ); }
    bool                   to_PT          ( PT   *res, PT nb_items ) const { return nb_items == 1 and convert( *res, val ); }
    void                   serialize      ( BBQ bq, const PT *num_children ) const { bq << this->num_in_tuple_types() << val; }
    static Rp             *deserialize    ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children ) { return Bn::New( bq ); }
    Rp                    *make_ot_wrapper() { return OtWrapperNumber::New( this ); }

    T                      val;
};

// create a Bn from a value
template<class T> Bn<T> *mBn( T val ) { return Bn<T>::New( val ); }

// unsigned versions
template<class R> struct UnsignedCp { using T = R; };
template<> struct UnsignedCp<SI8 > { using T = PI8 ; };
template<> struct UnsignedCp<SI16> { using T = PI16; };
template<> struct UnsignedCp<SI32> { using T = PI32; };
template<> struct UnsignedCp<SI64> { using T = PI64; };

// signed versions
template<class R> struct SignedCp { using T = R; };
template<> struct SignedCp<PI8 > { using T = SI8 ; };
template<> struct SignedCp<PI16> { using T = SI16; };
template<> struct SignedCp<PI32> { using T = SI32; };
template<> struct SignedCp<PI64> { using T = SI64; };

template<class T>
Rp *Bn<T>::neg() const { return - val > 0 ? Bn::New( - typename SignedCp<T>::T{ val } ) : Bn<T>::New( - val ) ; } // TODO: check if signed version is large enough...
>>>>>>> 3a38d0495062b156475c66701512e314eb4760ac:cpp/RiotBase/Variable/Number/Bn.h

// SI32, PI32, SI64, PI64

} // namespace RiotBase
