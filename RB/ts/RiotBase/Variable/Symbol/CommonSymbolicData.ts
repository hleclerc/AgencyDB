
export default class CommonSymbolicData {
    parents             = new Array<Sym>();
    lazyfier_parents    = new Array<Lazyfier>();
    op_mp               : any;
    op_id               = 0;
    sched_id            : number;
    ww_then             : ( Variable ) => void; // for webworker
    ww_val              : Variable;

    static cur_op_id    = 0;
    static cur_sched_id = 0;
}

import Variable from "../Core/Variable";
import Lazyfier from "./Lazyfier";
import Sym      from "./Sym";
