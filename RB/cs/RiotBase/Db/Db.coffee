Method        = require "../Variable/Core/Method.js"
ObjectId      = require "../Variable/ObjectId.js"
String        = require "../Variable/String.js"
Map           = require "../Variable/Map.js"

SortedArray   = require "../System/SortedArray.js"
Inode         = require "../System/Inode.js"
DevId         = require "../System/DevId.js"
UsrId         = require "../System/UsrId.js"

TrackerClient = require "./Core/TrackerClient.js"
DbTable       = require "./Core/DbTable.js"
DbItem        = require "./Core/DbItem.js"


#
class Directory extends Map
    constructor: ( a, b ) ->
        if a? then super a, b else super String, ObjectId


# Local database.
# May link to an server (specifying an url)
#
module.exports =
class Db
    # shortcuts
    @Directory = Directory
    @String    = String

    # sudefinitions:
    #   - url, login, password
    #   - login, password (url coming from window.location)
    #   - url
    constructor: ( user, pass ) ->
        @timeout_for_items_with_stuff_to_send = null
        @pending_dev_conn_messages            = new Map.NativeType # DevId -> buffer with data to send
        @items_with_stuff_to_send             = []
        @trackers                             = []
        @dev_conn                             = new Map.NativeType # DevId -> data channel
        @table                                = new DbTable
        @dev                                  = new DevId
        @usr                                  = new UsrId
        @rootd                                = @table.insert new DbItem this, Inode.known_inodes.root_dir, new Directory
        @locid                                = 0

        if user? and pass?
            @connect user, pass

    # connect to a tracker
    connect: ( user, pass, ip = "localhost:8676" ) ->
        # tracker for the corresponding ip
        tracker = undefined
        for t in @trackers
            if t.ip == ip
                tracker = t
                break
        if not tracker?
            tracker = new TrackerClient ip, this
            @trackers.push tracker

        #
        tracker.send_auth user, pass, ( dev, usr ) =>
            @_set_dev_and_usr_id dev, usr

    bind: ( url, type ) ->
        Method.methods.bind_by_path.call_1 String.rp_conv( url ), this, type

    bind_by_object_id: ( id, type ) ->
        Method.methods.bind_by_object_id.call_1 ObjectId.rp_conv( id ), this, type

    tracker_for_inode: ( inode ) ->
        @trackers[ 0 ]

    tracker_for_dev: ( dev )->
        @trackers[ 0 ]

    _set_dev_and_usr_id: ( dev, usr ) ->
        @dev = dev
        @usr = usr

        if @dev.ext()
            @_send_stuff()

    _make_new_item: ( object_id, type ) ->
        inode = new Inode new DevId, @locid += 1
        object_id.set inode
        @table.insert new DbItem this, inode, new type

    _sig_stuff_to_send: ( db_item ) ->
        if not ( db_item.flags & DbItem.FLAG_in_db_stuff_to_send )
            db_item.flags |= DbItem.FLAG_in_db_stuff_to_send
            @items_with_stuff_to_send.push db_item
            # timer
            if not @timeout_for_items_with_stuff_to_send and @dev.ext()
                @timeout_for_items_with_stuff_to_send = setTimeout ( => @_send_stuff() ), 1

    _send_stuff: ->
        cp = @items_with_stuff_to_send
        @items_with_stuff_to_send = []
        for item in cp
            item.flags &= ~ DbItem.FLAG_in_db_stuff_to_send
            item.send_stuff()


Method.methods.bind_by_path.add_surdef
    pert: 2
    test: ( type_0 ) -> type_0.known_term_value and Method.methods.to_String.has_surdef_for( type_0 )
    func: ( url, db, type ) ->
        srl = url.toString()
        while srl.startsWith '/'
            srl = srl.substr 1
        while srl.endsWith '/'
            srl = srl.substr 0, srl.length - 1

        # root dir ?
        if srl.length
            # parent directory
            base_dir = if ( isl = srl.lastIndexOf '/' ) >= 0
                dir = srl.substr 0, isl
                str = srl.substr isl + 1
                db.bind dir, Directory
            else
                db.table.find( Inode.known_inodes.root_dir ).make_ptr()


            object_id = base_dir.get srl
            db.bind_by_object_id object_id, type
        else
            ( db.table.find Inode.known_inodes.root_dir ).make_ptr()

Method.methods.bind_by_object_id.add_surdef
    pert: 2
    test: ( type_0 ) ->
        type_0.known_term_value and Method.methods.to_Inode.has_surdef_for( type_0 )
    func: ( object_id, db, type ) ->
        inode = Method.methods.to_Inode.call_1 object_id
        ( if inode.num then db.table.find inode else db._make_new_item object_id, type ).make_ptr()
