#pragma once

#include "../System/Vec.h"
#include <QtGui/QFont>
#include <string>

namespace RiotBase {

///
class WordSep {
public:
    enum {
        ITEM_STRING,
        ITEM_SPACE,
        ITEM_TAB,
        ITEM_CR,

        nb_w = 2,
    };

    struct Item {
        Item( int type, QString data, PT num_char ) : type( type ), data( data ), num_char( num_char ) {}
        PT num_char_end() const { return num_char + data.size() + ( type != ITEM_STRING ); }

        int     type;
        QString data;
        PT      num_char;    ///< in the text (nb chars)

        QPointF pos[ nb_w ];
        int     height;
    };

    void        update      ( const std::string &str );
    void        set_font    ( const QFont &font, qreal w[ nb_w ], int tab_size = 4 );
    int         height      ( int num_w ) const;
    bool        same_w      ( const qreal a[ nb_w ] ) const;
    QRect       get_pos     ( PT num_char, int num_w ) const;
    PT          num_char_at ( QPoint pos, int num_w ) const;

    std::string ostr;
    Vec<Item>   items;
    int         w[ nb_w ], ws, hs, tab_size;
    QFont       font;
    qreal       avg_font_height;
};

} // namespace RiotBase
