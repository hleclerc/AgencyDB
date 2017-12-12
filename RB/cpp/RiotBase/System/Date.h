#pragma once

#include "BinStream.h"

namespace RiotBase {

struct Date {
    Date( PI64 msec );
    Date();

    void        write_to_stream( std::ostream &os ) const;

    operator    double         () const;
    operator    bool           () const;

    Date        operator-      ( const Date &d ) const;
    Date        operator+      ( const Date &d ) const;

    bool        operator>=     ( const Date &d ) const;
    bool        operator<      ( const Date &d ) const;
    bool        operator!=     ( const Date &d ) const;
    bool        operator==     ( const Date &d ) const;

    void        operator+=     ( const Date &d );
    void        operator-=     ( const Date &d );

    static Date current();

    template<class Cq>
    void        write_to       ( BinStream<Cq> &b ) const { b.write_unsigned( msec ); }

    template<class Cq>
    static Date read_from      ( BinStream<Cq> &b ) { PI64 msec = b.read_unsigned(); return msec; }

    // helper
    static void sleep          ( double duration ); // in sec

    PI64        msec;
};

} // namespace RiotBase

