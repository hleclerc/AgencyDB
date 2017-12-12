#pragma once

#include "../../System/BinStream.h"
#include "../../System/UsrId.h"
#include "../../System/Vec.h"

namespace RiotBase {
class DbSession;

void deserialize_lazy_graph( DbSession *session, PI64 dst_id, const Vec<PT> &out_offsets, CbStringPtr graph, const FqId &id );

} // namespace RiotBase
