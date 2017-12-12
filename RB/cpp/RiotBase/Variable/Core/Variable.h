#pragma once

#include "../../System/EnableIf.h"
#include "CallbackFunctor.h"
#include "Methods.h"

namespace RiotBase {
class CbQueue;

///
class Variable {
public:
    struct CreatorId {}; ///< for constructor specifying who is the creator (elswhere, we assume that {} is the creator)

    Variable( Rp *rp ) : rp( rp ) {}
    ~Variable() { if ( rp ) RO_CALL( del, rp ); }

    // display
    void             write_to_stream    ( std::ostream &os ) const;
    void             write_structure    ( std::ostream &os ) const;

    void             write_to           ( BBQ bq ) const;
    static Rp       *read_from          ( BinStream<CbStringPtr> bq );

    template<class Cq>
    static Rp       *read_from          ( BinStream<Cq> bq );

    // callback              
    void             on_change          ( Callback *c );
    void             then               ( Callback *c );
    void             wait               ();
                     
    template<class Functor>
    void             on_change          ( const Functor &c ) { on_change( (Callback *)new CallbackFunctor<Functor>( c ) ); }
                     
    // generic information
    bool             is_symbolic        ();

    // rights
    Vec<std::string> get_possible_rights() const;
    Vec<GrpId>       get_groups_in_acl  () const;
    Vec<UsrId>       get_users_in_acl   () const;
    Vec<std::string> get_grp_rights     ( GrpId grp = {} ) const;
    Vec<std::string> get_usr_rights     ( UsrId usr = {} ) const;
    bool             add_grp_right      ( const std::string &right, GrpId grp = {} );
    bool             add_usr_right      ( const std::string &right, UsrId usr = {} );
    bool             rem_grp_right      ( const std::string &right, GrpId grp = {} );
    bool             rem_usr_right      ( const std::string &right, UsrId usr = {} );

    Rp  *rp;
};

Rp *_lazyfy( const Variable &var );
Rp *_lazyfy( Variable &&var );

// symbolic
template<class T>
auto lazyfy( T &&v ) -> typename Unref<decltype( as_a_Variable( v ) )>::T { return _lazyfy( as_a_Variable( v ) ); }


/// enable trans of strings, numbers, ... to their Variable equivalents
template<class T>
typename EnableIf<std::is_base_of<Variable,typename Unref<T>::T>::value,T>::T as_a_Variable( T &&val ) {
    return std::forward<T>( val );
}

template<class Cq>
Rp *Variable::read_from( BinStream<Cq> bq ) {
    CbStringPtr ct( *bq.buf );
    BinStream<CbStringPtr> bt( &ct );
    PT os = ct.size();
    Rp *res = read_from( bt );
    bq.skip_some( ct.size() - os );
    return res;
}

} // namespace RiotBase
