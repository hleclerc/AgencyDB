SortedArray  = require "../../System/SortedArray.js"
PatchId      = require "../../System/PatchId.js"
DevId        = require "../../System/DevId.js"
Variable     = require "../Core/Variable.js"
Method       = require "../Core/Method.js"
OtWrapper    = require "../Ot/OtWrapper.js"

#
module.exports =
class OtWrapperMap extends OtWrapper
    # patch types
    @op_types = OtWrapper.concattr OtWrapper.op_types,
        op_new_key:
            attrs: "PatchId key_id*, Uint8Array key_data, Uint8Array val_data"
            right: ( flags, key_id, key_data, val_data ) -> flags & @can_add_key.flag
            apply_op: ( aod, key_id, key_data, val_data ) ->
                key = new @category_key; km = key.new_ot_merger()
                val = new @category_val; vm = val.new_ot_merger()
                km.new_patch new BinaryWriter, key_data, aod.usr
                vm.new_patch new BinaryWriter, val_data, aod.usr

                node = new Node key, val, key_id

                va = new Variable this
                key.onchange_par va, node
                val.onchange_par va, node

                @kmap.set key, node
                @imap.set key_id, new IdCmpNode node
                @sig_change()
            undo: ( aod, key_id, key_data, val_data ) ->
                @op_rem_key.apply_op.call this, aod, key_id
        op_rem_key:
            attrs: "PatchId key_id*"
            right: ( flags, key_id ) -> flags & @can_rem_key.flag
            apply_op: ( aod, key_id ) ->
                if ( @usr_right( aod.usr ) & @can_rem_key.flag ) and ( ic = @imap.get key_id )?
                    @_rem_node ic.node
            undo: ( aod, key_id ) ->
                console.error "TODO"
        op_rem_key_und:
            attrs: "PatchId key_id*, Uint8Array key_data, Uint8Array val_data"
            right: ( flags, key_id, key_data, val_data ) -> flags & @can_rem_key.flag
            apply_op: ( aod, key_id, key_data, val_data ) ->
                @op_rem_key.apply_op.call this, aod, key_id
            undo: ( aod, key_id, key_data, val_data ) ->
                console.error "TODO"
        op_mod_key:
            attrs: "PatchId key_id*, Uint8Array key_data"
            right: ( flags, key_id, key_data ) -> true
            apply_op: ( aod, key_id, key_data ) ->
                id_cmp = @imap.get key_id
                if id_cmp?
                    km = id_cmp.node.key.new_ot_merger()
                    switch aod.type
                        when 0 then km.new_patch new BinaryWriter, val_data, aod.usr
                        else console.error "TODO"
            undo: ( aod, key_id, key_data ) ->
                console.error "TODO"
        op_mod_val:
            attrs: "PatchId key_id*, Uint8Array val_data"
            right: ( flags, key_id, val_data ) -> true
            apply_op: ( aod, key_id, val_data ) ->
                id_cmp = @imap.get key_id
                if id_cmp?
                    vm = id_cmp.node.val.new_ot_merger()
                    switch aod.type
                        when 0 then vm.new_patch new BinaryWriter, val_data, aod.usr
                        else console.error "TODO"
            undo: ( aod, key_id, val_data ) ->
                console.error "TODO"

    # right types
    @right_types = OtWrapper.right_types.concat " can_add_key can_rem_key"

    #
    class Node
        constructor: ( @key, @val, @id ) ->
        inf: ( key ) -> @key.inf_immediate key
        sup: ( key ) -> @key.sup_immediate key

    class IdCmpNode
        constructor: ( node )-> @node = node
        inf: ( id ) -> @node.id.inf id
        sup: ( id ) -> @node.id.sup id

    #
    OtWrapper.make_templ this

    init: ( category_key, category_val ) ->
        super()
        @category_key = category_key
        @category_val = category_val
        @loc_id_num   = 0
        @crea_nodes   = []
        @mod_nodes    = []
        @rem_nodes    = []
        @kmap         = new SortedArray # key -> node
        @imap         = new SortedArray # id -> node

    to_String: ->
        "{#{ "#{node.key}:#{node.val}" for node in @kmap.items }}"

    sig_change: ( node ) ->
        super()
        if node?
            if node not in @mod_nodes
                @mod_nodes.push node

    freeze_last_changes: ->
        # new nodes
        for n in @crea_nodes
            key_data = new BinaryWriter
            val_data = new BinaryWriter
            n.key.pull_ot_changes key_data
            n.val.pull_ot_changes val_data
            @op_new_key.reg_op @last_ops, n.id, key_data.to_Uint8Array(), val_data.to_Uint8Array()
        @crea_nodes = []

        # mod keys and vals
        for n in @mod_nodes
            key_data = new BinaryWriter
            val_data = new BinaryWriter
            n.key.pull_ot_changes key_data
            n.val.pull_ot_changes val_data
            if key_data.size then @op_mod_key.reg_op @last_ops, n.id, key_data.to_Uint8Array()
            if val_data.size then @op_mod_val.reg_op @last_ops, n.id, val_data.to_Uint8Array()
        @mod_nodes = []

        # rem_nodes
        for n in @rem_nodes
            @op_rem_key_und.reg_op @last_ops, n.id, new Uint8Array, new Uint8Array # IMPORTANT_TODO
        @rem_nodes = []

    _rem_item: ( key ) ->
        if ( n = @kmap.get key )?
            @_rem_node n

    _create_node_4k: ( key, val ) ->
        id   = new PatchId new DevId, @loc_id_num += 1
        node = new Node key, val, id
        @crea_nodes.push node

        va = new Variable this
        key.onchange_par va, node
        val.onchange_par va, node

        # @kmap.set key, node
        @imap.set id, new IdCmpNode node
        node

    _rem_node: ( node ) ->
        @kmap.del node.key
        @imap.del node.id
        @sig_change()
        # for the next freeze_last_changes
        if ( i = @crea_nodes.indexOf node ) >= 0
            return @crea_nodes.splice i, 1
        if ( i = @mod_nodes.indexOf node ) >= 0
            return @mod_nodes.splice i, 1
        @rem_nodes.push node

Method.methods.mod_select.add_surdef
    pert: 2
    test: ( type_0, type_1 ) -> type_0 == OtWrapperMap and type_1.known_term_value
    func: ( map, key ) ->
        map.rp.kmap.get( key, ->
            if map.rp.usr_right() & map.rp.can_add_key.flag
                vnkey = Method.methods.copy.call_1 key
                vnval = new map.rp.category_val
                map.rp._create_node_4k vnkey, vnval
        ).val

Method.methods.rem_item.add_surdef
    pert: 2
    test: ( type_0, type_1 ) -> type_0 == OtWrapperMap and type_1.known_term_value
    func: ( map, key ) -> map.rp._rem_item key

Method.methods.key_type.add_surdef
    pert: 2
    test: ( type_0 ) -> type_0 == OtWrapperMap
    func: ( map ) -> map.rp.category_key

Method.methods.val_type.add_surdef
    pert: 2
    test: ( type_0 ) -> type_0 == OtWrapperMap
    func: ( map ) -> map.rp.category_val


BinaryWriter       = require "../../System/BinaryWriter.js"
OtWrapperMapSelect = require "./OtWrapperMapSelect.js"
