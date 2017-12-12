#include <RiotBase/Variable/Symbol/LazySourceKnownValue.h>
#include <RiotBase/Variable/Symbol/LazySourceExtValue.h>
#include <RiotBase/Variable/Symbol/Graphviz.h>
#include <RiotBase/Variable/Number/Bn.h>
#include <RiotBase/Variable/Number.h>
#include <RiotBase/Db/Core/DbItem.h>
#include <RiotBase/System/Check.h>
#include <RiotBase/Db/Db.h>
#include "OtMachine.h"
using namespace RiotBase;

void test_lazyext() {
    Db db;
    DbItem *item_d = new DbItem{ Directory::category, Inode{ 100, 1 }, &db };
    Directory d = LazySourceExtValue::mkl( item_d );
    ObjectId  i = d[ "yop" ];

    //    {
    //        Graphviz gr;
    //        gr << d.rp;
    //        gr << i.rp;
    //        gr.display();
    //    }

    //    CHECK( t, "10" );
    //    CHECK( o, "15" );
}

int main() {
    test_lazyext();
}


