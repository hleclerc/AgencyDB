#include <RiotBase/System/AbsLinearSystem.h>
#include <RiotBase/System/Check.h>
using namespace RiotBase;



void test_uni() {
    // |x0-1| + |x0| +  |x0+1|
    AbsLinearSolver l;
    l.x << 10.0;

    AbsLinearSolver::Term *t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 0,  1 };
    t->offset = -1;

    t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 0, 1 };

    t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 0,  1 };
    t->offset = 1;

    l.solve();
    CHECK( l.x, "0" );
}


void test_sep() {
    // |x0-1| + |2-x1|
    AbsLinearSolver l;

    AbsLinearSolver::Term *t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 0,  1 };
    t->offset = -1;

    t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 1, -1 };
    t->offset = 2;

    l.solve();
    CHECK( l.x, "1 2" );
}

void test_reg_line_1() {
    // |x0-1| + |x1-x0| + |x1-2|
    AbsLinearSolver l;

    l.terms.push_back( -1.0 )->items << AbsLinearSolver::Item{ 0, 1 };
    l.terms.push_back(  0.0 )->items << AbsLinearSolver::Item{ 1, 1 } << AbsLinearSolver::Item{ 0, -1 };
    l.terms.push_back( -2.0 )->items << AbsLinearSolver::Item{ 1, 1 };

    l.solve();
    CHECK( l.x, "1 2" );
}

void test_reg_line_2() {
    // |x0-1| + |x1-x0| + |x2-x1| + |x2-2|
    AbsLinearSolver l;

    l.terms.push_back( -1.0 )->items << AbsLinearSolver::Item{ 0, 1 };
    l.terms.push_back(  0.0 )->items << AbsLinearSolver::Item{ 1, 1 } << AbsLinearSolver::Item{ 0, -1 };
    l.terms.push_back(  0.0 )->items << AbsLinearSolver::Item{ 2, 1 } << AbsLinearSolver::Item{ 1, -1 };
    l.terms.push_back( -2.0 )->items << AbsLinearSolver::Item{ 2, 1 };

    l.solve();
    CHECK( l.x, "1 1 2" );
}

void test_reg_line_3() {
    // |x0-1| + |x1-x0| + |x2-x1| + |x3-x2| + |2-x3|
    AbsLinearSolver l;

    l.terms.push_back( -1.0 )->items << AbsLinearSolver::Item{ 0,  1 };
    l.terms.push_back(  0.0 )->items << AbsLinearSolver::Item{ 1,  1 } << AbsLinearSolver::Item{ 0, -1 };
    l.terms.push_back(  0.0 )->items << AbsLinearSolver::Item{ 2,  1 } << AbsLinearSolver::Item{ 1, -1 };
    l.terms.push_back(  0.0 )->items << AbsLinearSolver::Item{ 3,  1 } << AbsLinearSolver::Item{ 2, -1 };
    l.terms.push_back(  2.0 )->items << AbsLinearSolver::Item{ 3, -1 };

    l.solve();
    CHECK( l.x, "1 1 2 2" );
}

void test_line() {
    // |x0-1| + |2*x1-2*x0| + |2*x2-2*x1| + |x3-x2| + |2-x3|
    AbsLinearSolver l;

    AbsLinearSolver::Term *t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 0,  1 };
    t->offset = -1;

    t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 1,  2 };
    t->items << AbsLinearSolver::Item{ 0, -2 };

    t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 2,  2 };
    t->items << AbsLinearSolver::Item{ 1, -2 };

    t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 3,  1 };
    t->items << AbsLinearSolver::Item{ 2, -1 };

    t = l.terms.push_back();
    t->items << AbsLinearSolver::Item{ 3, -1 };
    t->offset = 2;

    l.solve();
    CHECK( l.x, "1 1 1 2" );
}

int main() {
    //test_uni();
    //test_sep();
    //test_reg_line_1();
    test_reg_line_2();
    test_reg_line_3();
    test_line();
}
