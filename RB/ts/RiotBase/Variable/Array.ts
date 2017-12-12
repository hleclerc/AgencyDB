import Variable from "./Core/Variable"
import Rp       from "./Core/Rp"

declare class Proxy { constructor( o: any, h: any ); }; // <T>( obj : T, handler ) : Array<T>;
declare var global: any;

//
export default
class Array<T> extends Variable {
    static NativeType = ( typeof self != "undefined" ? self : global ).Array // shortcut for people willing to import this as 'Array'

    constructor( val : any = [] ) {
        if      ( val instanceof Rp               ) super( val );
        else if ( val instanceof Array.NativeType ) super( new GenArray( val ) );
        else if ( val instanceof Array            ) super( val.copy().rp );
        else throw new Error( "TODO" );

        if ( typeof Proxy != "undefined" ) {
            let proxy_handler = {
                get( o, n ) {
                    if ( typeof( n ) == "string" ) {
                        let index = parseInt( n, 10 );
                        if ( ! isNaN( index ) )
                            return o.at( index );
                    }
                    return o[ n ];
                },
                set( o, n, v ) {
                    o[ n ] = v;
                    return true;
                }
            }
            return <Array<T>>( new Proxy( this, proxy_handler ) );
        }
    }

    copy() : Array<T> {
        return Method.methods["copy"].call_1( this );
    }

    get length() : Number {
        return Method.methods["get_size"].call_1( this );
    }

    get val() {
        return this.toArray();
    }

    set val( val ) {
        let ol = this.length.val;
        let nl = val.length;
        if ( ol > nl )
            this.splice( nl, ol - nl );
        for( let i = 0; i < Math.min( ol, nl ); i += 1 )
            this.set_item( i, val[ i ] );
        for( let i = ol; i < nl; i += 1 )
            this.push( val[ i ] );
    }

    set_item( key : number | Number, val ) : boolean {
        let rp = Variable.rp_conv_on( val );
        return rp.o ? Method.methods["self_set_o"].call_2( this.get_item( key ), rp.v ) : Method.methods["self_set_b"].call_2( this.get_item( key ), rp.v );
    }

    get_item( key : number | Number ) : T {
        return Method.methods["mod_select"].call_2( this, Number.rp_conv( key ) );
    }

    at( key : number | Number ) : T {
        return this.get_item( key );
    }

    push( item ) : Array<T> {
        this.insert( this.length, item );
        return this;
    }

    insert( pos, item ) : Array<T> {
        let rp = Variable.rp_conv_on( item );
        if ( rp.o )
            Method.methods["self_insert_bo"].call_3( this, Number.rp_conv( pos ), rp.v );
        else
            Method.methods["self_insert_bb"].call_3( this, Number.rp_conv( pos ), rp.v );
        return this;
    }

    splice( pos : number | Number, len : number | Number = 1, ...items ) : Array<T> {
        let vlen = <Number>( Number.rp_conv( len ) );
        if ( vlen.val )
            Method.methods["self_remove_bb"].call_3( this, Number.rp_conv( pos ), vlen );
        for( let item of items ) {
            let rp = Variable.rp_conv_on( item );
            if ( rp.o )
                Method.methods["self_insert_bo"].call_3( this, Number.rp_conv( pos ), rp.v );
            else
                Method.methods["self_insert_bb"].call_3( this, Number.rp_conv( pos ), rp.v );
        }
        return this;
    }

    // concat()	Joins two or more arrays, and returns a copy of the joined arrays
    // copyWithin()	Copies array elements within the array, to and from specified positions
    // every()	Checks if every element in an array pass a test
    // fill()	Fill the elements in an array with a static value
    // filter()	Creates a new array with every element in an array that pass a test
    // find()	Returns the value of the first element in an array that pass a test
    // findIndex()	Returns the index of the first element in an array that pass a test
    // forEach()	Calls a function for each array element
    // indexOf()	Search the array for an element and returns its position
    // isArray()	Checks whether an object is an array
    // join()	Joins all elements of an array into a string
    // lastIndexOf()	Search the array for an element, starting at the end, and returns its position
    // map()	Creates a new array with the result of calling a function for each array element
    // pop()	Removes the last element of an array, and returns that element
    // push()	Adds new elements to the end of an array, and returns the new length
    // reduce()	Reduce the values of an array to a single value (going left-to-right)
    // reduceRight()	Reduce the values of an array to a single value (going right-to-left)
    // reverse()	Reverses the order of the elements in an array
    // shift()	Removes the first element of an array, and returns that element
    // slice()	Selects a part of an array, and returns the new array
    // some()	Checks if any of the elements in an array pass a test
    // sort()	Sorts the elements of an array
    // splice()	Adds/Removes elements from an array
    // toString()	Converts an array to a string, and returns the result
    // unshift()	Adds new elements to the beginning of an array, and returns the new length
    // valueOf()	Returns the primitive value of an array
}

import GenArray from "./Array/GenArray";
import DevId    from "../System/DevId";
import Method   from "./Core/Method";
import Number   from "./Number";
