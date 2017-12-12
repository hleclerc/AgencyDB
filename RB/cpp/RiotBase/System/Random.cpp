#include <openssl/rand.h>
#include "Random.h"
#include "Assert.h"

namespace RiotBase {

Random::Random() {
    if ( RAND_load_file( "/dev/urandom", 128 ) != 128 ) ERROR( "Pb seeding..." );
}

void Random::get( void *buf, int len ) {
    if ( not RAND_bytes( (unsigned char *)buf, len ) )
        ERROR( "Pb rand..." );
}


} // namespace RiotBase
