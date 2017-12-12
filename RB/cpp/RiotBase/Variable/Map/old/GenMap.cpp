#include "../../System/BinStreamWithOffsets.h"
#include "../../System/Ccast.h"
#include "../Core/Category.h"
#include "../Core/Surdef.h"
#include "../Core/Error.h"
#include "../Number/Bn.h"
#include "PatchMergerGenMap.h"
#include "GenMapSelect.h"
#include "GenMap.h"

namespace RiotBase {

Rp *new_GenMap( const Category *category_map, const Category *category_key, const Category *category_val, Inode crea_usr ) {
    return GenMap::New( category_map, category_key, category_val, crea_usr );
}

GenMap::GenMap( const Category *category_map, const Category *category_key, const Category *category_val, Inode crea_usr ) : category_map( category_map ), category_key( category_key ), category_val( category_val ) {
    usr_rights[ crea_usr ] = base_rights;
    loc_id_num             = 0;
}

GenMap::~GenMap() {
    // Vec of node for postponed deletion. TODO: optimize
    Vec<Node *> v; v.reserve( kmap.size() );
    for( Node &p : kmap )
        v << &p;

    //
    kmap.clear();
    imap.clear();
    for( Node *n : v )
        delete n;
}

Rp *GenMap::mod_select( const Rp *key, UsrId as_usr ) {
    //if ( as_usr and not  )
    //    TODO;
    
    // try to find the key
    KMap::iterator iter = kmap.find( key, Node::KCmp() );
    if ( iter != kmap.end() )
        return GenMapSelect::New( &*iter, this );

    // else, make a new item in the map
    Node *n = add_item( Inode( 0, ++loc_id_num ), RO_CALL( copy, key ), category_val->create() );
    new_nodes << n;

    return GenMapSelect::New( n, this );
}

Rp *GenMap::select_bb( const Rp *key ) const {
    KMap::const_iterator iter = kmap.find( key, Node::KCmp() );
    if ( iter != kmap.end() )
        return GenMapSelect::New( ccast( &*iter ), ccast( this ) );
    return Error::New( "no corresponding key" );
}

void GenMap::add_key( const Rp *key ) {
    KMap::iterator iter = kmap.find( key, Node::KCmp() );
    if ( iter == kmap.end() )
        new_nodes << add_item( Inode( 0, ++loc_id_num ), RO_CALL( copy, key ), category_val->create() );
}

void GenMap::get_changes( CbQueue &cq, const FqId &id ) {
    BinStreamWithOffsets<CbQueue> bq( &cq );

    for( Node *n : new_nodes ) {
        bq << NEW_NODE;
        bq << n->id.glo_dev( cur_dev );
        bq.beg_mark(); RO_CALL( get_changes, n->key, cq, db ); bq.end_mark();
        bq.beg_mark(); RO_CALL( get_changes, n->val, cq, db ); bq.end_mark();
    }
    new_nodes.clear();

    for( Node *n : mod_keys ) {
        CbQueue tq;
        RO_CALL( get_changes, n->key, tq, db );
        if ( not tq.empty() )
            bq << MOD_KEY << n->id.glo_dev( cur_dev ) << std::move( tq );
    }
    mod_keys.clear();

    for( Node *n : mod_vals ) {
        CbQueue tq;
        RO_CALL( get_changes, n->val, tq, db );
        if ( not tq.empty() )
            bq << MOD_VAL << n->id.glo_dev( cur_dev ) << std::move( tq );
    }
    mod_vals.clear();

    bq.crunch();
}

void GenMap::sig_change_val( Node *n ) {
    mod_vals << n;

    for( RcPtr<Callback> &c : n->on_change_vals )
        c->reg_wait();
    sig_change();
}

void GenMap::sig_change_key( Node *n ) {
    mod_keys << n;

    for( RcPtr<Callback> &c : n->on_change_keys )
        c->reg_wait();
    sig_change();
}

void GenMap::sig_change() {
    for( RcPtr<Callback> &c : on_change_list )
        c->reg_wait();
}

GenMap::Node *GenMap::add_item( Inode id, Rp *key, Rp *val ) {
    // new item
    Node *n = new Node;
    n->key = key;
    n->val = val;
    n->id  = id;

    // store it in kmap and imap
    kmap.insert( *n );
    imap.insert( *n );

    // Rk: we do not reg it in new_nodes, because of PatchMergerGenMap
    return n;
}

std::string GenMap::type_name() {
    return "GenMap";
}

void GenMap::write_to_stream( std::ostream &os ) const {
    int cpt = 0;
    for( const Node &kp : kmap ) {
        if ( cpt++ )
            os << " ";
        RO_CALL( write_to_stream, kp.key, os );
        os << ":";
        RO_CALL( write_to_stream, kp.val, os );
    }
}

const Category *GenMap::category() const {
    return category_map;
}

void GenMap::self_on_change( Callback *c ) {
    on_change_list << c;
    c->reg_wait();
}

PatchMerger *GenMap::new_patch_merger() {
    return new PatchMergerGenMap( this );
}

Rp *GenMap::get_size() const {
    return Bn<PT>::New( kmap.size() );
}

Rp *GenMap::copy() const {
    GenMap *res = GenMap::New( category_map, category_key, category_val );
    res->loc_id_num = loc_id_num;
    for( const Node &p : kmap )
        res->new_nodes << res->add_item( p.id, RO_CALL( copy, p.key ), RO_CALL( copy, p.val ) );
    return res;
}

Vec<std::string> GenMap::get_possible_rights() const {
    return { "read" };
}

Vec<GrpId> GenMap::get_groups_in_acl() const {
    TODO;
    return {};
}

Vec<UsrId> GenMap::get_users_in_acl() const {
    TODO;
    return {};
}

Vec<std::string> GenMap::get_grp_rights( GrpId grp ) const {
    TODO;
    return {};
}

Vec<std::string> GenMap::get_usr_rights( UsrId usr ) const {
    TODO;
    return {};
}

bool GenMap::add_grp_right( GrpId grp, const std::string &right ) {
    TODO;
    return {};
}

bool GenMap::add_usr_right( UsrId usr, const std::string &right ) {
    TODO;
    return {};
}

bool GenMap::rem_grp_right( GrpId grp, const std::string &right ) {
    TODO;
    return {};
}

bool GenMap::rem_usr_right( UsrId usr, const std::string &right ) {
    TODO;
    return {};
}



namespace {

RiotBase_DEF_FUNC_2( select_bb, 1, tv0->isa<GenMap>() and tv1->known_term_value() ) {
    return static_cast<const GenMap *>( v0 )->select_bb( v1 );
}
RiotBase_DEF_FUNC_2( mod_select, 1, tv0->isa<GenMap>() and tv1->known_term_value(), UsrId as_usr ) {
    return static_cast<GenMap *>( v0 )->mod_select( v1, as_usr );
}
RiotBase_DEF_FUNC_2( self_add_key_b, 1, tv0->isa<GenMap>() and tv1->known_term_value() ) {
    static_cast<GenMap *>( v0 )->add_key( v1 );
}

}

} // namespace RiotBase
