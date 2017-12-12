import Cookie from "../RiotBase/Dom/Core/Cookie";
import check  from "../RiotBase/System/Check";

function test_get_set() {
    Cookie.set( "a", "B", 1.0 / 24 );
    Cookie.set( "a", "A", 1.0 / 24 );
    Cookie.set( "c", "C", 1.0 / 24 );
    check( Cookie.get( "a" ), "A" );
    check( Cookie.get( "b" ), ""  );
    check( Cookie.get( "c" ), "C" );
}

test_get_set();
