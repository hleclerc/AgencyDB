#include "Inode.h"

/// predefined inodes (and data to construct new ones)
namespace RiotBase {
namespace KnownInodes {
    constexpr Inode  root_dir       ( KnownDevId::dev_known_inodes, 2  );
    constexpr Inode  root_home      ( KnownDevId::dev_known_inodes, 3  );
    constexpr Inode  type_Directory ( KnownDevId::dev_known_inodes, 10 );
    constexpr Inode  type_String    ( KnownDevId::dev_known_inodes, 11 );
    constexpr Inode  type_Number    ( KnownDevId::dev_known_inodes, 12 );
}
}

