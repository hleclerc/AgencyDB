#pragma once

#include "Core/VariableTempl.h"
#include "Core/AutoRpDel.h"
#include "Array/Tuple.h"

namespace RiotBase {

///
class Image : public VariableTempl<Image> {
public:
    Image( Rp *rp ) : VariableTempl<Image>( rp ) {}
    Image( PT w, PT h, PI8 r, PI8 g, PI8 b, PI8 a );
    Image( PT w = 0, PT h = 0 );

    static Image       make_pixel( PI8 r, PI8 g, PI8 b, PI8 a = 255 );

    Image             &operator= ( const Image &o );

    const Ref<Image>   operator()( PT x, PT y ) const;
    Ref<Image>         operator()( PT x, PT y );

    void               apply_brush( double x, double y, const Image &brush );

    bool               get_data  ( PI32 *res, PT nb_items ) const;

    static std::string category_name();
};

} // namespace RiotBase
