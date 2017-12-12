#include "Image/GenImage.h"
#include "Image.h"

namespace RiotBase {

Image::Image( PT w, PT h, PI8 r, PI8 g, PI8 b, PI8 a ) : Image( GenImage<boost::gil::rgba8_image_t>::New( w, h, boost::gil::rgba8_pixel_t( r, g, b, a ) ) ) {

}

Image::Image( PT w, PT h ) : Image( GenImage<boost::gil::rgba8_image_t>::New( w, h, boost::gil::rgba8_pixel_t( 255, 255, 255, 255 ) ) ) {
}

Image Image::make_pixel( PI8 r, PI8 g, PI8 b, PI8 a ) {
    return GenImage<boost::gil::rgba8_image_t>::New( 1, 1, boost::gil::rgba8_pixel_t( r, g, b, a ) );
}

Image &Image::operator=( const Image &o ) {
    RO_CALL( self_set_b, rp, o.rp );
    return *this;
}

Ref<Image> Image::operator()( PT x, PT y ) {
    AutoRpDel ind = Tuple<PT,PT>::New( x, y );
    return RO_CALL( mod_select, this->rp, ind.rp, {} );
}

void Image::apply_brush( double x, double y, const Image &brush ) {
    AutoRpDel c = Tuple<double,double>::New( x, y );
    Rp *args[] = { c.rp, brush.rp };
    RO_CALL( self_special_func, rp, "apply_brush", args, 2 );
}

const Ref<Image> Image::operator()( PT x, PT y ) const {
    AutoRpDel ind = Tuple<PT,PT>::New( x, y );
    return RO_CALL( select_bb, this->rp, ind.rp );
}

bool Image::get_data( PI32 *res, PT nb_items ) const {
    return RO_CALL( to_PI32, rp, res, nb_items );
}

std::string Image::category_name() {
    return "Image";
}


} // namespace RiotBase
