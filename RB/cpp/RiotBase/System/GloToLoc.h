#pragma once

namespace RiotBase {
struct DevId;
struct UsrId;

// inplace transformation of a global representation to a local one
template<class T>
void glo_to_loc( T &val, const DevId &cur_dev, const UsrId &cur_usr ) {}

}
