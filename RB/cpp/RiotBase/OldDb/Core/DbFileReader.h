#pragma once

#include "../../System/CbString.h"
#include "../../System/Buffer.h"
#include "../../System/Inode.h"

namespace RiotBase {
class DataFile;
class Db;

///
class DbFileReader {
public:
    enum class ParseStatusCode {
        #define DECL_ERR( NE, TXT ) NE,
        #include "DbFileReaderParseStatusCodesDecl.h"
        #undef DECL_ERR
    };

    DbFileReader( Db *db, DataFile *datafile );

    ParseStatusCode parse      ( Buffer *buffer, PI64 offset );

    void            new_changes( Inode inode, std::string category, CbString &data, PI64 offset, PI64 len );

    void           *inp_cont;
    Buffer         *pending_cb_queue;    ///< if we need to add the current buffer to a previous one
    unsigned        tmp_shift_read;      ///< used by READ_UINT, READ_INT, ...
    unsigned        tmp_str_length;      ///< used by READ_STRING
    Db             *db;
    DataFile       *datafile;

    Inode           tmp_inode_0;         ///<
    CbString        tmp_str_0;           ///<
    CbString        tmp_str_1;           ///<
};

} // namespace RiotBase
