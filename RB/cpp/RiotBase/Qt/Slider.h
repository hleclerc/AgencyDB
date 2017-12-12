#pragma once

#include "../Variable/Number.h"
#include <QtWidgets/QSlider>

namespace RiotBase {

///
class Slider  : public QSlider {
    Q_OBJECT
public:
    Slider( Number *n, QWidget *parent = 0 );

private slots:
    void on_change( int v );

protected:
    Number *n;
};

} // namespace RiotBase
