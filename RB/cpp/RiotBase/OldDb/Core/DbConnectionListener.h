#pragma once

#include "../../EvLoop/TcpListener.h"
#include <string>

namespace RiotBase {
class Db;

TcpListener *new_DbTcpListener( Db *db, const std::string &prov, bool is_a_named_socket );

} // namespace RiotBase
