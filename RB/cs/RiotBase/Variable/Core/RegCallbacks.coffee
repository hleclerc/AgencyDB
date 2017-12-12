callbacks = []
timeout   = undefined

call_all = ->
    timeout = undefined
    for c in callbacks
        c()
    callbacks = []

module.exports =
    add: ( l ) ->
        if l instanceof Array
            for f in l
                this.add f
        else
            if l not in callbacks
                callbacks.push l
                if not timeout?
                    timeout = setTimeout call_all, 1
