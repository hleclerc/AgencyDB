#include "DbFileReader.h"
#include "DbCommands.h"
#include "../Db.h"

namespace RiotBase {

DbFileReader::DbFileReader( Db *db, DataFile *datafile ) : inp_cont( 0 ), pending_cb_queue( 0 ), db( db ), datafile( datafile ) {
}

DbFileReader::ParseStatusCode DbFileReader::parse( Buffer *buffer, PI64 offset ) {
    if ( not buffer->used )
        return ParseStatusCode::ok;
    unsigned char *data = buffer->data, *end = data + buffer->used;

    // if we were interrupted reading a String
    if ( pending_cb_queue ) {
        pending_cb_queue->next = inc_ref( buffer );
        pending_cb_queue = buffer;
    }

    // jump to previous "program counter" if previously stopped
    if ( inp_cont )
        goto *inp_cont;

    // macro to ask for more. If not available, wait for more (modification of inp_cont)
    #define READ_DATA( N ) \
        if ( ++data >= end ) { inp_cont = &&m_##N; return ParseStatusCode::ok; } m_##N:

    // Read compressed repr of a uint number.
    #define READ_UINT( VAR, LAB ) \
        READ_DATA( LAB##_0 ); \
        if ( *data >= 128 ) { \
            VAR = *data - 128; \
            tmp_shift_read = 7; \
            while ( true ) { \
                READ_DATA( LAB##_1 ); \
                if ( *data < 128 ) { \
                    VAR += *data << tmp_shift_read; \
                    break; \
                } \
                VAR += ( *data - 128 ) << tmp_shift_read; \
                tmp_shift_read += 7; \
            } \
        } else \
            VAR = *data

    // Read compressed repr of an inode
    #define READ_INODE( VAR, LAB ) \
        READ_UINT( VAR.num, LAB##_num ); \
        if ( VAR.num ) { READ_UINT( VAR.dev.val, LAB##_dev ); if ( VAR.dev == db->get_dev_id() ) VAR.dev = 0; } else VAR.dev = 0

    // Read compressed repr of a UsrId
    #define READ_USR_ID( VAR, LAB ) \
        READ_UINT( VAR.num, LAB##_num ); \
        if ( VAR.num ) { READ_UINT( VAR.dev.val, LAB##_dev ); if ( VAR.dev == db->get_dev_id() ) VAR.dev = 0; if ( VAR == db->usr_id ) VAR = UsrId{}; } else VAR.dev = 0

    // read LENGTH bytes, in OUTPUT which should be a CbReader
    #define READ_SIZED( VAR, LENGTH, LABEL ) \
        if ( LENGTH ) { \
            READ_DATA( m_##VAR##_##LENGTH##_##LABEL##_0 ); /* we need at least one byte */ \
            VAR = CbString( IKnowWhatIDo(), buffer, data - buffer->data, LENGTH );\
            unsigned remaining_data = buffer->used - ( data - buffer->data ); \
            if ( LENGTH > remaining_data ) { \
                LENGTH -= remaining_data; \
                pending_cb_queue = buffer; \
                inp_cont = &&m_##VAR##_##LENGTH##_##LABEL; \
                return ParseStatusCode::ok; \
              m_##VAR##_##LENGTH##_##LABEL: \
                if ( LENGTH > buffer->used ) { \
                    LENGTH -= buffer->used; \
                    return ParseStatusCode::ok; /* we loop with the same inp_cont */ \
                } \
                pending_cb_queue = 0; \
            } \
            data += LENGTH - 1; \
        } else \
            VAR.free()

    // read String (-> CbReader) in binary format (cuint for length then data)
    #define READ_STRING( OUTPUT, LABEL ) \
        READ_UINT( tmp_str_length, LABEL ); \
        READ_SIZED( OUTPUT, tmp_str_length, LABEL )

    //
    while ( true ) {
        switch ( *data ) {
        case PI8( DbCommands::new_changes ): goto l_new_changes;
        default: return ParseStatusCode::uknown_command;
        }

      l_new_changes:
        READ_INODE ( tmp_inode_0        , l_new_changes_0 );
        READ_STRING( tmp_str_0          , l_new_changes_1 ); // category
        READ_STRING( tmp_str_1          , l_new_changes_2 );
        TODO; // why 2 strings ?
        new_changes( tmp_inode_0, tmp_str_0, tmp_str_1, offset + ( data + 1 - buffer->data ) - tmp_str_1.size(), tmp_str_1.size() );
        READ_DATA( l_new_changes_c );
        continue;
    }
}

void DbFileReader::new_changes( Inode inode, std::string category, CbString &data, PI64 offset, PI64 len ) {
    if ( DbItem *item = db->table.find( inode ) ) { // if found, update object
        CbQueue ans;
        IMPORTANT_TODO;
        // RO_CALL( app_changes, item->rp, &ans, data, db->fqid(), FqId{ item->db, KnownInodes::dev_perm_storage, {} }, db->fqid() );
    } else { // else, add an entry with a symbol creator
        Category *c = Category::find( category );
        if ( not c )
            ERROR( "TODO: category %s", category.c_str() );
        item = db->table.insert( new DbItem( c, inode, db ) );
        item->datafile        = datafile;
        item->datafile_offset = offset;
        item->datafile_len    = len;
    }
}


} // namespace RiotBase
