Inode  = require "../../System/Inode.js"
Method = require "../Core/Method.js"
Rp     = require "../Core/Rp.js"

#
module.exports =
class ObjectId_Inode extends Rp
    @term_value = true
    Rp.make_templ this

    init: ( inode = new Inode ) ->
        @inode = inode

    to_Inode: ->
        @inode

    to_String: ->
        @inode.toString()

    make_ot_wrapper: ->
        OtWrapperAtomic.New new ObjectId this

    write_to: ( bw ) ->
        bw.write_PT ObjectId_Inode.num_for_RW
        bw.write_Inode @inode

    copy: ->
        new ObjectId @inode

    obj_type: ->
        ObjectId

    @read_from = ( br, usr_id ) ->
        kv = new ObjectId br.read_Inode()
        new ObjectId OtWrapperAtomic.New kv, usr_id


Method.methods.self_set_b.add_surdef
    test: ( type_0, type_1 ) -> type_0 == ObjectId_Inode and Method.methods.to_Inode.has_surdef_for( type_1 )
    func: ( obj, sup ) -> obj.rp.inode = Method.methods.to_Inode.call_1 sup
    pert: 1

OtWrapperAtomic = require "../Ot/OtWrapperAtomic.js"
ObjectId        = require "../ObjectId.js"
