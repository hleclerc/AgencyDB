#pragma once

#include "../../System/KsAllocator.h"
#include "TupleTypes.h"
#include "TypeTempl.h"
#include "Surdef.h"
#include "Rp.h"

namespace RiotBase {
class Category;

///
template<class T>
class RpTempl : public Rp {
public:
    enum {
        unknown_value = false,
        error_value   = false,
        term_value    = false,
        atomic_value  = false,
    };
    using BaseRpTempl = RpTempl<T>; // used for surdefs (to see of there is a more specialized def than the one in this)
    using ParentClass = void;       // for inheritance

    //
    RpTempl() {
        assign_type_to( this );
    }

    // creation helpers
    template<class... Args>
    static T *New( Args&& ...args ) {
        T *res = _NewPtr( N<sizeof(T)>() + T::additionnal_size_for( args... ), std::forward<Args>( args )... );
        return res;
    }

    static T *assign_type_to( Rp *res ) {
        res->type_off = StaticStore<TypeTempl<T>>::data.type_off;
        res->type_num = StaticStore<TypeTempl<T>>::data.type_num;
        return reinterpret_cast<T *>( res );
    }

    template<class... Args>
    static N<0>            additionnal_size_for  ( const Args &... ) { return {}; }
    N<0>                   additionnal_size      () { return {}; }
    void                   take_rese             ( PT ) {}

    //
    static const Category *static_category       () { return 0; }

    static PT              num_in_tuple_types    () { return RiotBase::num_in_tuple_types<T>(); }

    static void            init_static_attributes() {
        RiotBase_REG_FUNC_1( del                 , 0, tv0->isa<T>(), T::_del             );
        RiotBase_REG_FUNC_1( write_to_stream     , 0, tv0->isa<T>(), T::_write_to_stream );
        RiotBase_REG_FUNC_1( write_structure     , 1, tv0->isa<T>(), T::_write_structure );
        RiotBase_REG_FUNC_1( write_graphviz      , 0, tv0->isa<T>(), T::_write_graphviz  );
        RiotBase_REG_FUNC_1( is_symbolic         , 0, tv0->isa<T>(), T::_is_symbolic     );
        RiotBase_REG_FUNC_1( sig_change          , 0, tv0->isa<T>(), T::_sig_change      );
        RiotBase_REG_FUNC_1( get_cursors         , 0, tv0->isa<T>(), T::_get_cursors     );
    }

    static void            _del                  ( Rp *rp ) { T::_destroy( rp ); KsAllocator::free( rp, N<sizeof(T)>() + static_cast<T *>( rp )->additionnal_size() ); }
    static void            _destroy              ( Rp *rp ) { static_cast<T *>( rp )->~T(); }
    static void            _write_to_stream      ( const Rp *rp, std::ostream &os ) { os << T::type_name(); }
    static void            _write_structure      ( const Rp *rp, std::ostream &os ) { os << T::type_name(); }
    static void            _write_graphviz       ( const Rp *rp, std::ostream &os ) {
        os << "node_" << rp << " [label=\"";
        if ( RO_CALL( nb_symbolic_children, rp ) )
            os << T::type_name();
        else
            RO_CALL( write_to_stream, rp, os );
        os << "\"];";
    }
    static bool            _is_symbolic          ( const Rp *rp ) { return T::unknown_value; }
    static void            _sig_change           ( Rp *rp ) {}
    static void            _get_cursors          ( const Rp *, Vec<CursorData> & ) {}

protected:
    template<class... Args>
    static auto _NewPtr( PT size, Args&& ...args ) {
        T *res = new ( KsAllocator::malloc( size ) ) T( std::forward<Args>( args )... );
        res->take_rese( size - sizeof( T ) );
        return res;
    }
    template<PT size,class... Args>
    static auto _NewPtr( N<size> n, Args&& ...args ) {
        T *res = new ( KsAllocator::malloc( n ) ) T( std::forward<Args>( args )... );
        res->take_rese( N<KsAllocator::GetN<size>::v-sizeof(T)>() );
        return res;
    }
};

} // namespace RiotBase
