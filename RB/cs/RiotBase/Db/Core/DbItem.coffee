PatchManager = require "../../Variable/Ot/PatchManager.js"
DbItemPtr    = require "./DbItemPtr.js"

#
module.exports =
class DbItem
    @FLAG_in_db_stuff_to_send   = 1
    @FLAG_snd_signal_to_tracker = 2
    @FLAG_ack_signal_to_tracker = 4

    constructor: ( db, inode, variable ) ->
        @patch_manager = new PatchManager variable
        @inode         = inode
        @db            = db
        @flags         = 0

        db._sig_stuff_to_send this

    inf: ( inode ) ->
        @inode.inf inode

    sup: ( inode ) ->
        @inode.sup inode

    equ: ( inode ) ->
        @inode.equ inode

    make_ptr: ->
        new @patch_manager.variable.constructor DbItemPtr.New this

    send_stuff: ->
        @db.tracker_for_inode( @inode ).send_bind_obj @inode, @patch_manager.vector_clock
