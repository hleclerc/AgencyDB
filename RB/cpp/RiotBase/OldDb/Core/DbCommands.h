#pragma once

#include "../../System/TypeConfig.h"

namespace RiotBase {
namespace DbCommands {

// authentication
constexpr PI8 req_identity           =   1; ///< request usr and dev id from the other size (for an instance capable to handle random values and to asymetric cryptography, with a stored list of public keys for trusted instances. => [ usr_id, and a random key encrypted by the known public keys ]+ 0u (the random value is the same accross the list)
constexpr PI8 ans_identity           =   2; ///< encrypted string for 123456789u, dev_id and usr_id (encrypted by the random value). string size = 0 means anon_usr.
constexpr PI8 ack_identity           =   3; ///< 0 -> bad. 1 -> ok

constexpr PI8 req_auth_std           =   4; ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a login and to password. => login and password.
constexpr PI8 req_auth_tok           =   5; ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a token that may be used for authentication. => token.
constexpr PI8 req_auth_yub           =   6; ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a login, a password and a yubikey. => dev_id, login (or a zero sized string if not specified), the password and the yubikey.
constexpr PI8 ans_auth               =  10; ///< answer to a PKI based connection. => [ dev_id, usr_id ] or 0u if bad credentials


// content / patches
constexpr PI8 req_content            =  11; ///< inode
constexpr PI8 req_changes            =  12; ///< inode, id_last_mod (SetOfPatchId)
constexpr PI8 new_changes            =  13; ///<
constexpr PI8 ack_changes            =  14; ///< inode, patch_id

// graphs
constexpr PI8 graph_cmd              =  15; ///< out_offsets, msg
constexpr PI8 res_graph              =  16; ///< dst_id, num_out, msg

// errors
constexpr PI8 corrupted_data         =  20; ///< code
constexpr PI8 corrupted_command      =  21; ///< code, inode
constexpr PI8 incompatible_do_type   =  22; ///< inode
constexpr PI8 unknown_do_type        =  23; ///< inode
constexpr PI8 inode_does_not_exist   =  24; ///< inode

constexpr PI8 G                      =  71; ///< 'G', beginning of GET
constexpr PI8 P                      =  80; ///< 'P', beginning of PUT, POST, ...

// helpers
constexpr PI8 nop                    = 128; ///< a single byte (used e.g. for alignment)
constexpr PI8 nnop                   = 129; ///< n worthless bytes (used e.g. for alignment)
constexpr PI8 end                    = 130; ///< graceful end

} // namespace DbCommands
} // namespace RiotBase
