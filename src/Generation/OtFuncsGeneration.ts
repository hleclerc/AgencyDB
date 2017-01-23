class Operation {
    name : string;
    attrs: Array<{ name: string, type: string }>;
}
class Right {
    name : string;
}

export default
class OtFuncsGeneration {
    reg_operation( name: string, attrs: Array<{ name: string, type: string }> ) {
        this.operations.push( { name, attrs } );
    }
    
    reg_right( name: string ) {
        this.rights.push( { name } );
    }

    reg_nuf( op_n: string, op_o: string, func: ( asu_n, bq_n, bq_o, n, o ) => any ) {
    }


    operations = new Array<Operation>();
    rights     = new Array<Right>();
    nufs       = {} as { [key: string]: ( asu_n, bq_n, bq_o, n, o ) => any };
} 
