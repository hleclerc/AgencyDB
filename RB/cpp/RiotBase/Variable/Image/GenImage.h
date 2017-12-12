#pragma once

#include "../../System/TypeInfo.h"
#include "../Array/Tuple.h"
#include "../Number/Bn.h"
#include "../Image.h"

#include <boost/gil/gil_all.hpp>
#include "PatchCreatorImage.h"

namespace RiotBase {

///
template<class GilImageType>
class GenImage : public RpTempl<GenImage<GilImageType>> {
public:
    using pixel_t = typename GilImageType::value_type;
    enum { term_value = true };

    // methods
    GenImage( PT w, PT h, pixel_t v ) : GenImage( w, h ) { boost::gil::fill_pixels( img._view, v ); }
    GenImage( PT w, PT h ) : img( w, h ) {}

    static std::string     type_name         () { return "GenImage<...>"; }
    static const Category *static_category   () { return Image::category; }
    const Category        *category          () const { return Image::category; }
    void                   write_to_stream   ( std::ostream &os ) const;
    Rp                    *copy              () const { GenImage *res = GenImage::New( img.width(), img.height() ); res->img = img; return res; }
    bool                   to_PI32           ( PI32 *res, PT nb_items ) const;
    Rp                    *new_patch_creator () { return PatchCreatorImage::New( this ); }

    static Rp             *_select_bb_tup    ( const Rp *_img, const Rp *_tup );
    static Rp             *_mod_select_tup   ( Rp *&_img, const Rp *_tup, UsrId as_usr );
    static void            _self_special_func( Rp *&_img, const char *func, Rp **parms, int nb_args );

    static void            init_static_attributes();

    // attributes
    GilImageType           img;
};

} // namespace RiotBase

#include "GenImageSelect.h"

namespace RiotBase {

template<class GilImageType>
void GenImage<GilImageType>::write_to_stream( std::ostream &os ) const {
    using namespace boost::gil;
    // PRINT( img.width() ); PRINT( (int)boost::gil::at_c<0>( img._view( 0, 0 ) ) );
    for( ssize_t y=0; y< img.height(); ++y ) {
        typename GilImageType::view_t::x_iterator img_xit = img._view.row_begin( y );
        for( ssize_t x = 0; x < img.width(); ++x )
            for( int c = 0; c < num_channels<GilImageType>::value; ++c )
                os << ( c ? " " : "" ) << (int)img_xit[ x ][ c ];
        os << "\n";
    }
}


template<class GilImageType>
bool GenImage<GilImageType>::to_PI32( PI32 *res, PT nb_items ) const {
    if ( nb_items != PT( img.width() * img.height() ) )
        return false;
    using namespace boost::gil;
    copy_pixels(
        color_converted_view<rgba8_pixel_t>( img._view ),
        interleaved_view( img.width(), img.height(), (rgba8_pixel_t *)res, 4 * img.width() )
    );
    return true;
}

template<class GilImageType>
Rp *GenImage<GilImageType>::_select_bb_tup(const Rp *_img, const Rp *_tup) {
    const GenImage     *img = scast( _img );
    const Tuple<PT,PT> *tup = scast( _tup );
    return GenImage::New( 1, 1, img->img._view( std::get<0>( tup->val ), std::get<1>( tup->val ) ) );
}

template<class GilImageType>
Rp *GenImage<GilImageType>::_mod_select_tup( Rp *&_img, const Rp *_tup, UsrId as_usr ) {
    const Tuple<PT,PT> *tup = scast( _tup );
    return GenImageSelect<GilImageType>::New( scast( _img ), std::get<0>( tup->val ), std::get<1>( tup->val ), 1, 1 );
}

template<class GilImageType>
void GenImage<GilImageType>::_self_special_func( Rp *&_img, const char *func, Rp **parms, int nb_args ) {
    GenImage *img = scast( _img );
    if ( strcmp( func, "apply_brush" ) == 0 ) {
        _img = img->new_patch_creator();
        RO_CALL( self_special_func, _img, func, parms, nb_args );
    } else if ( strcmp( func, "apply_brush_raw" ) == 0 ) {
        if ( nb_args != 2 ) { WARNING( "Expecting 2 args" ); return; }
        if ( parms[ 0 ]->type_num != type_num_of<Tuple<double,double>>() ) { WARNING( "Expecting a Tuple<double,double> as 1st arg" ); return; }
        if ( parms[ 1 ]->type_num != type_num_of<GenImage>() ) { WARNING( "Expecting a image of the same type as 2st arg" ); return; }
        Tuple<double,double> *t  = scast( parms[ 0 ] );
        GenImage             *b  = scast( parms[ 1 ] );
        double                bw = b->img.width ();
        double                bh = b->img.height();
        double                bx = std::get<0>( t->val ) - bw / 2.0;
        double                by = std::get<1>( t->val ) - bh / 2.0;
        for( PT y = std::max( by, 0.0 ); y < std::min( PT( by + bh + 2 ), PT( img->img.height() ) ); ++y ) {
            for( PT x = std::max( bx, 0.0 ); x < std::min( PT( bx + bw + 2 ), PT( img->img.width() ) ); ++x ) {
                double fx = x - bx + 1; // TODO: factorise
                double fy = y - by + 1;
                ST ix = fx; ix -= 1;
                ST iy = fy; iy -= 1;
                fx -= ix + 1;
                fy -= iy + 1;

                auto v = [&]( ST x, ST y ) { return x >= 0 and x < b->img.width() and y >= 0 and y < b->img.height() ? b->img._view( x, y ) : pixel_t( 0, 0, 0, 0 ); };
                pixel_t p_00 = v( ix + 0, iy + 0 );
                pixel_t p_10 = v( ix + 1, iy + 0 );
                pixel_t p_01 = v( ix + 0, iy + 1 );
                pixel_t p_11 = v( ix + 1, iy + 1 );

                double p[ 4 ]; // brush value
                for( int c = 0; c < 4; ++c )
                    p[ c ] = ( 1 - fx ) * ( 1 - fy ) * p_00[ c ] +
                             ( 0 + fx ) * ( 1 - fy ) * p_10[ c ] +
                             ( 1 - fx ) * ( 0 + fy ) * p_01[ c ] +
                             ( 0 + fx ) * ( 0 + fy ) * p_11[ c ];

                pixel_t &d   = img->img._view( x, y ); // ref on img value
                double   a_1 = p[ 3 ];
                using    pod = typename boost::gil::channel_type<typename GilImageType::value_type>::type;
                double   a_0 = std::min( double( d[ 3 ] ), boost::gil::channel_traits<pod>::max_value() - p[ 3 ] );
                for( int c = 0; c < 3; ++c )
                    d[ c ] = ( d[ c ] * a_0 + p[ c ] * a_1 ) / ( a_0 + a_1 );
                d[ 3 ] = a_0 + a_1;
            }
        }
    } else
        WARNING( "Uknown special_func" );
}

template<class GilImageType>
void GenImage<GilImageType>::init_static_attributes() {
    using TPP = Tuple<PT,PT>;
    RpTempl<GenImage<GilImageType>>::init_static_attributes();
    RiotBase_REG_FUNC_2( select_bb        , 1, tv0->isa<GenImage>() and tv1->isa<TPP>(), _select_bb_tup     );
    RiotBase_REG_FUNC_2( mod_select       , 1, tv0->isa<GenImage>() and tv1->isa<TPP>(), _mod_select_tup    );
    RiotBase_REG_FUNC_1( self_special_func, 1, tv0->isa<GenImage>()                    , _self_special_func );
}

} // namespace RiotBase
