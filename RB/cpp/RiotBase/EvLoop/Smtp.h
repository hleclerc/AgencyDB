#pragma once

#include "EvLoop.h"
#include <functional>
#include <string>

namespace RiotBase {
class EvLoop;
class Dns;

enum class SmtpStatus {
    ok,
    no_dns_entry,

    pb_after_GREETING,
    pb_after_HELO,
    pb_after_AUTH_LOGIN,
    pb_after_AUTH_USER,
    pb_after_AUTH_PASS,
    pb_after_MAIL_FROM,
    pb_after_RCPT_TO,
    pb_after_DATA,
    pb_after_CONTENT
};

void send_mail( const std::string &to, const std::string &subject, const std::string &msg, std::function<void(SmtpStatus)> cb, EvLoop *evl = 0, Dns *dns = 0 );

} // namespace RiotBase
