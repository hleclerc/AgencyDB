#include "../System/Print.h"
#include "Slider.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_path /usr/lib/x86_64-linux-gnu
#pragma lib_name Qt5Widgets
#pragma lib_name Qt5Core
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

Slider::Slider( Number *n, QWidget *parent ) : QSlider( Qt::Horizontal, parent ), n( n ) {
    connect( this, SIGNAL(valueChanged(int)), this, SLOT(on_change(int)));
    n->on_change( [ this ]{ setValue( SI32( *this->n ) ); } );
}

void Slider::on_change( int v ) {
    *n = v;
}


} // namespace RiotBase
