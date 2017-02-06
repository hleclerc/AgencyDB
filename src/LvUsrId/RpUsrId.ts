import UsrId           from "../System/UsrId"
import methods         from "../Core/methods"
import Rp              from "../Core/Rp"

export default 
class RpUsrId extends Rp {
    constructor( data: UsrId ) {
        super( RpUsrId );
        this.data = data;
    }

    variable_type__b() {
        return UsrId;
    }

    to_String__b(): string {
        return this.data.toString();
    }

    js_repr__b( prec: number ): string {
        return `new UsrId("${ this.data.dev.toString() },${ this.data.num })")`;
    }

    copy__b(): Rp {
        return new RpUsrId( this.data.copy() );
    }

    to_Boolean__b(): boolean {
        return this.data.ext;
    }

    data: UsrId;
} 
Rp.make_templ( RpUsrId );

methods[ "set__sb" ].add_surdef( 10, [ RpUsrId, RpUsrId ], ( a: RpUsrId, b: RpUsrId ) => { a.data = b.data.copy(); return a; } );



