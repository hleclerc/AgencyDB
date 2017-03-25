// This file has been generated using /home/hugo/.nsmake/build/make_cpp_decl_from_cmds-1.js and /home/hugo/AgencyDB/Tracker/TrackerCmdsDecl.cmds

/// login with a password
void pwd_login( const Hpipe::CbStringPtr &login, const Hpipe::CbStringPtr &passwd );
/// answer to a login request
void ans_login( UsrId usr_id );

/// login with a password
void write_pwd_login( Hpipe::BinStream<Hpipe::CbQueue> &bw, const Hpipe::CbStringPtr &login, const Hpipe::CbStringPtr &passwd ) { bw.write_byte( 0 ); bw << login; bw << passwd; }
/// answer to a login request
void write_ans_login( Hpipe::BinStream<Hpipe::CbQueue> &bw, UsrId usr_id ) { bw.write_byte( 1 ); usr_id.write_to( bw ); }
