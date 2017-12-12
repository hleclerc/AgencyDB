import Handlebars from '../../../common/Hb';

let inodes = {
    known : {
        // ---------- directories ----------
        root_dir      :  2, // /
        root_home     :  3, // /root

        // ------------ types --------------
        type_Directory: 10,
        type_String   : 11,
        type_Number   : 12,
    }
};

var template = Handlebars.compile( `#include "Inode.h"

/// predefined inodes (and data to construct new ones)
namespace RiotBase {
namespace KnownInodes {
    {{#each known}}
    constexpr Inode  {{#format 15}}{{@key}}{{/format}}( KnownDevId::dev_known_inodes, {{#format 2}}{{.}}{{/format}} );
    {{/each}}
}
}
` );

console.log( template( inodes ) );
