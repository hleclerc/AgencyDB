#pragma once

#include <QtWidgets/QAbstractScrollArea>
#include <QtCore/QMap>
#include "../Variable/String.h"
#include "WordSep.h"

namespace RiotBase {

///
class TextEdit : public QAbstractScrollArea {
public:
    TextEdit( String *str, QWidget *parent = 0 );
    ~TextEdit();

protected:
    struct CursorView { int num_color; bool used; };
    using CVMap = QMap<CursorData::Id,CursorView>;

    virtual void paintEvent     ( QPaintEvent *event );
    virtual void mousePressEvent( QMouseEvent *event );
    virtual void keyPressEvent  ( QKeyEvent   *event );
    virtual void focusOutEvent  ( QFocusEvent *event );
    virtual void focusInEvent   ( QFocusEvent *event );
    void         blink_cursor   ();

    String      *str;           ///< text to display
    Rp          *cursor;        ///< May be null. Cursor position.
    WordSep      word_sep;
    int          num_w;
    CVMap        cursor_views;  ///<
    bool         cursor_blink;
    QTimer      *timer_blink;
};

} // namespace RiotBase
