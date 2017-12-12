#include <RiotBase/System/BinStream.h>
#include <RiotBase/System/MsgQueue.h>
#include <RiotBase/System/Check.h>
using namespace RiotBase;

void test_MsgQueue() {
    MsgQueue mq( 512 );
    BinStream<MsgQueue> bq( &mq );
    bq << 1u << 2u << flush << 3u << 4u;
    CHECK( mq, "[01 02] pending=[03 04]" );
}

int main() {
    test_MsgQueue();
}


