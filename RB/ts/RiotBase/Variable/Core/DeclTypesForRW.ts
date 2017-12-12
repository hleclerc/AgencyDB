/// This file is generated (by bin/Method_template.py). Do not modify it directly.

///
export default {
    num: {
        GenString: 0,
        OtWrapperString: 1,
        GenArray: 2,
        Bn_PI32: 3,
        Bn_PI64: 4,
        Bn_FP32: 5,
        Bn_FP64: 6,
        ObjectId_Inode: 7,
    },
    skip_functions: [
        function( br: BinaryReader ) { br.skip_String(); },
        function( br: BinaryReader ) { OtWrapperString._skip_OtWrapperString_data_from( br ); },
        function( br: BinaryReader ) { br.skip_list( Variable ); },
        function( br: BinaryReader ) { br.skip_PT(); },
        function( br: BinaryReader ) { br.skip_PT(); },
        function( br: BinaryReader ) { br.skip_FP32(); },
        function( br: BinaryReader ) { br.skip_FP64(); },
        function( br: BinaryReader ) { br.skip_Inode(); },
    ],
    files: [
        "/ts/RiotBase/Variable/String/GenString.js",
        "/ts/RiotBase/Variable/String/OtWrapperString.js",
        "/ts/RiotBase/Variable/Array/GenArray.js",
        "/ts/RiotBase/Variable/Number/Bn.js",
        "/ts/RiotBase/Variable/Number/Bn.js",
        "/ts/RiotBase/Variable/Number/Bn.js",
        "/ts/RiotBase/Variable/Number/Bn.js",
        "/ts/RiotBase/Variable/ObjectId/ObjectId_Inode.js",
    ],
    readers: [
    ]
};

import OtWrapperString from "../String/OtWrapperString";
import BinaryReader    from "../../System/BinaryReader";
import Variable        from "../Core/Variable";
