#pragma once

namespace RiotBase {
struct DevId;
struct UsrId;

// inplace transformation of a local representation to a global one
template<class T>
void loc_to_glo( T &val, const DevId &cur_dev, const UsrId &cur_usr ) {}

}
