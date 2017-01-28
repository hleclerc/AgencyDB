/// <reference path="../../node_modules/@types/node/index.d.ts"/>
import Variable  from "../Core/Variable"
import { cd }    from "./Codegen"
import Sym       from "./Sym"

export default 
class GenSymbol extends Sym {
    constructor( type: typeof Variable, name: string, p = GenSymbol as any ) {
        super( p );
        this.type = type;
        this.name = name;
    }

    variable_type__b() {
        return this.type;
    }

    to_String__b(): string {
        return this.name;
    }

    block_code( cg, options ): void {
        cd( this ).var_names.push( this.name );
    }

    // make_ot_wrapper() : Rp {
    //     return new OtWrapperString( this );
    // }

    // get_patch_type( bw : BinaryWriter ) {
    //     bw.write_PT( PatchTypes.num.OtWrapperString_GenSymbol );
    // }

    // cmp_patch_type( br : BinaryReader ) : boolean {
    //     return br.read_PT() == PatchTypes.num.OtWrapperString_GenSymbol;
    // }

    // copy() {
    //     return new String( this.data );
    // }

    // write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
    //     bw.write_PT( DeclTypesForRW.num.GenSymbol );
    //     bw.write_String( this.data );
    // }

    // static read_GenSymbol_from( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : String {
    //     return new String( br.read_String() );
    // }

    type: typeof Variable; /**  */
    name: string;
} 
Sym.make_templ( GenSymbol );
