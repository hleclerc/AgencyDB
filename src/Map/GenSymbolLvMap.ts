import GenSymbol  from "../Symbol/GenSymbol"
import { VarAnc } from "../Core/Variable"
import Rp         from "../Core/Rp"
import LvMap      from "../LvMap"

export default class GenSymbolLvMap extends GenSymbol {
    constructor( key_type: { new( rp?: Rp ): VarAnc; }, val_type: { new( rp?: Rp ): VarAnc; }, name: string ) {
        super( LvMap, name, GenSymbolLvMap );
        this.key_type = key_type;
        this.val_type = val_type;
    }

    key_type__b() {
        return this.key_type;
    }

    val_type__b() {
        return this.val_type;
    }

    key_type: { new( rp?: Rp ): VarAnc; };
    val_type: { new( rp?: Rp ): VarAnc; };
}
GenSymbol.make_templ( GenSymbolLvMap );
