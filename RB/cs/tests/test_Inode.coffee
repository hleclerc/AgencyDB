Inode = require "../../js/RiotBase/System/Inode.js"
check = require "../../js/RiotBase/System/Check.js"

test_Inode_RW = ->
    n = new Inode
    check s.binary_repr(), "11010000000000000000001"
    check s.str(), "J??_"

test_Inode_RW()
