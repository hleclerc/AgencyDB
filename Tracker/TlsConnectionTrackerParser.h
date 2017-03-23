// ----------------------------------------------------------------------------------------------------
#ifdef HPIPE_PRELIMINARIES
#ifndef HPIPE_SIZE_T
#define HPIPE_SIZE_T size_t
#endif // HPIPE_SIZE_T

#ifndef HPIPE_CB_STRING_T
#define HPIPE_CB_STRING_T Hpipe::CbString
#endif // HPIPE_CB_STRING_T

#ifndef HPIPE_CB_STRING__ASSIGN_BEG_END
#define HPIPE_CB_STRING__ASSIGN_BEG_END( var, beg_buf, beg_ptr, end_buf, end_ptr ) var = HPIPE_CB_STRING_T( HPIPE_CB_STRING_T::NoIncRef(), beg_buf, beg_ptr, end_buf, end_ptr )
#endif // HPIPE_CB_STRING__ASSIGN_BEG_END

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
  #ifdef __cplusplus
  HPIPE_DATA_STRUCT_NAME() { HPIPE_DATA_STRUCT_NAME *hpipe_data = this;
    hpipe_data->inp_cont = 0;
  }
  #endif // __cplusplus
  unsigned __PI32_0;
  unsigned __PI32_1;
  HPIPE_CB_STRING_T __String_0;
  HPIPE_CB_STRING_T __String_1;
  const HPIPE_BUFF_T * __beg___String_0_buf;
  const HPIPE_CHAR_T * __beg___String_0_data;
  HPIPE_SIZE_T __beg___String_0_off;
  const HPIPE_BUFF_T * __beg___String_1_buf;
  const HPIPE_CHAR_T * __beg___String_1_data;
  HPIPE_SIZE_T __beg___String_1_off;
  const HPIPE_BUFF_T * __beg_websocket_content_buf;
  const HPIPE_CHAR_T * __beg_websocket_content_data;
  HPIPE_SIZE_T __beg_websocket_content_off;
  const HPIPE_BUFF_T * __beg_websocket_key_buf;
  const HPIPE_CHAR_T * __beg_websocket_key_data;
  HPIPE_SIZE_T __beg_websocket_key_off;
  HPIPE_SIZE_T __bytes_to_skip;
  HPIPE_BUFF_T * pending_buf;
  unsigned char websocket_byte_0;
  HPIPE_CB_STRING_T websocket_content;
  HPIPE_CB_STRING_T websocket_key;
  __int32_t websocket_mask;
  size_t websocket_msg_length;
  unsigned websocket_version;
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

#ifndef HPIPE_BUFF_T__SKIP_N
#define HPIPE_BUFF_T__SKIP_N( buf, ptr, N, K ) HPIPE_BUFF_T::skip( buf, ptr, N, K )
#endif

#ifndef HPIPE_BUFF_T__DEC_REF_UPTO
#define HPIPE_BUFF_T__DEC_REF_UPTO( beg, end ) beg->dec_ref_upto( end )
#endif

#ifndef HPIPE_BUFF_T__DEC_REF_UPTO_N
#define HPIPE_BUFF_T__DEC_REF_UPTO_N( beg, end, K ) beg->dec_ref_upto( end, K )
#endif

unsigned HPIPE_DEFINITION_PREFIX HPIPE_PARSE_FUNC_NAME( HPIPE_ADDITIONAL_ARGS HPIPE_BUFF_T *buf, bool last_buf, const HPIPE_CHAR_T *data, const HPIPE_CHAR_T *end_m1 ) {
    if ( ! data ) data = buf->data;
    if ( ! end_m1 ) end_m1 = buf->data - 1 + buf->used;
    if ( HPIPE_DATA.inp_cont ) goto *HPIPE_DATA.inp_cont;
  l_1:
    if ( data > end_m1 ) goto c_1;
  l_89:
    if ( data[ 0 ] == 0 ) goto l_88;
    if ( data[ 0 ] != 'G' ) goto l_81;
  l_2:
    if ( data >= end_m1 ) goto c_2;
    ++data;
  l_90:
    if ( data[ 0 ] != 'E' ) goto l_81;
  l_3:
    if ( data >= end_m1 ) goto c_3;
    ++data;
  l_91:
    if ( data[ 0 ] != 'T' ) goto l_81;
  l_4:
    if ( data >= end_m1 ) goto c_4;
    ++data;
  l_92:
    if ( data[ 0 ] != ' ' ) goto l_81;
  l_5:
    if ( data >= end_m1 ) goto c_5;
    ++data;
  l_93:
    if ( data[ 0 ] != '/' ) goto l_81;
  l_6:
    if ( data >= end_m1 ) goto c_6;
    ++data;
  l_94:
    if ( __builtin_expect( data[ 0 ] == 'w', 0 ) ) goto l_18;
    if ( data[ 0 ] == 'f' ) goto l_7;
    if ( data[ 0 ] != ' ' ) goto l_81;
    { index_html(); }
  l_81:
    HPIPE_DATA.inp_cont = &&c_7;
    return RET_OK;
  l_7:
    if ( data >= end_m1 ) goto c_8;
    ++data;
  l_95:
    if ( data[ 0 ] != 'a' ) goto l_81;
  l_8:
    if ( data >= end_m1 ) goto c_9;
    ++data;
  l_96:
    if ( data[ 0 ] != 'v' ) goto l_81;
  l_9:
    if ( data >= end_m1 ) goto c_10;
    ++data;
  l_97:
    if ( data[ 0 ] != 'i' ) goto l_81;
  l_10:
    if ( data >= end_m1 ) goto c_11;
    ++data;
  l_98:
    if ( data[ 0 ] != 'c' ) goto l_81;
  l_11:
    if ( data >= end_m1 ) goto c_12;
    ++data;
  l_99:
    if ( data[ 0 ] != 'o' ) goto l_81;
  l_12:
    if ( data >= end_m1 ) goto c_13;
    ++data;
  l_100:
    if ( data[ 0 ] != 'n' ) goto l_81;
  l_13:
    if ( data >= end_m1 ) goto c_14;
    ++data;
  l_101:
    if ( data[ 0 ] != '.' ) goto l_81;
  l_14:
    if ( data >= end_m1 ) goto c_15;
    ++data;
  l_102:
    if ( data[ 0 ] != 'i' ) goto l_81;
  l_15:
    if ( data >= end_m1 ) goto c_16;
    ++data;
  l_103:
    if ( data[ 0 ] != 'c' ) goto l_81;
  l_16:
    if ( data >= end_m1 ) goto c_17;
    ++data;
  l_104:
    if ( data[ 0 ] != 'o' ) goto l_81;
  l_17:
    if ( data >= end_m1 ) goto c_18;
    ++data;
  l_105:
    if ( data[ 0 ] != ' ' ) goto l_81;
    { favicon(); }
    goto l_81;
  l_18:
    if ( data >= end_m1 ) goto c_19;
    ++data;
  l_106:
    if ( data[ 0 ] != 's' ) goto l_81;
  l_19:
    if ( data >= end_m1 ) goto c_20;
    ++data;
  l_107:
    if ( data[ 0 ] != ' ' ) goto l_81;
  l_20:
    if ( data >= end_m1 ) goto c_21;
    ++data;
  l_87:
    if ( data[ 0 ] != 10 ) goto l_20;
  l_21:
    if ( data >= end_m1 ) goto c_22;
    ++data;
  l_108:
    if ( __builtin_expect( data[ 0 ] == 'S', 0 ) ) goto l_49;
    if ( data[ 0 ] == 13 ) goto l_48;
  l_86:
    if ( data[ 0 ] != 10 ) goto l_20;
    { ws_handshake(); }
  l_22:
    if ( data >= end_m1 ) goto c_23;
    ++data;
  l_109:
    { HPIPE_DATA.websocket_byte_0 = *data; }
  l_23:
    if ( data >= end_m1 ) goto c_24;
    ++data;
  l_110:
    if ( __builtin_expect( data[ 0 ] == 255, 0 ) ) goto l_40;
    if ( __builtin_expect( data[ 0 ] == 254, 0 ) ) goto l_38;
    if ( __builtin_expect( data[ 0 ] >= 128, 0 ) ) goto l_111;
    if ( data[ 0 ] == 127 ) goto l_26;
    if ( data[ 0 ] == '~' ) goto l_24;
    { HPIPE_DATA.websocket_msg_length  = *data; }
  l_83:
    HPIPE_DATA.__beg_websocket_content_off = 1;
    HPIPE_DATA.__beg_websocket_content_buf = buf;
    HPIPE_DATA.__beg_websocket_content_data = data;
    if ( data + HPIPE_DATA.websocket_msg_length <= end_m1 ) {
        data += HPIPE_DATA.websocket_msg_length;
    } else {
        HPIPE_DATA.__bytes_to_skip = HPIPE_DATA.websocket_msg_length - ( buf->data + buf->used - data );
      t_25:
        if ( ! buf->next ) {
            if ( last_buf )
                goto l_84;
            HPIPE_DATA.pending_buf = buf;
            HPIPE_BUFF_T__INC_REF( buf );
            HPIPE_DATA.inp_cont = &&e_25;
            return RET_CONT;
          e_25:
            HPIPE_DATA.pending_buf->next = buf;
            HPIPE_DATA.pending_buf = buf;
            goto c_25;
        }
        buf = buf->next;
      c_25:
        if ( HPIPE_DATA.__bytes_to_skip >= buf->used ) {
            HPIPE_DATA.__bytes_to_skip -= buf->used;
            goto t_25;
        }
        data = buf->data + HPIPE_DATA.__bytes_to_skip;
        end_m1 = buf->data + buf->used - 1;
    }
    HPIPE_BUFF_T__SKIP( &HPIPE_DATA.__beg_websocket_content_buf, HPIPE_DATA.__beg_websocket_content_data, HPIPE_DATA.__beg_websocket_content_off );
    HPIPE_BUFF_T__INC_REF( buf );
    HPIPE_CB_STRING__ASSIGN_BEG_END( HPIPE_DATA.websocket_content, HPIPE_DATA.__beg_websocket_content_buf, HPIPE_DATA.__beg_websocket_content_data, buf, data + 1 );
  l_85:
    { ws_content(); }
    goto l_22;
  l_84:
    HPIPE_BUFF_T::skip( &HPIPE_DATA.__beg_websocket_content_buf, HPIPE_DATA.__beg_websocket_content_data, HPIPE_BUFF_T::size_between( HPIPE_DATA.__beg_websocket_content_buf, HPIPE_DATA.__beg_websocket_content_data, buf, data ) );
  l_82:
    HPIPE_DATA.inp_cont = &&c_26;
    return RET_KO;
  l_24:
    if ( data >= end_m1 ) goto c_27;
    ++data;
  l_112:
    { HPIPE_DATA.websocket_msg_length  = __uint16_t( *data ) << 8; }
  l_25:
    if ( data >= end_m1 ) goto c_28;
    ++data;
  l_113:
    { HPIPE_DATA.websocket_msg_length += __uint16_t( *data ) << 0; }
    goto l_83;
  l_26:
    if ( data >= end_m1 ) goto c_29;
    ++data;
  l_114:
    { HPIPE_DATA.websocket_msg_length  = __uint64_t( *data ) << 56; }
  l_27:
    if ( data >= end_m1 ) goto c_30;
    ++data;
  l_115:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 48; }
  l_28:
    if ( data >= end_m1 ) goto c_31;
    ++data;
  l_116:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 40; }
  l_29:
    if ( data >= end_m1 ) goto c_32;
    ++data;
  l_117:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 32; }
  l_30:
    if ( data >= end_m1 ) goto c_33;
    ++data;
  l_118:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 24; }
  l_31:
    if ( data >= end_m1 ) goto c_34;
    ++data;
  l_119:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 16; }
  l_32:
    if ( data >= end_m1 ) goto c_35;
    ++data;
  l_120:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) <<  8; }
  l_33:
    if ( data >= end_m1 ) goto c_36;
    ++data;
  l_121:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) <<  0; }
    goto l_83;
  l_111:
    { HPIPE_DATA.websocket_msg_length  = *data - 128; }
  l_34:
    if ( data >= end_m1 ) goto c_37;
    ++data;
  l_122:
    { reinterpret_cast<unsigned char *>( &HPIPE_DATA.websocket_mask )[ 0 ] = *data; }
  l_35:
    if ( data >= end_m1 ) goto c_38;
    ++data;
  l_123:
    { reinterpret_cast<unsigned char *>( &HPIPE_DATA.websocket_mask )[ 1 ] = *data; }
  l_36:
    if ( data >= end_m1 ) goto c_39;
    ++data;
  l_124:
    { reinterpret_cast<unsigned char *>( &HPIPE_DATA.websocket_mask )[ 2 ] = *data; }
  l_37:
    if ( data >= end_m1 ) goto c_40;
    ++data;
  l_125:
    { reinterpret_cast<unsigned char *>( &HPIPE_DATA.websocket_mask )[ 3 ] = *data; }
    HPIPE_DATA.__beg_websocket_content_off = 1;
    HPIPE_DATA.__beg_websocket_content_buf = buf;
    HPIPE_DATA.__beg_websocket_content_data = data;
    if ( data + HPIPE_DATA.websocket_msg_length <= end_m1 ) {
        data += HPIPE_DATA.websocket_msg_length;
    } else {
        HPIPE_DATA.__bytes_to_skip = HPIPE_DATA.websocket_msg_length - ( buf->data + buf->used - data );
      t_41:
        if ( ! buf->next ) {
            if ( last_buf )
                goto l_84;
            HPIPE_DATA.pending_buf = buf;
            HPIPE_BUFF_T__INC_REF( buf );
            HPIPE_DATA.inp_cont = &&e_41;
            return RET_CONT;
          e_41:
            HPIPE_DATA.pending_buf->next = buf;
            HPIPE_DATA.pending_buf = buf;
            goto c_41;
        }
        buf = buf->next;
      c_41:
        if ( HPIPE_DATA.__bytes_to_skip >= buf->used ) {
            HPIPE_DATA.__bytes_to_skip -= buf->used;
            goto t_41;
        }
        data = buf->data + HPIPE_DATA.__bytes_to_skip;
        end_m1 = buf->data + buf->used - 1;
    }
    HPIPE_BUFF_T__SKIP( &HPIPE_DATA.__beg_websocket_content_buf, HPIPE_DATA.__beg_websocket_content_data, HPIPE_DATA.__beg_websocket_content_off );
    HPIPE_BUFF_T__INC_REF( buf );
    HPIPE_CB_STRING__ASSIGN_BEG_END( HPIPE_DATA.websocket_content, HPIPE_DATA.__beg_websocket_content_buf, HPIPE_DATA.__beg_websocket_content_data, buf, data + 1 );
    { ws_unmask(); }
    goto l_85;
  l_38:
    if ( data >= end_m1 ) goto c_42;
    ++data;
  l_126:
    { HPIPE_DATA.websocket_msg_length  = __uint16_t( *data ) << 8; }
  l_39:
    if ( data >= end_m1 ) goto c_43;
    ++data;
  l_127:
    { HPIPE_DATA.websocket_msg_length += __uint16_t( *data ) << 0; }
    goto l_34;
  l_40:
    if ( data >= end_m1 ) goto c_44;
    ++data;
  l_128:
    { HPIPE_DATA.websocket_msg_length  = __uint64_t( *data ) << 56; }
  l_41:
    if ( data >= end_m1 ) goto c_45;
    ++data;
  l_129:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 48; }
  l_42:
    if ( data >= end_m1 ) goto c_46;
    ++data;
  l_130:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 40; }
  l_43:
    if ( data >= end_m1 ) goto c_47;
    ++data;
  l_131:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 32; }
  l_44:
    if ( data >= end_m1 ) goto c_48;
    ++data;
  l_132:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 24; }
  l_45:
    if ( data >= end_m1 ) goto c_49;
    ++data;
  l_133:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) << 16; }
  l_46:
    if ( data >= end_m1 ) goto c_50;
    ++data;
  l_134:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) <<  8; }
  l_47:
    if ( data >= end_m1 ) goto c_51;
    ++data;
  l_135:
    { HPIPE_DATA.websocket_msg_length += __uint64_t( *data ) <<  0; }
    goto l_34;
  l_48:
    if ( data >= end_m1 ) goto c_52;
    ++data;
    goto l_86;
  l_49:
    if ( data >= end_m1 ) goto c_53;
    ++data;
  l_136:
    if ( data[ 0 ] != 'e' ) goto l_87;
  l_50:
    if ( data >= end_m1 ) goto c_54;
    ++data;
  l_137:
    if ( data[ 0 ] != 'c' ) goto l_87;
  l_51:
    if ( data >= end_m1 ) goto c_55;
    ++data;
  l_138:
    if ( data[ 0 ] != '-' ) goto l_87;
  l_52:
    if ( data >= end_m1 ) goto c_56;
    ++data;
  l_139:
    if ( data[ 0 ] != 'W' ) goto l_87;
  l_53:
    if ( data >= end_m1 ) goto c_57;
    ++data;
  l_140:
    if ( data[ 0 ] != 'e' ) goto l_87;
  l_54:
    if ( data >= end_m1 ) goto c_58;
    ++data;
  l_141:
    if ( data[ 0 ] != 'b' ) goto l_87;
  l_55:
    if ( data >= end_m1 ) goto c_59;
    ++data;
  l_142:
    if ( data[ 0 ] != 'S' ) goto l_87;
  l_56:
    if ( data >= end_m1 ) goto c_60;
    ++data;
  l_143:
    if ( data[ 0 ] != 'o' ) goto l_87;
  l_57:
    if ( data >= end_m1 ) goto c_61;
    ++data;
  l_144:
    if ( data[ 0 ] != 'c' ) goto l_87;
  l_58:
    if ( data >= end_m1 ) goto c_62;
    ++data;
  l_145:
    if ( data[ 0 ] != 'k' ) goto l_87;
  l_59:
    if ( data >= end_m1 ) goto c_63;
    ++data;
  l_146:
    if ( data[ 0 ] != 'e' ) goto l_87;
  l_60:
    if ( data >= end_m1 ) goto c_64;
    ++data;
  l_147:
    if ( data[ 0 ] != 't' ) goto l_87;
  l_61:
    if ( data >= end_m1 ) goto c_65;
    ++data;
  l_148:
    if ( data[ 0 ] != '-' ) goto l_87;
  l_62:
    if ( data >= end_m1 ) goto c_66;
    ++data;
  l_149:
    if ( __builtin_expect( data[ 0 ] == 'V', 0 ) ) goto l_68;
    if ( data[ 0 ] != 'K' ) goto l_87;
  l_63:
    if ( data >= end_m1 ) goto c_67;
    ++data;
  l_150:
    if ( data[ 0 ] != 'e' ) goto l_87;
  l_64:
    if ( data >= end_m1 ) goto c_68;
    ++data;
  l_151:
    if ( data[ 0 ] != 'y' ) goto l_87;
  l_65:
    if ( data >= end_m1 ) goto c_69;
    ++data;
  l_152:
    if ( data[ 0 ] != ':' ) goto l_87;
  l_66:
    if ( data >= end_m1 ) goto c_70;
    ++data;
  l_153:
    if ( data[ 0 ] != ' ' ) goto l_87;
    HPIPE_DATA.__beg_websocket_key_off = 1;
    HPIPE_DATA.__beg_websocket_key_buf = buf;
    HPIPE_DATA.__beg_websocket_key_data = data;
  l_67:
    if ( data >= end_m1 ) goto c_71;
    ++data;
  l_154:
    if ( data[ 0 ] == 13 ) goto l_155;
    if ( data[ 0 ] != 10 ) goto l_67;
    HPIPE_BUFF_T__SKIP( &HPIPE_DATA.__beg_websocket_key_buf, HPIPE_DATA.__beg_websocket_key_data, HPIPE_DATA.__beg_websocket_key_off );
    if ( data > buf->data ) HPIPE_BUFF_T__INC_REF( buf );
    HPIPE_CB_STRING__ASSIGN_BEG_END( HPIPE_DATA.websocket_key, HPIPE_DATA.__beg_websocket_key_buf, HPIPE_DATA.__beg_websocket_key_data, buf, data );
    goto l_21;
  l_155:
    HPIPE_BUFF_T__SKIP( &HPIPE_DATA.__beg_websocket_key_buf, HPIPE_DATA.__beg_websocket_key_data, HPIPE_DATA.__beg_websocket_key_off );
    if ( data > buf->data ) HPIPE_BUFF_T__INC_REF( buf );
    HPIPE_CB_STRING__ASSIGN_BEG_END( HPIPE_DATA.websocket_key, HPIPE_DATA.__beg_websocket_key_buf, HPIPE_DATA.__beg_websocket_key_data, buf, data );
    goto l_20;
  l_68:
    if ( data >= end_m1 ) goto c_72;
    ++data;
  l_156:
    if ( data[ 0 ] != 'e' ) goto l_87;
  l_69:
    if ( data >= end_m1 ) goto c_73;
    ++data;
  l_157:
    if ( data[ 0 ] != 'r' ) goto l_87;
  l_70:
    if ( data >= end_m1 ) goto c_74;
    ++data;
  l_158:
    if ( data[ 0 ] != 's' ) goto l_87;
  l_71:
    if ( data >= end_m1 ) goto c_75;
    ++data;
  l_159:
    if ( data[ 0 ] != 'i' ) goto l_87;
  l_72:
    if ( data >= end_m1 ) goto c_76;
    ++data;
  l_160:
    if ( data[ 0 ] != 'o' ) goto l_87;
  l_73:
    if ( data >= end_m1 ) goto c_77;
    ++data;
  l_161:
    if ( data[ 0 ] != 'n' ) goto l_87;
  l_74:
    if ( data >= end_m1 ) goto c_78;
    ++data;
  l_162:
    if ( data[ 0 ] != ':' ) goto l_87;
  l_75:
    if ( data >= end_m1 ) goto c_79;
    ++data;
  l_163:
    if ( data[ 0 ] != ' ' ) goto l_87;
  l_76:
    if ( data >= end_m1 ) goto c_80;
    ++data;
  l_164:
    if ( data[ 0 ] == 10 ) goto l_21;
    if ( data[ 0 ] <= '/' ) goto l_20;
    if ( __builtin_expect( data[ 0 ] >= ':', 0 ) ) goto l_20;
    { HPIPE_DATA.websocket_version = *data - '0'; }
  l_77:
    if ( data >= end_m1 ) goto c_81;
    ++data;
  l_165:
    if ( data[ 0 ] == 10 ) goto l_21;
    if ( data[ 0 ] <= '/' ) goto l_20;
    if ( __builtin_expect( data[ 0 ] >= ':', 0 ) ) goto l_20;
    { HPIPE_DATA.websocket_version = 10 * HPIPE_DATA.websocket_version + *data - '0'; }
    goto l_77;
  l_88:
    { HPIPE_DATA.__PI32_0 = 0; HPIPE_DATA.__PI32_1 = 0; }
  l_78:
    if ( data >= end_m1 ) goto c_82;
    ++data;
  l_166:
    if ( data[ 0 ] >= 128 ) goto l_167;
    { HPIPE_DATA.__PI32_0 += PI32( *data       ) << HPIPE_DATA.__PI32_1; }
    HPIPE_DATA.__beg___String_0_off = 1;
    HPIPE_DATA.__beg___String_0_buf = buf;
    HPIPE_DATA.__beg___String_0_data = data;
    if ( data + HPIPE_DATA.__PI32_0 <= end_m1 ) {
        data += HPIPE_DATA.__PI32_0;
    } else {
        HPIPE_DATA.__bytes_to_skip = HPIPE_DATA.__PI32_0 - ( buf->data + buf->used - data );
      t_83:
        if ( ! buf->next ) {
            if ( last_buf )
                goto l_168;
            HPIPE_DATA.pending_buf = buf;
            HPIPE_BUFF_T__INC_REF( buf );
            HPIPE_DATA.inp_cont = &&e_83;
            return RET_CONT;
          e_83:
            HPIPE_DATA.pending_buf->next = buf;
            HPIPE_DATA.pending_buf = buf;
            goto c_83;
        }
        buf = buf->next;
      c_83:
        if ( HPIPE_DATA.__bytes_to_skip >= buf->used ) {
            HPIPE_DATA.__bytes_to_skip -= buf->used;
            goto t_83;
        }
        data = buf->data + HPIPE_DATA.__bytes_to_skip;
        end_m1 = buf->data + buf->used - 1;
    }
    HPIPE_BUFF_T__SKIP( &HPIPE_DATA.__beg___String_0_buf, HPIPE_DATA.__beg___String_0_data, HPIPE_DATA.__beg___String_0_off );
    HPIPE_BUFF_T__INC_REF( buf );
    HPIPE_CB_STRING__ASSIGN_BEG_END( HPIPE_DATA.__String_0, HPIPE_DATA.__beg___String_0_buf, HPIPE_DATA.__beg___String_0_data, buf, data + 1 );
    { HPIPE_DATA.__PI32_0 = 0; HPIPE_DATA.__PI32_1 = 0; }
  l_79:
    if ( data >= end_m1 ) goto c_84;
    ++data;
  l_169:
    if ( data[ 0 ] >= 128 ) goto l_170;
    { HPIPE_DATA.__PI32_0 += PI32( *data       ) << HPIPE_DATA.__PI32_1; }
    HPIPE_DATA.__beg___String_1_off = 1;
    HPIPE_DATA.__beg___String_1_buf = buf;
    HPIPE_DATA.__beg___String_1_data = data;
    if ( data + HPIPE_DATA.__PI32_0 <= end_m1 ) {
        data += HPIPE_DATA.__PI32_0;
    } else {
        HPIPE_DATA.__bytes_to_skip = HPIPE_DATA.__PI32_0 - ( buf->data + buf->used - data );
      t_85:
        if ( ! buf->next ) {
            if ( last_buf )
                goto l_171;
            HPIPE_DATA.pending_buf = buf;
            HPIPE_BUFF_T__INC_REF( buf );
            HPIPE_DATA.inp_cont = &&e_85;
            return RET_CONT;
          e_85:
            HPIPE_DATA.pending_buf->next = buf;
            HPIPE_DATA.pending_buf = buf;
            goto c_85;
        }
        buf = buf->next;
      c_85:
        if ( HPIPE_DATA.__bytes_to_skip >= buf->used ) {
            HPIPE_DATA.__bytes_to_skip -= buf->used;
            goto t_85;
        }
        data = buf->data + HPIPE_DATA.__bytes_to_skip;
        end_m1 = buf->data + buf->used - 1;
    }
    HPIPE_BUFF_T__SKIP( &HPIPE_DATA.__beg___String_1_buf, HPIPE_DATA.__beg___String_1_data, HPIPE_DATA.__beg___String_1_off );
    HPIPE_BUFF_T__INC_REF( buf );
    HPIPE_CB_STRING__ASSIGN_BEG_END( HPIPE_DATA.__String_1, HPIPE_DATA.__beg___String_1_buf, HPIPE_DATA.__beg___String_1_data, buf, data + 1 );
    { pwd_login( HPIPE_DATA.__String_0, HPIPE_DATA.__String_1 ); }
  l_80:
    if ( data >= end_m1 ) goto c_86;
    ++data;
  l_172:
    if ( data[ 0 ] >= 1 ) goto l_81;
    goto l_88;
  l_171:
    HPIPE_BUFF_T::skip( &HPIPE_DATA.__beg___String_1_buf, HPIPE_DATA.__beg___String_1_data, HPIPE_BUFF_T::size_between( HPIPE_DATA.__beg___String_1_buf, HPIPE_DATA.__beg___String_1_data, buf, data ) );
    goto l_82;
  l_170:
    { HPIPE_DATA.__PI32_0 += PI32( *data - 128 ) << HPIPE_DATA.__PI32_1; HPIPE_DATA.__PI32_1 += 7; }
    goto l_79;
  l_168:
    HPIPE_BUFF_T::skip( &HPIPE_DATA.__beg___String_0_buf, HPIPE_DATA.__beg___String_0_data, HPIPE_BUFF_T::size_between( HPIPE_DATA.__beg___String_0_buf, HPIPE_DATA.__beg___String_0_data, buf, data ) );
    goto l_82;
  l_167:
    { HPIPE_DATA.__PI32_0 += PI32( *data - 128 ) << HPIPE_DATA.__PI32_1; HPIPE_DATA.__PI32_1 += 7; }
    goto l_78;
  c_1:
    if ( ! buf ) goto l_81;
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_89; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_1;
    return RET_CONT;
  e_1:
    if ( data > end_m1 ) goto c_1;
    goto l_89;
  c_2:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_90; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_2;
    return RET_CONT;
  e_2:
    if ( data > end_m1 ) goto c_2;
    goto l_90;
  c_3:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_91; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_3;
    return RET_CONT;
  e_3:
    if ( data > end_m1 ) goto c_3;
    goto l_91;
  c_4:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_92; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_4;
    return RET_CONT;
  e_4:
    if ( data > end_m1 ) goto c_4;
    goto l_92;
  c_5:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_93; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_5;
    return RET_CONT;
  e_5:
    if ( data > end_m1 ) goto c_5;
    goto l_93;
  c_6:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_94; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_6;
    return RET_CONT;
  e_6:
    if ( data > end_m1 ) goto c_6;
    goto l_94;
  c_7:
    return RET_ENDED_OK;
  c_8:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_95; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_8;
    return RET_CONT;
  e_8:
    if ( data > end_m1 ) goto c_8;
    goto l_95;
  c_9:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_96; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_9;
    return RET_CONT;
  e_9:
    if ( data > end_m1 ) goto c_9;
    goto l_96;
  c_10:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_97; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_10;
    return RET_CONT;
  e_10:
    if ( data > end_m1 ) goto c_10;
    goto l_97;
  c_11:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_98; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_11;
    return RET_CONT;
  e_11:
    if ( data > end_m1 ) goto c_11;
    goto l_98;
  c_12:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_99; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_12;
    return RET_CONT;
  e_12:
    if ( data > end_m1 ) goto c_12;
    goto l_99;
  c_13:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_100; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_13;
    return RET_CONT;
  e_13:
    if ( data > end_m1 ) goto c_13;
    goto l_100;
  c_14:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_101; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_14;
    return RET_CONT;
  e_14:
    if ( data > end_m1 ) goto c_14;
    goto l_101;
  c_15:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_102; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_15;
    return RET_CONT;
  e_15:
    if ( data > end_m1 ) goto c_15;
    goto l_102;
  c_16:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_103; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_16;
    return RET_CONT;
  e_16:
    if ( data > end_m1 ) goto c_16;
    goto l_103;
  c_17:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_104; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_17;
    return RET_CONT;
  e_17:
    if ( data > end_m1 ) goto c_17;
    goto l_104;
  c_18:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_105; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_18;
    return RET_CONT;
  e_18:
    if ( data > end_m1 ) goto c_18;
    goto l_105;
  c_19:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_106; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_19;
    return RET_CONT;
  e_19:
    if ( data > end_m1 ) goto c_19;
    goto l_106;
  c_20:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_107; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_20;
    return RET_CONT;
  e_20:
    if ( data > end_m1 ) goto c_20;
    goto l_107;
  c_21:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_87; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_21;
    return RET_CONT;
  e_21:
    if ( data > end_m1 ) goto c_21;
    goto l_87;
  c_22:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_108; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_22;
    return RET_CONT;
  e_22:
    if ( data > end_m1 ) goto c_22;
    goto l_108;
  c_23:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_109; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_23;
    return RET_CONT;
  e_23:
    if ( data > end_m1 ) goto c_23;
    goto l_109;
  c_24:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_110; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_24;
    return RET_CONT;
  e_24:
    if ( data > end_m1 ) goto c_24;
    goto l_110;
  c_26:
    return RET_ENDED_KO;
  c_27:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_112; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_27;
    return RET_CONT;
  e_27:
    if ( data > end_m1 ) goto c_27;
    goto l_112;
  c_28:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_113; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_28;
    return RET_CONT;
  e_28:
    if ( data > end_m1 ) goto c_28;
    goto l_113;
  c_29:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_114; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_29;
    return RET_CONT;
  e_29:
    if ( data > end_m1 ) goto c_29;
    goto l_114;
  c_30:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_115; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_30;
    return RET_CONT;
  e_30:
    if ( data > end_m1 ) goto c_30;
    goto l_115;
  c_31:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_116; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_31;
    return RET_CONT;
  e_31:
    if ( data > end_m1 ) goto c_31;
    goto l_116;
  c_32:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_117; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_32;
    return RET_CONT;
  e_32:
    if ( data > end_m1 ) goto c_32;
    goto l_117;
  c_33:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_118; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_33;
    return RET_CONT;
  e_33:
    if ( data > end_m1 ) goto c_33;
    goto l_118;
  c_34:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_119; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_34;
    return RET_CONT;
  e_34:
    if ( data > end_m1 ) goto c_34;
    goto l_119;
  c_35:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_120; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_35;
    return RET_CONT;
  e_35:
    if ( data > end_m1 ) goto c_35;
    goto l_120;
  c_36:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_121; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_36;
    return RET_CONT;
  e_36:
    if ( data > end_m1 ) goto c_36;
    goto l_121;
  c_37:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_122; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_37;
    return RET_CONT;
  e_37:
    if ( data > end_m1 ) goto c_37;
    goto l_122;
  c_38:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_123; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_38;
    return RET_CONT;
  e_38:
    if ( data > end_m1 ) goto c_38;
    goto l_123;
  c_39:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_124; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_39;
    return RET_CONT;
  e_39:
    if ( data > end_m1 ) goto c_39;
    goto l_124;
  c_40:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_125; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_40;
    return RET_CONT;
  e_40:
    if ( data > end_m1 ) goto c_40;
    goto l_125;
  c_42:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_126; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_42;
    return RET_CONT;
  e_42:
    if ( data > end_m1 ) goto c_42;
    goto l_126;
  c_43:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_127; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_43;
    return RET_CONT;
  e_43:
    if ( data > end_m1 ) goto c_43;
    goto l_127;
  c_44:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_128; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_44;
    return RET_CONT;
  e_44:
    if ( data > end_m1 ) goto c_44;
    goto l_128;
  c_45:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_129; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_45;
    return RET_CONT;
  e_45:
    if ( data > end_m1 ) goto c_45;
    goto l_129;
  c_46:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_130; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_46;
    return RET_CONT;
  e_46:
    if ( data > end_m1 ) goto c_46;
    goto l_130;
  c_47:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_131; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_47;
    return RET_CONT;
  e_47:
    if ( data > end_m1 ) goto c_47;
    goto l_131;
  c_48:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_132; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_48;
    return RET_CONT;
  e_48:
    if ( data > end_m1 ) goto c_48;
    goto l_132;
  c_49:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_133; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_49;
    return RET_CONT;
  e_49:
    if ( data > end_m1 ) goto c_49;
    goto l_133;
  c_50:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_134; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_50;
    return RET_CONT;
  e_50:
    if ( data > end_m1 ) goto c_50;
    goto l_134;
  c_51:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_135; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_51;
    return RET_CONT;
  e_51:
    if ( data > end_m1 ) goto c_51;
    goto l_135;
  c_52:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_86; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_52;
    return RET_CONT;
  e_52:
    if ( data > end_m1 ) goto c_52;
    goto l_86;
  c_53:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_136; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_53;
    return RET_CONT;
  e_53:
    if ( data > end_m1 ) goto c_53;
    goto l_136;
  c_54:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_137; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_54;
    return RET_CONT;
  e_54:
    if ( data > end_m1 ) goto c_54;
    goto l_137;
  c_55:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_138; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_55;
    return RET_CONT;
  e_55:
    if ( data > end_m1 ) goto c_55;
    goto l_138;
  c_56:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_139; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_56;
    return RET_CONT;
  e_56:
    if ( data > end_m1 ) goto c_56;
    goto l_139;
  c_57:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_140; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_57;
    return RET_CONT;
  e_57:
    if ( data > end_m1 ) goto c_57;
    goto l_140;
  c_58:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_141; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_58;
    return RET_CONT;
  e_58:
    if ( data > end_m1 ) goto c_58;
    goto l_141;
  c_59:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_142; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_59;
    return RET_CONT;
  e_59:
    if ( data > end_m1 ) goto c_59;
    goto l_142;
  c_60:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_143; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_60;
    return RET_CONT;
  e_60:
    if ( data > end_m1 ) goto c_60;
    goto l_143;
  c_61:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_144; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_61;
    return RET_CONT;
  e_61:
    if ( data > end_m1 ) goto c_61;
    goto l_144;
  c_62:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_145; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_62;
    return RET_CONT;
  e_62:
    if ( data > end_m1 ) goto c_62;
    goto l_145;
  c_63:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_146; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_63;
    return RET_CONT;
  e_63:
    if ( data > end_m1 ) goto c_63;
    goto l_146;
  c_64:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_147; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_64;
    return RET_CONT;
  e_64:
    if ( data > end_m1 ) goto c_64;
    goto l_147;
  c_65:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_148; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_65;
    return RET_CONT;
  e_65:
    if ( data > end_m1 ) goto c_65;
    goto l_148;
  c_66:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_149; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_66;
    return RET_CONT;
  e_66:
    if ( data > end_m1 ) goto c_66;
    goto l_149;
  c_67:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_150; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_67;
    return RET_CONT;
  e_67:
    if ( data > end_m1 ) goto c_67;
    goto l_150;
  c_68:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_151; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_68;
    return RET_CONT;
  e_68:
    if ( data > end_m1 ) goto c_68;
    goto l_151;
  c_69:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_152; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_69;
    return RET_CONT;
  e_69:
    if ( data > end_m1 ) goto c_69;
    goto l_152;
  c_70:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_153; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_70;
    return RET_CONT;
  e_70:
    if ( data > end_m1 ) goto c_70;
    goto l_153;
  c_71:
    while ( buf->next ) { HPIPE_BUFF_T__INC_REF_N( buf, 0 ); buf = buf->next; if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_154; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.pending_buf = buf;
    HPIPE_DATA.inp_cont = &&e_71;
    HPIPE_BUFF_T__INC_REF( buf );
    return RET_CONT;
  e_71:
    HPIPE_DATA.pending_buf->next = buf;
    HPIPE_DATA.pending_buf = buf;
    if ( data > end_m1 ) goto c_71;
    goto l_154;
  c_72:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_156; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_72;
    return RET_CONT;
  e_72:
    if ( data > end_m1 ) goto c_72;
    goto l_156;
  c_73:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_157; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_73;
    return RET_CONT;
  e_73:
    if ( data > end_m1 ) goto c_73;
    goto l_157;
  c_74:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_158; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_74;
    return RET_CONT;
  e_74:
    if ( data > end_m1 ) goto c_74;
    goto l_158;
  c_75:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_159; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_75;
    return RET_CONT;
  e_75:
    if ( data > end_m1 ) goto c_75;
    goto l_159;
  c_76:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_160; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_76;
    return RET_CONT;
  e_76:
    if ( data > end_m1 ) goto c_76;
    goto l_160;
  c_77:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_161; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_77;
    return RET_CONT;
  e_77:
    if ( data > end_m1 ) goto c_77;
    goto l_161;
  c_78:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_162; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_78;
    return RET_CONT;
  e_78:
    if ( data > end_m1 ) goto c_78;
    goto l_162;
  c_79:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_163; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_79;
    return RET_CONT;
  e_79:
    if ( data > end_m1 ) goto c_79;
    goto l_163;
  c_80:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_164; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_80;
    return RET_CONT;
  e_80:
    if ( data > end_m1 ) goto c_80;
    goto l_164;
  c_81:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_165; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_81;
    return RET_CONT;
  e_81:
    if ( data > end_m1 ) goto c_81;
    goto l_165;
  c_82:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_166; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_82;
    return RET_CONT;
  e_82:
    if ( data > end_m1 ) goto c_82;
    goto l_166;
  c_84:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_169; } }
    if ( last_buf ) goto l_82;
    HPIPE_DATA.inp_cont = &&e_84;
    return RET_CONT;
  e_84:
    if ( data > end_m1 ) goto c_84;
    goto l_169;
  c_86:
    while ( buf->next ) { HPIPE_BUFF_T *old = buf; buf = buf->next; HPIPE_BUFF_T__DEC_REF( old ); if ( buf->used ) { data = buf->data; end_m1 = buf->data + buf->used - 1; goto l_172; } }
    if ( last_buf ) goto l_81;
    HPIPE_DATA.inp_cont = &&e_86;
    return RET_CONT;
  e_86:
    if ( data > end_m1 ) goto c_86;
    goto l_172;
}
#endif // HPIPE_DEFINITIONS
