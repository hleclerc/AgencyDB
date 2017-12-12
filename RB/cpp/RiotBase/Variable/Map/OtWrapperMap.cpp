#include "../../System/AutoPtr.h"
#include "../../System/Ccast.h"
#include "../Ot/OtMerger.h"
#include "../Core/Error.h"
#include "../Number/Bn.h"

#include "OtWrapperMapSelect.h"

namespace RiotBase {

Rp *new_OtWrapperMap( const Category *category_map, const Category *category_key, const Category *category_val, UsrId usr_id ) {
    return OtWrapperMap::New( category_map, category_key, category_val, usr_id );
}

OtWrapperMap::OtWrapperMap( const Category *category_map, const Category *category_key, const Category *category_val, UsrId usr_id ) : OtWrapper<OtWrapperMap>( usr_id ), category_map( category_map ), category_key( category_key ), category_val( category_val ) {
    loc_id_num = 0;
}

OtWrapperMap::~OtWrapperMap() {
    kmap.clear();
    imap.clear_and_dispose( []( Node *p ) { delete p; } );
}

std::string OtWrapperMap::type_name() {
    return "OtWrapperMap";
}

void OtWrapperMap::write_to_stream( std::ostream &os ) const {
    int cpt = 0;
    os << '{';
    for( const Node &kp : kmap ) {
        RO_CALL( write_to_stream, kp.key, os << ( cpt++ ? "," : "" ) );
        RO_CALL( write_to_stream, kp.val, os << ":" );
    }
    os << '}';
}

const Category *OtWrapperMap::category() const {
    return category_map;
}

Rp *OtWrapperMap::get_size() const {
    return Bn<PT>::New( kmap.size() );
}

Rp *OtWrapperMap::copy() const {
    OtWrapperMap *res = OtWrapperMap::New( category_map, category_key, category_val, UsrId{} );
    res->loc_id_num = loc_id_num;
    for( const Node &p : kmap )
        res->new_nodes << res->add_item_wocr( p.id, RO_CALL( copy, p.key ), RO_CALL( copy, p.val ) );
    return res;
}



Rp *OtWrapperMap::mod_select( const Rp *key ) {
    // try to find the key
    KMap::iterator iter = kmap.find( key, Node::KCmp() );
    if ( iter != kmap.end() )
        return OtWrapperMapSelect::New( &*iter, this );

    // else, make a new item in the map
    if ( not has_right( can_add_key{} ) )
        return Error::New( "No right to add key" );
    Node *n = add_item_wocr( PatchId( 0, ++loc_id_num ), RO_CALL( copy, key ), category_val->create() );
    new_nodes << n;

    return OtWrapperMapSelect::New( n, this );
}

Rp *OtWrapperMap::select_bb( const Rp *key ) const {
    KMap::const_iterator iter = kmap.find( key, Node::KCmp() );
    if ( iter != kmap.end() )
        return OtWrapperMapSelect::New( ccast( &*iter ), ccast( this ) );
    return Error::New( "no corresponding key" );
}

void OtWrapperMap::add_key( const Rp *key ) {
    if ( has_right( can_add_key{} ) ) {
        KMap::iterator iter = kmap.find( key, Node::KCmp{} );
        if ( iter == kmap.end() )
            new_nodes << add_item_wocr( PatchId( 0, ++loc_id_num ), RO_CALL( copy, key ), category_val->create() );
    }
}

void OtWrapperMap::rem_key( const Rp *key ) {
    if ( has_right( can_rem_key{} ) ) {
        KMap::iterator iter = kmap.find( key, Node::KCmp{} );
        if ( iter != kmap.end() ) {
            Node *node = &*iter;

            // remove from mod_nodes and new_nodes
            if ( node->prev ) new_nodes.remove( node );
            if ( node->prev ) mod_nodes.remove( node );

            // register the operation
            CbQueue key_dat; RO_CALL( pull_ot_changes, node->key, &key_dat );
            CbQueue val_dat; RO_CALL( pull_ot_changes, node->val, &val_dat );
            reg_op( op_rem_key_und{}, iter->id, key_dat, val_dat );

            // do it
            rem_item_wocr( node );
        }
    }
}


void OtWrapperMap::sig_change_val( Node *n ) {
    RO_CALL( sig_change, n->val );
    mod_nodes << n;
    sig_change();
}

void OtWrapperMap::sig_change_key( Node *n ) {
    RO_CALL( sig_change, n->key );
    mod_nodes << n;
    sig_change();
}

OtWrapperMap::Node *OtWrapperMap::add_item_wocr( PatchId id, Rp *key, Rp *val ) {
    // new item
    Node *n = new Node;
    n->key = key;
    n->val = val;
    n->id  = id;

    // store it in kmap and imap
    kmap.insert( *n );
    imap.insert( *n );
    return n;
}

void OtWrapperMap::rem_item_wocr( PatchId id ) {
    IMap::iterator iter = imap.find( id, Node::ICmp{} );
    if ( iter != imap.end() ) {
        imap.erase( iter );
        kmap.erase( iter->key, Node::KCmp{} );
        delete &*iter;
    }
}

void OtWrapperMap::rem_item_wocr( OtWrapperMap::Node *n ) {
    imap.erase( n->id , Node::ICmp{} );
    kmap.erase( n->key, Node::KCmp{} );
    delete n;
}

void OtWrapperMap::freeze_last_changes() {
    for( Node *n : new_nodes ) {
        CbQueue m_key; RO_CALL( pull_ot_changes, n->key, &m_key );
        CbQueue m_val; RO_CALL( pull_ot_changes, n->val, &m_val );
        reg_op( op_new_key{}, n->id, m_key, m_val );
    }
    new_nodes.clear();

    for( Node *n : mod_nodes ) {
        CbQueue m_key; RO_CALL( pull_ot_changes, n->key, &m_key );
        CbQueue m_val; RO_CALL( pull_ot_changes, n->val, &m_val );
        if ( not m_key.empty() ) reg_op( op_mod_key{}, n->id, m_key );
        if ( not m_val.empty() ) reg_op( op_mod_val{}, n->id, m_val );
    }
    mod_nodes.clear();
}

void OtWrapperMap::ot_merger_finalize() {
    key_mergers.clear();
    val_mergers.clear();
}

OtMerger *OtWrapperMap::get_key_merger( PatchId key_id ) {
    MapMerger::iterator iter = key_mergers.find( key_id );
    if ( iter == key_mergers.end() ) {
        IMap::iterator node = imap.find( key_id, Node::ICmp() );
        if ( node == imap.end() )
            return nullptr; // no corresponding node
        iter = key_mergers.insert( iter, std::make_pair( key_id, std::make_pair( RO_CALL( new_ot_merger, node->key ), &*node ) ) );
    }
    return iter->second.first.ptr();
}

OtMerger *OtWrapperMap::get_val_merger( PatchId key_id ) {
    MapMerger::iterator iter = val_mergers.find( key_id );
    if ( iter == val_mergers.end() ) {
        IMap::iterator node = imap.find( key_id, Node::ICmp() );
        if ( node == imap.end() )
            return nullptr; // no corresponding node
        iter = val_mergers.insert( iter, std::make_pair( key_id, std::make_pair( RO_CALL( new_ot_merger, node->val ), &*node ) ) );
    }
    return iter->second.first.ptr();
}

void OtWrapperMap::apply_op( op_new_key, Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data ) {
    add_item_wocr( key_id, category_key->create( aod.as_usr, key_data ), category_val->create( aod.as_usr, val_data ) );
}

void OtWrapperMap::apply_op( op_rem_key, Aod &aod, PatchId key_id ) {
    rem_item_wocr( key_id );
}

void OtWrapperMap::apply_op( op_rem_key_und, Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data ) {
    apply_op( op_rem_key{}, aod, key_id );
}

void OtWrapperMap::apply_op( op_mod_key, Aod &aod, PatchId key_id, CbString &key_data ) {
    if ( OtMerger *pm = get_key_merger( key_id ) ) {
        CbQueue new_key_data;

        switch( aod.type ) {
        case Aod::NEW_PATCH: pm->new_patch( &new_key_data, key_data, aod.as_usr, 1, 1 ); key_data = std::move( new_key_data ); break;
        case Aod::UNK_PATCH: pm->unk_patch( &new_key_data, key_data, aod.as_usr, 1, 1 ); key_data = std::move( new_key_data ); break;
        case Aod::STD_PATCH: pm->std_patch( &new_key_data, key_data, aod.as_usr, 1, 1 ); key_data = std::move( new_key_data ); break;
        case Aod::UNDO     : pm->undo     ( key_data, aod.as_usr ); break;
        }
    }
}

void OtWrapperMap::apply_op( op_mod_val, Aod &aod, PatchId key_id, CbString &val_data ) {
    if ( OtMerger *pm = get_val_merger( key_id ) ) {
        CbQueue new_val_data;

        switch( aod.type ) {
        case Aod::NEW_PATCH: pm->new_patch( &new_val_data, val_data, aod.as_usr, 1, 1 ); val_data = std::move( new_val_data ); break;
        case Aod::UNK_PATCH: pm->unk_patch( &new_val_data, val_data, aod.as_usr, 1, 1 ); val_data = std::move( new_val_data ); break;
        case Aod::STD_PATCH: pm->std_patch( &new_val_data, val_data, aod.as_usr, 1, 1 ); val_data = std::move( new_val_data ); break;
        case Aod::UNDO     : pm->undo     ( val_data, aod.as_usr ); break;
        }
    }
}

// Undo
void OtWrapperMap::undo_patch( op_new_key, Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data ) {
    apply_op( op_rem_key_und{}, aod, key_id, key_data, val_data );
}

void OtWrapperMap::undo_patch( op_rem_key, Aod &aod, PatchId key_id ) {
    WARNING( "A non _und operation should not b undoed" );
}

void OtWrapperMap::undo_patch( op_rem_key_und, Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data ) {
    apply_op( op_new_key{}, aod, key_id, key_data, val_data );
}

void OtWrapperMap::undo_patch( op_mod_key, Aod &aod, PatchId key_id, CbString &key_data ) {
    if ( OtMerger *pm = get_key_merger( key_id ) )
        pm->undo( key_data, aod.as_usr );
}

void OtWrapperMap::undo_patch( op_mod_val, Aod &aod, PatchId key_id, CbString &val_data ) {
    if ( OtMerger *pm = get_val_merger( key_id ) )
        pm->undo( val_data, aod.as_usr );
}

// transmission
void OtWrapperMap::transmission( op_rem_key_und, BBQ br, PatchId key_id, CbString &key_data, CbString &val_data ) {
    transmission( op_rem_key{}, br, key_id );
}

void OtWrapperMap::register_patch_op( op_rem_key, BBQ bq, PatchId key_id ) {
    IMap::iterator node = imap.find( key_id, Node::ICmp{} );
    if ( node != imap.end() ) {
        CbQueue key_dat; RO_CALL( pull_ot_changes, node->key, &key_dat );
        CbQueue val_dat; RO_CALL( pull_ot_changes, node->val, &val_dat );
        reg_op_in( bq, op_rem_key_und{}, key_id, key_dat, val_dat );
    }
}



namespace {

RiotBase_DEF_FUNC_2( select_bb, 1, tv0->isa<OtWrapperMap>() and tv1->known_term_value() ) {
    return static_cast<const OtWrapperMap *>( v0 )->select_bb( v1 );
}
RiotBase_DEF_FUNC_2( mod_select, 1, tv0->isa<OtWrapperMap>() and tv1->known_term_value() ) {
    return static_cast<OtWrapperMap *>( v0 )->mod_select( v1 );
}
RiotBase_DEF_FUNC_2( self_add_key_b, 10, tv0->isa<OtWrapperMap>() and tv1->known_term_value() ) {
    static_cast<OtWrapperMap *>( v0 )->add_key( v1 );
}
RiotBase_DEF_FUNC_2( self_rem_key_b, 1, tv0->isa<OtWrapperMap>() and tv1->known_term_value() ) {
    static_cast<OtWrapperMap *>( v0 )->rem_key( v1 );
}

}

} // namespace RiotBase
