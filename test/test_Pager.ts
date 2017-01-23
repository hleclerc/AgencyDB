/// <reference path="../node_modules/@types/mocha/index.d.ts"/>

/**  */
class Pager {
    write( sig: string, msg: string ) {
        if ( ! this.ini ) {
            this.ini = true;

            const nc = ( process.stdout as any ).columns;
            process.stdout.write( "#".repeat( nc ) + "\n" );
        }

        // where to place the message
        let ind = this.lst.findIndex( x => x.sig == sig );
        if ( ind < 0 ) {
            ind = this.lst.findIndex( x => x.free );
            if ( ind >= 0 ) {
                this.lst[ ind ].free = false;
                this.lst[ ind ].msg = msg;
            } else {
                this.lst.push( { sig, msg, free: false } );
                process.stdout.write( "\n" );
            }
        } else
            this.lst[ ind ].msg = msg;

        //
        this._display_all();
    }

    close( sig: string ) {
        const ind = this.lst.findIndex( x => x.sig == sig );
        if ( ind >= 0 ) {
            // clear the line
            // const nc = ( process.stdout as any ).columns;
            //process.stdout.write( `\u001b[${ ind + 1 }B\u001b[K\r\u001b[${ ind + 1 }A` );

            // say that it's available
            this.lst[ ind ].free = true;

            //
            this._display_all();
        }
    }

    _display_all() {
        const nc = ( process.stdout as any ).columns;
        console.log( "nc", nc );
        

        // rewind
        process.stdout.write( `\u001b[${ this.lst.length + 1 }A` );

        // header
        process.stdout.write( "#".repeat( nc ) + "\n" );

        // messages
        for( let { msg, free } of this.lst ) {
            if ( free ) {
                process.stdout.write( "\r\u001b[K\n" );
            } else {
                if ( nc && msg.length > nc )
                    msg = msg.substr( 0, nc - 3 ) + "...";
                else
                    msg += " ".repeat( nc - msg.length );
                process.stdout.write( msg.replace( /\n/g, ' ' ) + "\n" );
            }
        }
    }

    lst = new Array<{ sig: string, msg: string, free: boolean }>();
    ini = false;   /** if initialized*/
}

/**
 * OnChange; 
 *   changement permanent => remonte à chaque opération _sxx
 *   lorsque ce n'est pas symbolique => prop: on fait un proxy, mais celui-ci transfère les fonctions lorsque ça devient symbolique
 * 
 * Autre approche: 
 */
let pager = new Pager;
const seq = [
    function() { pager.write( "1", "l -- 1" ); },
    function() { pager.write( "2", "l -- 2" ); },
    function() { pager.close( "1" ); },
    function() { pager.write( "3", "3 smurf" ); },
    function() { pager.write( "4", "4" ); },
    function() { pager.write( "5", "5" ); },
];
function run() {
    setTimeout( () => { if ( seq.length ) { seq.shift()(); run(); } }, 500 );
}
run();
