#pragma once

#include "../../System/BinStream.h"
#include "../../System/UsrId.h"

namespace RiotBase {
class FqId;

class OtMerger {
public:
    virtual ~OtMerger() {}

    virtual bool glo_to_loc( BBQ loc, CbStringPtr glo, DevId cur_dev, UsrId cur_usr ) const = 0; ///< also enable to validate a message
    virtual bool loc_to_glo( BBQ glo, CbStringPtr loc, DevId cur_dev, UsrId cur_usr ) const = 0;

    virtual void new_patch ( BBQ res, CbStringPtr msg, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) = 0; ///< res = udpated version of msg (according to unknown or std patches)
    virtual void unk_patch ( BBQ res, CbStringPtr msg, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) = 0; ///< res = udpated version of msg (according to new or std patches)
    virtual void std_patch ( BBQ res, CbStringPtr msg, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) = 0; ///< res = udpated version of msg (according to new or unknown patches)

    virtual void finalize  () {}

    virtual void undo      ( CbStringPtr data, UsrId as_usr ) = 0;
};

} // namespace RiotBase
