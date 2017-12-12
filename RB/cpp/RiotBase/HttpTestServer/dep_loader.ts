function __dep_loader( dependencies : Array<string>, callback ) {
    let nb_to_load = dependencies.length;
    for( let dep of dependencies ) {
        let xhr = new XMLHttpRequest;
        xhr.open( "GET", dep, true );
        xhr.onreadystatechange = function( aEvt ) {
            if ( xhr.readyState == 4 ) {
                if ( xhr.status == 200 ) {
                    eval( xhr.responseText );
                    if ( --nb_to_load == 0 )
                        callback();
                } else
                    console.error( "Pb loading " + dep + ".", xhr.responseText );
             }
        }
        xhr.send();
    }
}
