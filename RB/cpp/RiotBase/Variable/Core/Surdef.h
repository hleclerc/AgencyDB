#pragma once

#include "../../System/ArgType.h"
#include "Type.h"

#include <functional>

namespace RiotBase {

// DEF and REG funcs
#define _RiotBase_CONCAT_IMPL( x, y ) x##y
#define _RiotBase_MACRO_CONCAT( x, y ) _RiotBase_CONCAT_IMPL( x, y )

#define _RiotBase_DEF_FUNC_1( FUNC_NAME, PERTINENCE, COND, PROP_NAME, ... ) \
    typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF>::T PROP_NAME( typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,0>::T v0, ##__VA_ARGS__ ); \
    RiotBase_REG_FUNC_1( FUNC_NAME, PERTINENCE, COND, PROP_NAME ); \
    typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF>::T PROP_NAME( typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,0>::T v0, ##__VA_ARGS__ )
#define _RiotBase_DEF_FUNC_2( FUNC_NAME, PERTINENCE, COND, PROP_NAME, ... ) \
    typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF>::T PROP_NAME( typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,0>::T v0, typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,1>::T v1, ##__VA_ARGS__ ); \
    RiotBase_REG_FUNC_2( FUNC_NAME, PERTINENCE, COND, PROP_NAME ); \
    typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF>::T PROP_NAME( typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,0>::T v0, typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,1>::T v1, ##__VA_ARGS__ )
#define _RiotBase_DEF_FUNC_3( FUNC_NAME, PERTINENCE, COND, PROP_NAME, ... ) \
    typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF>::T PROP_NAME( typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,0>::T v0, typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,1>::T v1, typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,2>::T v2, ##__VA_ARGS__ ); \
    RiotBase_REG_FUNC_3( FUNC_NAME, PERTINENCE, COND, PROP_NAME ); \
    typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF>::T PROP_NAME( typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,0>::T v0, typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,1>::T v1, typename RiotBase::ArgType<typename RF_##FUNC_NAME::TF,2>::T v2, ##__VA_ARGS__ )


#define RiotBase_REG_FUNC_1( FUNC_NAME, PERTINENCE, COND, PROP ) static RiotBase::Surdef_1<RF_##FUNC_NAME> _RiotBase_MACRO_CONCAT( rf_, __COUNTER__ )( []( Type *tv0 ) { return COND; }, PROP, PERTINENCE, __FILE__, __LINE__ );
#define RiotBase_REG_FUNC_2( FUNC_NAME, PERTINENCE, COND, PROP ) static RiotBase::Surdef_2<RF_##FUNC_NAME> _RiotBase_MACRO_CONCAT( rf_, __COUNTER__ )( []( Type *tv0, Type *tv1 ) { return COND; }, PROP, PERTINENCE, __FILE__, __LINE__ );
#define RiotBase_REG_FUNC_3( FUNC_NAME, PERTINENCE, COND, PROP ) static RiotBase::Surdef_3<RF_##FUNC_NAME> _RiotBase_MACRO_CONCAT( rf_, __COUNTER__ )( []( Type *tv0, Type *tv1, Type *tv2 ) { return COND; }, PROP, PERTINENCE, __FILE__, __LINE__ );

#define RiotBase_DEF_FUNC_1( FUNC_NAME, PERTINENCE, COND, ... ) _RiotBase_DEF_FUNC_1( FUNC_NAME, PERTINENCE, COND, _RiotBase_MACRO_CONCAT( f_, __COUNTER__ ), ##__VA_ARGS__ )
#define RiotBase_DEF_FUNC_2( FUNC_NAME, PERTINENCE, COND, ... ) _RiotBase_DEF_FUNC_2( FUNC_NAME, PERTINENCE, COND, _RiotBase_MACRO_CONCAT( f_, __COUNTER__ ), ##__VA_ARGS__ )
#define RiotBase_DEF_FUNC_3( FUNC_NAME, PERTINENCE, COND, ... ) _RiotBase_DEF_FUNC_3( FUNC_NAME, PERTINENCE, COND, _RiotBase_MACRO_CONCAT( f_, __COUNTER__ ), ##__VA_ARGS__ )



///
struct Surdef {
    Surdef( const char *file, int line ) : file( file ), line( line ) { }
    void        write_to_stream( std::ostream &os ) const { if ( file ) os << file << ":" << line; else os << "Surdef"; }
    const char *file;
    int         line;
};

///
template<class FuncName>
struct Surdef_1 : Surdef {
    using TF = typename FuncName::TF;

    Surdef_1( std::function<bool(Type *)> cond, TF *func, double pertinence = 1, const char *file = 0, int line = 0 ) : Surdef( file, line ), cond( cond ), func( func ), pertinence( pertinence ) {
        prev_surdef           = FuncName::last_surdef;
        FuncName::last_surdef = this;
    }

    std::function<bool(Type *)> cond;
    TF                         *func;
    double                      pertinence;
    Surdef_1                   *prev_surdef;
};


///
template<class FuncName>
struct Surdef_2 : Surdef {
    using TF = typename FuncName::TF;

    Surdef_2( std::function<bool(Type *,Type *)> cond, TF *func, double pertinence = 1, const char *file = 0, int line = 0 ) : Surdef( file, line ), cond( cond ), func( func ), pertinence( pertinence ) {
        prev_surdef           = FuncName::last_surdef;
        FuncName::last_surdef = this;
    }

    std::function<bool(Type *,Type *)> cond;
    TF                                *func;
    double                             pertinence;
    Surdef_2                          *prev_surdef;
};

///
template<class FuncName>
struct Surdef_3 : Surdef {
    using TF = typename FuncName::TF;

    Surdef_3( std::function<bool(Type *,Type *,Type *)> cond, TF *func, double pertinence = 1, const char *file = 0, int line = 0 ) : Surdef( file, line ), cond( cond ), func( func ), pertinence( pertinence ) {
        prev_surdef           = FuncName::last_surdef;
        FuncName::last_surdef = this;
    }

    std::function<bool(Type *,Type *,Type *)> cond;
    TF                                       *func;
    double                                    pertinence;
    Surdef_3                                 *prev_surdef;
};

} // namespace RiotBase
