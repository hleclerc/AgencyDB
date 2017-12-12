export default {
    set( cname : string, cvalue : string, exdays : number ) {
        var d = new Date;
        d.setTime( d.getTime() + ( exdays * 24 * 60 * 60 * 1000 ) );
        document.cookie = cname + "=" + cvalue + "; expires=" + d.toUTCString();
    },

    del( cname : string ) {
        document.cookie = cname + "=; expires=Thu, 01 Jan 1970 00:00:00 UTC";
    },

    get( cname : string ) {
        var name = cname + "=";
        var ca = document.cookie.split( ';' );
        for( let i = 0; i < ca.length; i++ ) {
            let c = ca[i];
            while ( c.charAt(0) == ' ' )
                c = c.substring( 1 );
            if ( c.indexOf( name ) == 0 )
                return c.substring( name.length, c.length );
        }
        return "";
    },
};
