#pragma once

#include "../../System/BitFieldExtSize.h"
#include "../../System/IsSame.h"
#include "../Core/RpAndUse.h"
#include "ApproxOutputSize.h"
#include "RpTemplSym.h"
#include "Lazyfier.h"

namespace RiotBase {
struct Op_then;

///
///
/// Op_then is a particular case: until execution, the corresponding object can't be deleted
template<class Op,bool self_for_arg>
class SymOp : public RpTemplSym<SymOp<Op,self_for_arg>,Op::nb_args> {
public:
    template<class... Children>
    SymOp( Op op, const Category *category, Children... children ) : RpTemplSym<SymOp<Op,self_for_arg>,Op::nb_args>( category, children... ), op( op ) {}

    static std::string     type_name           () { return Op::name() + std::string( self_for_arg ? "_s" : "" ); }
    // Rp                    *exec                ( BitFieldExtSize own, Rp **ch ) const { return _exec( own, ch, N<Op::nb_args>(), N<self_for_arg>() ); }
    bool                   is_a_s_op           () const { return self_for_arg; }
    void                   del                 ();
    void                   serialize           ( BBQ bq, const PT *num_children ) const;
    void                   get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo **children_info ) const { const Category *ch_cat[ Op::nb_args ]; _get_ch_cat( ch_cat ); info.output_size = ApproxOutputSize<Op>::value( children_info, ch_cat ); }
    static Rp             *deserialize         ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children ) { return _deserialize( bq, children, N<Op::nb_args>{} ); }
    Rp                    *exec_mp             () { return _exec_mp( N<Op::nb_args>(), N<self_for_arg>() ); }

    Op                     op;

private:
    //    Rp                    *_exec               ( BitFieldExtSize own, Rp **ch, N<1>, N<0> ) const;
    //    Rp                    *_exec               ( BitFieldExtSize own, Rp **ch, N<1>, N<1> ) const;
    //    Rp                    *_exec               ( BitFieldExtSize own, Rp **ch, N<2>, N<0> ) const;
    //    Rp                    *_exec               ( BitFieldExtSize own, Rp **ch, N<2>, N<1> ) const;
    //    Rp                    *_exec               ( BitFieldExtSize own, Rp **ch, N<3>, N<0> ) const;
    //    Rp                    *_exec               ( BitFieldExtSize own, Rp **ch, N<3>, N<1> ) const;

    Rp                    *_exec_mp            ( N<1>, N<0> );
    Rp                    *_exec_mp            ( N<1>, N<1> );
    Rp                    *_exec_mp            ( N<2>, N<0> );
    Rp                    *_exec_mp            ( N<2>, N<1> );
    Rp                    *_exec_mp            ( N<3>, N<0> );
    Rp                    *_exec_mp            ( N<3>, N<1> );

    static Rp             *_deserialize        ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children, N<0> );
    static Rp             *_deserialize        ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children, N<1> );
    static Rp             *_deserialize        ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children, N<2> );

    void                   _get_ch_cat         ( const Category **ch_cat ) const { for( int i = 0; i < Op::nb_args; ++i ) ch_cat[ i ] = RO_CALL( category, this->children[ i ] ); }
};

inline Rp *copy_without_lazifier( const Rp *val ) {
    return RO_CALL( copy, val->type_num == StaticStore<TypeTempl<Lazyfier>>::data.type_num ? static_cast<const Lazyfier *>( val )->cur : val );
}

inline Rp *ref_without_lazifier( Rp *val ) {
    if ( val->type_num == StaticStore<TypeTempl<Lazyfier>>::data.type_num ) {
        Rp *res = static_cast<Lazyfier *>( val )->cur;
        static_cast<Lazyfier *>( val )->cur = 0;
        RO_CALL( del, val );
        return res;
    }
    return val;
}

template<class Op,class... Args>
Rp *make_sym_op( Op op, const Args& ...args ) {
    return Lazyfier::New( SymOp<Op>::New( op, Op::category( args... ), copy_without_lazifier( args )... ) );
}



template<class Op> bool _Op_then_get_done( const Op &op ) { return false; }
template<class Op> void _Op_then_set_done( Op &op ) {}
bool _Op_then_get_done( const Op_then &op );
void _Op_then_set_done( Op_then &op );

template<class Op,bool self_for_arg>
void SymOp<Op,self_for_arg>::del() {
    if ( this->cpt_use )
        --this->cpt_use;
    else if ( IsSame<Op_then,Op>::v and not _Op_then_get_done( op ) )
        this->cpt_use = -1; // to say that there is a pending deletion (will be done during exec)
    else
        RpTempl<SymOp<Op,self_for_arg>>::_del( this );
}

template<class Op,bool self_for_arg>
void SymOp<Op,self_for_arg>::serialize( BBQ bq, const PT *num_children ) const {
    if ( Op::nb_args and not num_children ) {
        TODO; // -> special type number to store first the children
    }
    bq << this->num_in_tuple_types();
    for( int i = 0; i < Op::nb_args; ++i )
        bq << num_children[ i ];
}

template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children, N<0> ) {
    return SymOp<Op,self_for_arg>::New( Op{}, nullptr );
}

template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children, N<1> ) {
    PT a = bq.read_unsigned(); if ( a >= children.size() ) { bq.buf->ack_error(); return 0; }
    ++children[ a ].use;
    return SymOp<Op,self_for_arg>::New( Op{}, nullptr, children[ a ].rp );
}

template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children, N<2> ) {
    PT a = bq.read_unsigned(); if ( a >= children.size() ) { bq.buf->ack_error(); return 0; }
    PT b = bq.read_unsigned(); if ( b >= children.size() ) { bq.buf->ack_error(); return 0; }
    ++children[ a ].use;
    ++children[ b ].use;
    return SymOp<Op,self_for_arg>::New( Op{}, nullptr, children[ a ].rp, children[ b ].rp );
}

//template<class Op,bool self_for_arg>
//Rp *SymOp<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<1>, N<0> ) const {
//    Rp *res = op.exec_b( ch[ 0 ] );
//    for( int i = 0; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used an _b version
//    return res;
//}
//template<class Op,bool self_for_arg>
//Rp *SymOp<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<1>, N<1> ) const {
//    Rp *res = op.exec_s( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) ? ch[ 0 ] : RO_CALL( copy, ch[ 0 ] ) );
//    if ( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) and not own.select( N<2*Op::nb_args>(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
//        ccast( this )->children[ 0 ] = 0;
//    if ( IsSame<Op_then,Op>::v ) {
//        _Op_then_set_done( ccast( op ) );
//        if ( this->cpt_use == -1 ) {
//            this->cpt_use = 0;
//            ccast( this )->del();
//        }
//    }
//    return res;
//}

//template<class Op,bool self_for_arg>
//Rp *SymOp<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<2>, N<0> ) const {
//    Rp *res = op.exec_bb( ch[ 0 ], ch[ 1 ] );
//    for( int i = 0; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
//}

//template<class Op,bool self_for_arg>
//Rp *SymOp<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<2>, N<1> ) const {
//    Rp *res = op.exec_sb( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) ? ch[ 0 ] : RO_CALL( copy, ch[ 0 ] ), ch[ 1 ] );
//    if ( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) and not own.select( N<2*Op::nb_args>(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
//        ccast( this )->children[ 0 ] = 0;
//    for( int i = 1; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
//}

//template<class Op,bool self_for_arg>
//Rp *SymOp<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<3>, N<0> ) const {
//    Rp *res = op.exec_bbb( ch[ 0 ], ch[ 1 ], ch[ 2 ] );
//    for( int i = 0; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
//}

//template<class Op,bool self_for_arg>
//Rp *SymOp<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<3>, N<1> ) const {
//    Rp *res = op.exec_sbb( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) ? ch[ 0 ] : RO_CALL( copy, ch[ 0 ] ), ch[ 1 ], ch[ 2 ] );
//    if ( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) and not own.select( N<2*Op::nb_args>(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
//        ccast( this )->children[ 0 ] = 0;
//    for( int i = 1; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
//}



template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_exec_mp( N<1>, N<0> ) {
    // TODO: exec_oo
    return op.exec_b( this->children[ 0 ] );
}
template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_exec_mp( N<1>, N<1> ) {
    TODO;
    return 0;
}

template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_exec_mp( N<2>, N<0> ) {
    // TODO: exec_oo
    return op.exec_bb( this->children[ 0 ], this->children[ 1 ] );
//    Rp *res = op.exec_bb( ch[ 0 ], ch[ 1 ] );
//    for( int i = 0; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
}

template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_exec_mp( N<2>, N<1> ) {
    TODO;
    return 0;
//    Rp *res = op.exec_sb( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) ? ch[ 0 ] : RO_CALL( copy, ch[ 0 ] ), ch[ 1 ] );
//    if ( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) and not own.select( N<2*Op::nb_args>(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
//        ccast( this )->children[ 0 ] = 0;
//    for( int i = 1; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
}

template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_exec_mp( N<3>, N<0> ) {
    TODO;
    return 0;
//    Rp *res = op.exec_bbb( ch[ 0 ], ch[ 1 ], ch[ 2 ] );
//    for( int i = 0; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
}

template<class Op,bool self_for_arg>
Rp *SymOp<Op,self_for_arg>::_exec_mp( N<3>, N<1> ) {
    TODO;
    return 0;
//    Rp *res = op.exec_sbb( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) ? ch[ 0 ] : RO_CALL( copy, ch[ 0 ] ), ch[ 1 ], ch[ 2 ] );
//    if ( own.select( N<2*Op::nb_args>(), 2 * 0 + 0 ) and not own.select( N<2*Op::nb_args>(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
//        ccast( this )->children[ 0 ] = 0;
//    for( int i = 1; i < Op::nb_args; ++i )
//        if ( own.select( N<2*Op::nb_args>(), 2 * i + 0 ) and own.select( N<2*Op::nb_args>(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
//            RO_CALL( del, ch[ i ] ); // because we have used a _b version
//    return res;
}

} // namespace RiotBase
