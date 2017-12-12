#pragma once

namespace RiotBase {

struct Flush {
    template<class Bq>
    void write_to( Bq bq ) const {
        bq.buf->flush();
    }
};

constexpr Flush flush;

} // namespace RiotBase
