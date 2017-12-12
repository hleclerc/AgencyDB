#pragma once

#include "../../System/IsSame.h"
#include "../../System/Print.h"
#include "../../System/Ccast.h"

#include "FunctionTable_Ancestor.h"

#include "AutoRpDel.h"
#include "Surdef.h"

#include <limits>

namespace RiotBase {

template<class FuncName>
struct FunctionTable_1 : FunctionTable_Ancestor {
    using TF = typename FuncName::TF;
    using TR = typename ArgType<TF,-1>::T;
    using T0 = typename ArgType<TF, 0>::T;
    enum{ c0 = IsSame<T0,const Rp *>::v };

    FunctionTable_1() : size( 0 ) {
        stat_resize_table();
    }

    ~FunctionTable_1() {
        delete [] FuncName::table;
    }

    virtual void resize_table() {
        stat_resize_table();
    }

    void stat_resize_table() { // we need a non virtual version because initialization may come before vtables ready
        int new_size = Type::rs_instanciated_types();
        if ( size == new_size )
            return;
        TF **old_table = FuncName::table;

        FuncName::table = new TF*[ new_size ];
        for( int i = 0; i < size; ++i )
            FuncName::table[ i ] = old_table[ i ];
        for( int i = size; i < new_size; ++i )
            FuncName::table[ i ] = IncompleteFunc<TF>::f;

        if ( size )
            delete [] old_table;
        size = new_size;
    }

    static std::pair<TF *,double> find_best_func( Type *type_0, bool warn = false ) {
        // registered surdefs (via RiotBase_DEF...)
        int    n_of_best = 0;
        TF    *best_func = 0;
        double best_pert = -std::numeric_limits<double>::max();
        for( Surdef_1<FuncName> *rf = FuncName::last_surdef; rf; rf = rf->prev_surdef ) {
            if ( rf->cond( type_0 ) and best_pert <= rf->pertinence ) {
                n_of_best = best_pert == rf->pertinence ? n_of_best + 1 : 1;
                best_pert = rf->pertinence;
                best_func = rf->func;
            }
        }

        double pert_trial;
        if ( TF *res = type_0->surdef_for( FuncName(), pert_trial ) ) {
            if ( best_pert <= pert_trial ) {
                n_of_best = best_pert == pert_trial ? n_of_best + 1 : 1;
                best_pert = pert_trial;
                best_func = res;
            }
        }

        // proxy ?
        constexpr bool prox_valid = not (
            IsSame<FuncName,RF_write_structure>::v or
            IsSame<FuncName,RF_prox_ptr       >::v or
            IsSame<FuncName,RF_prox_ref       >::v or
            IsSame<FuncName,RF_prox_val       >::v or
            IsSame<FuncName,RF_del            >::v
        );
        if ( best_pert < 1 and prox_valid ) {
            if ( FunctionTable_1<RF_prox_ptr>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,0,c0>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ref>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,1,c0>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_val>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,2,c0>::get_f() ) return std::pair<TF *,double>( f, 1 );
        }

        // errors
        if ( warn ) {
            if ( not best_func )
                ERROR( "Impossible to find valid surdefinition for function '%s( %s, ... )'", FuncName::name(), type_0->name().c_str() );
            if ( n_of_best >= 2 )
                WARNING( "Ambiguous overload for function '%s( %s, ... )'", FuncName::name(), type_0->name().c_str() );
        }
        return std::pair<TF *,double>( best_func, best_pert );
    }

    static void update_table_for( const Rp *obj ) {
        // type info
        Type *type = Type::get_type_of( obj );
        type->init_static_attributes();

        // type_off (if where in update_table_for, type_off may be equal to 0 -- i.e. undefined)
        ccast( obj )->type_off = type->potentially_updated_type_off();

        // store the best function
        FuncName::get_f( obj ) = find_best_func( type, true ).first;
    }

    ///
    template<class TF>
    struct IncompleteFunc;

    template<typename... Args>
    struct IncompleteFunc<TR(T0,Args...)> {
        static TR f( T0 v0, Args... args ) {
            update_table_for( v0 );
            return FuncName::get_f( v0 )( v0, args... );
        }
    };

    //
    template<class TF,bool valid,int type,bool c>
    struct ProxFuncGetter {
        static TF *get_f() { return 0; }
    };

    template<typename... Args>
    struct ProxFuncGetter<TR(T0,Args...),true,0,false> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, Args... args ) {
            Rp **ptr = RO_CALL( prox_ptr, v0 );
            return FuncName::get_f( *ptr )( *ptr, args... );
        }
    };

    template<typename... Args>
    struct ProxFuncGetter<TR(T0,Args...),true,1,true> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, Args... args ) {
            const Rp *ptr = RO_CALL( prox_ref, v0 );
            return FuncName::get_f( ptr )( ptr, args... );
        }
    };

    template<typename... Args,bool c>
    struct ProxFuncGetter<TR(T0,Args...),true,2,c> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, Args... args ) {
            if ( not c ) {
                PRINTE( Type::get_type_of( v0 )->name() );
                TODO; // use reassign
            }
            AutoRpDel ptr = RO_CALL( prox_val, v0 );
            return FuncName::get_f( ptr.rp )( ptr.rp, args... );
        }
    };

    //
    int size;
};

template<class FuncName>
struct FunctionTable_2 : FunctionTable_Ancestor {
    using TF = typename FuncName::TF;
    using TR = typename ArgType<TF,-1>::T;
    using T0 = typename ArgType<TF, 0>::T;
    using T1 = typename ArgType<TF, 1>::T;
    enum{ c0 = IsSame<T0,const Rp *>::v };
    enum{ c1 = IsSame<T1,const Rp *>::v };

    FunctionTable_2() : size( 0 ) {
        stat_resize_table();
    }

    ~FunctionTable_2() {
        delete [] FuncName::table;
    }

    virtual void resize_table() {
        stat_resize_table();
    }

    void stat_resize_table() { // we need a non virtual version because initialization may come before vtables ready
        ST new_size = Type::rs_instanciated_types();
        if ( size == new_size )
            return;
        TF **old_data = FuncName::table;

        FuncName::table = new TF*[ new_size * new_size ];
        // default value
        for( ST y = 0; y < new_size; ++y )
            for( ST x = 0; x < new_size; ++x )
                FuncName::table[ new_size * y + x ] = IncompleteFunc<TF>::f;
        // old ones
        for( ST y = 0; y < size; ++y )
            for( ST x = 0; x < size; ++x )
                FuncName::table[ new_size * y + x ] = old_data[ size * y + x ];

        if ( size )
            delete [] old_data;
        size = new_size;
    }

    static std::pair<TF *,double> find_best_func( Type *type_0, Type *type_1, bool warn = false ) {
        // registered functions
        int    n_of_best = 0;
        TF    *best_func = 0;
        double best_pert = -std::numeric_limits<double>::max();
        for( Surdef_2<FuncName> *rf = FuncName::last_surdef; rf; rf = rf->prev_surdef ) {
            if ( rf->cond( type_0, type_1 ) and best_pert <= rf->pertinence ) {
                n_of_best = best_pert == rf->pertinence ? n_of_best + 1 : 1;
                best_pert = rf->pertinence;
                best_func = rf->func;
            }
        }

        // proxy ?
        constexpr bool prox_valid = true;
        if ( prox_valid and best_pert < 1 ) {
            if ( FunctionTable_1<RF_prox_ptr>::find_best_func( type_1 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,0,c1,1>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ref>::find_best_func( type_1 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,1,c1,1>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_val>::find_best_func( type_1 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,2,c1,1>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ptr>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,0,c0,0>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ref>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,1,c0,0>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_val>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,2,c0,0>::get_f() ) return std::pair<TF *,double>( f, 1 );
        }

        // errors
        if ( warn ) {
            if ( not best_func )
                ERROR( "Impossible to find valid surdefinition for function '%s( %s, %s, ... )'", FuncName::name(), type_0->name().c_str(), type_1->name().c_str() );
            if ( n_of_best >= 2 )
                WARNING( "Ambiguous overload for function '%s( %s, %s, ... )'", FuncName::name(), type_0->name().c_str(), type_1->name().c_str() );
        }
        return std::pair<TF *,double>( best_func, best_pert );
    }

    static void update_table_for( const Rp *obj_0, const Rp *obj_1 ) {
        // type info
        Type *type_0 = Type::get_type_of( obj_0 );
        Type *type_1 = Type::get_type_of( obj_1 );
        type_0->init_static_attributes();
        type_1->init_static_attributes();

        // type_off (if where in update_table_for, type_off may be equal to 0 -- i.e. undefined)
        ccast( obj_0 )->type_off = type_0->potentially_updated_type_off();
        ccast( obj_1 )->type_off = type_1->potentially_updated_type_off();

        // store the best function
        FuncName::get_f( obj_0, obj_1 ) = find_best_func( type_0, type_1, true ).first;
    }

    ///
    template<class TF>
    struct IncompleteFunc;

    template<typename... Args>
    struct IncompleteFunc<TR(T0,T1,Args...)> {
        static TR f( T0 obj_0, T1 obj_1, Args... args ) {
            update_table_for( obj_0, obj_1 );
            return FuncName::get_f( obj_0, obj_1 )( obj_0, obj_1, args... );
        }
    };

    //
    template<class TF,bool valid,int type,bool c,int nc>
    struct ProxFuncGetter {
        static TF *get_f() { return 0; }
    };

    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,Args...),true,0,false,0> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, Args... args ) {
            Rp **ptr = RO_CALL( prox_ptr, v0 );
            return FuncName::get_f( *ptr, v1 )( *ptr, v1, args... );
        }
    };
    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,Args...),true,0,false,1> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, Args... args ) {
            Rp **ptr = RO_CALL( prox_ptr, v1 );
            return FuncName::get_f( v0, *ptr )( v0, *ptr, args... );
        }
    };

    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,Args...),true,1,true,0> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, Args... args ) {
            const Rp *ptr = RO_CALL( prox_ref, v0 );
            return FuncName::get_f( ptr, v1 )( ptr, v1, args... );
        }
    };
    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,Args...),true,1,true,1> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, Args... args ) {
            const Rp *ptr = RO_CALL( prox_ref, v1 );
            return FuncName::get_f( v0, ptr )( v0, ptr, args... );
        }
    };

    template<typename... Args,bool c>
    struct ProxFuncGetter<TR(T0,T1,Args...),true,2,c,0> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, Args... args ) {
            if ( not c ) {
                PRINTE( Type::get_type_of( v0 )->name() );
                PRINTE( Type::get_type_of( v1 )->name() );
                TODO; // use reassign
            }
            AutoRpDel ptr = RO_CALL( prox_val, v0 );
            return FuncName::get_f( ptr.rp, v1 )( ptr.rp, v1, args... );
        }
    };
    template<typename... Args,bool c>
    struct ProxFuncGetter<TR(T0,T1,Args...),true,2,c,1> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, Args... args ) {
            if ( not c )
                TODO; // use reassign
            AutoRpDel ptr = RO_CALL( prox_val, v1 );
            return FuncName::get_f( v0, ptr.rp )( v0, ptr.rp, args... );
        }
    };

    int size;
};

template<class FuncName>
struct FunctionTable_3 : FunctionTable_Ancestor {
    using TF = typename FuncName::TF;
    using TR = typename ArgType<TF,-1>::T;
    using T0 = typename ArgType<TF, 0>::T;
    using T1 = typename ArgType<TF, 1>::T;
    using T2 = typename ArgType<TF, 2>::T;
    enum{ c0 = IsSame<T0,const Rp *>::v };
    enum{ c1 = IsSame<T1,const Rp *>::v };
    enum{ c2 = IsSame<T2,const Rp *>::v };

    FunctionTable_3() {
        FuncName::table = new Vec<typename FuncName::Item>[ 128 ];
    }

    ~FunctionTable_3() {
        delete [] FuncName::table;
    }

    virtual void resize_table() {
    }

    static std::pair<TF *,double> find_best_func( Type *type_0, Type *type_1, Type *type_2, bool warn = false ) {
        // registered functions
        int    n_of_best = 0;
        TF    *best_func = 0;
        double best_pert = -std::numeric_limits<double>::max();
        for( Surdef_3<FuncName> *rf = FuncName::last_surdef; rf; rf = rf->prev_surdef ) {
            if ( rf->cond( type_0, type_1, type_2 ) and best_pert <= rf->pertinence ) {
                n_of_best = best_pert == rf->pertinence ? n_of_best + 1 : 1;
                best_pert = rf->pertinence;
                best_func = rf->func;
            }
        }

        // proxy ?
        constexpr bool prox_valid = true;
        if ( prox_valid and best_pert < 1 ) {
            if ( FunctionTable_1<RF_prox_ptr>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,0,c0,0>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ref>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,1,c0,0>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_val>::find_best_func( type_0 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,2,c0,0>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ptr>::find_best_func( type_1 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,0,c1,1>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ref>::find_best_func( type_1 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,1,c1,1>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_val>::find_best_func( type_1 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,2,c1,1>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ptr>::find_best_func( type_2 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,0,c2,2>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_ref>::find_best_func( type_2 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,1,c2,2>::get_f() ) return std::pair<TF *,double>( f, 1 );
            if ( FunctionTable_1<RF_prox_val>::find_best_func( type_2 ).first ) if ( TF *f = ProxFuncGetter<TF,prox_valid,2,c2,2>::get_f() ) return std::pair<TF *,double>( f, 1 );
        }

        // errors
        if ( warn ) {
            if ( not best_func )
                ERROR( "Impossible to find valid surdefinition for function '%s( %s, %s, %s, ... )'", FuncName::name(), type_0->name().c_str(), type_1->name().c_str(), type_2->name().c_str() );
            if ( n_of_best >= 2 )
                WARNING( "Ambiguous overload for function '%s( %s, %s, %s, ... )'", FuncName::name(), type_0->name().c_str(), type_1->name().c_str(), type_2->name().c_str() );
        }
        return std::pair<TF *,double>( best_func, best_pert );
    }

    static void update_table_for( const Rp *obj_0, const Rp *obj_1, const Rp *obj_2 ) {
        // type info
        Type *type_0 = Type::get_type_of( obj_0 );
        Type *type_1 = Type::get_type_of( obj_1 );
        Type *type_2 = Type::get_type_of( obj_2 );
        type_0->init_static_attributes();
        type_1->init_static_attributes();
        type_2->init_static_attributes();

        // type_off (if where in update_table_for, type_off may be equal to 0 -- i.e. undefined)
        ccast( obj_0 )->type_off = type_0->potentially_updated_type_off();
        ccast( obj_1 )->type_off = type_1->potentially_updated_type_off();
        ccast( obj_2 )->type_off = type_2->potentially_updated_type_off();

        // store the best function
        FuncName::get_f( obj_0, obj_1, obj_2 ) = find_best_func( type_0, type_1, type_2, true ).first;
    }

    ///
    template<class TF>
    struct IncompleteFunc;

    template<typename... Args>
    struct IncompleteFunc<TR(T0,T1,T2,Args...)> {
        static TR f( T0 obj_0, T1 obj_1, T2 obj_2, Args... args ) {
            update_table_for( obj_0, obj_1, obj_2 );
            return FuncName::get_f( obj_0, obj_1, obj_2 )( obj_0, obj_1, obj_2, args... );
        }
    };

    //
    template<class TF,bool valid,int type,bool c,int nc>
    struct ProxFuncGetter {
        static TF *get_f() { return 0; }
    };

    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,0,false,0> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            Rp **ptr = RO_CALL( prox_ptr, v0 );
            return FuncName::get_f( *ptr, v1, v2 )( *ptr, v1, v2, args... );
        }
    };
    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,0,false,1> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            Rp **ptr = RO_CALL( prox_ptr, v1 );
            return FuncName::get_f( v0, *ptr, v2 )( v0, *ptr, v2, args... );
        }
    };
    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,0,false,2> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            Rp **ptr = RO_CALL( prox_ptr, v2 );
            return FuncName::get_f( v0, *ptr, v2 )( v0, v1, *ptr, args... );
        }
    };

    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,1,true,0> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            const Rp *ptr = RO_CALL( prox_ref, v0 );
            return FuncName::get_f( ptr, v1, v2 )( ptr, v1, v2, args... );
        }
    };
    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,1,true,1> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            const Rp *ptr = RO_CALL( prox_ref, v1 );
            return FuncName::get_f( v0, ptr, v2 )( v0, ptr, v2, args... );
        }
    };
    template<typename... Args>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,1,true,2> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            const Rp *ptr = RO_CALL( prox_ref, v2 );
            return FuncName::get_f( v0, v1, ptr )( v0, v1, ptr, args... );
        }
    };

    template<typename... Args,bool c>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,2,c,0> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            if ( not c )
                TODO; // use reassign
            AutoRpDel ptr = RO_CALL( prox_val, v0 );
            return FuncName::get_f( ptr.rp, v1, v2 )( ptr.rp, v1, v2, args... );
        }
    };
    template<typename... Args,bool c>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,2,c,1> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            if ( not c )
                TODO; // use reassign
            AutoRpDel ptr = RO_CALL( prox_val, v1 );
            return FuncName::get_f( v0, ptr.rp, v2 )( v0, ptr.rp, v2, args... );
        }
    };
    template<typename... Args,bool c>
    struct ProxFuncGetter<TR(T0,T1,T2,Args...),true,2,c,2> {
        static TF *get_f() { return f; }
        static TR f( T0 v0, T1 v1, T2 v2, Args... args ) {
            if ( not c )
                TODO; // use reassign
            AutoRpDel ptr = RO_CALL( prox_val, v2 );
            return FuncName::get_f( v0, v1, ptr.rp )( v0, v1, ptr.rp, args... );
        }
    };

    int size;
};

} // namespace RiotBase
