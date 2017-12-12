BitFieldInStr_Writer = require "../../js/RiotBase/System/BitFieldInStr_Writer.js"
BitFieldInStr_Reader = require "../../js/RiotBase/System/BitFieldInStr_Reader.js"
check                = require "../../js/RiotBase/System/Check.js"

test_base = ->
    s = new BitFieldInStr_Writer
    s.append_uint 1, 1
    s.append_uint 0, 1
    s.append_uint 1, 1
    s.append_uint 1, 1
    console.log s.binary_repr()

test_BitFieldInStr_cons = ( val, nbits, expected_bits, expected_str ) ->
    s = new BitFieldInStr_Writer
    s.append_uint val, nbits
    check s.binary_repr(), expected_bits
    check s.str(), expected_str

test_BitFieldInStr_append = ->
    s = new BitFieldInStr_Writer
    s.append_uint   1, 1
    s.append_uint   1, 2
    s.append_uint   1, 10
    s.append_uint 512, 10
    check s.binary_repr(), "10000000000000000001011"
    check s.str(), "J??O"

test_reader = ->
    s = new BitFieldInStr_Writer
    s.append_uint 34185 % 256, 8
    s.append_uint 34185 / 256, 8
    console.log s.binary_repr(), s.str()

    r = new BitFieldInStr_Reader "HUG"
    check r.size(), 18
    # console.log ( r.read 1 for i in [ 0 ... 16 ] ).reverse().join ""
    check r.read( 8 ), 34185 % 256 | 0
    check r.read( 8 ), 34185 / 256 | 0

test_base()
# test_reader()
# test_BitFieldInStr_cons     1,  1,                "1", "@"  # 95 = 63 + 32
# test_BitFieldInStr_cons     1,  2,               "01", "@"  # 79 = 63 + 16
# test_BitFieldInStr_cons     1, 10,       "0000000001", "@?" # 64 = 63 +  1; 63 = 63 + 0
# test_BitFieldInStr_cons   512, 10,       "1000000000", "?G" # 63 = 63 +  0; 95 = 63 + 32
# test_BitFieldInStr_cons 34185, 16, "1000010110001001", "HUG" # 63 = 63 +  0; 95 = 63 + 32
#
# test_BitFieldInStr_append()
#
