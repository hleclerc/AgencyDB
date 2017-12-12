#pragma once

#include "GenImage.h"

namespace RiotBase {

///
template<class GilImageType>
class GenImageSelect : public RpTempl<GenImageSelect<GilImageType>> {
public:
    using GI = GenImage<GilImageType>;

    GenImageSelect( GI *img, PT x, PT y, PT w, PT h ) : img( img ), x( x ), y( y ), w( w ), h( h ) {
        // TODO: check validity img during the life of an GenImageSelect
    }

    static std::string type_name      () { return "GenImageSelect"; }
    Rp                *prox_val       () const { GI *res = GI::New( w, h ); boost::gil::copy_pixels( res->img._view, boost::gil::subimage_view( img->img._view, x, y, w, h ) ); return res; }

    static void        _self_set_b_img( Rp *&_sel, const Rp *_img );

    static void        init_static_attributes();

    GenImage<GilImageType> *img;
    PT x, y, w, h;
};

template<class GilImageType>
void GenImageSelect<GilImageType>::_self_set_b_img( Rp *&_sel, const Rp *_img ) {
    GenImageSelect               *sel = scast( _sel );
    const GenImage<GilImageType> *img = scast( _img );
    boost::gil::copy_pixels(
        img->img._view,
        boost::gil::subimage_view( sel->img->img._view, sel->x, sel->y, sel->w, sel->h )
    );
}

template<class GilImageType>
void GenImageSelect<GilImageType>::init_static_attributes() {
    RpTempl<GenImageSelect<GilImageType>>::init_static_attributes();
    RiotBase_REG_FUNC_2( self_set_b, 2, tv0->isa<GenImageSelect<GilImageType>>() and tv1->isa<GenImage<GilImageType>>(), _self_set_b_img );
}

} // namespace RiotBase
