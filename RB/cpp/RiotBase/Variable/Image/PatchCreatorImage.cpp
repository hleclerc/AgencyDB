#include "PatchCreatorImage.h"
#include "GenImage.h"

namespace RiotBase {

PatchCreatorImage::PatchCreatorImage( Rp *src ) : PatchCreator<PatchCreatorImage>( src ) {
}

std::string PatchCreatorImage::type_name() {
    return "PatchCreatorImage";
}

void PatchCreatorImage::self_special_func( const char *func, Rp **parms, int nb_args ) {
    if ( strcmp( func, "apply_brush" ) == 0 ) {
        if ( nb_args != 2 ) { WARNING( "Expecting 2 args" ); return; }
        if ( parms[ 0 ]->type_num != type_num_of<Tuple<double,double>>() ) { WARNING( "Expecting a Tuple<double,double> as 1st arg" ); return; }
        Tuple<double,double> *t  = scast( parms[ 0 ] );
        double                bx = std::get<0>( t->val );
        double                by = std::get<1>( t->val );

        reg_op( pcd.bo, apply_brush_1pix(), pmult * bx, pmult * by );
        RO_CALL( self_special_func, src, "apply_brush_raw", parms, nb_args );
    }
}

void PatchCreatorImage::apply_brush_1pix::apply( PatchCreatorImage *pc, PT x, PT y ) const {
    AutoRpDel t = Tuple<double,double>::New( x / double( pmult ), y / double( pmult ) );
    AutoRpDel i = GenImage<boost::gil::rgba8_image_t>::New( 1, 1, boost::gil::rgba8_pixel_t( 0, 0, 0, 255 ) );
    Rp *parms[] = { t.rp, i.rp };
    RO_CALL( self_special_func, pc->src, "apply_brush_raw", parms, 2 );
}

} // namespace RiotBase
