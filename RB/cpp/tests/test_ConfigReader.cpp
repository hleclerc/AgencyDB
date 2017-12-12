//// nsmake global inc_path ..

#include <RiotBase/System/ConfigReader.h>
#include <RiotBase/System/Check.h>
#include <queue>
using namespace RiotBase;

int main() {
    struct MyConfigReader : ConfigReader {
        virtual void onargs( Vec<std::string> &args ) {
            CHECK( args, to_read.front() );
            to_read.pop();

            os += write( args );
        }
        std::queue<std::string> to_read;
        std::string os;
    };

    std::istringstream ss( "n0 a00  a01\n  n1 \"long\\targ\" # comment\nn2" );

    MyConfigReader cr;
    cr.to_read.push( "n0 a00 a01" );
    cr.to_read.push( "n1 long\targ" );
    cr.to_read.push( "n2" );
    cr.parse( ss );

    CHECK( cr.os, "n0 a00 a01\nn1 \"long\\targ\"\nn2\n" );
}

