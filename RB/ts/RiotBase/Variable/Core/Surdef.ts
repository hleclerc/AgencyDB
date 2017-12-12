declare class Error {
    stack: string;
}

///
export default
class Surdef {
    constructor( pertinence : number, test, func ) {
        this.pertinence = pertinence;
        this.stack      = new Error().stack;
        this.test       = test;
        this.func       = func;
    }
    pertinence : number;
    stack      : string;
    test       : any;
    func       : ( ...any ) => any;
}
