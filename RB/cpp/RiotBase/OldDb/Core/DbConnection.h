#pragma once

#include <string>

namespace RiotBase {
class TcpRawConnection;
class DbSession;
class Db;

TcpRawConnection *new_db_connection( DbSession *s, int fd, bool server );


} // namespace RiotBase
