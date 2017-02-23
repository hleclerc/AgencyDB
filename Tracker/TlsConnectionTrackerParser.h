#ifdef HPIPE_PRELIMINARIES
    #ifdef SIPE_MAIN
#include <string>
#endif // SIPE_MAIN
#endif // HPIPE_PRELIMINARIES

#ifdef HPIPE_DECLARATIONS
// constants
static const unsigned RET_CONT      = 0;
static const unsigned RET_OK        = 1;
static const unsigned RET_KO        = 2;
static const unsigned RET_ENDED_OK  = 3;
static const unsigned RET_ENDED_KO  = 4;
static const unsigned RET_STOP_CONT = 5;
struct HpipeData {
    HpipeData() : inp_cont( 0 ) {}
    std::string url;
    void *inp_cont;
};
#ifndef HPIPE_BUFFER
#define HPIPE_BUFFER Hpipe::Buffer
#endif
unsigned parse( HPIPE_BUFFER *buf, bool last_buf, const unsigned char *data = 0, const unsigned char *end_m1 = 0 );
#endif // HPIPE_DECLARATIONS

#ifdef HPIPE_DEFINITIONS
#ifndef HPIPE_METHOD_PREFIX
#define HPIPE_METHOD_PREFIX
#endif
unsigned HPIPE_METHOD_PREFIX parse( HPIPE_BUFFER *buf, bool last_buf, const unsigned char *data, const unsigned char *end_m1 ) {
    HpipeData *sipe_data = &hpipe_data;
    if ( ! data ) data = buf->data;
    if ( ! end_m1 ) end_m1 = buf->data - 1 + buf->used;
    if ( sipe_data->inp_cont ) goto *sipe_data->inp_cont;
    if ( data > end_m1 ) goto c_1;
  l_3:
    if ( data[ 0 ] == 'G' ) goto l_4;
  l_1:
    sipe_data->inp_cont = &&c_2;
    return RET_KO;
  l_4:
    if ( data >= end_m1 ) goto c_3;
    ++data;
  l_5:
    if ( data[ 0 ] != 'E' ) goto l_1;
    if ( data >= end_m1 ) goto c_4;
    ++data;
  l_6:
    if ( data[ 0 ] != 'T' ) goto l_1;
    if ( data >= end_m1 ) goto c_5;
    ++data;
  l_7:
    if ( data[ 0 ] != ' ' ) goto l_1;
    { sipe_data->url.clear(); }
  l_2:
    if ( data >= end_m1 ) goto c_6;
    ++data;
  l_9:
    if ( data[ 0 ] == ' ' ) goto l_10;
    { sipe_data->url += *data; }
    goto l_2;
  l_10:
    { I( "GETOUILLE", sipe_data->url ); }
  l_8:
    sipe_data->inp_cont = &&c_7;
    return RET_OK;
  c_1:
    if ( last_buf ) goto l_1;
    sipe_data->inp_cont = &&e_1;
    return RET_CONT;
  e_1:
    if ( data > end_m1 ) goto c_1;
    goto l_3;
  c_2:
    return RET_ENDED_KO;
  c_3:
    if ( last_buf ) goto l_1;
    sipe_data->inp_cont = &&e_3;
    return RET_CONT;
  e_3:
    if ( data > end_m1 ) goto c_3;
    goto l_5;
  c_4:
    if ( last_buf ) goto l_1;
    sipe_data->inp_cont = &&e_4;
    return RET_CONT;
  e_4:
    if ( data > end_m1 ) goto c_4;
    goto l_6;
  c_5:
    if ( last_buf ) goto l_1;
    sipe_data->inp_cont = &&e_5;
    return RET_CONT;
  e_5:
    if ( data > end_m1 ) goto c_5;
    goto l_7;
  c_6:
    if ( last_buf ) goto l_8;
    sipe_data->inp_cont = &&e_6;
    return RET_CONT;
  e_6:
    if ( data > end_m1 ) goto c_6;
    goto l_9;
  c_7:
    return RET_ENDED_OK;
}
#endif // HPIPE_DEFINITIONS
