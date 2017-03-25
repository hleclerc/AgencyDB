// This file has been generated using /home/hugo/.nsmake/build/make_ts_decl_from_cmds-1.js and /home/hugo/AgencyDB/Tracker/TrackerCmdsDecl.cmds
import BinaryWriter from '../src/ts/System/BinaryWriter'
import UsrId        from '../src/ts/System/UsrId'

export default {
    write: {
        /// login with a password
        pwd_login( bw: BinaryWriter, login: string, passwd: string ) { bw.write_PI8( 0 ); bw.write_String( login ); bw.write_String( passwd ); },
        /// answer to a login request
        ans_login( bw: BinaryWriter, usr_id: UsrId ) { bw.write_PI8( 1 ); usr_id.write_to( bw ); },
    },
}
