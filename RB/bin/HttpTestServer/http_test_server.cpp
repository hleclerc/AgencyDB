#include <RiotBase/HttpTestServer/HttpTestServer.h>
using namespace RiotBase;

int main() {
    HttpTestServer ht;
    ht.run( 8001, "localhost.cert", "localhost.key" );
}
