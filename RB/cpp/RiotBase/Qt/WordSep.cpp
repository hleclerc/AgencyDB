#include <QtGui/QFontMetrics>
#include "WordSep.h"

namespace RiotBase {

void WordSep::update( const std::string &str ) {
    if ( ostr == str )
        return;
    ostr = str;

    items.clear();
    for( PT p = 0, o = 0; ; ++p ) {
        if ( p == str.size() ) {
            if ( o != p )
                items.push_back( ITEM_STRING, QString::fromLocal8Bit( &str[ o ], p - o ), o );
            break;
        }

        switch ( str[ p ] ) {
        case ' ':
            if ( o != p )
                items.push_back( ITEM_STRING, QString::fromLocal8Bit( &str[ o ], p - o ), o );
            items.push_back( ITEM_SPACE, QString{}, p );
            o = p + 1;
            break;
        case '\t':
            if ( o != p )
                items.push_back( ITEM_STRING, QString::fromLocal8Bit( &str[ o ], p - o ), o );
            items.push_back( ITEM_TAB, QString{}, p );
            o = p + 1;
            break;
        case '\r':
            if ( o != p )
                items.push_back( ITEM_STRING, QString::fromLocal8Bit( &str[ o ], p - o ), o );
            if ( p + 1 < str.size() and str[ p + 1 ] == '\n' )
                items.push_back( ITEM_CR, QString{}, p++ );
            o = p + 1;
            break;
        case '\n':
            if ( o != p )
                items.push_back( ITEM_STRING, QString::fromLocal8Bit( &str[ o ], p - o ), o );
            items.push_back( ITEM_CR, QString{}, p );
            o = p + 1;
            break;
        }
    }

    // say that the dimensions are not correct anymore
    for( int i = 0; i < nb_w; ++i )
        w[ i ] = 0;
}

void WordSep::set_font( const QFont &font, qreal w[ nb_w ], int tab_size ) {
    if ( this->font == font and same_w( w ) and this->tab_size == tab_size )
        return;
    this->font = font;
    this->tab_size = tab_size;
    for( int i = 0; i < nb_w; ++i )
        this->w[ i ] = w[ i ];

    QFontMetrics qm( font );
    ws = qm.width( ' ' ) + 5;
    hs = qm.height();

    QPointF pos[ nb_w ];
    for( int i = 0; i < nb_w; ++i )
        pos[ i ] = QPointF( 0, 0 );
    avg_font_height = 0;

    for( PT ni = 0; ni < items.size(); ++ni ) {
        Item &item = items[ ni ];

        switch( item.type ) {
        case ITEM_STRING: {
            QRect r = qm.boundingRect( item.data );
            avg_font_height += r.height();
            item.height = r.height();

            for( int i = 0; i < nb_w; ++i ) {
                if ( pos[ i ].x() + r.width() > w[ i ] and pos[ i ].x() ) {
                    pos[ i ].ry() += r.height();
                    pos[ i ].rx() = 0;
                }
                item.pos[ i ] = pos[ i ] + QPointF( 0, r.height() );
                pos[ i ].rx() += r.width();
            }
            break;
        }
        case ITEM_SPACE:
            for( int i = 0; i < nb_w; ++i ) {
                if ( pos[ i ].x() + ws > w[ i ] and pos[ i ].x() and ni and items[ ni - 1 ].type != ITEM_STRING ) {
                    pos[ i ].ry() += hs;
                    pos[ i ].rx() = 0;
                }
                item.pos[ i ] = pos[ i ] + QPointF( 0, hs );
                pos[ i ].rx() += ws;
            }
            break;
        case ITEM_TAB:
            for( int i = 0; i < nb_w; ++i ) {
                if ( pos[ i ].x() + tab_size * ws > w[ i ] and pos[ i ].x() and ni and items[ ni - 1 ].type != ITEM_STRING ) {
                    pos[ i ].ry() += hs;
                    pos[ i ].rx() = 0;
                }
                item.pos[ i ] = pos[ i ] + QPointF( 0, hs );
                pos[ i ].rx() += tab_size * ws;
            }
            break;
        case ITEM_CR:
            for( int i = 0; i < nb_w; ++i ) {
                item.pos[ i ] = pos[ i ] + QPointF( 0, hs );
                pos[ i ].ry() += hs;
                pos[ i ].rx() = 0;
            }
            break;
        }
    }

    avg_font_height /= items.size();
}

int WordSep::height( int num_w ) const {
    return items.size() ? items.back().pos[ num_w ].y() : 0;
}

bool WordSep::same_w( const qreal a[ nb_w ] ) const {
    for( int i = 0; i < nb_w; ++i )
        if ( w[ i ] != a[ i ] )
            return false;
    return true;
}

QRect WordSep::get_pos( PT num_char, int num_w ) const {
    // TODO: log(n) search
    if ( items.size() ) {
        for( PT i = 1; ; ++i ) {
            if ( i == items.size() or items[ i ].num_char > num_char ) {
                QFontMetrics qm( font );
                const Item &item = items[ i - 1 ];
                return QRect( item.pos[ num_w ].x() + qm.width( item.data.left( num_char - item.num_char ) ), item.pos[ num_w ].y() - qm.ascent(), 0, qm.ascent() + qm.descent() );
            }
        }
    }
    return QRect();
}

PT WordSep::num_char_at( QPoint pos, int num_w ) const {
    // TODO: log(n) search
    if ( items.size() ) {
        QFontMetrics qm( font );
        for( PT i = 1; ; ++i ) {
            if ( i != items.size() ) {
                qreal bx = items[ i ].pos[ num_w ].x(), by = items[ i ].pos[ num_w ].y() - qm.ascent(), ey = items[ i ].pos[ num_w ].y() + qm.descent();
                if ( pos.y() >= by and ( pos.y() < by or pos.y() >= ey or pos.x() >= bx ) )
                    continue;
            }
            if ( i == items.size() ) {
                qreal ey = items.back().pos[ num_w ].y() + qm.descent();
                if ( pos.y() >= ey )
                    return items.back().num_char_end();
            }
            const Item &item = items[ i - 1 ];
            qreal d = pos.x() - item.pos[ num_w ].x();
            switch ( item.type ) {
            case ITEM_STRING: {
                int os = 0;
                for( int n = 1; n <= item.data.size(); ++n ) {
                    int ns = qm.width( item.data.left( n ) );
                    if ( ns > d )
                        return item.num_char + n - ( d <= ( os + ns ) / 2 );
                    os = ns;
                }
                return item.num_char + item.data.size();
            }
            case ITEM_SPACE:
                return item.num_char + ( d > ws / 2 and ( i == items.size() or items[ i - 1 ].pos[ num_w ].y() == items[ i ].pos[ num_w ].y() ) );
            case ITEM_TAB:
                return item.num_char + ( d > tab_size * ws / 2 and ( i == items.size() or items[ i - 1 ].pos[ num_w ].y() == items[ i ].pos[ num_w ].y() ) );
            case ITEM_CR:
                return item.num_char;
            }
        }
    }
    return 0;
}

} // namespace RiotBase
