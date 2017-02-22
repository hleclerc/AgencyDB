#ifndef HPIPE_NO_DECL
// constants
static const unsigned RET_CONT      = 0;
static const unsigned RET_OK        = 1;
static const unsigned RET_KO        = 2;
static const unsigned RET_ENDED_OK  = 3;
static const unsigned RET_ENDED_KO  = 4;
static const unsigned RET_STOP_CONT = 5;
struct HpipeData {
    HpipeData() : inp_cont( 0 ) {}
    void *inp_cont;
};
#ifndef HPIPE_BUFFER
#define HPIPE_BUFFER Hpipe::Buffer
#endif
unsigned parse( HpipeData *sipe_data, HPIPE_BUFFER *buf, bool last_buf, const unsigned char *data = 0, const unsigned char *end_m1 = 0 );
#endif // HPIPE_NO_DECL

#ifndef HPIPE_NO_DEF
#ifndef HPIPE_METHOD_PREFIX
#define HPIPE_METHOD_PREFIX
#endif
unsigned HPIPE_METHOD_PREFIX parse( HpipeData *sipe_data, HPIPE_BUFFER *buf, bool last_buf, const unsigned char *data, const unsigned char *end_m1 ) {
    if ( ! data ) data = buf->data;
    if ( ! end_m1 ) end_m1 = buf->data - 1 + buf->used;
    if ( sipe_data->inp_cont ) goto *sipe_data->inp_cont;
    if ( data > end_m1 ) goto c_1;
  l_2:
    if ( data[ 0 ] == 'G' ) goto l_3;
  l_1:
    sipe_data->inp_cont = &&c_2;
    return RET_KO;
  l_3:
    if ( data >= end_m1 ) goto c_3;
    ++data;
  l_4:
    if ( data[ 0 ] != 'E' ) goto l_1;
    if ( data >= end_m1 ) goto c_4;
    ++data;
  l_5:
    if ( data[ 0 ] != 'T' ) goto l_1;
    { I( "GETOUILLE" ); }
    sipe_data->inp_cont = &&c_5;
    return RET_OK;
  c_1:
    if ( last_buf ) goto l_1;
    sipe_data->inp_cont = &&e_1;
    return RET_CONT;
  e_1:
    if ( data > end_m1 ) goto c_1;
    goto l_2;
  c_2:
    return RET_ENDED_KO;
  c_3:
    if ( last_buf ) goto l_1;
    sipe_data->inp_cont = &&e_3;
    return RET_CONT;
  e_3:
    if ( data > end_m1 ) goto c_3;
    goto l_4;
  c_4:
    if ( last_buf ) goto l_1;
    sipe_data->inp_cont = &&e_4;
    return RET_CONT;
  e_4:
    if ( data > end_m1 ) goto c_4;
    goto l_5;
  c_5:
    return RET_ENDED_OK;
}
#endif // HPIPE_NO_DEF
