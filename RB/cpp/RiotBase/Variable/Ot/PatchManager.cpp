#include "PatchManager.h"
#include "OtMerger.h"

namespace RiotBase {

PatchManager::PatchManager( Rp *rp ) : rp( rp ) {
}

PatchManager::~PatchManager() {
    if ( rp )
        RO_CALL( del, rp );
}

OtMessage *PatchManager::get_changes( PVec<DevId> dsts, DevId cur_dev, UsrId cur_usr ) {
    // update the patches if there are changes
    freeze_last_mod();

    // find data to be sent for each dev
    Vec<std::pair<SetOfPatchId,Vec<DevId>>> be;
    for( DevId d : dsts ) {
        SetOfPatchId to_be_sent = SetOfPatchId( patch_ids ) - ext_states[ d ].sent;
        if ( not to_be_sent.empty() ) {
            for( PT i = 0; ; ++i ) {
                if ( i == be.size() ) {
                    std::pair<SetOfPatchId,Vec<DevId>> *ne = be.push_back();
                    ne->second << d;
                    ne->first = to_be_sent;
                    break;
                }
                if ( be[ i ].first == to_be_sent ) {
                    be[ i ].second << d;
                    break;
                }
            }
        }
    }

    // nothing to be sent
    if ( not be.size() )
        return 0;

    // for each patch set
    OtMessage *res = 0;
    for( std::pair<SetOfPatchId,Vec<DevId>> &ps : be ) {
        // ptr of patches to be sent
        const SetOfPatchId &new_patches = ps.first;
        Vec<const Patch *> patches( Rese(), new_patches.size() );
        for( const auto &p : new_patches.data ) {
            DevId dev = p.first;
            for( PI64 num : p.second ) {
                PatchId pid( dev, num );
                for( const Patch *p = last_patch.ptr(); p; p = p->prev.ptr() ) {
                    if ( p->patch_id == pid ) {
                        patches << p;
                        break;
                    }
                }
            }
        }

        // make the message
        if ( patches.size() ) {
            // sort new patches by position (needed because we use the prev one to known where to place the new one)
            std::sort( patches.begin(), patches.end(), []( const Patch *a, const Patch *b ) {
                return a->num < b->num;
            } );

            // make a new OtMessage
            res = new OtMessage( res );
            res->dsts = ps.second;

            // msg header
            BinStream<CbQueue> bq( &res->msg );

            // patches that are assumed to be known on the other side
            TODO; // VectorClock
            SetOfPatchId assumed_to_be_known = SetOfPatchId( patch_ids ) - new_patches;
            assumed_to_be_known.write_to( bq, cur_dev );

            // msg content
            AutoPtr<OtMerger> ot_merger = RO_CALL( new_ot_merger, rp );
            for( const Patch *p : patches ) {
                // patch header
                bq << ( p->prev ? p->prev->patch_id.glo_dev( cur_dev ) : PatchId{} ); // coordinates of the previous patch
                bq << p->patch_id.glo_dev( cur_dev );                                 // coordinates of the current patch
                bq << p->usr_id.glo_dev( cur_dev, cur_usr );                          // user who has created this patch
                bq << ( p->prev ? p->date - p->prev->date : p->date );                // (delta) creation date

                // content
                CbQueue glo;
                ot_merger->loc_to_glo( &glo, p->data, cur_dev, cur_usr );
                bq << std::move( glo );

                // say that the patch has been sent
                for( const DevId &d : res->dsts )
                    ext_states[ d ].sent += p->patch_id;
            }
        }
    }

    return res;
}

void PatchManager::app_changes( BBQ ans, CbStringPtr changes, const DevId &src, PVec<DevId> dsts, DevId cur_dev, UsrId cur_usr ) {
    // update the patches if there are changes
    freeze_last_mod();

    //
    BinStream<CbStringPtr> bp( &changes );

    //
    SetOfPatchId assumed_to_be_known = SetOfPatchId::read_from( bp, cur_dev );

    SetOfPatchId should_be_known = assumed_to_be_known - patch_ids;
    if ( not should_be_known.empty() )
        IMPORTANT_TODO; // complain :)

    // read the patches
    SetOfPatchId unkn = SetOfPatchId( patch_ids ) - assumed_to_be_known; // patches that are not known by the sender
    AutoPtr<OtMerger> merger = RO_CALL( new_ot_merger, rp );
    Vec<Patch *> new_patches;
    while ( not bp.empty() ) {
        // find p, the previous patch (if there is one)
        Patch *p = 0;
        PatchId pid( bp, cur_dev );
        if ( pid.num ) {
            for( p = last_patch.ptr(); p; p = p->prev.ptr() )
                if ( p->patch_id == pid )
                    break;
            if ( not p )
                break;
        } else
            p = 0;

        // patch header
        PatchId new_patch_id( bp, cur_dev );
        UsrId   new_usr_id ( bp, cur_dev, cur_usr );
        Date    new_date = bp.read();

        // update ext_states (what is known and can be assumed to be sent to the other machines)
        ext_states[ src ].known += new_patch_id;
        ext_states[ src ].sent  += new_patch_id;
        for( DevId d : dsts )
            if ( d != cur_dev )
                ext_states[ d ].sent += new_patch_id;

        // we already have this patch ?
        if ( patch_ids.contains( new_patch_id ) ) {
            unkn -= new_patch_id;
            bp.skip_string();
            continue;
        }

        // else, create the patch and read the content
        Patch *n = new Patch;
        n->patch_id = new_patch_id;
        n->usr_id   = new_usr_id;
        n->date     = p ? p->date + new_date : new_date;

        if ( not merger->glo_to_loc( &n->data, bp.read_CbString(), cur_dev, cur_usr ) ) {
            delete n;
            break;
        }

        new_patches << n;

        // say that we known it (as the sender, and the subscribers to which the message has been sent)
        patch_ids += new_patch_id;

        // find where to place to patch
        auto glo_dev = [ cur_dev ]( DevId dev ) { return dev ? dev : cur_dev; };

        if ( p )
            while( p->next and ( p->next->date < n->date or ( p->next->date == n->date and glo_dev( p->next->patch_id.dev ) < glo_dev( new_patch_id.dev ) ) ) )
                p = p->next;
        else if ( Patch *f = first() )
            for( n->date = f->date; f and f->date == n->date and glo_dev( f->patch_id.dev ) < glo_dev( new_patch_id.dev ); f = f->next )
                p = f;

        // place it
        if ( p ) { // after p
            if ( p->next )
                p->next->prev.data = n;
            else
                last_patch.data = n;
            n->next = p->next;
            n->prev = p;
            p->next = n;
            n->num  = p->num + 1;
        } else { // or at the beginning
            if ( Patch *f = first() ) {
                f->prev = n;
                n->next = f;
            } else {
                last_patch = n;
                n->next    = 0;
            }
            n->prev = 0;
            n->num  = 0;
        }

        // update num for subsequent patches
        for( Patch *o = n->next; o; o = o->next )
            o->num = o->prev->num + 1;
    }

    // update patch content
    if ( new_patches.size() ) {
        // find the first patch in new patches
        Patch *f = new_patches[ 0 ];
        for( PT i = 1; i < new_patches.size(); ++i )
            if ( f->num > new_patches[ i ]->num )
                f = new_patches[ i ];
        // and in unknown patches
        for( const auto &pa : unkn.data )
            for( PI64 pn : pa.second )
                if ( Patch *p = find_patch( pn, pa.first ) )
                    if ( f->num > p->num )
                        f = p;

        // undo stuff made in unk and std patches
        PT nb_std = 0;
        for( Patch *o = last_patch.ptr(); o; o = o->prev.ptr() ) {
            if ( not new_patches.contains( o ) )
                merger->undo( o->data, o->usr_id );
            ++nb_std;
            if ( o == f )
                break;
        }


        // merge the data
        PT nb_new = new_patches.size(), nb_unk = unkn.size();
        nb_std -= nb_new + nb_unk;
        do {
            //if ( not f->prev )
            //    merger->set_creator( f->usr_id );

            CbQueue pd = std::move( f->data );
            if ( new_patches.contains( f ) ) {
                merger->new_patch( &f->data, pd, f->usr_id, nb_unk, nb_std );
                --nb_new;
            } else if ( unkn.contains( f->patch_id ) ) {
                merger->unk_patch( &f->data, pd, f->usr_id, nb_new, nb_std );
                --nb_unk;
            } else {
                merger->std_patch( &f->data, pd, f->usr_id, nb_unk, nb_new );
                --nb_std;
            }
        } while (( f = f->next ));

        merger->finalize();
    }

    // acknowledgement
    // BinStream<CbQueue> bns( &ans );
    // patch_ids.write_to( bns, cur_dev );
}

void PatchManager::freeze_last_mod() {
    CbQueue operations;
    RO_CALL( pull_ot_changes, rp, &operations );
    if ( not operations.empty() ) {
        // new patch
        Patch *p        = new Patch;
        p->data         = std::move( operations );
        p->patch_id.dev = 0;
        p->patch_id.num = new_patch_id_num();
        p->date         = Date::current();
        p->num          = last_patch ? last_patch->num + 1 : 0;

        // store it in the list (at the end)
        p->prev = last_patch.data;
        p->next = 0;
        if ( last_patch )
            last_patch->next = p;
        last_patch.data = p;

        // reg the new patch id
        patch_ids += p->patch_id;
    }
}

PI64 PatchManager::new_patch_id_num() {
    for( const Patch *p = last_patch.ptr(); p; p = p->prev.ptr() )
        if ( not p->patch_id.dev )
            return p->patch_id.num + 1;
    return 1;
}

PatchManager::Patch *PatchManager::first() {
    if ( Patch *res = last_patch.ptr() ) {
        while( res->prev )
            res = res->prev.ptr();
        return res;
    }
    return 0;
}


PatchManager::Patch *PatchManager::find_patch( PI64 num, DevId dev ) {
    for( Patch *p = last_patch.ptr(); p; p = p->prev.ptr() )
        if ( p->patch_id.num == num and p->patch_id.dev == dev )
            return p;
    return 0;
}

} // namespace RiotBase
