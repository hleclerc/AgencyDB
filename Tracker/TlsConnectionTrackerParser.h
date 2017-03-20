// ----------------------------------------------------------------------------------------------------
#ifdef HPIPE_PRELIMINARIES
#endif // HPIPE_PRELIMINARIES

// ----------------------------------------------------------------------------------------------------
#ifdef HPIPE_DECLARATIONS
enum {
    RET_CONT      = 0,
    RET_OK        = 1,
    RET_KO        = 2,
    RET_ENDED_OK  = 3,
    RET_ENDED_KO  = 4,
    RET_STOP_CONT = 5
};

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
  #ifdef __cplusplus
  HPIPE_DATA_STRUCT_NAME() { HPIPE_DATA_STRUCT_NAME *hpipe_data = this;
    hpipe_data->inp_cont = 0;
}
  #endif // __cplusplus
  void *inp_cont;
};
static void HPIPE_DATA_CTOR_NAME( HPIPE_DATA_STRUCT_NAME *hpipe_data ) {
    hpipe_data->inp_cont = 0;
}
unsigned HPIPE_PARSE_FUNC_NAME( HPIPE_ADDITIONAL_ARGS HPIPE_BUFF_T *buf, bool last_buf, const HPIPE_CHAR_T *data = 0, const HPIPE_CHAR_T *end_m1 = 0 );
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

unsigned HPIPE_DEFINITION_PREFIX HPIPE_PARSE_FUNC_NAME( HPIPE_ADDITIONAL_ARGS HPIPE_BUFF_T *buf, bool last_buf, const HPIPE_CHAR_T *data, const HPIPE_CHAR_T *end_m1 ) {
    if ( ! data ) data = buf->data;
    if ( ! end_m1 ) end_m1 = buf->data - 1 + buf->used;
    if ( HPIPE_DATA.inp_cont ) goto *HPIPE_DATA.inp_cont;
  l_1:
    if ( data > end_m1 ) goto c_1;
  l_11:
    if ( data[ 0 ] != 'G' ) goto l_9;
  l_2:
    if ( data >= end_m1 ) goto c_2;
    ++data;
  l_12:
    if ( data[ 0 ] != 'E' ) goto l_9;
  l_3:
    if ( data >= end_m1 ) goto c_3;
    ++data;
  l_13:
    if ( data[ 0 ] != 'T' ) goto l_9;
  l_4:
    if ( data >= end_m1 ) goto c_4;
    ++data;
  l_14:
    if ( data[ 0 ] != ' ' ) goto l_9;
  l_5:
    if ( data >= end_m1 ) goto c_5;
    ++data;
  l_15:
    if ( data[ 0 ] != '/' ) goto l_9;
  l_6:
    if ( data >= end_m1 ) goto c_6;
    ++data;
  l_16:
    if ( data[ 0 ] == 'w' ) goto l_7;
    if ( data[ 0 ] != ' ' ) goto l_9;
    { index_html(); }
  l_10:
    HPIPE_DATA.inp_cont = &&c_7;
    return RET_OK;
  l_9:
    HPIPE_DATA.inp_cont = &&c_8;
    return RET_KO;
  l_7:
    if ( data >= end_m1 ) goto c_9;
    ++data;
  l_17:
    if ( data[ 0 ] != 's' ) goto l_9;
  l_8:
    if ( data >= end_m1 ) goto c_10;
    ++data;
  l_18:
    if ( data[ 0 ] != ' ' ) goto l_9;
    { websocket(); }
    goto l_10;
  c_1:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_11; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_1;
    return RET_CONT;
  e_1:
    if ( data > end_m1 ) goto c_1;
    goto l_11;
  c_2:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_12; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_2;
    return RET_CONT;
  e_2:
    if ( data > end_m1 ) goto c_2;
    goto l_12;
  c_3:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_13; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_3;
    return RET_CONT;
  e_3:
    if ( data > end_m1 ) goto c_3;
    goto l_13;
  c_4:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_14; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_4;
    return RET_CONT;
  e_4:
    if ( data > end_m1 ) goto c_4;
    goto l_14;
  c_5:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_15; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_5;
    return RET_CONT;
  e_5:
    if ( data > end_m1 ) goto c_5;
    goto l_15;
  c_6:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_16; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_6;
    return RET_CONT;
  e_6:
    if ( data > end_m1 ) goto c_6;
    goto l_16;
  c_7:
    return RET_ENDED_OK;
  c_8:
    return RET_ENDED_KO;
  c_9:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_17; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_9;
    return RET_CONT;
  e_9:
    if ( data > end_m1 ) goto c_9;
    goto l_17;
  c_10:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_18; } }
    if ( last_buf ) goto l_9;
    HPIPE_DATA.inp_cont = &&e_10;
    return RET_CONT;
  e_10:
    if ( data > end_m1 ) goto c_10;
    goto l_18;
}
#endif // HPIPE_DEFINITIONS
