#include "../../System/Date.h"
#include "../../System/Log.h"
#include "IdGenerator.h"

namespace RiotBase {

IdGenerator::IdGenerator( unsigned base_id ) : base_id( base_id ), counter( 0 ), pr_date( 0 ) {
    if ( base_id >= ( 1 << nb_bits_base_id ) )
        DISP_ERROR( "base_id >= {} in IdGenerator", 1 << nb_bits_base_id );
}

PI64 IdGenerator::gen() {
    PI64 v;
    while ( true ) {
        v = Date::current().msec;

        if ( v == pr_date ) {
            if ( counter < ( 1 << nb_bits_additionnal_counter ) - 1 ) {
                ++counter;
                break;
            }
            Date::sleep( 0.001 );
        } else {
            counter = 0;
            break;
        }
    }
    pr_date = v;

    constexpr PI64 m = ( PI64( 1 ) << ( 64 - nb_bits_additionnal_counter - nb_bits_base_id ) ) - 1; // mask for the date (base_id and counter has already been checked)
    return ( counter << ( 64 - nb_bits_additionnal_counter ) ) | ( base_id << ( 64 - nb_bits_additionnal_counter - nb_bits_base_id ) ) | ( v & m );
}

} // namespace RiotBase
