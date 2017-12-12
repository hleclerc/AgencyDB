DECL_CMD( changes,  1, Inode /*inode*/, BinaryData /*patch_data*/ )

//// authentication
//DECL_CMD( req_identity        ,  1  ); ///< request usr and dev id from the other size (for an instance capable to handle random values and to asymetric cryptography, with a stored list of public keys for trusted instances. => [ usr_id, and a random key encrypted by the known public keys ]+ 0u (the random value is the same accross the list)
//DECL_CMD( ans_identity        ,  2  ); ///< encrypted string for 123456789u, dev_id and usr_id (encrypted by the random value). string size = 0 means anon_usr.
//DECL_CMD( ack_identity        ,  3  ); ///< 0 -> bad. 1 -> ok

//DECL_CMD( req_auth_std        ,  4  ); ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a login and to password. => login and password.
//DECL_CMD( req_auth_tok        ,  5  ); ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a token that may be used for authentication. => token.
//DECL_CMD( req_auth_yub        ,  6  ); ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a login, a password and a yubikey. => dev_id, login (or a zero sized string if not specified), the password and the yubikey.
//DECL_CMD( ans_auth            , 10  ); ///< answer to a PKI based connection. => [ dev_id, usr_id ] or 0u if bad credentials


//// content / patches
//DECL_CMD( req_content         , 11, Inode  ); ///< inode
//DECL_CMD( req_changes         , 12, Inode, VectorClock ); ///< inode, id_last_mod (SetOfPatchId)
//DECL_CMD( new_changes         , 13, Inode,   ); ///< inode, patch_data
//DECL_CMD( ack_changes         , 14  ); ///< inode, patch_id

//// graphs
//DECL_CMD( graph_cmd           , 15  ); ///< msg
//DECL_CMD( graph_ans           , 16  ); ///< num, msg

//// errors
//DECL_CMD( corrupted_data      , 20  ); ///< code
//DECL_CMD( corrupted_command   , 21  ); ///< code, inode
//DECL_CMD( incompatible_do_type, 22  ); ///< inode
//DECL_CMD( unknown_do_type     , 23  ); ///< inode
//DECL_CMD( inode_does_not_exist, 24  ); ///< inode

//DECL_CMD( G                   , 71  ); ///< 'G', beginning of GET
//DECL_CMD( P                   , 80  ); ///< 'P', beginning of PUT, POST, ...

//// helpers
//DECL_CMD( nop                 , 128 ); ///< a single byte (used e.g. for alignment)
//DECL_CMD( nnop                , 129 ); ///< n worthless bytes (used e.g. for alignment)
//DECL_CMD( end                 , 130 ); ///< graceful end of the connection
//DECL_CMD( quit                , 131 ); ///< graceful end of the db instance
