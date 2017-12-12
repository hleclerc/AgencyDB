import WithComparison = require( "./WithComparison" );

// good for search. Bad for insertion
export default
class SortedArray<T extends WithComparison.CmpInterface> {
    constructor( items : Array<T> = [] ) {
        this.items = items;
    }

    toString() : string {
        return "" + this.items;
    }

    // if val_if_not_found is defined, it is called to make a new item
    // else, if not item found for key `key`, return undefined
    get( key, val_if_not_found? ) {
        let beg = 0;
        let end = this.items.length;
        while ( end > beg ) {
            let mid = Math.floor( beg + ( end - beg ) / 2 );
            if ( this.items[ mid ].inf( key ) )
                beg = mid + 1;
            else
                end = mid;
        }
        if ( beg == this.items.length || this.items[ beg ].sup( key ) ){
            var res;
            if ( val_if_not_found != null )
                res = val_if_not_found();
            if ( res != null )
                this.items.splice( beg, 0, res );
            return res;
        }
        return this.items[ beg ];
    }

    // set or replace
    set( key, val : T ) {
        let beg = 0;
        let end = this.items.length;
        while ( end > beg ) {
            let mid = Math.floor( beg + ( end - beg ) / 2 );
            if ( this.items[ mid ].inf( key ) )
                beg = mid + 1;
            else
                end = mid;
        }
        if ( beg == this.items.length || this.items[ beg ].sup( key ) )
            this.items.splice( beg, 0, val );
        else
            this.items.splice( beg, 1, val );
    }

    del( key ) {
        let beg = 0;
        let end = this.items.length;
        while ( end > beg ) {
            let mid = Math.floor( beg + ( end - beg ) / 2 );
            if ( this.items[ mid ].inf( key ) )
                beg = mid + 1;
            else
                end = mid;
        }
        if ( beg != this.items.length && ! this.items[ beg ].sup( key ) )
            this.items.splice( beg, 1 );
    }

    forEach( fun ) : void {
        this.items.forEach( fun );
    }

    items : Array<T>;
}
