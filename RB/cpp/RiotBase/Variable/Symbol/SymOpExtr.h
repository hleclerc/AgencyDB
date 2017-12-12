#pragma once

#include "../../System/BitFieldExtSize.h"
#include "RpTemplSym.h"

namespace RiotBase {

///
template<class Op,bool self_for_arg=false>
class SymOpExtr : public RpTemplSym<SymOpExtr<Op,self_for_arg>,-1> {
public:
    SymOpExtr( Op op, const Category *category, int nb_args, Rp **args ) : RpTemplSym<SymOpExtr<Op,self_for_arg>,-1>( category, nb_args, args ), op( op ) {}

    static std::string     type_name() { return Op::name() + std::string( self_for_arg ? "_extr_s" : "_extr" ); }
    Rp                    *exec     ( BitFieldExtSize own, Rp **ch ) const { return _exec( own, ch, N<Op::nb_args>(), N<self_for_arg>() ); }
    bool                   is_a_s_op() const { return self_for_arg; }

    Op                     op;

private:
    Rp                    *_exec    ( BitFieldExtSize own, Rp **ch, N<1>, N<0> ) const;
    Rp                    *_exec    ( BitFieldExtSize own, Rp **ch, N<1>, N<1> ) const;
    Rp                    *_exec    ( BitFieldExtSize own, Rp **ch, N<2>, N<0> ) const;
    Rp                    *_exec    ( BitFieldExtSize own, Rp **ch, N<2>, N<1> ) const;
    Rp                    *_exec    ( BitFieldExtSize own, Rp **ch, N<3>, N<0> ) const;
    Rp                    *_exec    ( BitFieldExtSize own, Rp **ch, N<3>, N<1> ) const;
};

template<class Op,bool self_for_arg>
Rp *SymOpExtr<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<1>, N<0> ) const {
    TODO;
    return {};
}

template<class Op,bool self_for_arg>
Rp *SymOpExtr<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<1>, N<1> ) const {
    Rp *res = op.exec_s_extr( ch, this->nb_symbolic_children() - Op::nb_args );
    if ( own.select( 2 * this->nb_symbolic_children(), 2 * 0 + 0 ) and not own.select( 2 * this->nb_symbolic_children(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
        ccast( this )->children[ 0 ] = 0;
    for( int i = 1; i < this->nb_symbolic_children(); ++i )
        if ( own.select( 2 * this->nb_symbolic_children(), 2 * i + 0 ) and own.select( 2 * this->nb_symbolic_children(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
            RO_CALL( del, ch[ i ] ); // because we have used an exec_sb
    return res;
}

template<class Op,bool self_for_arg>
Rp *SymOpExtr<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<2>, N<0> ) const {
    TODO;
    return {};
}

template<class Op,bool self_for_arg>
Rp *SymOpExtr<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<2>, N<1> ) const {
    Rp *res = op.exec_sb_extr( ch, this->nb_symbolic_children() - Op::nb_args );
    if ( own.select( 2 * this->nb_symbolic_children(), 2 * 0 + 0 ) and not own.select( 2 * this->nb_symbolic_children(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
        ccast( this )->children[ 0 ] = 0;
    for( int i = 1; i < this->nb_symbolic_children(); ++i )
        if ( own.select( 2 * this->nb_symbolic_children(), 2 * i + 0 ) and own.select( 2 * this->nb_symbolic_children(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
            RO_CALL( del, ch[ i ] ); // because we have used an exec_sb
    return res;
}

template<class Op,bool self_for_arg>
Rp *SymOpExtr<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<3>, N<0> ) const {
    TODO;
    return {};
}

template<class Op,bool self_for_arg>
Rp *SymOpExtr<Op,self_for_arg>::_exec( BitFieldExtSize own, Rp **ch, N<3>, N<1> ) const {
    Rp *res = op.exec_sbb_extr( ch, this->nb_symbolic_children() - Op::nb_args );
    if ( own.select( 2 * this->nb_symbolic_children(), 2 * 0 + 0 ) and not own.select( 2 * this->nb_symbolic_children(), 2 * 0 + 1 ) ) // if children[ 0 ] is used as self_arg, we don't want it to be deleted during the deletion of this
        ccast( this )->children[ 0 ] = 0;
    for( int i = 1; i < this->nb_symbolic_children(); ++i )
        if ( own.select( 2 * this->nb_symbolic_children(), 2 * i + 0 ) and own.select( 2 * this->nb_symbolic_children(), 2 * i + 1 ) ) // if ch == this->children (meaning own( 2 * i + 1 )==false), deletion will be made by the deletion of this
            RO_CALL( del, ch[ i ] ); // because we have used an exec_sb
    return res;
}

} // namespace RiotBase
