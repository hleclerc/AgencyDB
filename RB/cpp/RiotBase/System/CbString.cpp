#include "CbStringPtr.h"
#include "Assert.h"
#include <string.h>

using namespace RiotBase;

CbString::CbString( const CbString &bs, PT b_off, PT b_len ) : beg( 0 ), off( 0 ), end( 0 ) {
    if ( not b_len )
        return;
    bs.visitor( [ this, &b_off, b_len ]( const Buffer *b, PT bd, PT ed ) {
        PT sd = ed - bd;
        // we have to take this buffer ?
        if ( b_off < sd ) {
            inc_ref( b );

            // first buffer in this ?
            if ( not beg ) {
                beg = b;
                off = bd + b_off;
                end = off + b_len;
            }

            // that was the last buffer ?
            if ( b_off + b_len <= sd )
                return false;
            b_off = 0;
        } else
            b_off -= sd;
        return true;
    } );
}

CbString::CbString( const CbString &bs ) : beg( bs.beg ), off( bs.off ), end( bs.end ) {
    bs.visitor( []( const Buffer *b, PT, PT ) {
        inc_ref( b );
        return true;
    } );
}

CbString::CbString( const CbStringPtr &bs, PT b_off, PT b_len ) : beg( 0 ), off( 0 ), end( 0 ) {
    if ( not b_len )
        return;
    bs.visitor( [ this, &b_off, b_len ]( const Buffer *b, PT bd, PT ed ) {
        PT sd = ed - bd;
        // we have to take this buffer ?
        if ( b_off < sd ) {
            inc_ref( b );

            // first buffer in this ?
            if ( not beg ) {
                beg = b;
                off = bd + b_off;
                end = off + b_len;
            }

            // that was the last buffer ?
            if ( b_off + b_len <= sd )
                return false;
            b_off = 0;
        } else
            b_off -= sd;
        return true;
    } );
}

CbString::CbString( const CbStringPtr &bs ) : beg( bs.beg ), off( bs.off ), end( bs.end ) {
    bs.visitor( []( const Buffer *b, PT, PT ) {
        inc_ref( b );
        return true;
    } );
}

CbString::CbString( CbString &&bs, PT off, PT len ) {
    TODO;
}

CbString::CbString( const CbQueue &bs, PT s_off, PT s_len ) : beg( bs.beg ), off( bs.off + s_off ), end( bs.off + std::min( bs.size(), s_off + s_len ) ) {
    if ( end == off )
        return;
    while ( off >= beg->used ) {
        off -= beg->used;
        end -= beg->used;
        beg  = beg->next;
        if ( not beg ) {
            off = -1;
            end = -1;
            return;
        }
    }
    visitor( []( const Buffer *b, PT, PT ) {
        inc_ref( b );
        return true;
    } );
}

CbString::CbString( CbQueue &&bs, PT off, PT len ) {
    TODO;
}

CbString::CbString( const CbQueue &bs ) : beg( bs.beg ), off( bs.off ), end( off + bs.size() ) {
    bs.visitor( []( const Buffer *b, PT, PT ) {
        inc_ref( b );
        return true;
    } );
}

CbString::CbString( IKnowWhatIDo, Buffer *buff, PT off, PT len ) : beg( buff ), off( off ), end( off + len ) {
    ASSERT_IF_DEBUG( buff->next == 0 );
    if ( len )
        inc_ref( buff );
}

CbString::CbString( const std::string &bs ) {
    Buffer *buf = Buffer::New( bs.size() );
    memcpy( buf->data, bs.data(), bs.size() );
    buf->used = bs.size();

    end = bs.size();
    beg = buf;
    off = 0;
}

void CbString::free() {
    dec_ref_buffs();
    off = 0;
    end = 0;
}

bool CbString::ack_error() {
    dec_ref_buffs();
    off = -1;
    end = -1;
    return false;
}

CbString::operator std::string() const {
    std::string res; res.reserve( size() );

    data_visitor( [ &res ]( const PI8 *b, const PI8 *e ) {
        res.append( b, e );
    } );

    return res;
}

void CbString::read_some( void *data, PT size ) {
    if ( not size )
        return;
    visitor( [ this, &data, &size ]( const Buffer *b, PT bd, PT ed ) {
        // last buffer to be used for this reading ?
        PT lm = ed - bd;
        if ( size < lm ) {
            memcpy( data, b->data + bd, size );
            off += size;
            return false;
        }

        // else, read what we can
        size -= lm;
        memcpy( data, b->data + bd, lm );
        data = reinterpret_cast<PI8 *>( data ) + lm;

        // and remove b from the stream
        if ( end > b->used ) {
            end -= b->used;
            beg = b->next;
            off = 0;
            dec_ref( b );
            return true;
        }

        end = 0;
        off = 0;
        dec_ref( b );
        return true;
    } );
}

CbString &CbString::skip_some( PT size ) {
    visitor( [ this, &size ]( const Buffer *b, PT bd, PT ed ) {
        // last buffer to be used for this reading ?
        PT lm = ed - bd;
        if ( size < lm ) {
            off += size;
            return false;
        }

        size -= lm;

        if ( end > b->used ) {
            end -= b->used;
            beg = b->next;
            off = 0;
            dec_ref( b );
            return true;
        }

        end = 0;
        off = 0;
        dec_ref( b );
        return false;
    } );
    return *this;
}

CbString &CbString::skip_some_sr( ssize_t &size ) {
    if ( size < 0 )
        return *this;
    visitor( [ this, &size ]( const Buffer *b, PT bd, PT ed ) {
        PT lm = ed - bd;
        if ( PT( size ) < lm ) {
            off += size;
            size = 0;
            return false;
        }

        size -= lm;

        if ( end > b->used ) {
            end -= b->used;
            beg = b->next;
            off = 0;
            dec_ref( b );
            return true;
        }

        end = 0;
        off = 0;
        dec_ref( b );
        return false;
    } );
    return *this;
}

bool CbString::starts_with( const char *data ) {
    return starts_with( data, strlen( data ) );
}

bool CbString::starts_with( const void *data, PT size ) {
    visitor( [ &data, &size ]( const Buffer *b, unsigned off, unsigned used ) {
        // stuff to test inside b ?
        PT rem = used - off;
        if ( size <= rem ) {
            if ( memcmp( data, b->data + off, size ) == 0 )
                size = 0;
            return false;
        }
        // data size is larger
        if ( memcmp( data, b->data + off, rem ) )
            return false;
        data = (const char *)data + rem;
        size -= rem;
        return true;
    } );
    return size == 0;
}

CbString CbString::read_line( char sep, bool skip_void_lines ) {
    if ( skip_void_lines )
        while ( not empty() and *ptr() == sep )
            skip_byte();
    // find sep
    PT pcr = 0;
    visitor( [ &pcr, sep ]( const Buffer *b, unsigned off, unsigned used ) {
        for( unsigned val = off; val < used; ++val ) {
            if ( b->data[ val ] == sep ) {
                pcr += val - off;
                return false;
            }
        }
        pcr += used - off;
        return true;
    } );
    CbString res{ *this, 0, pcr };
    skip_some( pcr + 1 );
    return res;
}

void CbString::write_to_stream( std::ostream &os ) const {
    int cpt = 0;
    visitor( [ &os, &cpt ]( const Buffer *b, PT beg, PT end ) {
        for( PT i = beg; i < end; ++i )
            os << ( cpt++ ? " " : "" ) << unsigned( b->data[ i ] );
        //        static const char *c = "0123456789abcdef";
        //        for( PT i = beg; i < end; ++i )
        //            os << ( cpt++ ? " " : "" ) << c[ b->data[ i ] / 16 ] << c[ b->data[ i ] % 16 ];
        return true;
    } );
}


