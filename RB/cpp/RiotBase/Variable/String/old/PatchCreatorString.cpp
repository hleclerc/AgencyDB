#include "PatchCreatorStringCursor.h"
#include "../../System/Ccast.h"
#include "../String.h"
#include "../Number.h"

namespace RiotBase {

void PatchCreatorString::apply_op( op_insert, Aod &aod, PT pos, const CbString &str ) {
    // cursors
    for( AutoPtr<Cursor> &c : cursors )
        if ( c->pos >= pos )
            c->pos += str.size();
    // op
    Number n( pos ); String s{ (std::string)str };
    RO_CALL( self_insert_bb, src, n.rp, s.rp );
    sig_change();
}

void PatchCreatorString::apply_op( op_remove, Aod &aod, PT pos, PT len ) {
    // cursors
    for( AutoPtr<Cursor> &c : cursors ) {
        if ( c->pos >= pos + len )
            c->pos -= len;
        else if ( c->pos > pos )
            c->pos = pos;
    }

    // op
    Number p( pos ), l( len );
    RO_CALL( self_remove_bb, src, p.rp, l.rp );
    sig_change();
}

void PatchCreatorString::apply_op( op_remove_und, Aod &aod, PT pos, const CbString &str ) {
    apply_op( op_remove(), aod, pos, str.size() );
}

void PatchCreatorString::apply_op( op_new_cursor, Aod &aod, DevId dev_id, PI64 loc_id, PT pos ) {
    _new_cursor_wo_cr( dev_id, loc_id, pos, aod.as_usr, 0 );
}

void PatchCreatorString::apply_op( op_set_cursor, Aod &aod, DevId dev_id, PI64 loc_id, PT pos ) {
    for( AutoPtr<Cursor> &c : cursors )
        if ( c->dev_id == dev_id and c->loc_id == loc_id )
            return _set_cursor_wo_cr( c.ptr(), pos );
}

void PatchCreatorString::apply_op( op_set_cursor_und, Aod &aod, DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {
    apply_op( op_set_cursor{}, aod, dev_id, loc_id, pos );
}

void PatchCreatorString::apply_op( op_rem_cursor, Aod &aod, DevId dev_id, PI64 loc_id ) {
    for( PT i = 0; i < cursors.size(); ++i ) {
        if ( cursors[ i ]->dev_id == dev_id and cursors[ i ]->loc_id == loc_id ) {
            cursors.remove_unordered( i );
            sig_change();
            return;
        }
    }
}

void PatchCreatorString::apply_op( op_rem_cursor_und, Aod &aod, DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr ) {
    apply_op( op_rem_cursor{}, aod, dev_id, loc_id );
}

PatchCreatorString::PatchCreatorString( Rp *src, UsrId usr_id ) : PatchCreator<PatchCreatorString>( usr_id ), src( src ) {
    DaSi ds = RO_CALL( dasi, src );
    if ( ds.size )
        reg_op( pcd.bo, op_insert{}, 0, ds );

    cur_cursor_id = 0;
}

PatchCreatorString::~PatchCreatorString() {
    if ( src ) RO_CALL( del, src );
}

std::string PatchCreatorString::type_name() {
    return "PatchCreatorString";
}

const Rp *PatchCreatorString::prox_ref() const {
    return src;
}

void PatchCreatorString::write_structure( std::ostream &os ) const {
    RO_CALL( write_structure, src, os << type_name() << '(' ); os << ')';
}

Rp *PatchCreatorString::new_cursor( void *int_id ) {
    return PatchCreatorStringCursor::New( this, _new_cursor( 0, ++cur_cursor_id, 0, int_id ) );
}

void PatchCreatorString::rem_cursor( void *int_id ) {
    for( AutoPtr<Cursor> &c : cursors )
        if ( c->int_id == int_id )
            return _rem_cursor( c.ptr() );
}

void PatchCreatorString::get_cursors( Vec<CursorData> &res ) const {
    res.reserve( res.size() + cursors.size() );
    for( const AutoPtr<Cursor> &c : cursors )
        res.push_back( PatchCreatorStringCursor::New( ccast( this ), ccast( c.ptr() ) ), c->dev_id, c->loc_id, c->usr_id, c->int_id );
}

void PatchCreatorString::sig_change() {
    PatchCreator<PatchCreatorString>::sig_change();
    RO_CALL( sig_change, src );
}

_TCB void PatchCreatorString::_insert( PT pos, const Cb &data ) {
    if ( not right_to( op_insert{}, {}, pos, data ) )
         return;
    reg_op( pcd.bo, op_insert(), pos, data );
    Aod aod;
    apply_op( op_insert{}, aod, pos, data );
}

void PatchCreatorString::_remove( PT pos, PT len ) {
    if ( not right_to( op_remove{}, {}, pos, len ) )
         return;

    // OT. IMPORTANT_TODO: utf8
    DaSi ds = RO_CALL( dasi, src );
    pos = std::min( pos, ds.size );
    len = std::min( len, ds.size - pos );
    reg_op( pcd.bo, op_remove_und{}, pos, CmString( ds.beg() + pos, ds.beg() + pos + len ) );

    Aod aod;
    apply_op( op_remove(), aod, pos, len );
}

CmString PatchCreatorString::_substring( PT pos, PT len ) const {
    // IMPORTANT_TODO: utf8
    DaSi ds = RO_CALL( dasi, src );
    pos = std::min( pos, ds.size );
    len = std::min( len, ds.size - pos );
    return { ds.data + pos, ds.data + pos + len };
}

PT PatchCreatorString::_size() const {
    // IMPORTANT_TODO: utf8
    DaSi ds = RO_CALL( dasi, src );
    return ds.size;
}

PatchCreatorString::Cursor *PatchCreatorString::_new_cursor_wo_cr( DevId dev_id, PI64 loc_id, PT pos, UsrId as_usr, void *int_id ) {
    Cursor *c = new Cursor{ pos, loc_id, dev_id, as_usr, int_id };
    cursors << c;
    sig_change();
    return c;
}

void PatchCreatorString::_set_cursor_wo_cr( Cursor *cur, PT pos ) {
    if ( not cur or cur->pos == pos )
        return;
    cur->pos = pos;
    sig_change();
}

PatchCreatorString::Cursor *PatchCreatorString::_new_cursor( DevId dev_id, PI64 loc_id, PT pos, void *int_id ) {
    if ( not has_right( can_read{} ) )
        return 0;
    reg_op( pcd.bo, op_new_cursor{}, dev_id, loc_id, pos );
    return _new_cursor_wo_cr( dev_id, loc_id, pos, {}, int_id );
}

void PatchCreatorString::_set_cursor( PatchCreatorString::Cursor *cur, PT pos ) {
    if ( not has_right( can_read{} ) or not cur or cur->pos == pos )
        return;
    reg_op( pcd.bo, op_set_cursor_und{}, cur->dev_id, cur->loc_id, pos, cur->pos );
    _set_cursor_wo_cr( cur, pos );
}

void PatchCreatorString::_rem_cursor( Cursor *cur ) {
    if ( not has_right( can_read{} ) or not cur )
        return;
    reg_op( pcd.bo, op_rem_cursor_und{}, cur->dev_id, cur->loc_id, cur->pos, cur->usr_id );
    _rem_cursor_wo_cr( cur );
}

void PatchCreatorString::_rem_cursor_wo_cr( Cursor *cur ) {
    for( PT i = 0; i < cursors.size(); ++i ) {
        if ( cursors[ i ] == cur ) {
            cursors.remove_unordered( i );
            sig_change();
            return;
        }
    }
}

namespace {

RiotBase_DEF_FUNC_2( self_add_b, 1, tv0->isa<PatchCreatorString>() and tv1->has_surdef_for( RF_dasi() ) ) {
    PatchCreatorString *p = scast( v0 );
    DaSi ds0 = RO_CALL( dasi, p->src );
    DaSi ds1 = RO_CALL( dasi, v1 );
    p->_insert( ds0.size, CmString( ds1.beg(), ds1.end() ) );
}

RiotBase_DEF_FUNC_2( self_set_b, 1, tv0->isa<PatchCreatorString>() and tv1->has_surdef_for( RF_dasi() ) ) {
    if ( reinterpret_cast<String &>( v0 ).size() )
        TODO;
    RO_CALL( self_add_b, v0, v1 );
}

RiotBase_DEF_FUNC_2( self_set_o, 1, tv0->isa<PatchCreatorString>() and tv1->has_surdef_for( RF_dasi() ) ) {
    RO_CALL( self_set_b, v0, v1 );
    RO_CALL( del, v1 );
}

RiotBase_DEF_FUNC_3( self_insert_bb, 1, tv0->isa<PatchCreatorString>() and tv1->has_surdef_for( RF_to_PT() ) and tv2->has_surdef_for( RF_dasi() ) ) {
    PT pos; if ( not RO_CALL( to_PT, v1, &pos, 1 ) ) return;
    PatchCreatorString *p = scast( v0 );
    DaSi str = RO_CALL( dasi, v2 );
    p->_insert( pos, CmString( str.beg(), str.end() ) );
}

RiotBase_DEF_FUNC_3( self_remove_bb, 1, tv0->isa<PatchCreatorString>() and tv1->has_surdef_for( RF_to_PT() ) and tv2->has_surdef_for( RF_to_PT() ) ) {
    PT pos; if ( not RO_CALL( to_PT, v1, &pos, 1 ) ) return;
    PT len; if ( not RO_CALL( to_PT, v2, &len, 1 ) ) return;
    PatchCreatorString *p = scast( v0 );
    p->_remove( pos, len );
}

}

// undo
void PatchCreatorString::undo_patch( op_insert, Undo u, PT pos, const CbString &str ) {
    apply_op( op_remove{}, *u.aod, pos, str.size() );
}

void PatchCreatorString::undo_patch( op_remove, Undo, PT pos, PT len ) {
    WARNING( "Undo of a basic remove should not happen" );
}

void PatchCreatorString::undo_patch( op_remove_und, Undo u, PT pos, const CbString &str ) {
    apply_op( op_insert{}, *u.aod, pos, str );
}

void PatchCreatorString::undo_patch( op_new_cursor, Undo u, DevId dev_id, PI64 loc_id, PT pos ) {
    apply_op( op_rem_cursor{}, *u.aod, dev_id, loc_id );
}

void PatchCreatorString::undo_patch( op_set_cursor, Undo, DevId dev_id, PI64 loc_id, PT pos ) {
    WARNING( "Undoing a non und version should not happen" );
}

void PatchCreatorString::undo_patch( op_set_cursor_und, Undo u, DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {
    apply_op( PatchCreatorString::op_set_cursor(), *u.aod, dev_id, loc_id, old_pos );
}

void PatchCreatorString::undo_patch( op_rem_cursor, Undo, DevId dev_id, PI64 loc_id ) {
    WARNING( "Undoing a non und version should not happen" );
}

void PatchCreatorString::undo_patch( op_rem_cursor_und, Undo u, DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr ) {
    apply_op( PatchCreatorString::op_new_cursor(), *u.aod, dev_id, loc_id, old_pos );
}

// transmission
void PatchCreatorString::transmission( op_remove_und, BBQ br, const FqId &id, PT pos, CbString &old ) {
    transmission( op_remove{}, br, id, pos, old.size() );
}

void PatchCreatorString::transmission( op_set_cursor_und, BBQ br, const FqId &id, DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {
    transmission( op_set_cursor{}, br, id, dev_id, loc_id, pos );
}

void PatchCreatorString::transmission( op_rem_cursor_und, BBQ br, const FqId &id, DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr ) {
    transmission( op_rem_cursor{}, br, id, dev_id, loc_id );
}

// register (from a transmission)
void PatchCreatorString::register_patch_op( op_remove, BBQ bq, PT pos, PT len ) {
    reg_op( bq, PatchCreatorString::op_remove_und(), pos, _substring( pos, len ) );
}

void PatchCreatorString::register_patch_op( op_set_cursor, BBQ bq, DevId dev_id, PI64 loc_id, PT pos ) {
    PT old_pos = 0;
    for( AutoPtr<PatchCreatorString::Cursor> &c : cursors ) {
        if ( c->dev_id == dev_id and c->loc_id == loc_id ) {
            old_pos = c->pos;
            break;
        }
    }
    reg_op( bq, PatchCreatorString::op_set_cursor_und(), dev_id, loc_id, pos, old_pos );
}

void PatchCreatorString::register_patch_op( op_rem_cursor, BBQ bq, DevId dev_id, PI64 loc_id ) {
    PT old_pos = 0;
    UsrId old_usr;
    for( AutoPtr<PatchCreatorString::Cursor> &c : cursors ) {
        if ( c->dev_id == dev_id and c->loc_id == loc_id ) {
            old_usr = c->usr_id;
            old_pos = c->pos;
            break;
        }
    }
    reg_op( bq, PatchCreatorString::op_rem_cursor_und(), dev_id, loc_id, old_pos, old_usr );
}

void PatchCreatorString::unk_new_or_new_unk( op_insert, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o ) {
    if ( p_o > p_n ) {
        // orig 01234
        // real 0123unk4    INS(p_o=4,d_o=unk)
        // imag 0new1234    INS(p_n=1,d_n=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        p_o += d_n.size();
    } else {
        // orig 01234
        // real 0123unk4    INS(p_o=4,d_o=unk)
        // imag 0new1234    INS(p_n=1,d_n=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        p_n += d_o.size();
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_insert, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, PT &l_o ) {
    if ( p_n >= p_o + l_o ) {
        // orig 012345
        // real 0345      REM(p_o=1,l_o=2)
        // imag 01234new5 INS(p_n=5,d_n=new)
        // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
        p_n -= l_o; // 3
    } else if ( p_n <= p_o ) {
        // orig 012345
        // real 0125      REM(p_o=3,l_o=2)
        // imag 0new12345 INS(p_n=1,d_n=new)
        // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
        p_o += d_n.size();
    } else {
        // orig 012345
        // real 05        REM(p_o=1,l_o=4)
        // imag 012new345 INS(p_n=3,d_n=new)
        // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,2 + REM 4,2)
        reg_op( bq_o, op_remove{}, p_o, p_n - p_o ); // 1, 2
        l_o -= p_n - p_o;  // 2
        p_n = p_o;         // 1
        p_o += d_n.size(); // 4
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_insert, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o ) {
    if ( p_n >= p_o + d_o.size() ) {
        // orig 012345
        // real 0345      REM(p_o=1,l_o=2)
        // imag 01234new5 INS(p_n=5,d_n=new)
        // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
        p_n -= d_o.size(); // 3
    } else if ( p_n <= p_o ) {
        // orig 012345
        // real 0125      REM(p_o=3,l_o=2)
        // imag 0new12345 INS(p_n=1,d_n=new)
        // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
        p_o += d_n.size();
    } else {
        // orig 012345
        // real 05        REM(p_o=1,l_o=4)
        // imag 012new345 INS(p_n=3,d_n=new)
        // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,'01' + REM 4,'34')
        reg_op( bq_o, op_remove_und{}, p_o, d_o.beg_upto( p_n - p_o ) ); // 1, '12'
        d_o.skip_some( p_n - p_o );  // '34'
        p_n = p_o;         // 1
        p_o += d_n.size(); // 4
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_insert, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( pos_o >= p_n )
        pos_o += d_n.size();
}

void PatchCreatorString::unk_new_or_new_unk( op_insert, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( pos_o >= p_n )
        pos_o += d_n.size();
}

void PatchCreatorString::unk_new_or_new_unk( op_insert, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
    if ( pos_o >= p_n )
        pos_o += d_n.size();
}

void PatchCreatorString::unk_new_or_new_unk( op_remove, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, CbString &d_o ) {
    if ( p_o <= p_n ) {
        // orig 01234567
        // real 01ab234567 INS(p_o=2,d_o=ab)
        // imag 012347     REM(p_n=5,l_n=2)
        // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
        p_n += d_o.size();
    } else if ( p_o >= p_n + l_n ) {
        // orig 01234567
        // real 01234ab567 INS(p_o=5,d_o=ab)
        // imag 014567     REM(p_n=2,l_n=2)
        // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
        p_o -= l_n;
    } else {
        // orig 01234567
        // real 0123ab4567 INS(p_o=4,d_o=ab)
        // imag 0167       REM(p_n=2,l_n=4)
        // obj  01ab67     (real -> obj = new: REM 2,2 + REM 4,2; imag -> obj = unk: INS 2,ab)
        reg_op( bq_n, op_remove_und{}, p_n, _substring( p_n, p_o - p_n ) ); // 2, 2
        PatchCreatorString::Aod aod_n; aod_n.as_usr = asu_n;
        apply_op( op_remove{}, aod_n, p_n, p_o - p_n );
        l_n -= p_o - p_n;  // 2
        p_o = p_n;         // 2
        p_n += d_o.size(); // 4
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_remove, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, PT &l_o ) {
    if ( p_o <= p_n ) {
        if ( p_o + l_o <= p_n ) {
            // orig 0123456789
            // real 0456789    REM(p_o=1,l_o=3)
            // imag 01234589   REM(p_n=6,l_n=2)
            // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM 1,3)
            p_n -= l_o; // 3
        } else if ( p_o + l_o <= p_n + l_n ) {
            // orig 0123456789
            // real 06789      REM(p_o=1,l_o=5)
            // imag 012389     REM(p_n=4,l_n=4)
            // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM 1,3)
            PT d = p_o + l_o - p_n; // 2
            l_n -= d;  // 2
            l_o -= d;  // 3
            p_n = p_o; // 1
        } else {
            // orig 0123456789
            // real 09         REM(p_o=1,l_o=8)
            // imag 01236789   REM(p_n=4,l_n=2)
            // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM 1,6)
            l_o -= l_n;
            l_n = 0;
        }
    } else {
        if ( p_n + l_n <= p_o ) {
            // orig 0123456789
            // real 01234589   REM(p_o=6,l_o=2)
            // imag 0456789    REM(p_n=1,l_n=3)
            // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM 3,2)
            p_o -= l_n; // 3
        } else if ( p_n + l_n <= p_o + l_o ) {
            // orig 0123456789
            // real 012389     REM(p_o=4,l_o=4)
            // imag 06789      REM(p_n=1,l_n=5)
            // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM 1,2)
            PT d = p_n + l_n - p_o; // 2
            l_o -= d;  // 2
            l_n -= d;  // 3
            p_o = p_n; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(p_o=4,l_o=2)
            // imag 09         REM(p_n=1,l_n=8)
            // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
            l_n -= l_o;
            l_o = 0;
        }
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_remove, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, CbString &d_o ) {
    if ( p_o <= p_n ) {
        if ( p_o + d_o.size() <= p_n ) {
            // orig 0123456789
            // real 0456789    REM(p_o=1,d_o='123')
            // imag 01234589   REM(p_n=6,l_n=2)
            // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM_UND 1,'123')
            p_n -= d_o.size(); // 3
        } else if ( p_o + d_o.size() <= p_n + l_n ) {
            // orig 0123456789
            // real 06789      REM_UND(p_o=1,d_o='12345')
            // imag 012389     REM(p_n=4,l_n=4)
            // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM_UND 1,'123')
            PT d = p_o + d_o.size() - p_n; // 2
            d_o = d_o.beg_upto( d_o.size() - d ); // '123'
            l_n -= d;  // 2
            p_n = p_o; // 1
        } else {
            // orig 0123456789
            // real 09         REM_UND(p_o=1,d_o='12345678')
            // imag 01236789   REM(p_n=4,l_n=2)
            // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM_UND 1,'123678')
            CbQueue t;
            t.write_some( d_o.beg_upto( p_n - p_o ) );
            t.write_some( d_o.end_from( p_n - p_o + l_n ) );
            d_o = std::move( t );
            l_n = 0;
        }
    } else {
        if ( p_n + l_n <= p_o ) {
            // orig 0123456789
            // real 01234589   REM_UND(p_o=6,d_o='67')
            // imag 0456789    REM(p_n=1,l_n=3)
            // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 3,'67')
            p_o -= l_n; // 3
        } else if ( p_n + l_n <= p_o + d_o.size() ) {
            // orig 0123456789
            // real 012389     REM_UND(p_o=4,d_o='4567')
            // imag 06789      REM(p_n=1,l_n=5)
            // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 1,'67')
            PT d = p_n + l_n - p_o; // 2
            d_o.skip_some( d ); // '67'
            l_n -= d;  // 3
            p_o = p_n; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(p_o=4,d_o.size()=2)
            // imag 09         REM(p_n=1,l_n=8)
            // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
            l_n -= d_o.size();
            d_o.free();
        }
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_remove, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( pos_o >= p_n + l_n )
        pos_o -= l_n;
    else if ( pos_o > p_n )
        pos_o = p_n;
}

void PatchCreatorString::unk_new_or_new_unk( op_remove, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( pos_o >= p_n + l_n )
        pos_o -= l_n;
    else if ( pos_o > p_n )
        pos_o = p_n;
}

void PatchCreatorString::unk_new_or_new_unk( op_remove, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o , PT &old_pos_o ) {
    if ( pos_o >= p_n + l_n )
        pos_o -= l_n;
    else if ( pos_o > p_n )
        pos_o = p_n;
}

void PatchCreatorString::unk_new_or_new_unk( op_remove_und, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o ) {
    if ( p_o <= p_n ) {
        // orig 01234567
        // real 01ab234567 INS(p_o=2,d_o=ab)
        // imag 012347     REM(p_n=5,l_n=2)
        // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
        p_n += d_o.size();
    } else if ( p_o >= p_n + d_n.size() ) {
        // orig 01234567
        // real 01234ab567 INS(p_o=5,d_o=ab)
        // imag 014567     REM(p_n=2,l_n=2)
        // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
        p_o -= d_n.size();
    } else {
        // orig 01234567
        // real 0123ab4567 INS(p_1=4,d_1=ab)
        // imag 0167       REM_UND(p_0=2,d_0=2345)
        // obj  01ab67     (real -> obj = unk: REM_UND 2,23 + REM_UND 4,45; imag -> obj = new: INS 2,ab)
        reg_op( bq_n, op_remove_und{}, p_n, CbString( d_n, 0, p_o - p_n ) ); // 2, 23
        PatchCreatorString::Aod aod_n; aod_n.as_usr = asu_n;
        apply_op( op_remove(), aod_n, p_n, p_o - p_n );
        d_n.skip_some( p_o - p_n );  // 45
        p_o = p_n;                   // 2
        p_n += d_o.size();           // 4
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_remove_und, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, PT &l_o ) {
    if ( p_o <= p_n ) {
        if ( p_o + l_o <= p_n ) {
            // orig 0123456789
            // real 0456789    REM(p_o=1,l_o=3)
            // imag 01234589   REM_UND(p_n=6,l_n='67')
            // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = _n: REM 1,3)
            p_n -= l_o; // 3
        } else if ( p_o + l_o <= p_n + d_n.size() ) {
            // orig 0123456789
            // real 06789      REM(p_o=1,l_o=5)
            // imag 012389     REM_UND(p_n=4,d_n='4567')
            // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
            PT d = p_o + l_o - p_n; // 2
            l_o -= d; // '123'
            d_n.skip_some( d );  // '67'
            p_n = p_o; // 1
        } else {
            // orig 0123456789
            // real 09         REM(p_o=1,l_o=8)
            // imag 01236789   REM_UND(p_n=4,d_n='45')
            // obj  09         (real -> obj = _n: VOID; imag -> obj = _n: REM 1,'123678')
            l_o -= d_n.size();
            d_n.free();
        }
    } else {
        if ( p_n + d_n.size() <= p_o ) {
            // orig 0123456789
            // real 01234589   REM(p_o=6,l_o=2)
            // imag 0456789    REM_UND(p_n=1,d_n='123')
            // obj  04589      (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 3,2)
            p_o -= d_n.size(); // 3
        } else if ( p_n + d_n.size() <= p_o + l_o ) {
            // orig 0123456789
            // real 012389     REM(p_o=4,l_o=4)
            // imag 06789      REM_UND(p_n=1,d_n='12345')
            // obj  089        (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 1,2)
            PT d = p_n + d_n.size() - p_o; // 2
            l_o -= d;  // 2
            d_n = d_n.beg_upto( d_n.size() - d );  // '123'
            p_o = p_n; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(p_o=4,l_o=2)
            // imag 09         REM_UND(p_n=1,d_n='12345678')
            // obj  09         (real -> obj = _n: REM 1,'123678'; imag -> obj = _n: VOID)
            CbQueue t;
            t.write_some( d_n.beg_upto( p_o - p_n ) );
            t.write_some( d_n.end_from( p_o - p_n + l_o ) );
            d_n = std::move( t );
            l_o = 0;
        }
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_remove_und, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o ) {
    if ( p_o <= p_n ) {
        if ( p_o + d_o.size() <= p_n ) {
            // orig 0123456789
            // real 0456789    REM(p_o=1,d_o='123')
            // imag 01234589   REM(p_n=6,d_n='67')
            // obj  04589      (real -> obj = _n: REM_UND 3,2; imag -> obj = _o: REM_UND 1,'123')
            p_n -= d_o.size(); // 3
        } else if ( p_o + d_o.size() <= p_n + d_n.size() ) {
            // orig 0123456789
            // real 06789      REM_UND(p_o=1,d_o='12345')
            // imag 012389     REM(p_n=4,d_n='4567')
            // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
            PT d = p_o + d_o.size() - p_n; // 2
            d_o = d_o.beg_upto( d_o.size() - d ); // '123'
            d_n.skip_some( d );  // '67'
            p_n = p_o; // 1
        } else {
            // orig 0123456789
            // real 09         REM_UND(p_o=1,d_o='12345678')
            // imag 01236789   REM(p_n=4,d_n.size()=2)
            // obj  09         (real -> obj = _n: VOID; imag -> obj = _o: REM_UND 1,'123678')
            CbQueue t;
            t.write_some( d_o.beg_upto( p_n - p_o ) );
            t.write_some( d_o.end_from( p_n - p_o + d_n.size() ) );
            d_o = std::move( t );
            d_n.free();
        }
    } else {
        if ( p_n + d_n.size() <= p_o ) {
            // orig 0123456789
            // real 01234589   REM_UND(p_o=6,d_o='67')
            // imag 0456789    REM(p_n=1,d_n.size()='123')
            // obj  04589      (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 3,'67')
            p_o -= d_n.size(); // 3
        } else if ( p_n + d_n.size() <= p_o + d_o.size() ) {
            // orig 0123456789
            // real 012389     REM_UND(p_o=4,d_o='4567')
            // imag 06789      REM(p_n=1,d_n='12345')
            // obj  089        (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 1,'67')
            PT d = p_n + d_n.size() - p_o; // 2
            d_o.skip_some( d ); // '67'
            d_n = d_n.beg_upto( d_n.size() - d ); // '123'
            p_o = p_n; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(p_o=4,d_o='45')
            // imag 09         REM(p_n=1,d_n='12345678')
            // obj  09         (real -> obj = _n: REM_UND 1,'123678'; imag -> obj = _o: VOID)
            CbQueue t;
            t.write_some( d_n.beg_upto( p_o - p_n ) );
            t.write_some( d_n.end_from( p_o - p_n + d_o.size() ) );
            d_n = std::move( t );
            d_o.free();
        }
    }
}

void PatchCreatorString::unk_new_or_new_unk( op_remove_und, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( pos_o >= p_n + d_n.size() )
        pos_o -= d_n.size();
    else if ( pos_o > p_n )
        pos_o = p_n;
}

void PatchCreatorString::unk_new_or_new_unk( op_remove_und, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( pos_o >= p_n + d_n.size() )
        pos_o -= d_n.size();
    else if ( pos_o > p_n )
        pos_o = p_n;
}

void PatchCreatorString::unk_new_or_new_unk( op_remove_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
    if ( pos_o >= p_n + d_n.size() )
        pos_o -= d_n.size();
    else if ( pos_o > p_n )
        pos_o = p_n;
}

void PatchCreatorString::unk_new_or_new_unk( op_new_cursor, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
    if ( pos_n >= p_o )
        pos_n += d_o.size();
}

void PatchCreatorString::unk_new_or_new_unk( op_new_cursor, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, PT &l_o ) {
    if ( pos_n >= p_o + l_o )
        pos_n -= l_o;
    else if ( pos_n > p_o )
        pos_n = p_o;
}

void PatchCreatorString::unk_new_or_new_unk( op_new_cursor, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
    if ( pos_n >= p_o + d_o.size() )
        pos_n -= d_o.size();
    else if ( pos_n > p_o )
        pos_n = p_o;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
    if ( pos_n >= p_o )
        pos_n += d_o.size();
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, PT &l_o ) {
    if ( pos_n >= p_o + l_o )
        pos_n -= l_o;
    else if ( pos_n > p_o )
        pos_n = p_o;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
    if ( pos_n >= p_o + d_o.size() )
        pos_n -= d_o.size();
    else if ( pos_n > p_o )
        pos_n = p_o;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    // it is impossible to have the same ids
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
        loc_id_n = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
        loc_id_n = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, CbString &d_o ) {
    if ( pos_n >= p_o )
        pos_n += d_o.size();
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, PT &l_o ) {
    if ( pos_n >= p_o + l_o )
        pos_n -= l_o;
    else if ( pos_n > p_o )
        pos_n = p_o;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, CbString &d_o ) {
    if ( pos_n >= p_o + d_o.size() )
        pos_n -= d_o.size();
    else if ( pos_n > p_o )
        pos_n = p_o;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    // it is impossible to have the same ids
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
        loc_id_n = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_set_cursor_und, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
        loc_id_n = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor_und, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor_und, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor_und, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}

void PatchCreatorString::unk_new_or_new_unk( op_rem_cursor_und, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
    if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
        loc_id_o = 0;
}


//
void PatchCreatorString::unk_std_comb( op_insert, op_insert, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, CbString &d_u ) {
    if ( p_u > p_s ) {
        //  orig 012
        //  +unk 01a2  INS p_u=2,d_u=a
        //  +std 0b1a2 INS p_s=1,d_s=b
        // same as
        //  +std 0b12  INS p_s=1,d_s=b
        //  +unk 0b1a2 INS p_u=3,d_u=a
        p_u += d_s.size();
    } else {
        //  orig 012
        //  +unk 0a12  INS p_u=1,d_u=a
        //  +std 0a1b2 INS p_s=3,d_s=b
        // same as
        //  +std 01b2  INS p_s=2,d_s=b
        //  +unk 0a1b2 INS p_u=1,d_u=a
        p_s -= d_u.size();
    }
}
void PatchCreatorString::unk_std_comb( op_insert, op_remove, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, PT &l_u ) {
    if ( p_s > p_u ) {
        //  orig 012345678
        //  +unk 045678     REM p_u=1,l_u=3
        //  +std 04b5678    INS p_s=2,d_s=b
        // same as
        //  +std 01234b5678 INS p_s=5,d_s=b
        //  +unk 04b5678    REM p_u=3,l_u=a
        p_s += l_u;
    } else {
        //  orig 012345678
        //  +unk 012348     REM p_u=5,l_u=3
        //  +std 01b2348    INS p_s=2,d_s=b
        // same as
        //  +std 01b2345678 INS p_s=5,d_s=b
        //  +unk 01b2348    REM p_u=6,l_u=a
        p_u += d_s.size();
    }
}
void PatchCreatorString::unk_std_comb( op_insert, op_remove_und, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, CbString &d_u ) {
    if ( p_s > p_u ) {
        //  orig 012345678
        //  +unk 045678     REM p_u=1,l_u=3
        //  +std 04b5678    INS p_s=2,d_s=b
        // same as
        //  +std 01234b5678 INS p_s=5,d_s=b
        //  +unk 04b5678    REM p_u=3,l_u=a
        p_s += d_u.size();
    } else {
        //  orig 012345678
        //  +unk 012348     REM p_u=5,l_u=3
        //  +std 01b2348    INS p_s=2,d_s=b
        // same as
        //  +std 01b2345678 INS p_s=5,d_s=b
        //  +unk 01b2348    REM p_u=6,l_u=a
        p_u += d_s.size();
    }
}
void PatchCreatorString::unk_std_comb( op_insert, op_new_cursor, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( pos_u >= p_s ) {
        //  orig 012
        //  +unk 01|2  NEW_CURSOR pos=2
        //  +std 0b1|2 INS p_s=1,d_s=b
        // same as
        //  +std 0b12  INS p_s=1,d_s=b
        //  +unk 0b1|2 NEW_CURSOR pos=3
        pos_u += d_s.size();
    }
}
void PatchCreatorString::unk_std_comb( op_insert, op_set_cursor, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( pos_u >= p_s ) {
        //  orig 012
        //  +unk 01|2  NEW_CURSOR pos=2
        //  +std 0b1|2 INS p_s=1,d_s=b
        // same as
        //  +std 0b12  INS p_s=1,d_s=b
        //  +unk 0b1|2 NEW_CURSOR pos=3
        pos_u += d_s.size();
    }
}
void PatchCreatorString::unk_std_comb( op_insert, op_set_cursor_und, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u ) {
    if ( pos_u >= p_s ) {
        //  orig 012
        //  +unk 01|2  NEW_CURSOR pos=2
        //  +std 0b1|2 INS p_s=1,d_s=b
        // same as
        //  +std 0b12  INS p_s=1,d_s=b
        //  +unk 0b1|2 NEW_CURSOR pos=3
        pos_u += d_s.size();
    }
}

void PatchCreatorString::unk_std_comb( op_remove, op_insert, UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, PT &p_u, CbString &d_u ) {
    if ( p_u > p_s ) {
        //  orig 012345678
        //  +unk 0123456b78 INS p_u=7,d_u=b
        //  +std 0456b78    REM p_s=1,l_s=3
        // same as
        //  +std 045678     REM p_s=1,l_s=3
        //  +unk 0456b78    INS p_u=4,d_u=b
        p_u -= l_s;
    } else {
        //  orig 012345678
        //  +unk 01b2345678 INS p_u=2,d_u=b
        //  +std 01b2348    REM p_s=6,l_s=3
        // same as
        //  +std 012348     REM p_s=5,l_s=3
        //  +unk 01b2348    INS p_u=2,d_u=b
        p_s -= d_u.size();
    }
}
void PatchCreatorString::unk_std_comb( op_remove, op_remove, UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, PT &p_u, PT &l_u ) {
    if ( p_s >= p_u ) {
        //  orig 012345678
        //  +unk 0345678    REM p_u=1,l_u=2
        //  +std 03478      REM p_s=3,l_s=2
        // same as
        //  +std 0123478    REM p_s=5,l_s=3
        //  +unk 03478      REM p_u=1,l_u=2
        p_s += l_u;
    } else if ( p_s + l_s <= p_u ) {
        //  orig 012345678
        //  +unk 0123458    REM p_u=6,l_u=2
        //  +std 03458      REM p_s=1,l_s=2
        // same as
        //  +std 0345678    REM p_s=1,l_s=2
        //  +unk 03458      REM p_u=4,l_u=2
        p_u -= l_s;
    } else {
        //  orig 012345678
        //  +unk 0123678    REM p_u=4,l_u=2
        //  +std 08         REM p_s=1,l_s=5
        // same as
        //  +std 0678       REM p_s=1,l_s=5
        //  +unk 08         REM p_u=1,l_u=2
        p_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove, op_remove_und, UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, PT &p_u, CbString &d_u ) {
    if ( p_s >= p_u ) {
        //  orig 012345678
        //  +unk 0345678    REM p_u=1,l_u=2
        //  +std 03478      REM p_s=3,l_s=2
        // same as
        //  +std 0123478    REM p_s=5,l_s=3
        //  +unk 03478      REM p_u=1,l_u=2
        p_s += d_u.size();
    } else if ( p_s + l_s <= p_u ) {
        //  orig 012345678
        //  +unk 0123458    REM p_u=6,l_u=2
        //  +std 03458      REM p_s=1,l_s=2
        // same as
        //  +std 0345678    REM p_s=1,l_s=2
        //  +unk 03458      REM p_u=4,l_u=2
        p_u -= l_s;
    } else {
        //  orig 012345678
        //  +unk 0123678    REM p_u=4,l_u=2
        //  +std 08         REM p_s=1,l_s=5
        // same as
        //  +std 0678       REM p_s=1,l_s=5
        //  +unk 08         REM p_u=1,l_u=2
        p_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove, op_new_cursor, UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( pos_u >= p_s + l_s ) {
        //  orig 012345
        //  +unk 01234|5  NEW_CURSOR pos=5
        //  +std 014|5    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 014|5    NEW_CURSOR pos=3
        pos_u -= l_s;
    } else if ( pos_u > p_s ) {
        //  orig 012345
        //  +unk 012|345  NEW_CURSOR pos=3
        //  +std 01|45    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 01|45    NEW_CURSOR pos=2
        pos_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove, op_set_cursor, UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( pos_u >= p_s + l_s ) {
        //  orig 012345
        //  +unk 01234|5  NEW_CURSOR pos=5
        //  +std 014|5    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 014|5    NEW_CURSOR pos=3
        pos_u -= l_s;
    } else if ( pos_u > p_s ) {
        //  orig 012345
        //  +unk 012|345  NEW_CURSOR pos=3
        //  +std 01|45    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 01|45    NEW_CURSOR pos=2
        pos_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove, op_set_cursor_und, UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u ) {
    if ( pos_u >= p_s + l_s ) {
        //  orig 012345
        //  +unk 01234|5  SET_CURSOR pos=5
        //  +std 014|5    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 014|5    NEW_CURSOR pos=3
        pos_u -= l_s;
    } else if ( pos_u > p_s ) {
        //  orig 012345
        //  +unk 012|345  SET_CURSOR pos=3
        //  +std 01|45    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 01|45    NEW_CURSOR pos=2
        pos_u = p_s;
    }
}

void PatchCreatorString::unk_std_comb( op_remove_und, op_insert, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, CbString &d_u ) {
    if ( p_u > p_s ) {
        //  orig 012345678
        //  +unk 0123456b78 INS p_u=7,d_u=b
        //  +std 0456b78    REM p_s=1,l_s=3
        // same as
        //  +std 045678     REM p_s=1,l_s=3
        //  +unk 0456b78    INS p_u=4,d_u=b
        p_u -= d_s.size();
    } else {
        //  orig 012345678
        //  +unk 01b2345678 INS p_u=2,d_u=b
        //  +std 01b2348    REM p_s=6,l_s=3
        // same as
        //  +std 012348     REM p_s=5,l_s=3
        //  +unk 01b2348    INS p_u=2,d_u=b
        p_s -= d_u.size();
    }
}
void PatchCreatorString::unk_std_comb( op_remove_und, op_remove, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, PT &l_u ) {
    if ( p_s >= p_u ) {
        //  orig 012345678
        //  +unk 0345678    REM p_u=1,l_u=2
        //  +std 03478      REM p_s=3,l_s=2
        // same as
        //  +std 0123478    REM p_s=5,l_s=3
        //  +unk 03478      REM p_u=1,l_u=2
        p_s += l_u;
    } else if ( p_s + d_s.size() <= p_u ) {
        //  orig 012345678
        //  +unk 0123458    REM p_u=6,l_u=2
        //  +std 03458      REM p_s=1,l_s=2
        // same as
        //  +std 0345678    REM p_s=1,l_s=2
        //  +unk 03458      REM p_u=4,l_u=2
        p_u -= d_s.size();
    } else {
        //  orig 012345678
        //  +unk 0123678    REM p_u=4,l_u=2
        //  +std 08         REM p_s=1,l_s=5
        // same as
        //  +std 0678       REM p_s=1,l_s=5
        //  +unk 08         REM p_u=1,l_u=2
        p_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove_und, op_remove_und, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, CbString &d_u ) {
    if ( p_s >= p_u ) {
        //  orig 012345678
        //  +unk 0345678    REM p_u=1,l_u=2
        //  +std 03478      REM p_s=3,l_s=2
        // same as
        //  +std 0123478    REM p_s=5,l_s=3
        //  +unk 03478      REM p_u=1,l_u=2
        p_s += d_u.size();
    } else if ( p_s + d_s.size() <= p_u ) {
        //  orig 012345678
        //  +unk 0123458    REM p_u=6,l_u=2
        //  +std 03458      REM p_s=1,l_s=2
        // same as
        //  +std 0345678    REM p_s=1,l_s=2
        //  +unk 03458      REM p_u=4,l_u=2
        p_u -= d_s.size();
    } else {
        //  orig 012345678
        //  +unk 0123678    REM p_u=4,l_u=2
        //  +std 08         REM p_s=1,l_s=5
        // same as
        //  +std 0678       REM p_s=1,l_s=5
        //  +unk 08         REM p_u=1,l_u=2
        p_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove_und, op_new_cursor, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( pos_u >= p_s + d_s.size() ) {
        //  orig 012345
        //  +unk 01234|5  NEW_CURSOR pos=5
        //  +std 014|5    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 014|5    NEW_CURSOR pos=3
        pos_u -= d_s.size();
    } else if ( pos_u > p_s ) {
        //  orig 012345
        //  +unk 012|345  NEW_CURSOR pos=3
        //  +std 01|45    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 01|45    NEW_CURSOR pos=2
        pos_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove_und, op_set_cursor, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( pos_u >= p_s + d_s.size() ) {
        //  orig 012345
        //  +unk 01234|5  NEW_CURSOR pos=5
        //  +std 014|5    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 014|5    NEW_CURSOR pos=3
        pos_u -= d_s.size();
    } else if ( pos_u > p_s ) {
        //  orig 012345
        //  +unk 012|345  NEW_CURSOR pos=3
        //  +std 01|45    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 01|45    NEW_CURSOR pos=2
        pos_u = p_s;
    }
}
void PatchCreatorString::unk_std_comb( op_remove_und, op_set_cursor_und, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u ) {
    if ( pos_u >= p_s + d_s.size() ) {
        //  orig 012345
        //  +unk 01234|5  SET_CURSOR pos=5
        //  +std 014|5    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 014|5    NEW_CURSOR pos=3
        pos_u -= d_s.size();
    } else if ( pos_u > p_s ) {
        //  orig 012345
        //  +unk 012|345  SET_CURSOR pos=3
        //  +std 01|45    REM p_s=2,l_s=2
        // same as
        //  +std 0145     REM p_s=2,l_s=2
        //  +unk 01|45    NEW_CURSOR pos=2
        pos_u = p_s;
    }
}

void PatchCreatorString::unk_std_comb( op_set_cursor, op_new_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
     if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
         loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_set_cursor, op_set_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_set_cursor, op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}

void PatchCreatorString::unk_std_comb( op_set_cursor_und, op_new_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, PT &old_pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_set_cursor_und, op_set_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, PT &old_pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_set_cursor_und, op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, PT &old_pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}

void PatchCreatorString::unk_std_comb( op_rem_cursor, op_new_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_rem_cursor, op_set_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_rem_cursor, op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}

void PatchCreatorString::unk_std_comb( op_rem_cursor_und, op_new_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &old_pos_s, UsrId usr_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_rem_cursor_und, op_set_cursor, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &old_pos_s, UsrId usr_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}
void PatchCreatorString::unk_std_comb( op_rem_cursor_und, op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &old_pos_s, UsrId usr_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u ) {
    if ( dev_id_s == dev_id_u and loc_id_s == loc_id_u )
        loc_id_u = 0;
}


} // namespace RiotBase
