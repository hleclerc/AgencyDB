#pragma once

#include "RpTempl.h"

namespace RiotBase {

///
class Error : public RpTempl<Error> {
public:
    enum { error_value = true, };

    Error( std::string msg );

    static std::string     type_name();
    void                   write_to_stream( std::ostream &os ) const;

    std::string            msg;
};

} // namespace RiotBase
