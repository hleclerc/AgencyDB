#pragma once

#include "../Core/PatchCreatorWithRightFlagsTempl.h"
#include "../../System/CbStringPtr.h"
#include "../Core/RpTemplProxy.h"
#include "../Core/Callback.h"
#include <map>

namespace RiotBase {

/// enable get_change for "atomic" object (i.e. we set the whole value for each change)
/// `get_raw_data` must be defined for `src`
class PatchCreatorAtomic : public PatchCreatorWithRightFlagsTempl<PatchCreatorAtomic> {
public:
    // patch types for transmission
    static constexpr PI8 PATCH_TYPE_SET_VALUE     = END_PATCH_TYPE_FOR_RIGHT + 0; // pos, string

    // extra patch types that can be used to store operations internally (enabling fast undo, ...)
    static constexpr PI8 PATCH_TYPE_SET_VALUE_UND = END_PATCH_TYPE_FOR_RIGHT + 1; // value (as a string), old value (as a string)

    // rights
    enum {
        #define DECL_RIGHT( R ) NUM_RIGHT_TO_##R,
        #include "rightsdecl.h"
        #undef DECL_RIGHT
        NB_RIGHT_TYPES
    };

    #define DECL_RIGHT( R ) static constexpr unsigned RIGHT_TO_##R = 1 << unsigned( NUM_RIGHT_TO_##R ); bool _right_to_##R( UsrId usr_id = {} ) const { return _usr_right( usr_id, RIGHT_TO_##R ); }
    #include "rightsdecl.h"
    #undef DECL_RIGHT

    static constexpr unsigned base_rights = ( 1u << unsigned( NB_RIGHT_TYPES ) ) - 1;

    //
    struct Visitor : PatchCreatorWithRightFlagsTempl::Visitor {
        void set_value    ( const CbString &d ) {}
        void set_value_und( const CbString &d, const CbString &o ) {}
    };

    // methods
    PatchCreatorAtomic( Rp *src, Inode crea_usr = {} );

    static std::string   type_name          ();
    void                 get_changes        ( CbQueue &cq, const FqId &id );
    Rp                 **prox_ptr           ();
    PatchMerger         *new_patch_merger   ();
    void                 self_on_change     ( Callback *c );
    void                 sig_change         ();
    Vec<std::string>     get_possible_rights() const;

    _TCBO static int     operations_visitor        ( const Cb &cx, Op &op, CbQueue *res = 0 ); ///< return 0 or an error code, or 0. Update cx if update != 0 (according to modification a inner values)
    _TCBO static int     operations_reverse_visitor( const Cb &cx, Op &op, CbQueue *res = 0 ); ///< return 0 or an error code, or 0.

    void                 _set_raw_data             ( const CbString &data, DevId cur_dev );
    void                 _set_raw_data_wo_cr       ( const CbString &data, DevId cur_dev );
    void                 _get_raw_data             ( BinStream<CbQueue> bq ) const;

    Vec<RcPtr<Callback>> on_change_list; ///< callbacks
    AutoRpDel            old;            ///< old value
    bool                 changed;        ///< to say that maybe there is a change
};

_TCBO int PatchCreatorAtomic::operations_visitor( const Cb &co, Op &op, CbQueue *res ) {
    CbStringPtr cx( co ); BinStream<CbStringPtr> data( &cx );
    BinStream<CbQueue> bu( res );

    while ( not data.empty() ) {
        PI8 code = data.read_byte();

        switch ( code ) {
        case PatchCreatorAtomic::PATCH_TYPE_SET_VALUE: {
            CbString d = data.read();
            if ( data.error() )
                return 1;
            op.set_value( d );
            if ( res and d.size() )
                bu << PatchCreatorAtomic::PATCH_TYPE_SET_VALUE << d;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_ADD_GRP_RIGHT: {
            Inode    grp   = data.read();
            unsigned flags = data.read_unsigned();
            if ( data.error() )
                return 1;
            op.add_grp_right( grp, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_ADD_GRP_RIGHT << grp << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_REM_GRP_RIGHT: {
            Inode    grp   = data.read();
            unsigned flags = data.read_unsigned();
            if ( data.error() )
                return 1;
            op.rem_grp_right( grp, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_REM_GRP_RIGHT << grp << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_ADD_USR_RIGHT: {
            Inode    usr   = data.read();
            unsigned flags = data.read_unsigned();
            if ( data.error() )
                return 1;
            op.add_usr_right( usr, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_ADD_USR_RIGHT << usr << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_REM_USR_RIGHT: {
            Inode    usr   = data.read();
            unsigned flags = data.read_unsigned();
            if ( data.error() )
                return 1;
            op.rem_usr_right( usr, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_REM_USR_RIGHT << usr << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_SET_VALUE_UND: {
            CbString d = data.read();
            CbString o = data.read();
            if ( data.error() )
                return 1;
            op.set_value_und( d, o );
            if ( res and d.size() )
                bu << PatchCreatorAtomic::PATCH_TYPE_SET_VALUE_UND << d << o;
            break;
        }
        default:
            return 2;
        }
    }
    return 0;
}

_TCBO int PatchCreatorAtomic::operations_reverse_visitor( const Cb &cx, Op &op, CbQueue *res ) {
    Vec<CbStringPtr> positions; positions.reserve( cx.size() / 10 );
    CbStringPtr cp = cx; BinStream<CbStringPtr> data( &cp );
    BinStream<CbQueue> bu( res );

    // find positions in the binary
    while ( not data.empty() ) {
        positions << cp;

        PI8 code = data.read_byte();
        switch ( code ) {
        case PatchCreatorAtomic::PATCH_TYPE_SET_VALUE:
            data.skip_string();
            if ( data.error() )
                return 1;
            break;
        case PatchCreatorAtomic::PATCH_TYPE_ADD_GRP_RIGHT:
            data.read().operator Inode();
            data.skip_unsigned();
            if ( data.error() )
                return 1;
            break;
        case PatchCreatorAtomic::PATCH_TYPE_ADD_USR_RIGHT:
            data.read().operator Inode();
            data.skip_unsigned();
            if ( data.error() )
                return 1;
            break;
        case PatchCreatorAtomic::PATCH_TYPE_REM_GRP_RIGHT:
            data.read().operator Inode();
            data.skip_unsigned();
            if ( data.error() )
                return 1;
            break;
        case PatchCreatorAtomic::PATCH_TYPE_REM_USR_RIGHT:
            data.read().operator Inode();
            data.skip_unsigned();
            if ( data.error() )
                return 1;
            break;
        case PatchCreatorAtomic::PATCH_TYPE_SET_VALUE_UND:
            data.skip_string();
            data.skip_string();
            if ( data.error() )
                return 1;
            break;
        default:
            return 2;
        }
    }

    //
    for( PT np = positions.size(); np--; ) {
        CbStringPtr cb = positions[ np ];
        BinStream<CbStringPtr> data( &cb );

        PI8 code = data.read_byte();
        switch ( code ) {
        case PatchCreatorAtomic::PATCH_TYPE_SET_VALUE: {
            CbString d = data.read();
            op.set_value( d );
            if ( res and d.size() )
                bu << PatchCreatorAtomic::PATCH_TYPE_SET_VALUE << d;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_ADD_GRP_RIGHT: {
            Inode    grp   = data.read();
            unsigned flags = data.read_unsigned();
            op.add_grp_right( grp, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_ADD_GRP_RIGHT << grp << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_REM_GRP_RIGHT: {
            Inode    grp   = data.read();
            unsigned flags = data.read_unsigned();
            op.rem_grp_right( grp, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_REM_GRP_RIGHT << grp << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_ADD_USR_RIGHT: {
            Inode    usr   = data.read();
            unsigned flags = data.read_unsigned();
            op.add_usr_right( usr, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_ADD_USR_RIGHT << usr << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_REM_USR_RIGHT: {
            Inode    usr   = data.read();
            unsigned flags = data.read_unsigned();
            op.rem_usr_right( usr, flags );
            if ( res and flags )
                bu << PatchCreatorAtomic::PATCH_TYPE_REM_USR_RIGHT << usr << flags;
            break;
        }
        case PatchCreatorAtomic::PATCH_TYPE_SET_VALUE_UND: {
            CbString d = data.read();
            CbString o = data.read();
            op.set_value_und( d, o );
            if ( res and d.size() )
                bu << PatchCreatorAtomic::PATCH_TYPE_SET_VALUE_UND << d << o;
            break;
        }
        default:
            return 2;
        }
    }

    return 0;
}

} // namespace RiotBase
