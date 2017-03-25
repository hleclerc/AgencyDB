// ----------------------------------------------------------------------------------------------------
#ifdef HPIPE_PRELIMINARIES
#ifndef HPIPE_SIZE_T
#define HPIPE_SIZE_T size_t
#endif // HPIPE_SIZE_T

#ifndef HPIPE_CB_STRING_PTR_T
#define HPIPE_CB_STRING_PTR_T Hpipe::CbStringPtr
#endif // HPIPE_CB_STRING_PTR_T

#ifndef HPIPE_CB_STRING_PTR__ASSIGN_BEG_END
#define HPIPE_CB_STRING_PTR__ASSIGN_BEG_END( var, beg_buf, beg_ptr, end_buf, end_ptr ) var = HPIPE_CB_STRING_PTR_T( beg_buf, beg_ptr, end_buf, end_ptr )
#endif // HPIPE_CB_STRING_PTR__ASSIGN_BEG_END

#endif // HPIPE_PRELIMINARIES

// ----------------------------------------------------------------------------------------------------
#ifdef HPIPE_DECLARATIONS
#ifndef HPIPE_RET_CONSTANTS
#define HPIPE_RET_CONSTANTS
enum {
    RET_CONT      = 0,
    RET_OK        = 1,
    RET_KO        = 2,
    RET_ENDED_OK  = 3,
    RET_ENDED_KO  = 4,
    RET_STOP_CONT = 5
};
#endif // HPIPE_RET_CONSTANTS

#ifndef HPIPE_ADDITIONAL_ARGS
#define HPIPE_ADDITIONAL_ARGS
#endif // HPIPE_ADDITIONAL_ARGS

#ifndef HPIPE_PARSE_FUNC_NAME
#define HPIPE_PARSE_FUNC_NAME parse
#endif // HPIPE_PARSE_FUNC_NAME

#ifndef HPIPE_DATA_STRUCT_NAME
#define HPIPE_DATA_STRUCT_NAME HpipeData
#endif // HPIPE_DATA_STRUCT_NAME

#ifndef HPIPE_DATA_CTOR_NAME
#define HPIPE_DATA_CTOR_NAME init_##HPIPE_DATA_STRUCT_NAME
#endif // HPIPE_DATA_CTOR_NAME

#ifndef HPIPE_CHAR_T
#define HPIPE_CHAR_T unsigned char
#endif // HPIPE_CHAR_T

#ifndef HPIPE_BUFF_T
#define HPIPE_BUFF_T Hpipe::Buffer
#endif

struct HPIPE_DATA_STRUCT_NAME {
  PI32 __PI32_0;
  unsigned __PI32_1;
  PI64 __PI64_0;
  unsigned __PI64_1;
  HPIPE_CB_STRING_PTR_T __String_0;
  HPIPE_CB_STRING_PTR_T __String_1;
  PI64 __UsrId_0;
  const HPIPE_BUFF_T * __beg___String_0_buf;
  const HPIPE_CHAR_T * __beg___String_0_data;
  HPIPE_SIZE_T __beg___String_0_off;
  const HPIPE_BUFF_T * __beg___String_1_buf;
  const HPIPE_CHAR_T * __beg___String_1_data;
  HPIPE_SIZE_T __beg___String_1_off;
  HPIPE_SIZE_T __bytes_to_skip;
};
static void HPIPE_DATA_CTOR_NAME( HPIPE_DATA_STRUCT_NAME *hpipe_data ) {
}
unsigned HPIPE_PARSE_FUNC_NAME( HPIPE_ADDITIONAL_ARGS const HPIPE_BUFF_T *buf, size_t off, size_t end );
#endif // HPIPE_DECLARATIONS

// ----------------------------------------------------------------------------------------------------
#ifdef HPIPE_DEFINITIONS
#ifndef HPIPE_DATA
#define HPIPE_DATA hpipe_data
#endif // HPIPE_DATA

#ifndef HPIPE_DEFINITION_PREFIX
#define HPIPE_DEFINITION_PREFIX
#endif // HPIPE_DEFINITION_PREFIX

#ifndef HPIPE_PARSE_FUNC_NAME
#define HPIPE_PARSE_FUNC_NAME parse
#endif // HPIPE_PARSE_FUNC_NAME

#ifndef HPIPE_ADDITIONAL_ARGS
#define HPIPE_ADDITIONAL_ARGS
#endif // HPIPE_ADDITIONAL_ARGS

#ifndef HPIPE_BUFF_T
#define HPIPE_BUFF_T Hpipe::Buffer
#endif

#ifndef HPIPE_BUFF_T__DEC_REF
#define HPIPE_BUFF_T__DEC_REF( buf ) HPIPE_BUFF_T::dec_ref( buf )
#endif

#ifndef HPIPE_BUFF_T__INC_REF
#define HPIPE_BUFF_T__INC_REF( buf ) HPIPE_BUFF_T::inc_ref( buf )
#endif

#ifndef HPIPE_BUFF_T__INC_REF_N
#define HPIPE_BUFF_T__INC_REF_N( buf, N ) HPIPE_BUFF_T::inc_ref( buf, N )
#endif

#ifndef HPIPE_BUFF_T__SKIP
#define HPIPE_BUFF_T__SKIP( buf, ptr, N ) HPIPE_BUFF_T::skip( buf, ptr, N )
#endif

#ifndef HPIPE_BUFF_T__SKIP_N
#define HPIPE_BUFF_T__SKIP_N( buf, ptr, N, K ) HPIPE_BUFF_T::skip( buf, ptr, N, K )
#endif

#ifndef HPIPE_BUFF_T__DEC_REF_UPTO
#define HPIPE_BUFF_T__DEC_REF_UPTO( beg, end ) beg->dec_ref_upto( end )
#endif

#ifndef HPIPE_BUFF_T__DEC_REF_UPTO_N
#define HPIPE_BUFF_T__DEC_REF_UPTO_N( beg, end, K ) beg->dec_ref_upto( end, K )
#endif

unsigned HPIPE_DEFINITION_PREFIX HPIPE_PARSE_FUNC_NAME( HPIPE_ADDITIONAL_ARGS const HPIPE_BUFF_T *buf, size_t off, size_t end ) {
    while ( buf && off >= buf->used ) { off -= buf->used; end -= buf->used; buf = buf->next; }
    const HPIPE_CHAR_T *data = buf ? buf->data + off : (const HPIPE_CHAR_T *)1;
    const HPIPE_CHAR_T *end_m1 = buf ? buf->data - 1 + ( end > buf->used ? buf->used : end ) : 0;
    if ( data > end_m1 ) goto c_1;
  l_1:
    if ( data[ 0 ] >= 2 ) goto l_8;
    if ( data[ 0 ] == 1 ) goto l_9;
    { HPIPE_DATA.__PI32_0 = 0; HPIPE_DATA.__PI32_1 = 0; }
  l_4:
    if ( data >= end_m1 ) goto c_2;
    ++data;
  l_10:
    if ( data[ 0 ] >= 128 ) goto l_11;
    { HPIPE_DATA.__PI32_0 += PI32( *data       ) << HPIPE_DATA.__PI32_1; }
    HPIPE_DATA.__beg___String_0_off = 1;
    HPIPE_DATA.__beg___String_0_buf = buf;
    HPIPE_DATA.__beg___String_0_data = data;
    if ( data + HPIPE_DATA.__PI32_0 <= end_m1 ) {
        data += HPIPE_DATA.__PI32_0;
    } else {
        HPIPE_DATA.__bytes_to_skip = HPIPE_DATA.__PI32_0 - ( buf->data + buf->used - data );
      t_3:
        if ( ! buf->next ) {
            goto l_12;
        }
        end -= buf->used; buf = buf->next;
      c_3:
        if ( HPIPE_DATA.__bytes_to_skip >= buf->used ) {
            HPIPE_DATA.__bytes_to_skip -= buf->used;
            goto t_3;
        }
        data = buf->data + HPIPE_DATA.__bytes_to_skip;
        end_m1 = buf->data + ( end > buf->used ? buf->used : end ) - 1;
    }
    HPIPE_BUFF_T__SKIP_N( &HPIPE_DATA.__beg___String_0_buf, HPIPE_DATA.__beg___String_0_data, HPIPE_DATA.__beg___String_0_off, 1 );
    HPIPE_CB_STRING_PTR__ASSIGN_BEG_END( HPIPE_DATA.__String_0, HPIPE_DATA.__beg___String_0_buf, HPIPE_DATA.__beg___String_0_data, buf, data + 1 );
    { HPIPE_DATA.__PI32_0 = 0; HPIPE_DATA.__PI32_1 = 0; }
  l_2:
    if ( data >= end_m1 ) goto c_4;
    ++data;
  l_13:
    if ( data[ 0 ] >= 128 ) goto l_14;
    { HPIPE_DATA.__PI32_0 += PI32( *data       ) << HPIPE_DATA.__PI32_1; }
    HPIPE_DATA.__beg___String_1_off = 1;
    HPIPE_DATA.__beg___String_1_buf = buf;
    HPIPE_DATA.__beg___String_1_data = data;
    if ( data + HPIPE_DATA.__PI32_0 <= end_m1 ) {
        data += HPIPE_DATA.__PI32_0;
    } else {
        HPIPE_DATA.__bytes_to_skip = HPIPE_DATA.__PI32_0 - ( buf->data + buf->used - data );
      t_5:
        if ( ! buf->next ) {
            goto l_15;
        }
        end -= buf->used; buf = buf->next;
      c_5:
        if ( HPIPE_DATA.__bytes_to_skip >= buf->used ) {
            HPIPE_DATA.__bytes_to_skip -= buf->used;
            goto t_5;
        }
        data = buf->data + HPIPE_DATA.__bytes_to_skip;
        end_m1 = buf->data + ( end > buf->used ? buf->used : end ) - 1;
    }
    HPIPE_BUFF_T__SKIP_N( &HPIPE_DATA.__beg___String_1_buf, HPIPE_DATA.__beg___String_1_data, HPIPE_DATA.__beg___String_1_off, 1 );
    HPIPE_CB_STRING_PTR__ASSIGN_BEG_END( HPIPE_DATA.__String_1, HPIPE_DATA.__beg___String_1_buf, HPIPE_DATA.__beg___String_1_data, buf, data + 1 );
    { pwd_login( HPIPE_DATA.__String_0, HPIPE_DATA.__String_1 ); }
  l_5:
    if ( data >= end_m1 ) goto c_6;
    ++data;
    goto l_1;
  l_8:
    return RET_OK;
  l_15:
  l_3:
    return RET_KO;
  l_14:
    { HPIPE_DATA.__PI32_0 += PI32( *data - 128 ) << HPIPE_DATA.__PI32_1; HPIPE_DATA.__PI32_1 += 7; }
    goto l_2;
  l_12:
    goto l_3;
  l_11:
    { HPIPE_DATA.__PI32_0 += PI32( *data - 128 ) << HPIPE_DATA.__PI32_1; HPIPE_DATA.__PI32_1 += 7; }
    goto l_4;
  l_9:
    { HPIPE_DATA.__UsrId_0 = 0; HPIPE_DATA.__PI64_1 = 0; }
  l_7:
    if ( data >= end_m1 ) goto c_7;
    ++data;
  l_16:
    if ( data[ 0 ] >= 128 ) goto l_17;
    { HPIPE_DATA.__UsrId_0 += PI64( *data       ) << HPIPE_DATA.__PI64_1; }
    { HPIPE_DATA.__PI64_0 = 0; HPIPE_DATA.__PI64_1 = 0; }
  l_6:
    if ( data >= end_m1 ) goto c_8;
    ++data;
  l_18:
    if ( data[ 0 ] >= 128 ) goto l_19;
    { HPIPE_DATA.__PI64_0 += PI64( *data       ) << HPIPE_DATA.__PI64_1; }
    { ans_login( HPIPE_DATA.__UsrId_0 ); }
    goto l_5;
  l_19:
    { HPIPE_DATA.__PI64_0 += PI64( *data - 128 ) << HPIPE_DATA.__PI64_1; HPIPE_DATA.__PI64_1 += 7; }
    goto l_6;
  l_17:
    { HPIPE_DATA.__UsrId_0 += PI64( *data - 128 ) << HPIPE_DATA.__PI64_1; HPIPE_DATA.__PI64_1 += 7; }
    goto l_7;
  c_1:
    if ( ! buf ) goto l_8;
    while ( buf->next ) { end -= buf->used; buf = buf->next; if ( buf && buf->used ) { data = buf->data; end_m1 = buf->data + ( buf->used < end ? buf->used : end ) - 1; goto l_1; } }
    goto l_8;
  c_2:
    while ( buf->next ) { end -= buf->used; buf = buf->next; if ( buf && buf->used ) { data = buf->data; end_m1 = buf->data + ( buf->used < end ? buf->used : end ) - 1; goto l_10; } }
    goto l_3;
  c_4:
    while ( buf->next ) { end -= buf->used; buf = buf->next; if ( buf && buf->used ) { data = buf->data; end_m1 = buf->data + ( buf->used < end ? buf->used : end ) - 1; goto l_13; } }
    goto l_3;
  c_6:
    while ( buf->next ) { end -= buf->used; buf = buf->next; if ( buf && buf->used ) { data = buf->data; end_m1 = buf->data + ( buf->used < end ? buf->used : end ) - 1; goto l_1; } }
    goto l_8;
  c_7:
    while ( buf->next ) { end -= buf->used; buf = buf->next; if ( buf && buf->used ) { data = buf->data; end_m1 = buf->data + ( buf->used < end ? buf->used : end ) - 1; goto l_16; } }
    goto l_3;
  c_8:
    while ( buf->next ) { end -= buf->used; buf = buf->next; if ( buf && buf->used ) { data = buf->data; end_m1 = buf->data + ( buf->used < end ? buf->used : end ) - 1; goto l_18; } }
    goto l_3;
}
#endif // HPIPE_DEFINITIONS
