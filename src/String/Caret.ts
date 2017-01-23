// import BinaryWriter from "../../System/BinaryWriter";
// import BinaryReader from "../../System/BinaryReader";
import DevId        from "../System/DevId";
import UsrId        from "../System/UsrId";

export default
class Caret {
    // write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) : void {
    //     bw.write_PT( this.beg );
    //     bw.write_PT( this.end );
    //     bw.write_PT( this.foc );
    // }

    // static read_from( br: BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : Caret {
    //     let res = new Caret;
    //     res.beg = br.read_PT();
    //     res.end = br.read_PT();
    //     res.foc = br.read_PT();
    //     return res;
    // }

    // static skip_from( br: BinaryReader ) : void {
    //     br.skip_PT();
    //     br.skip_PT();
    //     br.skip_PT();
    // }

    static New( beg, end = beg, foc = 1 ) {
        let res = new Caret;
        res.beg = beg;
        res.end = end;
        res.foc = foc;
        return res;
    }

    equ( c : Caret ) : boolean {
        return c.beg == this.beg && c.end == this.end && c.foc == this.foc;
    }

    copy() : Caret {
        let res = new Caret;
        res.beg = this.beg;
        res.end = this.end;
        res.foc = this.foc;
        return res;
    }

    toString() : string {
        return `{beg:${this.beg},end:${this.end},foc:${this.foc}}`;
    }

    get min() : number {
        return Math.min( this.beg, this.end );
    }

    get max() : number {
        return Math.max( this.beg, this.end );
    }


    beg: number;
    end: number;
    foc: number;
}
