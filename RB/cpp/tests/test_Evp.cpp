#include <RiotBase/System/Check.h>
#include <RiotBase/System/Evp.h>
using namespace RiotBase;

void test_sym() {
    const char *key = "01234567890123456789012345678901";
    const char *iv  = "01234567890123456";

    Evp evp( key, iv );
    std::string encrypted = evp.encrypt( "hello" );
    std::string decrypted = evp.decrypt( encrypted );
    CHECK( decrypted, "hello" );
}

int main() {
    test_sym();
}

