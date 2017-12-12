DevId = require "./DevId.js"

#
module.exports =
class Inode
    # for known inodes
    @base_dev = new DevId "----------3~"  # val = 1

    @known_inodes =
        ###~g++ -E -I`pwd`
        #define KNOWN_INODE( NAME, NUM ) NAME: new Inode Inode.base_dev, NUM
        #include <cpp/RiotBase/System/KnownInodes_Decl.h>
        ~###

    constructor: ( dev = new DevId, num = 0 ) ->
        @dev = dev
        @num = num

    toString: ->
        "#{@num}@#{@dev}"

    inf: ( p ) ->
        if @num == p.num then @dev.inf( p.dev ) else @num < p.num

    sup: ( p ) ->
        if @num == p.num then @dev.sup( p.dev ) else @num > p.num

    equ: ( p ) ->
        @num == p.num and @dev.equ( p.dev )
