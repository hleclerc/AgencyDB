import methods       from './methods'  
import Rp            from './Rp'  

/**  */
export default
class RpList extends Rp {
    constructor( cur: Rp, nxt: Rp ) {
        super( RpList );
        this.cur = cur;
        this.nxt = nxt;
    }

    static append( lst: Rp, val: Rp ): Rp {
        return lst instanceof RpList ?
            new RpList( lst.cur, RpList.append( lst.nxt, val ) ) :
            new RpList( lst, val );
    }

    copy__b(): Rp {
        return new RpList( methods[ "copy__b" ].call_1( this.cur ), methods[ "copy__b" ].call_1( this.nxt ) );
    }

    to_String__b(): string {
        let lst = new Array<Rp>();
        this.get_lst( lst );
        return `[${ lst.map( x => methods[ "to_String__b" ].call_1( x ) ).join( "," ) }]`;
    }
   
    get_lst( lst: Array<Rp> ) {
        lst.push( this.cur );
        this.nxt instanceof RpList ? 
            this.nxt.get_lst( lst ) : 
            lst.push( this.nxt );
    }

    cur: Rp;
    nxt: Rp;
}
Rp.make_templ( RpList );

