import RpSymbol  from "./Symbol/RpSymbol"

// export stuff for convenience
export { _while } from "./Symbol/While"
export { _if }    from "./Symbol/If"

// register operations
require( "./Symbol/Operation" );

/** to make a new symbol */
export default
function symbol<T>( type, name?: string ): T {
    return new type( new RpSymbol( type, name ) ) as T;
}
