#pragma once

#include "../../System/TypeListAppender.h"
#include "../Ot/OtWrapper.h"

namespace RiotBase {

///
template<class A,class T>
class OtWrapperAtomic : public OtWrapper<A> {
public:
    using Aod = typename OtWrapper<A>::Aod;

    // operation types
    struct op_set     { static const char *name() { return "set";     } static bool valid( const T &val ) { return true; } };
    struct op_set_und { static const char *name() { return "set_und"; } static bool valid( const T &val, const T &old ) { return true; } };
    using OperationTypes = typename TypeListAppender<typename OtWrapper<A>::OtWrapperOperationTypes,op_set,op_set_und>::T;

    // rights types
    struct can_read  { static const char *name() { return "read" ; } };
    struct can_write { static const char *name() { return "write"; } };
    using RightTypes = typename TypeListAppender<typename OtWrapper<A>::OtWrapperRightTypes,can_read,can_write>::T;

    // methods
    OtWrapperAtomic( const T &val, UsrId usr_id = {} ) : OtWrapper<A>( usr_id ), val( val ) {}

    void                write_to_stream         ( std::ostream &os ) const { os << val; }
                                                
    void                set_val                 ( const T &n ) { val = n; if ( old != n ) this->sig_change(); }
                                                
    // OT surdefs                               
    void                freeze_last_changes     ();
                                                
    // operations                               
    using OtWrapper<A>::apply_op;
    void                apply_op                ( op_set    , Aod &aod, T &str         ) { old = val = str; this->sig_change(); }
    void                apply_op                ( op_set_und, Aod &aod, T &str, T &old ) { apply_op( op_set{}, aod, str ); }
                                                
    // rights                                   
    using OtWrapper<A>::right_to;
    bool                right_to                ( op_set    , UsrId as_usr, T &str         ) { return this->has_right( can_write{}, as_usr ); }
    bool                right_to                ( op_set_und, UsrId as_usr, T &str, T &old ) { return right_to( op_set{}, as_usr, str ); }

    // transmission
    using OtWrapper<A>::transmission;
    void                transmission            ( op_set_und, BBQ br, T &str, T &old ) { transmission( op_set{}, br, str ); }

    // to create _und version when there is a non und one
    using OtWrapper<A>::register_patch_op;
    void                register_patch_op       ( op_set, BBQ bq, T &str ) { this->reg_op_in( bq, op_set_und{}, str, val ); }
                                                
    // undo                                     
    using OtWrapper<A>::undo_patch;
    void                undo_patch              ( op_set    , Aod &aod, T &str ) { WARNING( "Undo on a non und version should not happen" ); }
    void                undo_patch              ( op_set_und, Aod &aod, T &str, T &old ) { apply_op( op_set{}, aod, old ); }

    T                   val;
    T                   old;
};


template<class A,class T>
void OtWrapperAtomic<A,T>::freeze_last_changes() {
    if ( old != val ) {
        this->reg_op( op_set_und{}, val, old );
        old = val;
    }
    OtWrapper<A>::freeze_last_changes();
}


} // namespace RiotBase
