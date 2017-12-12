#include "../../System/AutoPtr.h"
#include "PatchCreatorString.h"
#include "PatchMergerString.h"
#include "../Number/Bn.h"
#include "GenString.h"

namespace RiotBase {

namespace {

/// what to do with each data from unk and a new insertion (p_0, d_0)
struct UnkNewPatchVisitor_insertion : PatchCreatorString::Visitor {
    UnkNewPatchVisitor_insertion( PT &p_0, CbString &d_0, BinStream<CbQueue> tmp_bq_unk ) : p_0( p_0 ), d_0( d_0 ), tmp_bq_unk( tmp_bq_unk ) {
    }
    void insertion( PT &p_1, CbString &d_1 ) { // INS(unk)/INS(new)
        if ( p_1 > p_0 ) {
            // orig 01234
            // real 0123unk4    INS(p_1=4,d_1=unk)
            // imag 0new1234    INS(p_0=1,d_0=new)
            // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
            p_1 += d_0.size();
        } else {
            // orig 01234
            // real 0123unk4    INS(p_1=4,d_1=unk)
            // imag 0new1234    INS(p_0=1,d_0=new)
            // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
            p_0 += d_1.size();
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(unk)/INS(new)
        if ( p_0 >= p_1 + l_1 ) {
            // orig 012345
            // real 0345      REM(p_1=1,l_1=2)
            // imag 01234new5 INS(p_0=5,d_0=new)
            // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
            p_0 -= l_1; // 3
        } else if ( p_0 <= p_1 ) {
            // orig 012345
            // real 0125      REM(p_1=3,l_1=2)
            // imag 0new12345 INS(p_0=1,d_0=new)
            // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
            p_1 += d_0.size();
        } else {
            // orig 012345
            // real 05        REM(p_1=1,l_1=4)
            // imag 012new345 INS(p_0=3,d_0=new)
            // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,2 + REM 4,2)
            tmp_bq_unk << PatchCreatorString::REMOVAL << p_1 << p_0 - p_1; // 1, 2
            l_1 -= p_0 - p_1;  // 2
            p_0 = p_1;         // 1
            p_1 += d_0.size(); // 4
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) { // NEW_CURSOR(unk)/INS(new)
        if ( pos >= p_0 )
            pos += d_0.size();
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 )
            pos += d_0.size();
    }
    void removal_und( PT &p_1, CbString &d_1 ) {
        if ( p_0 >= p_1 + d_1.size() ) {
            // orig 012345
            // real 0345      REM(p_1=1,l_1=2)
            // imag 01234new5 INS(p_0=5,d_0=new)
            // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
            p_0 -= d_1.size(); // 3
        } else if ( p_0 <= p_1 ) {
            // orig 012345
            // real 0125      REM(p_1=3,l_1=2)
            // imag 0new12345 INS(p_0=1,d_0=new)
            // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
            p_1 += d_0.size();
        } else {
            // orig 012345
            // real 05        REM(p_1=1,l_1=4)
            // imag 012new345 INS(p_0=3,d_0=new)
            // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,'01' + REM 4,'34')
            tmp_bq_unk << PatchCreatorString::REMOVAL_UND << p_1 << d_1.beg_upto( p_0 - p_1 ); // 1, '12'
            d_1.skip_some( p_0 - p_1 );  // '34'
            p_0 = p_1;         // 1
            p_1 += d_0.size(); // 4
        }
    }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) {
        set_cursor( dev_id, loc_id, pos );
    }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {
        rem_cursor( dev_id, loc_id );
    }
    PT                 &p_0;
    CbString           &d_0;
    BinStream<CbQueue>  tmp_bq_unk;
};

/// what to do with each data from unk and a new removal (p_0, l_0)
struct UnkNewPatchVisitor_removal : PatchCreatorString::Visitor {
    UnkNewPatchVisitor_removal( PatchCreatorString *pcs, PT &p_0, PT &l_0, BinStream<CbQueue> tmp_bq_new ) : pcs( pcs ), p_0( p_0 ), l_0( l_0 ), tmp_bq_new( tmp_bq_new) {
    }
    void insertion( PT &p_1, CbString &d_1 ) { // INS(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            // orig 01234567
            // real 01ab234567 INS(p_1=2,d_1=ab)
            // imag 012347     REM(p_0=5,l_0=2)
            // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
            p_0 += d_1.size();
        } else if ( p_1 >= p_0 + l_0 ) {
            // orig 01234567
            // real 01234ab567 INS(p_1=5,d_1=ab)
            // imag 014567     REM(p_0=2,l_0=2)
            // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
            p_1 -= l_0;
        } else {
            // orig 01234567
            // real 0123ab4567 INS(p_1=4,d_1=ab)
            // imag 0167       REM(p_0=2,l_0=4)
            // obj  01ab67     (real -> obj = new: REM 2,2 + REM 4,2; imag -> obj = unk: INS 2,ab)
            tmp_bq_new << PatchCreatorString::REMOVAL_UND << p_0 << pcs->_substring( p_0, p_1 - p_0 ); // 2, 2
            pcs->_remove_wo_cr( p_0, p_1 - p_0 );
            l_0 -= p_1 - p_0;  // 2
            p_1 = p_0;         // 2
            p_0 += d_1.size(); // 4
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            if ( p_1 + l_1 <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM(p_1=1,l_1=3)
                // imag 01234589   REM(p_0=6,l_0=2)
                // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM 1,3)
                p_0 -= l_1; // 3
            } else if ( p_1 + l_1 <= p_0 + l_0 ) {
                // orig 0123456789
                // real 06789      REM(p_1=1,l_1=5)
                // imag 012389     REM(p_0=4,l_0=4)
                // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM 1,3)
                PT d = p_1 + l_1 - p_0; // 2
                l_0 -= d;  // 2
                l_1 -= d;  // 3
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM(p_1=1,l_1=8)
                // imag 01236789   REM(p_0=4,l_0=2)
                // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM 1,6)
                l_1 -= l_0;
                l_0 = 0;
            }
        } else {
            if ( p_0 + l_0 <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM(p_1=6,l_1=2)
                // imag 0456789    REM(p_0=1,l_0=3)
                // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM 3,2)
                p_1 -= l_0; // 3
            } else if ( p_0 + l_0 <= p_1 + l_1 ) {
                // orig 0123456789
                // real 012389     REM(p_1=4,l_1=4)
                // imag 06789      REM(p_0=1,l_0=5)
                // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM 1,2)
                PT d = p_0 + l_0 - p_1; // 2
                l_1 -= d;  // 2
                l_0 -= d;  // 3
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 01236789   REM(p_1=4,l_1=2)
                // imag 09         REM(p_0=1,l_0=8)
                // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
                l_0 -= l_1;
                l_1 = 0;
            }
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + l_0 )
            pos -= l_0;
        else if ( pos > p_0 )
            pos = p_0;
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + l_0 )
            pos -= l_0;
        else if ( pos > p_0 )
            pos = p_0;
    }
    void removal_und( PT &p_1, CbString &d_1 ) {
        if ( p_1 <= p_0 ) {
            if ( p_1 + d_1.size() <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM(p_1=1,d_1='123')
                // imag 01234589   REM(p_0=6,l_0=2)
                // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM_UND 1,'123')
                p_0 -= d_1.size(); // 3
            } else if ( p_1 + d_1.size() <= p_0 + l_0 ) {
                // orig 0123456789
                // real 06789      REM_UND(p_1=1,d_1='12345')
                // imag 012389     REM(p_0=4,l_0=4)
                // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM_UND 1,'123')
                PT d = p_1 + d_1.size() - p_0; // 2
                d_1 = d_1.beg_upto( d_1.size() - d ); // '123'
                l_0 -= d;  // 2
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM_UND(p_1=1,d_1='12345678')
                // imag 01236789   REM(p_0=4,l_0=2)
                // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM_UND 1,'123678')
                CbQueue t;
                t.write_some( d_1.beg_upto( p_0 - p_1 ) );
                t.write_some( d_1.end_from( p_0 - p_1 + l_0 ) );
                d_1 = std::move( t );
                l_0 = 0;
            }
        } else {
            if ( p_0 + l_0 <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM_UND(p_1=6,d_1='67')
                // imag 0456789    REM(p_0=1,l_0=3)
                // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 3,'67')
                p_1 -= l_0; // 3
            } else if ( p_0 + l_0 <= p_1 + d_1.size() ) {
                // orig 0123456789
                // real 012389     REM_UND(p_1=4,d_1='4567')
                // imag 06789      REM(p_0=1,l_0=5)
                // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 1,'67')
                PT d = p_0 + l_0 - p_1; // 2
                d_1.skip_some( d ); // '67'
                l_0 -= d;  // 3
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 01236789   REM(p_1=4,d_1.size()=2)
                // imag 09         REM(p_0=1,l_0=8)
                // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
                l_0 -= d_1.size();
                d_1.free();
            }
        }
    }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) {
        set_cursor( dev_id, loc_id, pos );
    }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {
        rem_cursor( dev_id, loc_id );
    }
    PatchCreatorString *pcs;
    PT                 &p_0;
    PT                 &l_0;
    BinStream<CbQueue> tmp_bq_new;
};

/// what to do with each data from unk and a new new_cursor (pos)
struct UnkNewPatchVisitor_new_cursor : PatchCreatorString::Visitor {
    UnkNewPatchVisitor_new_cursor( PT &pos ) : pos( pos ) { }
    void insertion     ( PT p_1, const CbString &d_1 ) { if ( pos >= p_1 ) pos += d_1.size(); }
    void removal       ( PT p_1, PT l_1 ) { if ( pos >= p_1 + l_1 ) pos -= l_1; else if ( pos > p_1 ) pos = p_1; }
    void removal_und   ( PT p, const CbString &d ) { removal( p, d.size() ); }
    void set_cursor_und( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    PT  &pos;
};

/// what to do with each data from unk and a new set_cursor
struct UnkNewPatchVisitor_set_cursor : PatchCreatorString::Visitor {
    UnkNewPatchVisitor_set_cursor( DevId &dev_id_new, PI64 &loc_id_new, PT &pos_new ) : dev_id_new( dev_id_new ), loc_id_new( loc_id_new ), pos_new( pos_new ) {}
    void insertion( PT p_1, const CbString &d_1 ) { if ( pos_new >= p_1 ) pos_new += d_1.size(); }
    void removal( PT p_1, PT l_1 ) { if ( pos_new >= p_1 + l_1 ) pos_new -= l_1; else if ( pos_new > p_1 ) pos_new = p_1; }
    void set_cursor( DevId dev_id, PI64 &loc_id, PT pos ) {  if ( dev_id == dev_id_new and loc_id == loc_id_new ) loc_id = 0; } // the position is defined by the new patch so we can delete the set_cursor from unk
    void rem_cursor( DevId dev_id, PI64 loc_id ) {  if ( dev_id == dev_id_new and loc_id == loc_id_new ) loc_id_new = 0; } // cursor has been deleted in an unk patch -> do not keep it in the new patch
    void removal_und    ( PT p, const CbString &d ) { PT l = d.size(); removal( p, l ); }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId              &dev_id_new;
    PI64               &loc_id_new;
    PT                 &pos_new;
};

/// what to do with each data from unk and a new rem_cursor
struct UnkNewPatchVisitor_rem_cursor : PatchCreatorString::Visitor {
    UnkNewPatchVisitor_rem_cursor( DevId &dev_id_new, PI64 &loc_id_new ) : dev_id_new( dev_id_new ), loc_id_new( loc_id_new ) {}
    void   new_cursor    ( DevId dev_id, PI64 &loc_id, PT pos ) { if ( dev_id == dev_id_new and loc_id == loc_id_new ) loc_id = 0; }
    void   set_cursor    ( DevId dev_id, PI64 &loc_id, PT pos ) { if ( dev_id == dev_id_new and loc_id == loc_id_new ) loc_id = 0; }
    void   rem_cursor    ( DevId dev_id, PI64 loc_id ) { if ( dev_id == dev_id_new and loc_id == loc_id_new ) loc_id = 0; } // should not happen
    void   set_cursor_und( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void   rem_cursor_und( DevId dev_id, PI64 &loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId &dev_id_new;
    PI64  &loc_id_new;
};


/// for each item in new patches...
/// data contain fully specified dev_id and usr_id (operations_visitor will transform some dev_id and usr_id to 0 if equal to cur_dev or cur_usr)
struct NewPatchVisitor : PatchCreatorString::NewPatchVisitor {
    using PatchCreatorString::NewPatchVisitor::NewPatchVisitor;

    void insertion( PT &p_0, CbString &d_0 ) {
        // update cq_unk, p_0 and d_0
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkNewPatchVisitor_insertion upv{ p_0, d_0, &pms->cq_unk };
        PatchCreatorString::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_insert_at( p_0, as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( d_0.size() ) {
            tmp_bq_new << PatchCreatorString::INSERTION << p_0 << d_0;
            pcs->_insert_wo_cr( p_0, d_0 );
        }
    }
    void removal( PT &p_0, PT &l_0 ) {
        // modifications
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkNewPatchVisitor_removal upv{ pcs, p_0, l_0, tmp_bq_new };
        PatchCreatorString::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_remove( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( l_0 ) {
            tmp_bq_new << PatchCreatorString::REMOVAL_UND << p_0 << pcs->_substring( p_0, l_0 );
            pcs->_remove_wo_cr( p_0, l_0 );
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        // updates for local storage
        if ( dev_id == cur_dev )
            dev_id = 0;

        // modifications
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkNewPatchVisitor_new_cursor upv{ pos };
        PatchCreatorString::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( loc_id ) {
            tmp_bq_new << PatchCreatorString::NEW_CURSOR << dev_id << loc_id << pos;
            pcs->_new_cursor_wo_cr( dev_id, loc_id, pos, as_usr );
        }
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        // updates for local storage
        if ( dev_id == cur_dev )
            dev_id = 0;

        // modifications
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkNewPatchVisitor_set_cursor upv{ dev_id, loc_id, pos };
        PatchCreatorString::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( loc_id ) {
            PT old_pos = 0;
            for( AutoPtr<PatchCreatorString::Cursor> &c : pcs->cursors ) {
                if ( c->dev_id == dev_id and c->loc_id == loc_id ) {
                    old_pos = c->pos;
                    break;
                }
            }
            tmp_bq_new << PatchCreatorString::SET_CURSOR_UND << dev_id << loc_id << pos << old_pos;
            pcs->_set_cursor_wo_cr( dev_id, loc_id, pos );
        }
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {
        // updates for local storage
        if ( dev_id == cur_dev )
            dev_id = 0;

        // modifications
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkNewPatchVisitor_rem_cursor upv{ dev_id, loc_id };
        PatchCreatorString::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( loc_id ) {
            Inode old_usr;
            PT old_pos = 0;
            for( AutoPtr<PatchCreatorString::Cursor> &c : pcs->cursors ) {
                if ( c->dev_id == dev_id and c->loc_id == loc_id ) {
                    old_usr = c->usr_id;
                    old_pos = c->pos;
                    break;
                }
            }
            tmp_bq_new << PatchCreatorString::REM_CURSOR_UND << dev_id << loc_id << old_pos << old_usr;
            pcs->_rem_cursor_wo_cr( dev_id, loc_id );
        }
    }
    void removal_und( PT &p, const CbString &d ) {
        WARNING( "Patches sent through the network should not contain _und patch type versions" );
        PT l = d.size();
        removal( p, l );
    }
    void set_cursor_und( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {
        WARNING( "Patches sent through the network should not contain _und patch type versions" );
        set_cursor( dev_id, loc_id, pos );
    }
    void rem_cursor_und( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {
        WARNING( "Patches sent through the network should not contain _und patch type versions" );
        rem_cursor( dev_id, loc_id );
    }
};

struct NewUnkPatchVisitor_insertion : PatchCreatorString::Visitor {
    NewUnkPatchVisitor_insertion( const FqId &id, PatchMergerString *pms, PatchCreatorString *pcs, PT &p_0, CbString &d_0, BinStream<CbQueue>  tmp_bq_new ) : cur_dev( cur_dev ), cur_usr( cur_usr ), pms( pms ), pcs( pcs ), p_0( p_0 ), d_0( d_0 ), tmp_bq_new( tmp_bq_new ) {
    }
    void insertion( PT &p_1, CbString &d_1 ) { // INS(new)/INS(unk)
        if ( p_1 > p_0 ) {
            // orig 01234
            // real 0123unk4    INS(p_1=4,d_1=unk)
            // imag 0new1234    INS(p_0=1,d_0=new)
            // obj  0new123unk4 (real -> obj = unk: INS 1,new; imag -> obj = new: INS 7,unk)
            p_1 += d_0.size();
        } else {
            // orig 01234
            // real 0123unk4    INS(p_1=4,d_1=unk)
            // imag 0new1234    INS(p_0=1,d_0=new)
            // obj  0new123unk4 (real -> obj = unk: INS 1,new; imag -> obj = new: INS 7,unk)
            p_0 += d_1.size();
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(new)/INS(unk)
        if ( p_0 >= p_1 + l_1 ) {
            // orig 012345
            // real 0345      REM(p_1=1,l_1=2)
            // imag 01234new5 INS(p_0=5,d_0=new)
            // obj  034new5   (real -> obj = unk: INS 3,new; imag -> obj = new: REM 1,2)
            p_0 -= l_1; // 3
        } else if ( p_0 <= p_1 ) {
            // orig 012345
            // real 0125      REM(p_1=3,l_1=2)
            // imag 0new12345 INS(p_0=1,d_0=new)
            // obj  0new125   (real -> obj = unk: INS 1,new; imag -> obj = new: REM 6,2)
            p_1 += d_0.size();
        } else {
            // orig 012345
            // real 05        REM(p_1=1,l_1=4)
            // imag 012new345 INS(p_0=3,d_0=new)
            // obj  0new5     (real -> obj = unk: INS 1,new; imag -> obj = new: REM 1,2 + REM 4,2)
            tmp_bq_new << PatchCreatorString::REMOVAL << p_1 << p_0 - p_1; // 1, 2
            l_1 -= p_0 - p_1;  // 2
            p_0 = p_1;         // 1
            p_1 += d_0.size(); // 4
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) { // NEW_CURSOR(new)/INS(unk)
        if ( pos >= p_0 )
            pos += d_0.size();
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 )
            pos += d_0.size();
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( Inode &grp, unsigned &flags ) {}
    void add_usr_right ( Inode &usr, unsigned &flags ) {}
    void rem_grp_right( Inode &grp, unsigned &flags ) {}
    void rem_usr_right ( Inode &usr, unsigned &flags ) {}
    void removal_und    ( PT p, const CbString &d ) { PT l = d.size(); removal( p, l ); }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &p_0;
    CbString           &d_0;
    BinStream<CbQueue>  tmp_bq_new;
};

struct NewUnkPatchVisitor_remove : PatchCreatorString::Visitor {
    NewUnkPatchVisitor_remove( const FqId &id, PatchMergerString *pms, PatchCreatorString *pcs, PT &p_0, PT &l_0, BinStream<CbQueue>  tmp_bq_unk ) : cur_dev( cur_dev ), cur_usr( cur_usr ), pms( pms ), pcs( pcs ), p_0( p_0 ), l_0( l_0 ), tmp_bq_unk( tmp_bq_unk ) {
    }
    void insertion( PT &p_1, CbString &d_1 ) { // INS(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            // orig 01234567
            // real 01ab234567 INS(p_1=2,d_1=ab)
            // imag 012347     REM(p_0=5,l_0=2)
            // obj  01ab2347   (real -> obj = unk: REM 7,2; imag -> obj = new: INS 2,ab)
            p_0 += d_1.size();
        } else if ( p_1 >= p_0 + l_0 ) {
            // orig 01234567
            // real 01234ab567 INS(p_1=5,d_1=ab)
            // imag 014567     REM(p_0=2,l_0=2)
            // obj  014ab567   (real -> obj = unk: REM 2,2; imag -> obj = new: INS 3,ab)
            p_1 -= l_0;
        } else {
            // orig 01234567
            // real 0123ab4567 INS(p_1=4,d_1=ab)
            // imag 0167       REM(p_0=2,l_0=4)
            // obj  01ab67     (real -> obj = unk: REM 2,2 + REM 4,2; imag -> obj = new: INS 2,ab)
            tmp_bq_unk << PatchCreatorString::REMOVAL_UND << p_0 << pcs->_substring( p_0, p_1 - p_0 ); // 2, 2
            pcs->_remove_wo_cr( p_0, p_1 - p_0 );
            l_0 -= p_1 - p_0;  // 2
            p_1 = p_0;         // 2
            p_0 += d_1.size(); // 4
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            if ( p_1 + l_1 <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM(p_1=1,l_1=3)
                // imag 01234589   REM(p_0=6,l_0=2)
                // obj  04589      (real -> obj = unk: REM 3,2; imag -> obj = new: REM 1,3)
                p_0 -= l_1; // 3
            } else if ( p_1 + l_1 <= p_0 + l_0 ) {
                // orig 0123456789
                // real 06789      REM(p_1=1,l_1=5)
                // imag 012389     REM(p_0=4,l_0=4)
                // obj  089        (real -> obj = unk: REM 1,2; imag -> obj = new: REM 1,3)
                PT d = p_1 + l_1 - p_0; // 2
                l_0 -= d;  // 2
                l_1 -= d;  // 3
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM(p_1=1,l_1=8)
                // imag 01236789   REM(p_0=4,l_0=2)
                // obj  09         (real -> obj = unk: VOID; imag -> obj = new: REM 1,8)
                l_1 -= l_0;
                l_0 = 0;
            }
        } else {
            if ( p_0 + l_0 <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM(p_1=6,l_1=2)
                // imag 0456789    REM(p_0=1,l_0=3)
                // obj  04589      (real -> obj = unk: REM 1,3; imag -> obj = new: REM 3,2)
                p_1 -= l_0; // 3
            } else if ( p_0 + l_0 <= p_1 + l_1 ) {
                // orig 0123456789
                // real 012389     REM(p_1=4,l_1=4)
                // imag 06789      REM(p_0=1,l_0=5)
                // obj  089        (real -> obj = unk: REM 1,3; imag -> obj = new: REM 1,2)
                PT d = p_0 + l_0 - p_1; // 2
                l_1 -= d;  // 2
                l_0 -= d;  // 3
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 09         REM(p_1=1,l_1=8)
                // imag 01236789   REM(p_0=4,l_0=2)
                // obj  09         (real -> obj = unk: VOID; imag -> obj = new: REM 1,8)
                l_0 -= l_1;
                l_1 = 0;
            }
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + l_0 )
            pos -= l_0;
        else if ( pos > p_0 )
            pos = p_0;
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + l_0 )
            pos -= l_0;
        else if ( pos > p_0 )
            pos = p_0;
    }
    void rem_cursor( DevId &dev_id, PI64 &loc_id ) {}
    void add_grp_right( Inode &grp, unsigned &flags ) {}
    void add_usr_right ( Inode &usr, unsigned &flags ) {}
    void rem_grp_right( Inode &grp, unsigned &flags ) {}
    void rem_usr_right ( Inode &usr, unsigned &flags ) {}
    void removal_und    ( PT p, const CbString &d ) { PT l = d.size(); removal( p, l ); }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &p_0;
    PT                 &l_0;
    BinStream<CbQueue>  tmp_bq_unk;
};

struct NewUnkPatchVisitor_new_cursor : PatchCreatorString::Visitor {
    NewUnkPatchVisitor_new_cursor( const FqId &id, PatchMergerString *pms, PatchCreatorString *pcs, PT &pos ) : cur_dev( cur_dev ), cur_usr( cur_usr ), pms( pms ), pcs( pcs ), pos( pos ) {
    }
    void insertion( PT p_1, const CbString &d_1 ) { // INS(new)/NEW_CURSOR(unk)
        if ( pos >= p_1 )
            pos += d_1.size();
    }
    void removal( PT p_1, PT l_1 ) { // REM(new)/NEW_CURSOR(unk)
        if ( pos >= p_1 + l_1 )
            pos -= l_1;
        else if ( pos > p_1 )
            pos = p_1;
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor( DevId dev_id, PI64 loc_id         ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) { removal( p, d.size() ); }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &pos;
};

struct NewUnkPatchVisitor_set_cursor : PatchCreatorString::Visitor {
    NewUnkPatchVisitor_set_cursor( const FqId &id, PatchMergerString *pms, PatchCreatorString *pcs, DevId &dev_id_unk, PI64 &loc_id_unk, PT &pos_unk ) : cur_dev( cur_dev ), cur_usr( cur_usr ), pms( pms ), pcs( pcs ), dev_id_unk( dev_id_unk ), loc_id_unk( loc_id_unk ), pos_unk( pos_unk ) {
    }
    void insertion( PT p_1, const CbString &d_1 ) { // INS(new)/NEW_CURSOR(unk)
        if ( pos_unk >= p_1 )
            pos_unk += d_1.size();
    }
    void removal( PT p_1, PT l_1 ) { // REM(new)/NEW_CURSOR(unk)
        if ( pos_unk >= p_1 + l_1 )
            pos_unk -= l_1;
        else if ( pos_unk > p_1 )
            pos_unk = p_1;
    }
    void new_cursor( DevId dev_id, PI64 &loc_id, PT pos ) {
        if ( dev_id == dev_id_unk and loc_id == loc_id_unk )
            loc_id = 0;
    }
    void set_cursor( DevId dev_id, PI64 &loc_id, PT pos ) {
        if ( dev_id == dev_id_unk and loc_id == loc_id_unk )
            loc_id = 0;
    }
    void rem_cursor( DevId dev_id, PI64 &loc_id ) {
        if ( dev_id == dev_id_unk and loc_id == loc_id_unk )
            loc_id = 0;
    }
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) { removal( p, d.size() ); }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    DevId              &dev_id_unk;
    PI64               &loc_id_unk;
    PT                 &pos_unk;
};

struct NewUnkPatchVisitor_rem_cursor : PatchCreatorString::Visitor {
    NewUnkPatchVisitor_rem_cursor( const FqId &id, PatchMergerString *pms, PatchCreatorString *pcs, DevId &dev_id_unk, PI64 &loc_id_unk ) : cur_dev( cur_dev ), cur_usr( cur_usr ), pms( pms ), pcs( pcs ), dev_id_unk( dev_id_unk ), loc_id_unk( loc_id_unk ) {
    }
    void insertion( PT p_1, const CbString &d_1 ) {
    }
    void removal( PT p_1, PT l_1 ) {
    }
    void new_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { // UNK(new_cursor) + NEW(rem_cursor)
        if ( dev_id == dev_id_unk and loc_id == loc_id_unk )
            loc_id = 0;
    }
    void set_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { // UNK(set_cursor) + NEW(rem_cursor)
        if ( dev_id == dev_id_unk and loc_id == loc_id_unk )
            loc_id = 0;
    }
    void rem_cursor( DevId dev_id, PI64 &loc_id ) { // UNK(rem_cursor) + NEW(rem_cursor)
        if ( dev_id == dev_id_unk and loc_id == loc_id_unk )
            loc_id = 0;
    }
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) { PT l = d.size(); removal( p, l ); }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    DevId              &dev_id_unk;
    PI64               &loc_id_unk;
};

struct NewUnkPatchVisitor_remove_und : PatchCreatorString::Visitor {
    NewUnkPatchVisitor_remove_und( const FqId &id, PatchMergerString  *pms, PatchCreatorString *pcs, PT &p_0, CbString &d_0, BinStream<CbQueue> tmp_bq_unk ) : cur_dev( cur_dev ), cur_usr( cur_usr ), pms( pms ), pcs( pcs ), p_0( p_0 ), d_0( d_0 ), tmp_bq_unk( tmp_bq_unk ) {
    }
    void insertion( PT &p_1, CbString &d_1 ) { // INS(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            // orig 01234567
            // real 01ab234567 INS(p_1=2,d_1=ab)
            // imag 012347     REM_UND(p_0=5,l_0=45)
            // obj  01ab2347   (real -> obj = unk: REM_UND 7,45; imag -> obj = new: INS 2,ab)
            p_0 += d_1.size();
        } else if ( p_1 >= p_0 + d_0.size() ) {
            // orig 01234567
            // real 01234ab567 INS(p_1=5,d_1=ab)
            // imag 014567     REM_UND(p_0=2,l_0=23)
            // obj  014ab567   (real -> obj = unk: REM_UND 2,23; imag -> obj = new: INS 3,ab)
            p_1 -= d_0.size();
        } else {
            // orig 01234567
            // real 0123ab4567 INS(p_1=4,d_1=ab)
            // imag 0167       REM_UND(p_0=2,d_0=2345)
            // obj  01ab67     (real -> obj = unk: REM_UND 2,23 + REM_UND 4,45; imag -> obj = new: INS 2,ab)
            tmp_bq_unk << PatchCreatorString::REMOVAL_UND << p_0 << CbString( d_0, 0, p_1 - p_0 ); // 2, 23
            pcs->_remove_wo_cr( p_0, p_1 - p_0 );
            d_0.skip_some( p_1 - p_0 );  // 45
            p_1 = p_0;                   // 2
            p_0 += d_1.size();           // 4
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            if ( p_1 + l_1 <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM(p_1=1,l_1=3)
                // imag 01234589   REM_UND(p_0=6,d_0=67)
                // obj  04589      (real -> obj = unk: REM_UND 3,67; imag -> obj = new: REM 1,3)
                p_0 -= l_1; // 3
            } else if ( p_1 + l_1 <= p_0 + d_0.size() ) {
                // orig 0123456789
                // real 06789      REM(p_1=1,l_1=5)
                // imag 012389     REM_UND(p_0=4,d_0=4567)
                // obj  089        (real -> obj = unk: REM_UND 1,67; imag -> obj = new: REM 1,3)
                PT d = p_1 + l_1 - p_0; // 2
                d_0.skip_some( d );  // 67
                l_1 -= d;  // 3
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM(p_1=1,l_1=8)
                // imag 01236789   REM_UND(p_0=4,d_0=45)
                // obj  09         (real -> obj = unk: VOID; imag -> obj = new: REM 1,8)
                l_1 -= d_0.size();
                d_0 = CbString();
            }
        } else {
            if ( p_0 + d_0.size() <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM(p_1=6,l_1=2)
                // imag 0456789    REM_UND(p_0=1,d_0=123)
                // obj  04589      (real -> obj = unk: REM_UND 1,123; imag -> obj = new: REM 3,2)
                p_1 -= d_0.size(); // 3
            } else if ( p_0 + d_0.size() <= p_1 + l_1 ) {
                // orig 0123456789
                // real 012389     REM(p_1=4,l_1=4)
                // imag 06789      REM_UND(p_0=1,d_0=12345)
                // obj  089        (real -> obj = unk: REM_UND 1,123; imag -> obj = new: REM 1,2)
                PT d = p_0 + d_0.size() - p_1; // 2
                l_1 -= d;  // 2
                d_0 = CbString( d_0, 0, d_0.size() - d );  // 123
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 01236789   REM(p_1=4,l_1=2)
                // imag 09         REM_UND(p_0=1,d_0=12345678)
                // obj  09         (real -> obj = unk: REM_UND(p_0=1,d_0=123678); imag -> obj = new: VOID)
                PT b = p_1 + l_1 - p_0; // 5

                CbQueue n_0;
                n_0.write_some( CbString( d_0, 0, p_1 - p_0 ) ); //  123
                n_0.write_some( CbString( d_0, b, d_0.size() - b ) ); // 5, 3 -> 678
                d_0 = std::move( n_0 );

                l_1 = 0;
            }
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + d_0.size() )
            pos -= d_0.size();
        else if ( pos > p_0 )
            pos = p_0;
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + d_0.size() )
            pos -= d_0.size();
        else if ( pos > p_0 )
            pos = p_0;
    }
    void rem_cursor( DevId &dev_id, PI64 &loc_id ) {}
    void add_grp_right( Inode &grp, unsigned &flags ) {}
    void add_usr_right ( Inode &usr, unsigned &flags ) {}
    void rem_grp_right( Inode &grp, unsigned &flags ) {}
    void rem_usr_right ( Inode &usr, unsigned &flags ) {}
    void removal_und    ( PT p, const CbString &d ) { PT l = d.size(); removal( p, l ); }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &p_0;
    CbString           &d_0;
    BinStream<CbQueue>  tmp_bq_unk;
};


struct UnkPatchVisitor : PatchCreatorString::UnkPatchVisitor {
    using PatchCreatorString::UnkPatchVisitor::UnkPatchVisitor;

    void insertion( PT &p_0, CbString &d_0 ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewUnkPatchVisitor_insertion upv{ db, pms, pcs, p_0, d_0, &pms->cq_new };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_insert_at( p_0, as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // execute the corresponding operation
        pcs->_insert_wo_cr( p_0, d_0 );
    }
    void removal( PT &p_0, PT &l_0 ) {
        WARNING( "Patches stored locally should not contain non _und patch type versions" );

        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewUnkPatchVisitor_remove upv{ db, pms, pcs, p_0, l_0, tmp_bq_unk };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_remove( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // execute the corresponding operation
        pcs->_remove_wo_cr( p_0, l_0 );
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewUnkPatchVisitor_new_cursor upv{ db, pms, pcs, pos };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // execute the corresponding operation
        pcs->_new_cursor_wo_cr( dev_id, loc_id, pos, as_usr );
    }
    void set_cursor( DevId &dev_id, PI64 &loc_id, PT &pos ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewUnkPatchVisitor_set_cursor upv{ db, pms, pcs, dev_id, loc_id, pos };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // execute the corresponding operation
        pcs->_set_cursor_wo_cr( dev_id, loc_id, pos );
    }
    void rem_cursor( DevId &dev_id, PI64 &loc_id ) {
        WARNING( "Patches stored locally should not contain non _und patch type versions" );

        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewUnkPatchVisitor_rem_cursor upv{ db, pms, pcs, dev_id, loc_id };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );
        if ( not loc_id )
            return;

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // execute the corresponding operation
        pcs->_rem_cursor_wo_cr( dev_id, loc_id );
    }
    void removal_und( PT &p_0, CbString &d_0 ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewUnkPatchVisitor_remove_und upv{ db, pms, pcs, p_0, d_0, tmp_bq_unk };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_remove( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // execute the corresponding operation
        pcs->_remove_wo_cr( p_0, d_0.size() );
    }
    void set_cursor_und( DevId dev_id, PI64 &loc_id, PT &pos, PT old_pos ) {
        set_cursor( dev_id, loc_id, pos );
    }
    void rem_cursor_und( DevId dev_id, PI64 &loc_id, PT pos, UsrId usr ) {
        rem_cursor( dev_id, loc_id );
    }
};

struct NewStdPatchVisitor_insertion {
    void insertion( PT &p_1, CbString &d_1 ) { // INS(new)/INS(std)
        if ( p_1 > p_0 ) {
            // orig 01234
            // real 0123unk4    INS(p_1=4,d_1=unk)
            // imag 0new1234    INS(p_0=1,d_0=new)
            // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
            p_1 += d_0.size();
        } else {
            // orig 01234
            // real 0123unk4    INS(p_1=4,d_1=unk)
            // imag 0new1234    INS(p_0=1,d_0=new)
            // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
            p_0 += d_1.size();
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(new)/INS(std)
        if ( p_0 >= p_1 + l_1 ) {
            // orig 012345
            // real 0345      REM(p_1=1,l_1=2)
            // imag 01234new5 INS(p_0=5,d_0=new)
            // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
            p_0 -= l_1; // 3
        } else if ( p_0 <= p_1 ) {
            // orig 012345
            // real 0125      REM(p_1=3,l_1=2)
            // imag 0new12345 INS(p_0=1,d_0=new)
            // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
            p_1 += d_0.size();
        } else {
            // orig 012345
            // real 05        REM(p_1=1,l_1=4)
            // imag 012new345 INS(p_0=3,d_0=new)
            // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 4,2 + REM 1,2)
            tmp_bq_new << PatchCreatorString::REMOVAL << p_1 << p_0 - p_1; // 1, 2
            l_1 -= p_0 - p_1;  // 2
            p_0 = p_1;         // 1
            p_1 = d_0.size();  // 4
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) { // NEW_CURSOR(new)/INS(std)
        if ( pos >= p_0 )
            pos += d_0.size();
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 )
            pos += d_0.size();
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right( UsrId usr, unsigned flags ) {}
    void removal_und( PT &p_1, CbString &d_1 ) {
        if ( p_0 >= p_1 + d_1.size() ) {
            // orig 012345
            // real 0345      REM(p_1=1,l_1=2)
            // imag 01234new5 INS(p_0=5,d_0=new)
            // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
            p_0 -= d_1.size(); // 3
        } else if ( p_0 <= p_1 ) {
            // orig 012345
            // real 0125      REM(p_1=3,l_1=2)
            // imag 0new12345 INS(p_0=1,d_0=new)
            // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
            p_1 += d_0.size();
        } else {
            // orig 012345
            // real 05        REM(p_1=1,l_1=4)
            // imag 012new345 INS(p_0=3,d_0=new)
            // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,'01' + REM 4,'34')
            tmp_bq_new << PatchCreatorString::REMOVAL_UND << p_1 << d_1.beg_upto( p_0 - p_1 ); // 1, '12'
            d_1.skip_some( p_0 - p_1 );  // '34'
            p_0 = p_1;         // 1
            p_1 += d_0.size(); // 4
        }
    }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &p_0;
    CbString           &d_0;
    BinStream<CbQueue> tmp_bq_new;
};

struct NewStdPatchVisitor_removal {
    void insertion( PT &p_1, CbString &d_1 ) { // INS(new)/REM(std)
        if ( p_1 <= p_0 ) {
            // orig 01234567
            // real 01ab234567 INS(p_1=2,d_1=ab)
            // imag 012347     REM(p_0=5,l_0=2)
            // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
            p_0 += d_1.size();
        } else if ( p_1 >= p_0 + l_0 ) {
            // orig 01234567
            // real 01234ab567 INS(p_1=5,d_1=ab)
            // imag 014567     REM(p_0=2,l_0=2)
            // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
            p_1 -= l_0;
        } else {
            // orig 01234567
            // real 0123ab4567 INS(p_1=4,d_1=ab)
            // imag 0167       REM(p_0=2,l_0=4)
            // obj  01ab67     (real -> obj = new: REM 2,'23' + REM 4,'45'; imag -> obj = unk: INS 2,ab)
            tmp_bq_std << PatchCreatorString::REMOVAL_UND << p_1 << pcs->_substring( p_0, p_1 - p_0 ); // 2, '23'
            pcs->_remove_wo_cr( p_0, p_1 - p_0 );
            l_0 -= p_1 - p_0;  // 2
            p_1 = p_0;         // 2
            p_0 += d_1.size(); // 4
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            if ( p_1 + l_1 <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM(p_1=1,l_1=3)
                // imag 01234589   REM(p_0=6,l_0=2)
                // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM 1,3)
                p_0 -= l_1; // 3
            } else if ( p_1 + l_1 <= p_0 + l_0 ) {
                // orig 0123456789
                // real 06789      REM(p_1=1,l_1=5)
                // imag 012389     REM(p_0=4,l_0=4)
                // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM 1,3)
                PT d = p_1 + l_1 - p_0; // 2
                l_0 -= d;  // 2
                l_1 -= d;  // 3
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM(p_1=1,l_1=8)
                // imag 01236789   REM(p_0=4,l_0=2)
                // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM 1,6)
                l_1 -= l_0;
                l_0 = 0;
            }
        } else {
            if ( p_0 + l_0 <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM(p_1=6,l_1=2)
                // imag 0456789    REM(p_0=1,l_0=3)
                // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM 3,2)
                p_1 -= l_0; // 3
            } else if ( p_0 + l_0 <= p_1 + l_1 ) {
                // orig 0123456789
                // real 012389     REM(p_1=4,l_1=4)
                // imag 06789      REM(p_0=1,l_0=5)
                // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM 1,2)
                PT d = p_0 + l_0 - p_1; // 2
                l_1 -= d;  // 2
                l_0 -= d;  // 3
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 01236789   REM(p_1=4,l_1=2)
                // imag 09         REM(p_0=1,l_0=8)
                // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
                l_0 -= l_1;
                l_1 = 0;
            }
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + l_0 )
            pos -= l_0;
        else if ( pos > p_0 )
            pos = l_0;
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + l_0 )
            pos -= l_0;
        else if ( pos > p_0 )
            pos = l_0;
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT &p_1, CbString &d_1 ) {
        if ( p_1 <= p_0 ) {
            if ( p_1 + d_1.size() <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM(p_1=1,d_1='123')
                // imag 01234589   REM(p_0=6,l_0=2)
                // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM_UND 1,'123')
                p_0 -= d_1.size(); // 3
            } else if ( p_1 + d_1.size() <= p_0 + l_0 ) {
                // orig 0123456789
                // real 06789      REM_UND(p_1=1,d_1='12345')
                // imag 012389     REM(p_0=4,l_0=4)
                // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM_UND 1,'123')
                PT d = p_1 + d_1.size() - p_0; // 2
                d_1 = d_1.beg_upto( d_1.size() - d ); // '123'
                l_0 -= d;  // 2
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM_UND(p_1=1,d_1='12345678')
                // imag 01236789   REM(p_0=4,l_0=2)
                // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM_UND 1,'123678')
                CbQueue t;
                t.write_some( d_1.beg_upto( p_0 - p_1 ) );
                t.write_some( d_1.end_from( p_0 - p_1 + l_0 ) );
                d_1 = std::move( t );
                l_0 = 0;
            }
        } else {
            if ( p_0 + l_0 <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM_UND(p_1=6,d_1='67')
                // imag 0456789    REM(p_0=1,l_0=3)
                // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 3,'67')
                p_1 -= l_0; // 3
            } else if ( p_0 + l_0 <= p_1 + d_1.size() ) {
                // orig 0123456789
                // real 012389     REM_UND(p_1=4,d_1='4567')
                // imag 06789      REM(p_0=1,l_0=5)
                // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 1,'67')
                PT d = p_0 + l_0 - p_1; // 2
                d_1.skip_some( d ); // '67'
                l_0 -= d;  // 3
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 01236789   REM(p_1=4,d_1.size()=2)
                // imag 09         REM(p_0=1,l_0=8)
                // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
                l_0 -= d_1.size();
                d_1.free();
            }
        }
    }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &p_0;
    PT                 &l_0;
    BinStream<CbQueue>  tmp_bq_std;
};

/// what to do with each data from unk and a new new_cursor (pos)
struct NewStdPatchVisitor_new_cursor {
    void insertion( PT p_1, const CbString &d_1 ) { // INS(unk)/NEW_CURSOR(new)
        if ( pos >= p_1 )
            pos += d_1.size();
    }
    void removal( PT p_1, PT l_1 ) { // REM(unk)/NEW_CURSOR(new)
        if ( pos >= p_1 + l_1 )
            pos -= l_1;
        else if ( pos > p_1 )
            pos = p_1;
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor( DevId dev_id, PI64 loc_id         ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) { removal( p, d.size() ); }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &pos;
};

/// what to do with each data from unk and a new set_cursor
struct NewStdPatchVisitor_set_cursor {
    void insertion( PT p_1, const CbString &d_1 ) { // INS(unk)/NEW_CURSOR(new)
        if ( pos_new >= p_1 )
            pos_new += d_1.size();
    }
    void removal( PT p_1, PT l_1 ) { // REM(unk)/NEW_CURSOR(new)
        if ( pos_new >= p_1 + l_1 )
            pos_new -= l_1;
        else if ( pos_new > p_1 )
            pos_new = p_1;
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {
    }
    void set_cursor( DevId dev_id, PI64 &loc_id, PT pos ) {
        // the position is defined by the new patch so we can delete the set_cursor from unk
        if ( dev_id == dev_id_new and loc_id == loc_id_new )
            loc_id = 0;
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {
        // cursor has been deleted in an unk patch -> do not keep it in the new patch
        if ( dev_id == dev_id_new and loc_id == loc_id_new )
            loc_id_new = 0;
    }
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) { PT l = d.size(); removal( p, l ); }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    DevId              &dev_id_new;
    PI64               &loc_id_new;
    PT                 &pos_new;
};

/// what to do with each data from unk and a new rem_cursor
struct NewStdPatchVisitor_rem_cursor {
    void insertion( PT p_1, const CbString &d_1 ) {
    }
    void removal( PT p_1, PT l_1 ) {
    }
    void new_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { // UNK(new_cursor) + NEW(rem_cursor)
        if ( dev_id == dev_id_new and loc_id == loc_id_new )
            loc_id = 0;
    }
    void set_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { // UNK(set_cursor) + NEW(rem_cursor)
        if ( dev_id == dev_id_new and loc_id == loc_id_new )
            loc_id = 0;
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) { // UNK(rem_cursor) + NEW(rem_cursor)
        if ( dev_id == dev_id_new and loc_id == loc_id_new )
            loc_id = 0;
    }
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) { PT l = d.size(); removal( p, l ); }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    DevId              &dev_id_new;
    PI64               &loc_id_new;
};

/// what to do with each data from unk and a new add_grp_right
struct NewStdPatchVisitor_add_grp_right {
    void insertion( PT p_1, const CbString &d_1 ) {}
    void removal( PT p_1, PT l_1 ) {}
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) {}
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {}
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {}
    GrpId               grp_new;
    unsigned           &flags_new;
};

/// what to do with each data from unk and a new add_usr_right
struct NewStdPatchVisitor_add_usr_right {
    void insertion( PT p_1, const CbString &d_1 ) {}
    void removal( PT p_1, PT l_1 ) {}
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned &flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    void rem_grp_right( GrpId grp, unsigned &flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    void removal_und    ( PT p, const CbString &d ) {}
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {}
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {}
    UsrId               usr_new;
    unsigned           &flags_new;
};

/// what to do with each data from unk and a new rem_grp_right
struct NewStdPatchVisitor_rem_grp_right {
    void insertion( PT p_1, const CbString &d_1 ) {}
    void removal( PT p_1, PT l_1 ) {}
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT p, const CbString &d ) {}
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {}
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {}
    GrpId               grp_new;
    unsigned           &flags_new;
};

/// what to do with each data from unk and a new rem_usr_right
struct NewStdPatchVisitor_rem_usr_right {
    void insertion( PT p_1, const CbString &d_1 ) {}
    void removal( PT p_1, PT l_1 ) {}
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned &flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    void rem_grp_right( GrpId grp, unsigned &flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    void removal_und    ( PT p, const CbString &d ) {}
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {}
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {}
    UsrId               usr_new;
    unsigned           &flags_new;
};


struct NewStdPatchVisitor_removal_und {
    void insertion( PT &p_1, CbString &d_1 ) { // INS(new)/REM(std)
        if ( p_1 <= p_0 ) {
            // orig 01234567
            // real 01ab234567 INS(p_1=2,d_1=ab)
            // imag 012347     REM_UND(p_0=5,d_0='56')
            // obj  01ab2347   (real -> obj = new: REM_UND 7,'56'; imag -> obj = unk: INS 2,ab)
            p_0 += d_1.size();
        } else if ( p_1 >= p_0 + d_0.size() ) {
            // orig 01234567
            // real 01234ab567 INS(p_1=5,d_1=ab)
            // imag 014567     REM_UND(p_0=2,d_0='23')
            // obj  014ab567   (real -> obj = new: REM_UND 2,'23'; imag -> obj = unk: INS 3,ab)
            p_1 -= d_0.size();
        } else {
            // orig 01234567
            // real 0123ab4567 INS(p_1=4,d_1=ab)
            // imag 0167       REM_UND(p_0=2,d_0='2345')
            // obj  01ab67     (real -> obj = new: REM_UND 2,'23' + REM 4,'45'; imag -> obj = unk: INS 2,ab)
            tmp_bq_std << PatchCreatorString::REMOVAL_UND << p_1 << d_0.beg_upto( p_1 - p_0 ); // 2, '23'
            pcs->_remove_wo_cr( p_0, p_1 - p_0 );
            d_0.skip_some( p_1 - p_0 );  // '45'
            p_1 = p_0;         // 2
            p_0 += d_1.size(); // 4
        }
    }
    void removal( PT &p_1, PT &l_1 ) { // REM(unk)/REM(new)
        if ( p_1 <= p_0 ) {
            if ( p_1 + l_1 <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM(p_1=1,l_1=3)
                // imag 01234589   REM_UND(p_0=6,d_0.size()=2)
                // obj  04589      (real -> obj = new: REM_UND 3,2; imag -> obj = unk: REM 1,3)
                p_0 -= l_1; // 3
            } else if ( p_1 + l_1 <= p_0 + d_0.size() ) {
                // orig 0123456789
                // real 06789      REM(p_1=1,l_1=5)
                // imag 012389     REM_UND(p_0=4,d_0='4567')
                // obj  089        (real -> obj = new: REM_UND 1,'67'; imag -> obj = unk: REM 1,3)
                PT d = p_1 + l_1 - p_0; // 2
                d_0.skip_some( d ); // '67'
                l_1 -= d;  // 3
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM(p_1=1,l_1=8)
                // imag 01236789   REM_UND(p_0=4,d_0.size()=2)
                // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM 1,6)
                l_1 -= d_0.size();
                d_0.free();
            }
        } else {
            if ( p_0 + d_0.size() <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM(p_1=6,l_1=2)
                // imag 0456789    REM_UND(p_0=1,d_0.size()=3)
                // obj  04589      (real -> obj = new: REM_UND 1,3; imag -> obj = unk: REM 3,2)
                p_1 -= d_0.size(); // 3
            } else if ( p_0 + d_0.size() <= p_1 + l_1 ) {
                // orig 0123456789
                // real 012389     REM(p_1=4,l_1=4)
                // imag 06789      REM_UND(p_0=1,d_0=12345)
                // obj  089        (real -> obj = new: REM_UND 1,'123'; imag -> obj = unk: REM 1,2)
                PT d = p_0 + d_0.size() - p_1; // 2
                l_1 -= d;  // 2
                d_0 = d_0.beg_upto( d_0.size() - d ); // '123'
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 01236789   REM(p_1=4,l_1=2)
                // imag 09         REM_UND(p_0=1,d_0=12345678)
                // obj  09         (real -> obj = new: REM_UND 1,6; imag -> obj = unk: VOID)
                CbQueue t;
                t.write_some( d_0.beg_upto( p_1 - p_0 ) );
                t.write_some( d_0.end_from( p_1 - p_0 + l_1 ) );
                l_1 = 0;
            }
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + d_0.size() )
            pos -= d_0.size();
        else if ( pos > p_0 )
            pos = d_0.size();
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + d_0.size() )
            pos -= d_0.size();
        else if ( pos > p_0 )
            pos = d_0.size();
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT &p_1, CbString &d_1 ) {
        if ( p_1 <= p_0 ) {
            if ( p_1 + d_1.size() <= p_0 ) {
                // orig 0123456789
                // real 0456789    REM_UND(p_1=1,d_1='123')
                // imag 01234589   REM_UND(p_0=6,d_0.size()=2)
                // obj  04589      (real -> obj = new: REM_UND 3,2; imag -> obj = unk: REM_UND 1,'123')
                p_0 -= d_1.size(); // 3
            } else if ( p_1 + d_1.size() <= p_0 + d_0.size() ) {
                // orig 0123456789
                // real 06789      REM_UND(p_1=1,d_1='12345')
                // imag 012389     REM_UND(p_0=4,d_0='4567')
                // obj  089        (real -> obj = new: REM_UND 1,'67'; imag -> obj = unk: REM_UND 1,'123')
                PT d = p_1 + d_1.size() - p_0; // 2
                d_1 = d_1.beg_upto( d_1.size() - d ); // '123'
                d_0.skip_some( d ); // '67'
                p_0 = p_1; // 1
            } else {
                // orig 0123456789
                // real 09         REM_UND(p_1=1,d_1='12345678')
                // imag 01236789   REM_UND(p_0=4,d_0.size()=2)
                // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM_UND 1,'123678')
                CbQueue t;
                t.write_some( d_1.beg_upto( p_0 - p_1 ) );
                t.write_some( d_1.end_from( p_0 - p_1 + d_0.size() ) );
                d_1 = std::move( t );
                d_0.free();
            }
        } else {
            if ( p_0 + d_0.size() <= p_1 ) {
                // orig 0123456789
                // real 01234589   REM_UND(p_1=6,d_1='67')
                // imag 0456789    REM_UND(p_0=1,d_0.size()=3)
                // obj  04589      (real -> obj = new: REM_UND 1,3; imag -> obj = unk: REM_UND 3,'67')
                p_1 -= d_0.size(); // 3
            } else if ( p_0 + d_0.size() <= p_1 + d_1.size() ) {
                // orig 0123456789
                // real 012389     REM_UND(p_1=4,d_1='4567')
                // imag 06789      REM_UND(p_0=1,d_0='12345')
                // obj  089        (real -> obj = new: REM_UND 1,'123'; imag -> obj = unk: REM_UND 1,'67')
                d_1.skip_some( p_0 + d_0.size() - p_1 ); // '67'
                d_0 = d_0.beg_upto( p_1 - p_0 );  // '123'
                p_1 = p_0; // 1
            } else {
                // orig 0123456789
                // real 01236789   REM_UND(p_1=4,d_1='45')
                // imag 09         REM_UND(p_0=1,d_0='12345678')
                // obj  09         (real -> obj = new: REM_UND 1,'123678'; imag -> obj = unk: VOID)
                CbQueue t;
                t.write_some( d_0.beg_upto( p_1 - p_0 ) );
                t.write_some( d_0.end_from( p_1 - p_0 + d_1.size() ) );
                d_0 = std::move( t );

                d_1.free();
            }
        }
    }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    Inode               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                 &p_0;
    CbString           &d_0;
    BinStream<CbQueue>  tmp_bq_std;
};

// with cq_new
struct StdPatchVisitor_New {
    void insertion( PT &p_0, CbString &d_0 ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_insertion npv{ db, pms, pcs, p_0, d_0, &pms->cq_new };
        PatchCreatorString::operations_visitor( cp_cq_new, npv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_insert_at( p_0, as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( d_0.size() )
            pcs->_insert_wo_cr( p_0, d_0 );
    }
    void removal( PT &p_0, PT &l_0 ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_removal npv{ db, pms, pcs, p_0, l_0, &pms->cq_new };
        PatchCreatorString::operations_visitor( cp_cq_new, npv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_remove( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( l_0 )
            pcs->_remove_wo_cr( p_0, l_0 );
    }
    void new_cursor( DevId &dev_id, PI64 &loc_id, PT &pos ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_new_cursor upv{ db, pms, pcs, pos };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( loc_id )
            pcs->_new_cursor_wo_cr( dev_id, loc_id, pos, as_usr );
    }
    void set_cursor( DevId &dev_id, PI64 &loc_id, PT &pos ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_set_cursor upv{ db, pms, pcs, dev_id, loc_id, pos };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( loc_id )
            pcs->_set_cursor_wo_cr( dev_id, loc_id, pos );
    }
    void rem_cursor( DevId dev_id, PI64 &loc_id ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_rem_cursor upv{ db, pms, pcs, dev_id, loc_id };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_read( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( loc_id )
            pcs->_rem_cursor_wo_cr( dev_id, loc_id );
    }
    void add_grp_right( GrpId grp, unsigned &flags ) {
        // check that XOR on flags would work
        flags &= ~ pcs->_grp_right( grp );
        if ( not flags )
            return;

        // modifications -> unk should not modify flags from `flags`
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_add_grp_right upv{ grp, flags };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_add_rights( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( flags )
            pcs->_add_grp_right_wo_cr( grp, flags );
    }
    void rem_grp_right( GrpId grp, unsigned &flags ) {
        // check that XOR on flags would work
        flags &= pcs->_grp_right( grp );
        if ( not flags )
            return;

        // modifications -> unk should not modify flags from `flags`
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_rem_grp_right upv{ grp, flags };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_rem_rights( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( flags )
            pcs->_rem_grp_right_wo_cr( grp, flags );
    }
    void add_usr_right( UsrId usr, unsigned &flags ) {
        // check that XOR on flags would work
        flags &= ~ pcs->_usr_right( usr );
        if ( not flags )
            return;

        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_add_usr_right upv{ usr, flags };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_add_rights( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( flags )
            pcs->_add_usr_right_wo_cr( usr, flags );
    }
    void rem_usr_right( UsrId usr, unsigned &flags ) {
        // check that XOR on flags would work
        flags &= pcs->_usr_right( usr );
        if ( not flags )
            return;

        // register the new operation
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_rem_usr_right upv{ usr, flags };
        PatchCreatorString::operations_visitor( cp_cq_new, upv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_rem_rights( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( flags )
            pcs->_rem_usr_right_wo_cr( usr, flags );
    }
    void removal_und( PT &p_0, CbString &d_0 ) {
        // modifications
        CbQueue cp_cq_new = std::move( pms->cq_new );
        NewStdPatchVisitor_removal_und npv{ db, pms, pcs, p_0, d_0, &pms->cq_new };
        PatchCreatorString::operations_visitor( cp_cq_new, npv, &pms->cq_new );

        // check the rights
        if ( not pcs->_right_to_remove( as_usr ) ) {
            pms->cq_new = std::move( cp_cq_new );
            return;
        }

        // register the new operation
        if ( d_0.size() )
            pcs->_remove_wo_cr( p_0, d_0.size() );
    }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT &pos, PT old_pos ) {
        set_cursor( dev_id, loc_id, pos );
    }
    void rem_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, UsrId usr ) {
        rem_cursor( dev_id, loc_id );
    }
    DevId               cur_dev;
    UsrId               cur_usr;
    UsrId               as_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    BinStream<CbQueue>  tmp_bq_std;
};

struct UnkStdPatchVisitor_insertion { // goal: reverse unk/std order (unk will contain patch to be applied AFTER std to mimic the effect of preceding unks -- )
    void insertion( PT &p_1, CbString &d_1 ) { // INS(unk)/INS(std)
        if ( p_1 > p_0 ) {
            //  orig 012
            //  +unk 01a2  INS p_1=2,d_1=a
            //  +std 0b1a2 INS p_0=1,d_0=b
            // same as
            //  +std 0b12  INS p_0=1,d_0=b
            //  +unk 0b1a2 INS p_1=3,d_1=a
            p_1 += d_0.size();
        } else {
            //  orig 012
            //  +unk 0a12  INS p_1=1,d_1=a
            //  +std 0a1b2 INS p_0=3,d_0=b
            // same as
            //  +std 01b2  INS p_0=2,d_0=b
            //  +unk 0a1b2 INS p_1=1,d_1=a
            p_0 -= d_1.size();
        }
    }
    void removal( PT &p_1, PT l_1 ) { // REM(unk)/INS(std)
        if ( p_0 > p_1 ) {
            //  orig 012345678
            //  +unk 045678     REM p_1=1,l_1=3
            //  +std 04b5678    INS p_0=2,d_0=b
            // same as
            //  +std 01234b5678 INS p_0=5,d_0=b
            //  +unk 04b5678    REM p_1=3,l_1=a
            p_0 += l_1;
        } else {
            //  orig 012345678
            //  +unk 012348     REM p_1=5,l_1=3
            //  +std 01b2348    INS p_0=2,d_0=b
            // same as
            //  +std 01b2345678 INS p_0=5,d_0=b
            //  +unk 01b2348    REM p_1=6,l_1=a
            p_1 += d_0.size();
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 ) {
            //  orig 012
            //  +unk 01|2  NEW_CURSOR pos=2
            //  +std 0b1|2 INS p_0=1,d_0=b
            // same as
            //  +std 0b12  INS p_0=1,d_0=b
            //  +unk 0b1|2 NEW_CURSOR pos=3
            pos += d_0.size();
        }
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 ) {
            //  orig 012
            //  +unk 01|2  NEW_CURSOR pos=2
            //  +std 0b1|2 INS p_0=1,d_0=b
            // same as
            //  +std 0b12  INS p_0=1,d_0=b
            //  +unk 0b1|2 NEW_CURSOR pos=3
            pos += d_0.size();
        }
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right( UsrId usr, unsigned flags ) {}
    void removal_und( PT &p_1, const CbString &d_1 ) { removal( p_1, d_1.size() ); }
    void set_cursor_und( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und( DevId dev_id, PI64 loc_id, PT &pos, UsrId usr ) {}
    DevId               cur_dev;
    UsrId               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                  p_0;
    CbString            d_0;
};

struct UnkStdPatchVisitor_removal {
    void insertion( PT &p_1, CbString &d_1 ) { // INS(unk)/REM(std)
        if ( p_1 > p_0 ) {
            //  orig 012345678
            //  +unk 0123456b78 INS p_1=7,d_1=b
            //  +std 0456b78    REM p_0=1,l_0=3
            // same as
            //  +std 045678     REM p_0=1,l_0=3
            //  +unk 0456b78    INS p_1=4,d_1=b
            p_1 -= l_0;
        } else {
            //  orig 012345678
            //  +unk 01b2345678 INS p_1=2,d_1=b
            //  +std 01b2348    REM p_0=6,l_0=3
            // same as
            //  +std 012348     REM p_0=5,l_0=3
            //  +unk 01b2348    INS p_1=2,d_1=b
            p_0 -= d_1.size();
        }
    }
    void removal( PT &p_1, PT l_1 ) { // REM(unk)/REM(std)
        if ( p_0 >= p_1 ) {
            //  orig 012345678
            //  +unk 0345678    REM p_1=1,l_1=2
            //  +std 03478      REM p_0=3,l_0=2
            // same as
            //  +std 0123478    REM p_0=5,l_0=3
            //  +unk 03478      REM p_1=1,l_1=2
            p_0 += l_1;
        } else if ( p_0 + l_0 <= p_1 ) {
            //  orig 012345678
            //  +unk 0123458    REM p_1=6,l_1=2
            //  +std 03458      REM p_0=1,l_0=2
            // same as
            //  +std 0345678    REM p_0=1,l_0=2
            //  +unk 03458      REM p_1=4,l_1=2
            p_1 -= l_0;
        } else {
            //  orig 012345678
            //  +unk 0123678    REM p_1=4,l_1=2
            //  +std 08         REM p_0=1,l_0=5
            // same as
            //  +std 0678       REM p_0=1,l_0=5
            //  +unk 08         REM p_1=1,l_1=2
            p_1 = p_0;
        }
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT &pos ) { // NEW_CURSOR(unk)/REM(std)
        if ( pos >= p_0 + l_0 ) {
            //  orig 012345
            //  +unk 01234|5  NEW_CURSOR pos=5
            //  +std 014|5    REM p_0=2,l_0=2
            // same as
            //  +std 0145     REM p_0=2,l_0=2
            //  +unk 014|5    NEW_CURSOR pos=3
            pos -= l_0;
        } else if ( pos > p_0 ) {
            //  orig 012345
            //  +unk 012|345  NEW_CURSOR pos=3
            //  +std 01|45    REM p_0=2,l_0=2
            // same as
            //  +std 0145     REM p_0=2,l_0=2
            //  +unk 01|45    NEW_CURSOR pos=2
            pos = l_0;
        }
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT &pos ) {
        if ( pos >= p_0 + l_0 ) {
            //  orig 012345
            //  +unk 01234|5  SET_CURSOR pos=5
            //  +std 014|5    REM p_0=2,l_0=2
            // same as
            //  +std 0145     REM p_0=2,l_0=2
            //  +unk 014|5    NEW_CURSOR pos=3
            pos -= l_0;
        } else if ( pos > p_0 ) {
            //  orig 012345
            //  +unk 012|345  SET_CURSOR pos=3
            //  +std 01|45    REM p_0=2,l_0=2
            // same as
            //  +std 0145     REM p_0=2,l_0=2
            //  +unk 01|45    NEW_CURSOR pos=2
            pos = l_0;
        }
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT &p_1, const CbString &d_1 ) { removal( p_1, d_1.size() ); }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT &pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId               cur_dev;
    UsrId               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
    PT                  p_0;
    PT                  l_0;
};


struct UnkStdPatchVisitor_set_cursor {
    void insertion( PT p_1, const CbString &d_1 ) {}
    void removal( PT p_1, PT l_1 ) {}
    void new_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { if ( dev_id_std == dev_id and loc_id_std == loc_id ) loc_id = 0; }
    void set_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { if ( dev_id_std == dev_id and loc_id_std == loc_id ) loc_id = 0; }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT &p_1, const CbString &d_1 ) { removal( p_1, d_1.size() ); }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId dev_id_std;
    PI64  loc_id_std;
};

struct UnkStdPatchVisitor_rem_cursor {
    void insertion( PT p_1, const CbString &d_1 ) {}
    void removal( PT p_1, PT l_1 ) {}
    void new_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { if ( dev_id_std == dev_id and loc_id_std == loc_id ) loc_id = 0; }
    void set_cursor( DevId dev_id, PI64 &loc_id, PT pos ) { if ( dev_id_std == dev_id and loc_id_std == loc_id ) loc_id = 0; }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right( GrpId grp, unsigned flags ) {}
    void add_usr_right( UsrId usr, unsigned flags ) {}
    void rem_grp_right( GrpId grp, unsigned flags ) {}
    void rem_usr_right( UsrId usr, unsigned flags ) {}
    void removal_und    ( PT &p_1, const CbString &d_1 ) { removal( p_1, d_1.size() ); }
    void set_cursor_und ( DevId dev_id, PI64 &loc_id, PT pos, PT old_pos ) { set_cursor( dev_id, loc_id, pos ); }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) { rem_cursor( dev_id, loc_id ); }
    DevId dev_id_std;
    PI64 loc_id_std;
};

struct UnkStdPatchVisitor_xxx_grp_right {
    void insertion     ( PT p_1, const CbString &d_1 ) {}
    void removal       ( PT p_1, PT l_1 ) {}
    void new_cursor    ( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor    ( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor    ( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right ( GrpId grp, unsigned &flags ) { if ( grp == grp_std ) flags &= ~flags_std; }
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right ( GrpId grp, unsigned &flags ) { if ( grp == grp_std ) flags &= ~flags_std; }
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
    void removal_und   ( PT p_1, const CbString &d_1 ) {}
    void set_cursor_und( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {}
    void rem_cursor_und( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {}
    GrpId    grp_std;
    unsigned flags_std;
};

struct UnkStdPatchVisitor_xxx_usr_right {
    void insertion     ( PT p_1, const CbString &d_1 ) {}
    void removal       ( PT p_1, PT l_1 ) {}
    void new_cursor    ( DevId dev_id, PI64 loc_id, PT pos ) {}
    void set_cursor    ( DevId dev_id, PI64 loc_id, PT pos ) {}
    void rem_cursor    ( DevId dev_id, PI64 loc_id ) {}
    void add_grp_right ( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned &flags ) { if ( usr == usr_std ) flags &= ~flags_std; }
    void rem_grp_right ( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned &flags ) { if ( usr == usr_std ) flags &= ~flags_std; }
    void removal_und   ( PT p_1, const CbString &d_1 ) {}
    void set_cursor_und( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {}
    void rem_cursor_und( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {}
    UsrId    usr_std;
    unsigned flags_std;
};


// with cq_unk
struct StdPatchVisitor_Unk {
    void insertion( PT p_0, const CbString &d_0 ) {
        CbQueue tmp_cq_unk;
        UnkStdPatchVisitor_insertion usv{ db, pms, pcs, p_0, d_0 };
        PatchCreatorString::operations_reverse_visitor( pms->cq_unk, usv, &tmp_cq_unk );

        pms->cq_unk.clear();
        PatchCreatorString::Visitor v;
        PatchCreatorString::operations_reverse_visitor( tmp_cq_unk, v, &pms->cq_unk );
    }
    void removal( PT p_0, PT l_0 ) {
        CbQueue tmp_cq_unk;
        UnkStdPatchVisitor_removal usv{ db, pms, pcs, p_0, l_0 };
        PatchCreatorString::operations_reverse_visitor( pms->cq_unk, usv, &tmp_cq_unk );

        pms->cq_unk.clear();
        PatchCreatorString::Visitor v;
        PatchCreatorString::operations_reverse_visitor( tmp_cq_unk, v, &pms->cq_unk );
    }
    void new_cursor( DevId dev_id, PI64 loc_id, PT pos ) {
    }
    void set_cursor( DevId dev_id, PI64 loc_id, PT pos ) {
        // only things to do: remove corresponding new_cursor and set_cursor from unk
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkStdPatchVisitor_set_cursor usv{ dev_id, loc_id };
        PatchCreatorString::operations_visitor( cp_cq_unk, usv, &pms->cq_unk );
    }
    void rem_cursor( DevId dev_id, PI64 loc_id ) {
        // only things to do: remove corresponding new_cursor and set_cursor from unk
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkStdPatchVisitor_rem_cursor usv{ dev_id, loc_id };
        PatchCreatorString::operations_visitor( cp_cq_unk, usv, &pms->cq_unk );
    }
    void add_grp_right( GrpId grp, unsigned flags ) {
        // only things to do: keep only what is necessary in flags_unk
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkStdPatchVisitor_xxx_grp_right usv{ grp, flags };
        PatchCreatorString::operations_visitor( cp_cq_unk, usv, &pms->cq_unk );
    }
    void add_usr_right( UsrId usr, unsigned flags ) {
        // only things to do: keep only what is necessary in flags_unk
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkStdPatchVisitor_xxx_usr_right usv{ usr, flags };
        PatchCreatorString::operations_visitor( cp_cq_unk, usv, &pms->cq_unk );
    }
    void rem_grp_right( GrpId grp, unsigned flags ) {
        // only things to do: keep only what is necessary in flags_unk
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkStdPatchVisitor_xxx_grp_right usv{ grp, flags };
        PatchCreatorString::operations_visitor( cp_cq_unk, usv, &pms->cq_unk );
    }
    void rem_usr_right( UsrId usr, unsigned &flags ) {
        // only things to do: keep only what is necessary in flags_unk
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkStdPatchVisitor_xxx_usr_right usv{ usr, flags };
        PatchCreatorString::operations_visitor( cp_cq_unk, usv, &pms->cq_unk );
    }
    void removal_und( PT p_0, const CbString &d_0 ) {
        removal( p_0, d_0.size() );
    }
    void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) {
        set_cursor( dev_id, loc_id, pos );
    }
    void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr ) {
        rem_cursor( dev_id, loc_id );
    }
    DevId               cur_dev;
    UsrId               cur_usr;
    PatchMergerString  *pms;
    PatchCreatorString *pcs;
};

}


PatchMergerString::PatchMergerString( PatchCreatorString *pcs ) : pcs( pcs ) {
}

bool PatchMergerString::is_valid( const CbString &msg ) const {
    CbStringPtr cp = msg;
    PatchCreatorString::Visitor v;
    return PatchCreatorString::operations_visitor( cp, v ) == 0;
}

void PatchMergerString::set_creator( UsrId usr_id ) {
    pcs->_set_creator( usr_id );
}

void PatchMergerString::new_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) {
    // goal:
    // - make a copy of the patch data, with local identifiers (if dev_id == cur_dev -> dev_id = 0, ...)
    // - make the changes
    // - update cq_new describing changes that std_ and unk_ patches do not know
    CbQueue tmp_cq_new;
    NewPatchVisitor npc{ cur_usr, cur_dev, as_usr, this, pcs, &tmp_cq_new };
    PatchCreatorString::operations_visitor( data, npc );
    if ( nb_rem_unk or nb_rem_std )
        cq_new.write_some( tmp_cq_new ); // if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
    data = std::move( tmp_cq_new );
}

void PatchMergerString::unk_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) {
    // goal:
    // - update the patch data (tmp_cq_unk) according to new stuff
    // - make the changes
    // - update cq_unk describing changes that new_ patches do not know
    CbQueue tmp_cq_unk;
    UnkPatchVisitor npc{ cur_usr, cur_dev, as_usr, this, pcs, &tmp_cq_unk };
    PatchCreatorString::operations_visitor( data, npc, &tmp_cq_unk );
    if ( nb_rem_new )
        cq_unk.write_some( tmp_cq_unk );
    data = std::move( tmp_cq_unk );
}

void PatchMergerString::std_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) {
    // goal:
    // - update std data according to new stuff
    // - make new' (new after std)
    // - make the operations
    CbQueue res;
    StdPatchVisitor_New npc{ db, as_usr, this, pcs, &res };
    PatchCreatorString::operations_visitor( data, npc, &res );
    data = std::move( res );

    // goal:
    // - make unk' (unk after std)
    StdPatchVisitor_Unk v_u{ db, this, pcs };
    PatchCreatorString::operations_visitor( data, v_u );
}

void PatchMergerString::undo( const CbString &data, const FqId &id ) {
    struct VisitorUndo {
        void insertion      ( PT p, const CbString &d )                       { pcs->_remove_wo_cr( p, d.size() ); }
        void removal        ( PT p, PT l )                                    { ERROR( "should not happen" ); }
        void new_cursor     ( DevId dev_id, PI64 loc_id, PT pos )             { pcs->_rem_cursor_wo_cr( dev_id, loc_id ); }
        void set_cursor     ( DevId dev_id, PI64 loc_id, PT pos )             { WARNING( "should not happen" ); }
        void rem_cursor     ( DevId dev_id, PI64 loc_id )                     { WARNING( "should not happen" ); }
        void add_grp_right  ( GrpId grp, unsigned flags )                     { pcs->_rem_grp_right_wo_cr( grp, flags ); }
        void add_usr_right  ( UsrId usr, unsigned flags )                     { pcs->_rem_usr_right_wo_cr( usr, flags ); }
        void rem_grp_right  ( GrpId grp, unsigned flags )                     { pcs->_add_grp_right_wo_cr( grp, flags ); }
        void rem_usr_right  ( UsrId usr, unsigned flags )                     { pcs->_add_usr_right_wo_cr( usr, flags ); }
        void removal_und    ( PT p, const CbString &d )                       { pcs->_insert_wo_cr( p, d ); }
        void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) { pcs->_set_cursor_wo_cr( dev_id, loc_id, old_pos ); }
        void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr )  { pcs->_new_cursor_wo_cr( dev_id, loc_id, pos, usr ); }
        Inode               cur_usr;
        DevId               cur_dev;
        PatchMergerString  *pms;
        PatchCreatorString *pcs;
    };
    VisitorUndo vund{ cur_usr, cur_dev, this, pcs };
    PatchCreatorString::operations_reverse_visitor( data, vund );
}

void PatchMergerString::finalize( const FqId &id ) {
}

void PatchMergerString::transmission( BinStream<CbQueue> bq, const CbString &patch_data, const FqId &id, UsrId as_usr ) {
    struct GetChangesVisitor {
        void insertion      ( PT p, const CbString &d                       ) { bn << PatchCreatorString::INSERTION << p << d; }
        void removal        ( PT p, PT l                                    ) { bn << PatchCreatorString::REMOVAL << p << l; WARNING( "This kind of removal should not appear in changes registered locally" ); }
        void new_cursor     ( DevId dev_id, PI64 loc_id, PT pos             ) { bn << PatchCreatorString::NEW_CURSOR << ( dev_id ? dev_id : cur_dev ) << loc_id << pos; }
        void set_cursor     ( DevId dev_id, PI64 loc_id, PT pos             ) { bn << PatchCreatorString::SET_CURSOR << ( dev_id ? dev_id : cur_dev ) << loc_id << pos; }
        void rem_cursor     ( DevId dev_id, PI64 loc_id                     ) { bn << PatchCreatorString::REM_CURSOR << ( dev_id ? dev_id : cur_dev ) << loc_id; WARNING( "This kind of removal should not appear in changes registered locally" ); }
        void add_grp_right  ( GrpId grp, unsigned flags                     ) { bn << PatchCreatorString::PATCH_TYPE_ADD_GRP_RIGHT << grp.glo_dev( cur_dev ) << flags; }
        void rem_grp_right  ( GrpId grp, unsigned flags                     ) { bn << PatchCreatorString::PATCH_TYPE_REM_GRP_RIGHT << grp.glo_dev( cur_dev ) << flags; }
        void add_usr_right  ( UsrId usr, unsigned flags                     ) { bn << PatchCreatorString::PATCH_TYPE_ADD_USR_RIGHT << ( usr ? usr : cur_usr ).glo_dev( cur_dev ) << flags; }
        void rem_usr_right  ( UsrId usr, unsigned flags                     ) { bn << PatchCreatorString::PATCH_TYPE_REM_USR_RIGHT << ( usr ? usr : cur_usr ).glo_dev( cur_dev ) << flags; }
        void removal_und    ( PT p, const CbString &d                       ) { bn << PatchCreatorString::REMOVAL << p << d.size(); }
        void set_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, PT old_pos ) { bn << PatchCreatorString::SET_CURSOR << ( dev_id ? dev_id : cur_dev ) << loc_id << pos; }
        void rem_cursor_und ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr  ) { bn << PatchCreatorString::REM_CURSOR << ( dev_id ? dev_id : cur_dev ) << loc_id; }
        BinStream<CbQueue> bn;
        DevId              cur_dev;
        Inode              cur_usr;
    };

    CbQueue cn;
    GetChangesVisitor gc{ &cn, db };
    PatchCreatorString::operations_visitor( patch_data, gc );
    bq << std::move( cn );
}

} // namespace RiotBase
