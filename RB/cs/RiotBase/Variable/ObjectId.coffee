Variable       = require "./Core/Variable.js"

#
module.exports =
class ObjectId extends Variable
    Variable.make_templ this

    constructor: ( val ) ->
         switch
             when val instanceof Rp    then @rp = val
             when val instanceof Inode then @rp = ObjectId_Inode.New val
             when not val?             then @rp = ObjectId_Inode.New new Inode
             else console.error "TODO"

     get: -> # to  native type
         @toInode()

     toInode: ->
        Method.methods.to_Inode.call_1 this


ObjectId_Inode = require "./ObjectId/ObjectId_Inode.js"
Inode          = require "../System/Inode.js"
Method         = require "./Core/Method.js"
Rp             = require "./Core/Rp.js"
