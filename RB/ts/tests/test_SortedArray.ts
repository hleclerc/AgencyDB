import SortedArray from "../RiotBase/System/SortedArray";
import check       from "../RiotBase/System/Check";

//
function test_SortedArray() {
    class Obj {
        val: number;
        constructor( val : number ) { this.val = val; }
        inf( val ) : boolean { return this.val < val }
        sup( val ) : boolean { return this.val > val }
        toString() { return "" + this.val; }
    }

    let arr = new SortedArray<Obj>( [ new Obj( 1 ) ] )
    for( let i = 0; i < 20; i += 1 ) {
        let val = Math.floor( Math.random() * 10 );
        arr.get( val, () => new Obj( val ) );
    }
    for( let i = 1; i < arr.items.length; i += 1 )
        check( arr.items[ i ] > arr.items[ i - 1 ], true );
    console.log( arr.toString() );
}

test_SortedArray()
