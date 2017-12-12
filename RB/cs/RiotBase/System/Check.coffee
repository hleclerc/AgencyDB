_make_req = ( msg ) ->
    req = new XMLHttpRequest
    req.open "GET", msg, true
    req.send null

module.exports = ( expr, value ) ->
    se = expr.toString()
    sv = value.toString()
    fl = ( new Error ).stack.split("\n")[ 2 ] # .replace( /^\s*at/g, '' ).replace( /^\s+|\s+$/g, '' )
    if window?
        _make_req "/_test?st=#{Number se == sv}&fl=#{encodeURIComponent fl}&se=#{encodeURIComponent se}&sv=#{encodeURIComponent sv}"
    else
        console.log fl, ( if se == sv then "OK" else "BAD" ), se

    # quit_server: ->
        # _make_req "/_quit_server"

#
#     # console feedback
#     fl = ( new Error ).stack.split("\n")[ 2 ].replace( /^\s*at/g, '' ).replace( /^\s+|\s+$/g, '' )
#     console.log fl, "(#{if se == sv then "OK" else "BAD"})", se
#
#     # send info to the server
#     req = new XMLHttpRequest
#     req.open "GET", , true
#     req.send null
