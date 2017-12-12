__require_map = {}

if window?
    window.process = env: NODE_ENV: "debug"

__req = ( path, url ) ->
    # ./foo -> /pa/th/foo; ./../foo -> /pa/foo
    rel = false
    while true
        if url.substr( 0, 2 ) == './'
            url = url.substr 2
            rel = true
        else if url.substr( 0, 3 ) == '../'
            path = path.substr 0, path.lastIndexOf '/'
            url = url.substr 3
            rel = true
        else
            break
    if rel
        url = path + '/' + url

    #
    if not __require_map[ url ]?
        if not __required_as[ url ]?
            console.error "Pb importing #{url}"
        url = __required_as[ url ]

    #
    src = __require_map[ url ]
    if not src?
        console.error "Pb with #{url} (coming from __required_as)"

    if not src.module?
        __req.execution.push url
        src.exports = {}
        src.module = exports: src.exports
        src.fun src.module, src.exports, ( wanted ) ->
            par = if url.lastIndexOf( '/' ) >= 0 then url.slice( 0, url.lastIndexOf( '/' ) ) else "" # parent directory
            __req par, wanted
        __req.execution.pop()
    # else if src.module.exports == src.base_exports and not src.module.exports.default?
    #     console.error "Recursion pb with #{url} (executing:\n  #{__req.execution.join "\n  "})"
    #src.module.exports
    if src.exports.default?
        src.exports
    else
        src.module.exports


__req.execution = []

__register = ( url, fun ) ->
    __require_map[ url ] = fun: fun
