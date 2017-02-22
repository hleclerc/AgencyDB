
///
export default
class Surdef {
    constructor( pertinence: number, test, func ) {
        this.pertinence = pertinence;
        this.stack      = new Error().stack;
        this.test       = test;
        this.func       = func;
    }

    disp_pos() : string {
        let p = this.stack.split( "\n" );
        return "\n->" + ( p.length > 2 ? p[ 2 ] : this.stack );
    }
    
    pertinence : number;
    stack      : string;
    test       : any;
    func       : ( ...any ) => any;
}
