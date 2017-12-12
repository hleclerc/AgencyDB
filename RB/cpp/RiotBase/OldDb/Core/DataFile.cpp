#include "../../Variable/Symbol/SetLazyVal.h"
#include "../../Variable/Symbol/Graphviz.h"
#include "../../EvLoop/Watcher.h"
#include "DbCommands.h"
#include "DataFile.h"
#include "DbItem.h"
#include "../Gev.h"
#include "../Db.h"
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>

#ifdef METIL_COMP_DIRECTIVE
#pragma lnk_flag -pthread
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

const PT   dio_alig    = 512;
const PT   dio_alig_m1 = dio_alig - 1;

void sched_th( DataFile *d ) {
    while ( true ) {
        // lock the mutex for the wait condition
        std::unique_lock<std::mutex> lock( d->mutex );
        
        // find the best thing to do (or wait)
        while ( not d->has_work_to_do() )
            d->waitc.wait( lock );

        if ( d->to_read.size() ) {
            DbItem *item = d->to_read.back();
            d->to_read.pop_back();
            lock.unlock();
            
            // read the data
            item->datafile_buffer = Buffer::New( item->datafile_len );
            item->datafile_buffer->used = item->datafile_len;
            lseek( d->fd, item->datafile_offset, SEEK_SET );
            PT to_read = item->datafile_len;
            // while ( to_read )
            PT r = read( d->fd, item->datafile_buffer->data, to_read );
            if ( r != to_read ) {
                perror( "reading..." );
                IMPORTANT_TODO;
            }
            
            // signal the availability
            DbItem::mutex_lwdb_list.lock();
            DbItem::list_with_dbuff << item;
            DbItem::mutex_lwdb_list.unlock();
            gev->sig();
        } else if ( d->to_write.size() ) {
            // copy data in an aligned memory zone
            CbQueue &cq = d->to_write.back(); PT os = cq.size(), ns = ( os + dio_alig_m1 ) & ~dio_alig_m1;
            char *buf = (char *)memalign( dio_alig, ns );
            cq.read_some( buf, os );

            // free cq and the lock
            d->to_write.pop_back();
            lock.unlock();

            // add nops is necessary
            if ( PT delta = ns - os ) {
                CmQueue cm( buf + os ); BinStream<CmQueue> bm( &cm );

                if ( delta == 1 )
                    bm << DbCommands::nop;
                else
                    bm << DbCommands::nnop << delta;
            }

            // write the data
            write( d->fd, buf, DataFile::use_dio ? ns : os );
            //            cq.data_visitor( [&]( const PI8 *b, const PI8 *e ) {
            //                int res = write( d->fd, b, e - b );
            //                PRINT( res );
            //                perror( "w" );
            //                return true;
            //            } );

            free( buf );
        } else if ( d->stop_th ) {
            break;
        } else
            ERROR( "Unknown kind of word" );
    }
}

DataFile::DataFile( std::string path, int fd, int id ) : path( path ), fd( fd ), id( id ) {
    sched_thread = std::thread( sched_th, this );
    stop_th = false;
}

DataFile::~DataFile() {
    // wait for the thread
    stop_th = true;
    waitc.notify_one();  // to wake up the thread if necessary
    sched_thread.join(); // wait

    // close
    if ( fd >= 0 )
        close( fd );
}

void DataFile::write_to_stream( std::ostream &os ) const {
    os << path;
}

bool DataFile::has_work_to_do() const {
    return stop_th or not ( to_read.empty() and to_write.empty() );
}

void DataFile::sched_read( DbItem *item ) {
    mutex.lock();
    to_read << item;
    mutex.unlock();
    waitc.notify_all();
}

void DataFile::sched_write( CbQueue &&cq ) {
    mutex.lock();
    to_write << std::move( cq );
    mutex.unlock();
    waitc.notify_all();
}

} // namespace RiotBase
