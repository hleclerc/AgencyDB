#include "Inode.h"

/// predefined inodes (and data to construct new ones)
namespace KnownInodes {
    % for n in known_inodes:
    constexpr Inode ${ n }( KnownDevId::dev_known_inodes, ${ known_inodes[ n ] } );
    % endfor
}
