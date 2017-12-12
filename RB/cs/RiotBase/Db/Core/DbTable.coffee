SortedArray = require "../../System/SortedArray.js"
DbItem      = require "./DbItem.js"

#
module.exports =
class DbTable
    constructor: () ->
        @data = new SortedArray # Inode -> DbItem. SortedArray is certainly not a perfect choice

    insert: ( db_item ) ->
        @data.set db_item.inode, db_item
        db_item

    find: ( inode ) ->
        @data.get inode
