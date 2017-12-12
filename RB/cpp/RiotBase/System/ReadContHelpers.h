#pragma once

#include "Unref.h"

// helpers
#define __CONCAT_IMPL( x, y ) x##y
#define __MACRO_CONCAT( x, y )  __CONCAT_IMPL( x, y )
#define __READ_DATA( LABEL ) do { if ( ++data >= end ) { inp_cont = &&LABEL; return true; } LABEL:; } while ( 0 )

// macro to ask for more. If not available, wait for more (modification of inp_cont)
#define READ_DATA \
     __READ_DATA( __MACRO_CONCAT( read_data_label, __COUNTER__ ) )

#define READ_STRING_UNTIL_SPACE( VAR ) \
    do {                                                                                                        \
        READ_DATA;                                                                                              \
        if ( *data == ' ' ) {                                                                                   \
            VAR.free();                                                                                         \
        } else {                                                                                                \
            VAR = CbString( IKnowWhatIDo(), inp_buff, data - inp_buff->data, 1 );                               \
            pending_cb_queue = inp_buff;                                                                        \
            while ( true ) {                                                                                    \
                READ_DATA;                                                                                      \
                if ( *data == ' ' or *data == 10 or *data == 13 )                                               \
                    break;                                                                                      \
                VAR.inc_length_wo_cr();                                                                         \
            }                                                                                                   \
            pending_cb_queue = 0;                                                                               \
            if ( data == inp_buff->data )                                                                       \
                dec_ref( inp_buff ); /* not going to be ref counted in CbString VAR */                          \
            --data; /* to compensate the next READ_DATA (that may be included in READ_UINT_HASHED, READ_...) */ \
        }                                                                                                       \
    } while ( 0 )

// Read ascii repr of a uint number.
#define READ_TXT_UINT( VAR ) \
    VAR = 0; \
    while ( true ) { \
        READ_DATA; \
        if ( *data < '0' or *data > '9' ) { \
            --data; \
            break; \
        } \
        VAR = 10 * VAR + ( *data - '0' ); \
    }

// Read compressed repr of a uint number.
#define READ_UINT_HASHED( VAR, SHIFT ) \
    READ_DATA; hash << *data; \
    if ( *data >= 128 ) { \
        VAR = *data - 128; \
        SHIFT = 7; \
        while ( true ) { \
            READ_DATA; hash << *data; \
            if ( *data < 128 ) { \
                VAR += Unref<decltype( VAR )>::T( *data ) << SHIFT; \
                break; \
            } \
            VAR += Unref<decltype( VAR )>::T( *data - 128 ) << SHIFT; \
            SHIFT += 7; \
        } \
    } else \
        VAR = *data

// Read compressed repr of a uint number.
#define READ_INODE_HASHED( VAR, SHIFT ) \
    READ_UINT_HASHED( VAR.num, SHIFT ); \
    if ( VAR.num ) { \
        READ_UINT_HASHED( VAR.dev.val, SHIFT ); \
    } else \
        VAR.dev = 0

// Read compressed repr of a UsrId
#define READ_DEV_ID_HASHED( DEV, SHIFT ) \
    READ_UINT_HASHED( DEV.val, SHIFT ); \
    if ( DEV == dev_id() ) \
        DEV.val = 0

// Read compressed repr of a UsrId
#define READ_USR_ID_HASHED( VAR, SHIFT ) \
    READ_UINT_HASHED( VAR.num, SHIFT ); \
    if ( VAR.num ) { \
        READ_DEV_ID_HASHED( VAR.dev, SHIFT ); \
        if ( VAR == usr_id() ) \
            VAR = UsrId{}; \
    }

// Read compressed repr of a VectorClock
#define READ_VectorClock_HASHED( VectorClock, DEV, SIZE, SHIFT ) \
    READ_UINT_HASHED( SIZE, SHIFT ); \
    while( ( SIZE )-- ) { \
        READ_DEV_ID_HASHED( DEV, SHIFT ); \
        READ_UINT_HASHED( VectorClock.data[ DEV.val ], SHIFT ); \
    }

// read LENGTH bytes, in OUTPUT which should be a CbReader
#define __READ_SIZED_HASHED( VAR, LENGTH, LABEL ) \
    if ( LENGTH ) { \
        READ_DATA; /* we need at least one byte */ \
        VAR = CbString( IKnowWhatIDo(), inp_buff, data - inp_buff->data, LENGTH );\
        if ( LENGTH > unsigned( inp_buff->used - ( data - inp_buff->data ) ) ) { \
            hash.update( data, inp_buff->used - ( data - inp_buff->data ) ); \
            LENGTH -= inp_buff->used - ( data - inp_buff->data ); \
            pending_cb_queue = inp_buff; \
            inp_cont = &&LABEL; \
            return true; \
          LABEL: \
            if ( LENGTH > inp_buff->used ) { \
                hash.update( data, inp_buff->used ); \
                LENGTH -= inp_buff->used; \
                return true; /* we loop with the same inp_cont */ \
            } \
            pending_cb_queue = 0; \
        } \
        hash.update( data, LENGTH ); \
        data += LENGTH - 1; \
    } else \
        VAR.free()

#define READ_SIZED_HASHED( VAR, LENGTH ) \
    __READ_SIZED_HASHED( VAR, LENGTH, __MACRO_CONCAT( READ_SIZED_HASHED_label, __COUNTER__ ) )

// read LENGTH bytes, in OUTPUT which should be a CbReader
#define __READ_SIZED( VAR, LENGTH, LABEL ) \
    if ( LENGTH ) { \
        READ_DATA; /* we need at least one byte */ \
        VAR = CbString( IKnowWhatIDo(), inp_buff, data - inp_buff->data, LENGTH );\
        if ( LENGTH > unsigned( inp_buff->used - ( data - inp_buff->data ) ) ) { \
            LENGTH -= inp_buff->used - ( data - inp_buff->data ); \
            pending_cb_queue = inp_buff; \
            inp_cont = &&LABEL; \
            return true; \
          LABEL: \
            if ( LENGTH > inp_buff->used ) { \
                LENGTH -= inp_buff->used; \
                return true; /* we loop with the same inp_cont */ \
            } \
            pending_cb_queue = 0; \
        } \
        data += LENGTH - 1; \
    } else \
        VAR.free()

#define READ_SIZED( VAR, LENGTH ) \
    __READ_SIZED( VAR, LENGTH, __MACRO_CONCAT( READ_SIZED_label, __COUNTER__ ) )

// read String (-> CbReader) in binary format (cuint for length then data)
#define READ_STRING_HASHED( OUTPUT, SIZE, SHIFT ) \
    READ_UINT_HASHED( SIZE, SHIFT ); \
    READ_SIZED_HASHED( OUTPUT, SIZE )


#define BEG_CMD_PC_READ( CMD ) \
    hash.reset(); \
    hash << *data; \
    beg_cmd<PC>(); \
    DISP_DEBUG( "Receiving " #CMD )

#define CHK_CMD_PC_READ \
    READ_DATA; inp_hash  = PI32( *data ) <<  0; \
    READ_DATA; inp_hash += PI32( *data ) <<  8; \
    READ_DATA; inp_hash += PI32( *data ) << 16; \
    READ_DATA; inp_hash += PI32( *data ) << 24; \
    if ( inp_hash != hash.digest() ) { \
        reinterpret_cast<PC *>( parsing_context )->~PC(); \
        new ( parsing_context ) ParsingContext; \
        DISP_ERROR( "Bad hash code in {}:{}", __FILE__, __LINE__ ); \
        return false; \
    }

#define END_CMD_PC_READ \
    reinterpret_cast<PC *>( parsing_context )->~PC(); \
    new ( parsing_context ) ParsingContext

#define PCP pc<PC>()
