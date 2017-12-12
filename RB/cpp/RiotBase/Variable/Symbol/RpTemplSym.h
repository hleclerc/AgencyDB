#pragma once

#include "../Core/RpTemplRefCounted.h"
#include "../../System/Ccast.h"
#include "CommonSymbolicData.h"
#include "ExecList.h"

namespace RiotBase {

template<class T>
class RpTemplSymCommon : public RpTemplRefCounted<T> {
public:
    CommonSymbolicData        *common_symbolic_data() const { return &csd; }
    // void                    repl_this_by        ( Rp *val ) const { for( const Rp *p : csd.parents ) for( int i = 0, n = RO_CALL( nb_symbolic_children, p ); i < n; ++i ) if ( RO_CALL( symbolic_child, p, i ) == this ) RO_CALL( set_child, ccast( p ), i, val ); }
    mutable CommonSymbolicData csd;
};

/// nb_children = -1 to have a dynamic number of children
template<class T,int nb_children=0>
class RpTemplSym : public RpTemplSymCommon<T> {
public:
    enum { unknown_value = true, term_value = true };

    template<class... Args> RpTemplSym( const Category *category, Args &&... args ) : RpTemplSym( N<nb_children>(), std::forward<Args>( args )... ) { _category = category; }
    RpTemplSym( const Category *category ) : _category( category ) {}

    ~RpTemplSym();

    Rp                        *copy                () const { ++this->cpt_use; return ccast( this ); } ///< assumes that nothing will modify this
    void                       write_to_stream     ( std::ostream &os ) const;
    void                       write_structure     ( std::ostream &os ) const;

    int                        nb_symbolic_children() const { return nb_children; }
    const Rp                  *symbolic_child      ( int n ) const { return children[ n ]; }
    const Category            *category            () const { return _category; }
    // void                       loc_to_glo         ( const FqId &id ) { for( PT i = 0; i < nb_children; ++i ) RO_CALL( loc_to_glo, children[ i ], id ); }
    // void                       glo_to_loc         ( const FqId &id ) { for( PT i = 0; i < nb_children; ++i ) RO_CALL( glo_to_loc, children[ i ], id ); }

    // const Rp                  *write_graphviz      ( std::ostream &os ) const { os << "node_" << this << " [label=\"" << T::type_name() << this << "\"];"; return this; }
    void                       del_or_cancel_exec  ();

    void                       set_child           ( int n, Rp *c, bool del_prev );
    void                       clr_child           ( int n );

    Rp                        *children[ nb_children ];
    const Category            *_category;

protected:
    template<PT n,class... Args>
    RpTemplSym( N<n>, Rp *ch, Args &&... args ) : RpTemplSym( N<n-1>(), std::forward<Args>( args )... ) { set_child( nb_children - n, ch, false ); }

    template<class... Args>
    RpTemplSym( N<0>, Args &&... args ) { for( int i = 0; i < nb_children; ++i ) children[ i ] = 0; }

    RpTemplSym( N<0> ) { for( int i = 0; i < nb_children; ++i ) children[ i ] = 0; }
};


template<class T,int nb_children>
RpTemplSym<T,nb_children>::~RpTemplSym() {
    for( int i = 0; i < nb_children; ++i ) {
        if ( not children[ i ] )
            continue;
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ i ] ) )
            csd->parents.remove_first_unordered( this );
        RO_CALL( del, children[ i ] );
    }
}

template<class T,int nb_children>
void RpTemplSym<T,nb_children>::write_to_stream( std::ostream &os ) const {
    os << T::type_name();
    if ( int nc = nb_symbolic_children() ) {
        for( int u = 0; u < nc; ++u ) {
            os << ( u ? ',' : '(' );
            if ( symbolic_child( u ) )
                RO_CALL( write_to_stream, symbolic_child( u ), os );
        }
        os << ')';
    }
}

template<class T,int nb_children>
void RpTemplSym<T,nb_children>::del_or_cancel_exec() {
    PT &status = reinterpret_cast<PT &>( this->csd.op_mp );
    if ( this->cpt_use ) {
        --this->cpt_use;
    } else if ( status == ExecList::OBJ_STATE_EXECUTING ) {
        status = ExecList::OBJ_STATE_CANCELED;
    } else {
        exec_list->leaves.remove_first_unordered( this );

        for( int i = 0; i < nb_children; ++i ) {
            if ( not children[ i ] )
                continue;
            if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ i ] ) )
                csd->parents.remove_first_unordered( this );
            RO_CALL( del_or_cancel_exec, children[ i ] );
            children[ i ] = 0; // because RpTempl<T>::_del( this ); will call the destructor anyway
        }

        RpTempl<T>::_del( this );
    }
}

template<class T,int nb_children>
void RpTemplSym<T,nb_children>::set_child( int n, Rp *c, bool del_prev ) {
    if ( del_prev and children[ n ] ) {
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ n ] ) )
            csd->parents.remove_first_unordered( this );
        RO_CALL( del, children[ n ] );
    }
    children[ n ] = c;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, c ) )
        csd->parents << this;
}

template<class T,int nb_children>
void RpTemplSym<T,nb_children>::clr_child( int n ) {
    if ( children[ n ] ) {
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ n ] ) )
            csd->parents.remove_first_unordered( this );
        children[ n ] = 0;
    }
}

template<class T,int nb_children>
void RpTemplSym<T,nb_children>::write_structure( std::ostream &os ) const {
    os << T::type_name(); // << "[cpt_use=" << this->cpt_use << "]";
    if ( int nc = nb_symbolic_children() ) {
        for( int u = 0; u < nc; ++u )
            if ( symbolic_child( u ) )
                RO_CALL( write_structure, symbolic_child( u ), os << ( u ? ',' : '(' ) );
        os << ')';
    }
}

// specialization for dynamic number of children
template<class T>
class RpTemplSym<T,-1> : public RpTemplRefCounted<T> {
public:
    enum { unknown_value = true, term_value = true };

    RpTemplSym( const Category *category, int nb_children, Rp **args );
    ~RpTemplSym();

    Rp                        *copy                () const { ++this->cpt_use; return ccast( this ); } ///< assumes that nothing will modify this
    void                       write_to_stream     ( std::ostream &os ) const;
    CommonSymbolicData        *common_symbolic_data() const { return &csd; }

    int                        nb_symbolic_children() const { return children.size(); }
    const Rp                  *symbolic_child      ( int n ) const { return children[ n ]; }
    const Category            *category            () const { return _category; }
    // static void                loc_to_glo          ( CbQueue &glo, CbStringPtr &loc, Gateway *gw ) const { for( PT i = 0; i < children.size(); ++i ) RO_CALL( loc_to_glo, children[ i ], glo, loc, gw ); }
    // static void                glo_to_loc          ( CbQueue &loc, CbStringPtr &glo, Gateway *gw ) const { for( PT i = 0; i < children.size(); ++i ) RO_CALL( glo_to_loc, children[ i ], loc, glo, gw ); }

    // const Rp                  *write_graphviz      ( std::ostream &os ) const { os << "node_" << this << " [label=\"" << T::type_name() << csd.op_id << "\"];"; return this; }
    void                       del_or_cancel_exec  ();

    void                       set_child           ( int n, Rp *c, bool del_prev );
    void                       clr_child           ( int n );

    mutable CommonSymbolicData csd;
    Vec<Rp *>                  children;
    const Category            *_category;
};

template<class T>
RpTemplSym<T,-1>::RpTemplSym( const Category *category, int nb_children, Rp **args ) : _category( category ) {
    children.resize( nb_children, nullptr );
    for( int i = 0; i < nb_children; ++i )
        set_child( i, args[ i ], false );
}

template<class T>
RpTemplSym<T,-1>::~RpTemplSym() {
    for( PT i = 0; i < children.size(); ++i ) {
        if ( not children[ i ] )
            continue;
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ i ] ) )
            csd->parents.remove_first_unordered( this );
        RO_CALL( del, children[ i ] );
    }
}

template<class T>
void RpTemplSym<T,-1>::write_to_stream( std::ostream &os ) const {
    os << T::type_name();
    if ( int nc = nb_symbolic_children() ) {
        for( int u = 0; u < nc; ++u )
            if ( symbolic_child( u ) )
                RO_CALL( write_to_stream, symbolic_child( u ), os << ( u ? ',' : '(' ) );
        os << ')';
    }
}


template<class T>
void RpTemplSym<T,-1>::del_or_cancel_exec() {
    PT &status = reinterpret_cast<PT &>( this->csd.op_mp );
    if ( this->cpt_use ) {
        --this->cpt_use;
    } else if ( status == ExecList::OBJ_STATE_EXECUTING ) {
        status = ExecList::OBJ_STATE_CANCELED;
    } else {
        exec_list->leaves.remove_first_unordered( this );

        for( PT i = 0; i < children.size(); ++i ) {
            if ( not children[ i ] )
                continue;
            if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ i ] ) )
                csd->parents.remove_first_unordered( this );
            RO_CALL( del_or_cancel_exec, children[ i ] );
            children[ i ] = 0; // because RpTempl<T>::_del( this ); will call the destructor anyway
        }

        RpTempl<T>::_del( this );
    }
}

template<class T>
void RpTemplSym<T,-1>::set_child( int n, Rp *c, bool del_prev ) {
    if ( del_prev and children[ n ] ) {
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ n ] ) )
            csd->parents.remove_first_unordered( this );
        RO_CALL( del, children[ n ] );
    }
    children[ n ] = c;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, c ) )
        csd->parents << this;
}

template<class T>
void RpTemplSym<T,-1>::clr_child( int n ) {
    if ( children[ n ] ) {
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, children[ n ] ) )
            csd->parents.remove_first_unordered( this );
        children[ n ] = 0;
    }
}

} // namespace RiotBase
