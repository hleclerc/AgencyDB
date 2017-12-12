#pragma once

#include "../../System/TypeConfig.h"

namespace RiotBase {

/**
  10 bits for the additionnal counter (if prev date already used)
  12 bits for the base_id
  42 bits for the date in ms since epoch

  -> should be OK for 139 years
*/
class IdGenerator {
public:
    enum {
        nb_bits_additionnal_counter = 10,
        nb_bits_base_id = 12
    };
    enum {
        REQ_NEW_ID = 0,
    };
    enum {
        ANS_NEW_ID = 0,
    };

    IdGenerator( unsigned base_id ); ///< base_id must be < ( 1 << nb_bits_base_id ) (and must be unique amongst all the other instances).

    PI64                         gen(); ///< generate a new id

    PI64                         base_id;
    PI64                         counter;
    PI64                         pr_date;
};

} // namespace RiotBase
