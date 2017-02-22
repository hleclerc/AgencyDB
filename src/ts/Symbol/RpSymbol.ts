/// <reference path="../../node_modules/@types/node/index.d.ts"/>
import Variable  from "../Core/Variable"
import { cd }    from "./Codegen"
import Sym       from "./Sym"

export default 
class RpSymbol extends Sym {
    constructor( type: typeof Variable, name: string ) {
        super( RpSymbol );
        this.type     = type;
        this.name     = name;
    }

    variable_type__b() {
        return this.type;
    }

    to_String__b(): string {
        return this.name;
    }

    key_type__b(): string {
        return ( this.type as any ).key_type;
    }

    val_type__b(): string {
        return ( this.type as any ).val_type;
    }

    block_code( cg, options ): void {
        cd( this ).var_names.push( this.name );
    }

    // make_ot_wrapper() : Rp {
    //     return new OtWrapperString( this );
    // }

    // get_patch_type( bw : BinaryWriter ) {
    //     bw.write_PT( PatchTypes.num.OtWrapperString_RpSymbol );
    // }

    // cmp_patch_type( br : BinaryReader ) : boolean {
    //     return br.read_PT() == PatchTypes.num.OtWrapperString_RpSymbol;
    // }

    // copy() {
    //     return new String( this.data );
    // }

    // write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
    //     bw.write_PT( DeclTypesForRW.num.RpSymbol );
    //     bw.write_String( this.data );
    // }

    // static read_RpSymbol_from( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : String {
    //     return new String( br.read_String() );
    // }

    type    : typeof Variable; /**  */
    name    : string;
} 
Sym.make_templ( RpSymbol );
