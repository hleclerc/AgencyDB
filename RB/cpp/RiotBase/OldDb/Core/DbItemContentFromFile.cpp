#include "DbItemContentFromFile.h"
#include "DataFile.h"

namespace RiotBase {

DbItemContentFromFile::DbItemContentFromFile( DbItemFromFile *diff ) : RpTemplSym<DbItemContentFromFile>( diff->category() ), diff( diff ) {
    if ( not diff->scheduled ) {
        diff->scheduled = true;
        diff->datafile->sched_reader( diff );
    }
}

std::string DbItemContentFromFile::type_name() {
    return "DbItemContentFromFile";
}

} // namespace RiotBase
