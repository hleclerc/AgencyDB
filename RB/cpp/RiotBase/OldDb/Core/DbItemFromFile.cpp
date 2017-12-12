#include "DbItemContentFromFile.h"
#include "DataFile.h"

#include "../../Variable/Symbol/Lazyfier.h"
#include "../../System/Ccast.h"

namespace RiotBase {

DbItemFromFile::DbItemFromFile( const Category *cat, DataFile *datafile, PI64 offset, PI64 len ) : datafile( datafile ), offset( offset ), len( len ), cat( cat ) {
    scheduled = false;
    content   = 0;
    db_item   = 0;
}

DbItemFromFile::~DbItemFromFile() {
    if ( content )
        RO_CALL( del, content );
    if ( scheduled )
        datafile->readers.remove_first_unordered( this );
}

std::string DbItemFromFile::type_name() {
    return "DbItemFromFile";
}

void DbItemFromFile::self_on_change( Callback *c ) {
    callbacks << inc_ref( c );
}

const Category *DbItemFromFile::category() const {
    return cat;
}

Rp **DbItemFromFile::prox_ptr() {
    if ( not content )
        content = Lazyfier::New( DbItemContentFromFile::New( ccast( this ) ) );
    return (Rp **)&content;
}

const Rp *DbItemFromFile::prox_ref() const {
    if ( not content )
        content = Lazyfier::New( DbItemContentFromFile::New( ccast( this ) ) );
    return content;
}

} // namespace RiotBase
