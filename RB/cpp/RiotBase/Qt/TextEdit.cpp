#include "../Variable/Number/Bn.h"
#include "../System/Print.h"
#include "TextEdit.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStyle>
#include <QtGui/QTextCursor>
#include <QtGui/QFocusEvent>
#include <QtGui/QPainter>
#include <QtCore/QTimer>

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_path /usr/lib/x86_64-linux-gnu
#pragma lib_name Qt5Widgets
#pragma lib_name Qt5Core
#pragma lib_name Qt5Gui
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

TextEdit::TextEdit( String *str, QWidget *parent ) : QAbstractScrollArea( parent ), str( str ), cursor( 0 ) {
    num_w = 0;

    str->on_change( [ this ]{
        if ( cursor ) {
            cursor_blink = 1;
            timer_blink->stop();
            timer_blink->start();
        }
        word_sep.update( *this->str );
        viewport()->update();
    } );
}

TextEdit::~TextEdit() {
    if ( cursor ) {
        RO_CALL( rem_cursor, str->rp, (void *)this );
        RO_CALL( del, cursor );
    }
}

void TextEdit::paintEvent( QPaintEvent *event ) {
    QAbstractScrollArea::paintEvent( event );
    QPainter painter( viewport() );

    if ( str->is_symbolic() ) {
        QPainter painter( viewport() );
        painter.drawText( viewport()->rect(), "⟲", QTextOption( Qt::AlignHCenter | Qt::AlignVCenter ) );
        return;
    }

    // painter prep
    int sup_h = 6;
    QRectF vrect = viewport()->rect();
    QScrollBar *vscroll = verticalScrollBar();
    qreal w[ 2 ] = { qreal( width() - sup_h - qApp->style()->pixelMetric( QStyle::PM_ScrollBarExtent ) ), qreal( width() - sup_h ) };

    // text prep
    word_sep.set_font( painter.font(), w );
    num_w = word_sep.height( 1 ) + sup_h <= vrect.height();

    // scroll bar
    vscroll->setMaximum   ( num_w ? 0 : word_sep.height( 0 ) + sup_h - vrect.height() );
    vscroll->setPageStep  ( vrect.height() );
    vscroll->setSingleStep( word_sep.avg_font_height );

    // display text
    QPointF off( 0, vscroll->value() );
    for( WordSep::Item &item : word_sep.items )
        if ( item.type == WordSep::ITEM_STRING )
            painter.drawText( item.pos[ num_w ] - off, item.data );

    // main cursor
    if ( cursor and cursor_blink ) {
        PT pos_cursor = 0; RO_CALL( to_PT, cursor, &pos_cursor, 1 );
        QRect c = word_sep.get_pos( pos_cursor, num_w ).translated( 0, - off.y() );
        painter.setPen( QColor( 255, 255, 255 ) );
        painter.drawRect( c );
    }

    // external cursors
    Vec<CursorData> cd;
    RO_CALL( get_cursors, str->rp, cd );
    if ( cd.size() ) {
        for( CursorView &c_view : cursor_views )
            c_view.used = false;

        for( const CursorData &cursor : cd ) {
            if ( cursor.id.int_id == this )
                continue;

            // find the CursorView (in `iter`)
            auto iter = cursor_views.find( cursor.id );
            if ( iter == cursor_views.end() ) {
                int num_color = 0;
                for( auto p : cursor_views )
                    num_color = std::max( num_color, p.num_color + 1 );
                iter = cursor_views.insert( iter, cursor.id, CursorView{ num_color, true } );
            } else
                iter.value().used = true;

            // draw it (with the correct color)
            PT pos_cursor = 0; RO_CALL( to_PT, cursor.val, &pos_cursor, 1 );
            QRect c = word_sep.get_pos( pos_cursor, num_w ).translated( 0, - off.y() );
            painter.setPen( QColor::fromHsv( int( log( 1 + iter.value().num_color / 5.0 ) * 256 ) % 256, 255, 255 ) );
            painter.drawRect( c );
        }

        // rem unused cursor views
        Vec<CVMap::iterator> cv_to_rem;
        for( CVMap::iterator iter = cursor_views.begin(); iter != cursor_views.end(); ++iter )
            if ( not iter.value().used )
                cv_to_rem << iter;
        for( CVMap::iterator r : cv_to_rem )
            cursor_views.erase( r );
    }
}

void TextEdit::mousePressEvent( QMouseEvent *event ) {
    if ( not cursor ) {
        cursor = RO_CALL( new_cursor, str->rp, this );

        timer_blink = new QTimer( this );
        connect( timer_blink, &QTimer::timeout, this, &TextEdit::blink_cursor );
        timer_blink->start( 500 );

        cursor_blink = 1;
    }

    QScrollBar *vscroll = verticalScrollBar();
    QPoint off( 0, vscroll->value() );
    reinterpret_cast<Number &>( cursor ) = word_sep.num_char_at( event->pos() + off, num_w );
}

void TextEdit::keyPressEvent( QKeyEvent *event ) {
    switch ( event->key() ) {
    case Qt::Key_Left:
        // IMPORTANT_TODO: \r, unicode, ... (+= 1 won't work)
        if ( cursor and PT( reinterpret_cast<Number &>( cursor ) ) )
            reinterpret_cast<Number &>( cursor ) -= PT( 1 );
        break;
    case Qt::Key_Right:
        // IMPORTANT_TODO: \r, unicode, ... (-= 1 won't work)
        if( cursor and PT( reinterpret_cast<Number &>( cursor ) ) < str->size() )
            reinterpret_cast<Number &>( cursor ) += PT( 1 );
        break;
    case Qt::Key_Up:
        if ( cursor ) {
            QRect r = word_sep.get_pos( PT( reinterpret_cast<Number &>( cursor ) ), num_w ); r.translate( 0, - r.height() );
            if ( r.center().y() >= 0 )
                reinterpret_cast<Number &>( cursor ) = word_sep.num_char_at( r.center(), num_w );
        }
        break;
    case Qt::Key_Down:
        if ( cursor ) {
            QRect r = word_sep.get_pos( PT( reinterpret_cast<Number &>( cursor ) ), num_w ); r.translate( 0, r.height() );
            if ( r.center().y() < word_sep.height( num_w ) )
                reinterpret_cast<Number &>( cursor ) = word_sep.num_char_at( r.center(), num_w );
        }
        break;
    case Qt::Key_Backspace:
        if ( cursor and reinterpret_cast<Number &>( cursor ) )
            str->remove( reinterpret_cast<Number &>( cursor ) - 1, 1 );
        break;
    case Qt::Key_Delete:
        if ( cursor )
            str->remove( reinterpret_cast<Number &>( cursor ), 1 );
        break;
    case Qt::Key_Return:
        if ( cursor )
            str->insert( reinterpret_cast<Number &>( cursor ), "\n" );
        break;
    default:
        if ( cursor ) {
            std::string s = event->text().toStdString();
            if ( s.size() )
                str->insert( reinterpret_cast<Number &>( cursor ), s );
        }
        break;
    }


}

void TextEdit::focusInEvent( QFocusEvent *event ) {
    QAbstractScrollArea::focusInEvent( event );
}

void TextEdit::blink_cursor() {
    cursor_blink ^= 1;
    viewport()->update();
}

void TextEdit::focusOutEvent( QFocusEvent *event ) {
    QAbstractScrollArea::focusOutEvent( event );

    //    TabFocusReason,
    //    BacktabFocusReason,
    //    ,
    //    PopupFocusReason,
    //    ShortcutFocusReason,
    //    MenuBarFocusReason,
    //    OtherFocusReason,
    //    NoFocusReason

    //    if ( event->reason() != Qt::ActiveWindowFocusReason ) {
    //        PRINTL( event->reason() );
    //        s->rem_cursor( cursor );
    //    }
}

} // namespace RiotBase
