Rp = require "../../Variable/Core/Rp.js"

#
module.exports =
class DbItemPtr extends Rp
    Rp.make_templ this

    init: ( db_item ) ->
        @db_item = db_item

    prox_ptr: ->
        @db_item.patch_manager.variable

    prox_ref: ->
        @db_item.patch_manager.variable
