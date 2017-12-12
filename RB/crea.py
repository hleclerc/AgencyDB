import os, sys

hb = """#pragma once

namespace RiotBase {

/**
*/
class MYCLASS {
public:
};

} // namespace RiotBase
"""

hp = """#pragma once

#include "MYPARENT.h"

namespace RiotBase {

/**
*/
class MYCLASS : public MYPARENT {
public:
};

} // namespace RiotBase
"""

hv = """#pragma once

#include "../Core/RpTempl.h"

namespace RiotBase {

///
class MYCLASS : public RpTempl<MYCLASS> {
public:
    static std::string     type_name();
    void                   write_to_stream( std::ostream &os ) const;
};

} // namespace RiotBase
"""

cb = """#include "MYHFILE"

namespace RiotBase {


} // namespace RiotBase
"""

cv = """#include "MYHFILE"

namespace RiotBase {

std::string MYCLASS::type_name() {
    return "MYCLASS";
}

void MYCLASS::write_to_stream( std::ostream &os ) const {
}

} // namespace RiotBase
"""

cs = """Rp = require "../Core/Rp.js"

#
module.exports =
class MYCLASS extends Rp
    Rp.make_templ this

    init: () ->

"""

ts = """import Rp from "../Core/Rp";

//
export default
class MYCLASS extends Rp {
    constructor() {
        super( MYCLASS );
    }
}
Rp.make_templ( MYCLASS );
"""

def crea_coffee( path, class_name ):
    cf = open( path, "w" )
    cf.write( cs.replace( "MYCLASS", class_name ) )

def crea_ts( path, class_name ):
    cf = open( path, "w" )
    cf.write( ts.replace( "MYCLASS", class_name ) )

# set up
pwd  = sys.argv[ 1 ]
vptp = False
if sys.argv[ 2 ] == "-rp":
    sys.argv.pop( 2 )
    vptp = True
name = sys.argv[ 2 ]

# coffee ?
if name.endswith( ".coffee" ):
    path = pwd + "/" + name
    crea_coffee( path, name[ 0: len( name ) - 7 ] )
    os.system( "./update_files.sh > /dev/null" )
    os.system( "git add " + path )

# coffee ?
if name.endswith( ".ts" ):
    path = pwd + "/" + name
    crea_ts( path, name[ 0: len( name ) - 3 ] )
    # os.system( "./update_files.sh > /dev/null" )
    os.system( "git add " + path )

# else, generate a .cpp and a .h
else:
    hn   = pwd + "/" + name + ".h"
    cn   = pwd + "/" + name + ".cpp"

    if os.path.exists( hn ) or os.path.exists( cn ):
        print( "Already existing files" )
        sys.exit( 1 )

    # h file
    hf = open( hn, "w" )
    if len( sys.argv ) <= 3:
        if vptp:
            hf.write( hv.replace( "MYCLASS", name ) )
        else:
            hf.write( hb.replace( "MYCLASS", name ) )
    else:
        hf.write( hp.replace( "MYCLASS", name ).replace( "MYPARENT", sys.argv[ 3 ] ) )

    # c file
    cf = open( cn, "w" )
    if vptp:
        cf.write( cv.replace( "MYHFILE", name + ".h" ).replace( "MYCLASS", name ) )
    else:
        cf.write( cb.replace( "MYHFILE", name + ".h" ) )

    os.system( "./update_files.sh > /dev/null" )
    os.system( "git add " + hn + " " + cn )
