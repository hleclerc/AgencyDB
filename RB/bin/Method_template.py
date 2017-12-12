from mako.template import Template
import sys, string, re, itertools

class Arg:
    def __init__( self, s ):
        self.n = re.search( '[a-zA-Z_][a-zA-Z0-9_]*$', s ).group( 0 )
        self.t = string.strip( s[ 0 : len( s ) - len( self.n ) ] )

    def isa_rp( self ):
        return self.t in [ 'Rp *', 'const Rp *', 'Rp *&' ]

    def __repr__( self ):
        return self.t + ' ' * ( not ( self.t[ -1 ] in "*&" ) ) + self.n

#
class Method:
    def __init__( self, name, ret, args ):
        self.name = string.strip( name )
        self.ret  = string.strip( ret  )
        self.args = [ Arg( string.strip( x ) ) for x in string.split( args, ',' ) ]

        self.nb_args = 0
        while self.nb_args < len( self.args ) and self.args[ self.nb_args ].isa_rp():
            self.nb_args += 1

        self.sub = [ self ]

    def decl_args( self, j = ", ", b = "", start = 0 ):
        return string.join( [ b + str( a ) for a in self.args[ start: ] ], j )

    def name_args( self, j = ", ", b = "", start = 0 ):
        return string.join( [ b + a.n for a in self.args[ start: ] ], j )

    def type_args( self, j = ", ", b = "", start = 0 ):
        return string.join( [ b + a.t for a in self.args[ start: ] ], j )

    def ret_os( self ):
        return self.ret + ' ' * ( not ( self.ret[ -1 ] in "*&" ) )

#
class StdOp:
    def __init__( self, name, nb_args, add_args ):
        self.name     = string.strip( name )
        self.nb_args  = nb_args
        self.add_args = []
        if len( string.strip( add_args ) ):
            self.add_args = [ Arg( string.strip( x ) ) for x in string.split( string.strip( add_args ), "," ) ]

        self.sub = []
        for v in itertools.product( ['o','b'], repeat = nb_args ):
            tl = [ "const " * ( v[ i ] == 'b' ) + "Rp *v" + str( i ) for i in range( self.nb_args ) ] + [ str( x ) for x in self.add_args ]
            self.sub += [ Method( self.name + "_" + string.join( v, "" ), "Rp *", string.join( tl, ", " ) ) ]


    def decl_add_args( self, j = ", ", b = "", e = "" ):
        return string.join( [ b + str( a ) + e for a in self.add_args ], j )

    def name_add_args( self, j = ", ", b = "" ):
        return string.join( [ b + a.n for a in self.add_args ], j )

    def name_base_args( self, j = ", ", b = "", start = 0 ):
        return string.join( [ b + "v" + str( i ) for i in range( start, self.nb_args ) ], j )

    def name_all_args( self, start = 0 ):
        return self.name_base_args( start = start ) + self.name_add_args( j='', b=', ' )

#
class SelfOp( StdOp ):
    def __init__( self, name, nb_args, add_args, add_attrs = [] ):
        self.name     = string.strip( name )
        self.nb_args  = nb_args
        self.add_args = []
        if len( string.strip( add_args ) ):
            self.add_args = [ Arg( string.strip( x ) ) for x in string.split( string.strip( add_args ), "," ) ]
        self.add_attrs = add_attrs

        # list of sub methods
        self.sub = []

        # self_b, self_o, ... methods
        for v in itertools.product( [ 'o', 'b' ], repeat = nb_args - 1 ):
            tl = [ "const " * ( v[ i ] == 'b' ) + "Rp *v" + str( i + 1 ) for i in range( len( v ) ) ] + [ str( x ) for x in self.add_args ]
            self.sub += [ Method( "self_" + self.name + "_" * ( nb_args > 1 ) + string.join( v, "" ), "void", string.join( [ "Rp *&v0" ] + tl, ", " ) ) ]

        # xxx_sb, xxx_so, xxx_sb_extr, xxx_so_extr, ...
        for v in itertools.product( [ 'o', 'b' ], repeat = nb_args - 1 ):
            tl = [ "const " * ( v[ i ] == 'b' ) + "Rp *v" + str( i + 1 ) for i in range( len( v ) ) ] + [ str( x ) for x in self.add_args ]
            self.sub += [ Method( self.name + "_s" + string.join( v, "" )          , "Rp *", string.join( [ "Rp *v0" ] + tl, ", " ) ) ]
            self.sub += [ Method( self.name + "_s" + string.join( v, "" ) + "_extr", "Rp *", string.join( [ "Rp *v0" ] + tl + [ "const Vec<Rp *> &extr" ], ", " ) ) ]

        # xxx_bb, xxx_bo, ...
        for v in itertools.product( [ 'o', 'b' ], repeat = nb_args ):
            tl = [ "const " * ( v[ i ] == 'b' ) + "Rp *v" + str( i ) for i in range( len( v ) ) ] + [ str( x ) for x in self.add_args ]
            self.sub += [ Method( self.name + "_" + string.join( v, "" ), "Rp *", string.join( tl, ", " ) ) ]

objects = [
    Method( 'del'                 , 'void                ', 'Rp *v0                                                                   ' ),
    Method( 'copy'                , 'Rp *                ', 'const Rp *v0                                                             ' ),

    # Method( 'reassign_b'          , 'void                ', 'Rp *&v0, const Rp *v1                                                    ' ),
    # Method( 'reassign_o'          , 'void                ', 'Rp *&v0, Rp *v1                                                          ' ),

    # Method( 'reassign_sb_extr'    , 'Rp *                ', 'Rp *v0, const Rp *v1, const Vec<Rp *> &keys                              ' ),

    # info
    Method( 'write_to_stream     ', 'void                ', 'const Rp *v0, std::ostream &os                                           ' ),
    Method( 'write_structure     ', 'void                ', 'const Rp *v0, std::ostream &os                                           ' ),
    Method( 'write_graphviz      ', 'void                ', 'const Rp *v0, std::ostream &os                                           ' ),

    Method( 'category'            , 'const Category *    ', 'const Rp *v0                                                             ' ),

    # conversion
    Method( 'to_std_string'       , 'bool                ', 'const Rp *v0, std::string *res, PT nb_items                              ' ),
    Method( 'to_Inode'            , 'bool                ', 'const Rp *v0, Inode *res, PT nb_items                                    ' ),
    Method( 'to_SI64'             , 'bool                ', 'const Rp *v0, SI64 *res, PT nb_items                                     ' ),
    Method( 'to_PI64'             , 'bool                ', 'const Rp *v0, PI64 *res, PT nb_items                                     ' ),
    Method( 'to_SI32'             , 'bool                ', 'const Rp *v0, SI32 *res, PT nb_items                                     ' ),
    Method( 'to_PI32'             , 'bool                ', 'const Rp *v0, PI32 *res, PT nb_items                                     ' ),
    Method( 'to_SI16'             , 'bool                ', 'const Rp *v0, SI16 *res, PT nb_items                                     ' ),
    Method( 'to_PI16'             , 'bool                ', 'const Rp *v0, PI16 *res, PT nb_items                                     ' ),
    Method( 'to_SI8'              , 'bool                ', 'const Rp *v0, SI8 *res, PT nb_items                                      ' ),
    Method( 'to_PI8'              , 'bool                ', 'const Rp *v0, PI8 *res, PT nb_items                                      ' ),
    Method( 'to_Bool'             , 'bool                ', 'const Rp *v0, Bool *res, PT nb_items                                     ' ),
    Method( 'to_PT'               , 'bool                ', 'const Rp *v0, PT *res, PT nb_items                                       ' ),
    Method( 'dasi'                , 'DaSi                ', 'const Rp *v0                                                             ' ),

    Method( 'apply_DaSi'          , 'bool                ', 'const Rp *v0, const std::function<void(DaSi)> &f                         ' ),

    # self operations
    SelfOp( 'set                 ', 2                     , '                                                                         ' ),

    SelfOp( 'add                 ', 2                     , '                                                                         ' ),
    SelfOp( 'sub                 ', 2                     , '                                                                         ' ),
    SelfOp( 'mul                 ', 2                     , '                                                                         ' ),
    SelfOp( 'div                 ', 2                     , '                                                                         ' ),
    SelfOp( 'mod                 ', 2                     , '                                                                         ' ),
    SelfOp( 'add_key             ', 2                     , '                                                                         ' ),
    SelfOp( 'rem_key             ', 2                     , '                                                                         ' ),
    SelfOp( 'push_back           ', 2                     , '                                                                         ' ),

    SelfOp( 'insert              ', 3                     , '                                                                         ' ),
    SelfOp( 'remove              ', 3                     , '                                                                         ' ),

    # simple bin op (wo self version)
    StdOp ( 'equ                 ', 2                     , '                                                                         ' ),
    StdOp ( 'neq                 ', 2                     , '                                                                         ' ),
    StdOp ( 'inf                 ', 2                     , '                                                                         ' ),
    StdOp ( 'sup                 ', 2                     , '                                                                         ' ),
    StdOp ( 'inf_eq              ', 2                     , '                                                                         ' ),
    StdOp ( 'sup_eq              ', 2                     , '                                                                         ' ),
    StdOp ( 'bor                 ', 2                     , '                                                                         ' ),

    StdOp ( 'select              ', 2                     , '                                                                         ' ), # select that returns a reference on the value, but does not modify per se the container

    StdOp ( 'neg                 ', 1                     , '                                                                         ' ),

    # operationnal transform
    Method( 'make_ot_wrapper     ', 'Rp *                ', 'Rp *v0                                                                   ' ), # return v0 or something that swallows v0 to manage the operationnal transform methods
    Method( 'pull_ot_changes     ', 'void                ', 'Rp *&v0, BBQ changes                                                     ' ),
    Method( 'write_ot_data       ', 'void                ', 'Rp *&v0, std::ostream &os, CbStringPtr data                              ' ),
    Method( 'new_ot_merger       ', 'OtMerger *          ', 'Rp *&v0                                                                  ' ),

    # cursors
    Method( 'new_cursor          ', 'Rp *                ', 'Rp *&v0, void *int_id                                                    ' ),
    Method( 'rem_cursor          ', 'void                ', 'Rp *v0, void *int_id                                                     ' ),
    Method( 'get_cursors         ', 'void                ', 'const Rp *v0, Vec<CursorData> &res                                       ' ),

    # rights
    Method( 'get_possible_rights ', 'Vec<std::string>    ', 'Rp *&v0                                                                  ' ),
    Method( 'get_groups_in_acl   ', 'Vec<GrpId>          ', 'const Rp *v0                                                             ' ),
    Method( 'get_users_in_acl    ', 'Vec<UsrId>          ', 'const Rp *v0                                                             ' ),
    Method( 'get_grp_rights      ', 'Vec<std::string>    ', 'const Rp *v0, GrpId grp                                                  ' ),
    Method( 'get_usr_rights      ', 'Vec<std::string>    ', 'const Rp *v0, UsrId usr                                                  ' ),
    Method( 'add_grp_right       ', 'bool                ', 'Rp *&v0, GrpId grp, const std::string &right                             ' ),
    Method( 'add_usr_right       ', 'bool                ', 'Rp *&v0, UsrId usr, const std::string &right                             ' ),
    Method( 'rem_grp_right       ', 'bool                ', 'Rp *&v0, GrpId grp, const std::string &right                             ' ),
    Method( 'rem_usr_right       ', 'bool                ', 'Rp *&v0, UsrId usr, const std::string &right                             ' ),

    # callbacks
    SelfOp( 'on_change           ', 1                     , 'Callback *c                                                              ' ),
    SelfOp( 'then                ', 1                     , 'Callback *c                                                              ', [ "bool done = false;" ] ),
    Method( 'sig_change          ', 'void                ', 'Rp *v0                                                                   ' ),

    # symbolic
    Method( 'common_symbolic_data', 'CommonSymbolicData *', 'const Rp *v0                                                             ' ),
    Method( 'nb_symbolic_children', 'int                 ', 'const Rp *v0                                                             ' ),
    Method( 'symbolic_child      ', 'const Rp *          ', 'const Rp *v0, int n                                                      ' ),
    Method( 'is_symbolic         ', 'bool                ', 'const Rp *v0                                                             ' ),
    Method( 'is_a_s_op           ', 'bool                ', 'const Rp *v0                                                             ' ),

    Method( 'exec                ', 'Rp *                ', 'const Rp *v0, BitFieldExtSize own, Rp **ch                               ' ), # BitField to say if Rp * has to be reused or deleted if not used
    Method( 'launch              ', 'void                ', 'Rp *v0                                                                   ' ), #
    Method( 'exec_mp             ', 'Rp *                ', 'Rp *v0                                                                   ' ), #
    Method( 'del_or_cancel_exec  ', 'void                ', 'Rp *v0                                                                   ' ), #

    Method( 'get_lazy_source_info', 'void                ', 'const Rp *v0, LazySourceInfo &info, const LazySourceInfo **children_info ' ), #

    Method( 'serialize           ', 'void                ', 'const Rp *v0, BBQ bq, const PT *num_children                             ' ), # num_children can be null
    Method( 'set_child           ', 'void                ', 'Rp *v0, int num_child, Rp *new_child, bool del_prev                      ' ),

    # proxy
    Method( 'prox_ptr            ', 'Rp **               ', 'Rp *v0                                                                   ' ), # return a reference that may be modified
    Method( 'prox_ref            ', 'const Rp *          ', 'const Rp *v0                                                             ' ), # return a reference (v0 is still the owner)
    Method( 'prox_val            ', 'Rp *                ', 'const Rp *v0                                                             ' ), # return a new value ()

    # array/map
    Method( 'mod_select          ', 'Rp *                ', 'Rp *&v0, const Rp *v1                                                    ' ), # select that may modify the container by adding a key

    Method( 'get_size            ', 'Rp *                ', 'const Rp *v0                                                             ' ),
    Method( 'get_size_PT         ', 'PT                  ', 'const Rp *v0                                                             ' ),

    Method( 'front_b             ', 'Rp *                ', 'const Rp *v0                                                             ' ),
    Method( 'back_b              ', 'Rp *                ', 'const Rp *v0                                                             ' ),
    Method( 'front_list_b        ', 'Rp *                ', 'const Rp *v0                                                             ' ),
    Method( 'back_list_b         ', 'Rp *                ', 'const Rp *v0                                                             ' ),
    Method( 'head_b              ', 'Rp *                ', 'const Rp *v0                                                             ' ),
    Method( 'tail_b              ', 'Rp *                ', 'const Rp *v0                                                             ' ),
    Method( 'slice_bbb           ', 'Rp *                ', 'const Rp *v0, const Rp *v1, const Rp *v2                                 ' ),

    StdOp ( 'split               ', 2                     , '                                                                         ' ),
    StdOp ( 'join                ', 2                     , '                                                                         ' ),
    Method( 'apply               ', 'void                ', 'const Rp *v0, std::function<void(const Rp *)> func                       ' ),

    Method( 'has_item_in         ', 'Rp *                ', 'const Rp *v0, const Rp *v1                                               ' ),

    # sorting/hashing
    Method( 'equ_immediate       ', 'bool                ', 'const Rp *v0, const Rp *v1                                               ' ),
    Method( 'inf_immediate       ', 'bool                ', 'const Rp *v0, const Rp *v1                                               ' ),

    # Db
    Method( 'bind_by_path        ', 'Rp *                ', 'const Rp *v0, const Category *cat                                        ' ),
    Method( 'bind_by_object_id   ', 'Rp *                ', 'Rp *&v0, const Category *cat                                             ' ), # version that may change the object id is zero (-> assignation of a new local inode in this case)
    StdOp ( 'bind_by_kn_object_id', 1                     , 'const Category *cat                                                      ' ), # version that assumes that the object id is correct
    SelfOp( 'make_inode_if_z     ', 1                     , 'const Category *cat                                                      ' ),

    # "special" function -> method specified by a const char *
    SelfOp( 'special_func        ', 1                     , 'const char *func, Rp **parm, int nb_parms                                ' ),
]

methods  = []
self_ops = []
std_ops  = []
for o in objects:
    for m in o.sub:
        methods.append( m )
    if isinstance( o, SelfOp ):
        self_ops.append( o )
    if isinstance( o, StdOp ):
        std_ops.append( o )

def jc( lst ):
    return string.join( lst, ', ' )

#
nb_TypeForRW = 0

class TypeForRW:
    def __init__( self, name, ts_path, ts_skip, cpp_path = "" ):
        global nb_TypeForRW
        self.num      = nb_TypeForRW
        self.name     = name
        self.ts_path  = ts_path
        self.ts_skip  = ts_skip
        self.cpp_path = cpp_path
        nb_TypeForRW += 1

decl_types_for_rw = [
    TypeForRW( "GenString"              , "/ts/RiotBase/Variable/String/GenString.js"       , "br.skip_String();"                                      ),
    TypeForRW( "OtWrapperString"        , "/ts/RiotBase/Variable/String/OtWrapperString.js" , "OtWrapperString._skip_OtWrapperString_data_from( br );" ),
    TypeForRW( "GenArray"               , "/ts/RiotBase/Variable/Array/GenArray.js"         , "br.skip_list( Variable );"                              ),
    TypeForRW( "Bn_PI32"                , "/ts/RiotBase/Variable/Number/Bn.js"              , "br.skip_PT();"                                          ),
    TypeForRW( "Bn_PI64"                , "/ts/RiotBase/Variable/Number/Bn.js"              , "br.skip_PT();"                                          ),
    TypeForRW( "Bn_FP32"                , "/ts/RiotBase/Variable/Number/Bn.js"              , "br.skip_FP32();"                                        ),
    TypeForRW( "Bn_FP64"                , "/ts/RiotBase/Variable/Number/Bn.js"              , "br.skip_FP64();"                                        ),
    TypeForRW( "ObjectId_Inode"         , "/ts/RiotBase/Variable/ObjectId/ObjectId_Inode.js", "br.skip_Inode();"                                       ),
    # TypeForRW( "GenArray"               , "/ts/RiotBase/Variable/Array/GenArray.js" ),
    # TypeForRW( "DelayedVariableCreation", "/ts/RiotBase/Variable/Symbol/DelayedVariableCreation.js" ),
    # # TypeForRW( "LazySourceKnownValueExt", "/ts/RiotBase/Variable/Symbol/LazySourceKnownValueExt.js" ),
    # # TypeForRW( "LazySourceExtValue", "LazySourceExtValue.js" ),
    # # TypeForRW( "RecvFromExtExec", "RecvFromExtExec.js" ),
    # # TypeForRW( "SendToExtExec", "SendToExtExec.js" ),
    # TypeForRW( "SymOp_Op_add_key_true"  , "/ts/RiotBase/Variable/Symbol/SymOp_Op_add_key_true.js" ),
    # TypeForRW( "SymOp_Op_split"         , "/ts/RiotBase/Variable/Symbol/SymOp_Op_split.js" ),
    # TypeForRW( "SymOp_Op_join"          , "/ts/RiotBase/Variable/Symbol/SymOp_Op_join.js" ),
]

#
nb_PatchTypeForRW = 0

class PatchTypeForRW:
    def __init__( self, name, ts_path, cpp_path = "" ):
        global nb_PatchTypeForRW
        self.num      = nb_PatchTypeForRW
        self.name     = name
        self.ts_path  = ts_path
        self.cpp_path = cpp_path
        nb_PatchTypeForRW += 1

decl_patch_types_for_rw = [
    PatchTypeForRW( "OtWrapperString_GenString", "/ts/RiotBase/Variable/String/OtWrapperString.js" ),
    PatchTypeForRW( "GenArray                 ", "/ts/RiotBase/Variable/Array/GenArray.js" ),
    PatchTypeForRW( "GenMap                   ", "/ts/RiotBase/Variable/Map/GenMap.js" ),
]


#
known_inodes = {
    # ---------- directories ----------
    "root_dir"      :  2, # /
    "root_home"     :  3, # /root

    # ------------ types --------------
    "type_Directory": 10,
    "type_String"   : 11,
    "type_Number"   : 12,
};

#
class Cmd:
    def __init__( self, name, num, args, comment = "" ):
        self.name = string.strip( name )
        self.num = num
        self.args = args
        self.comment = comment

    def args_ts( self ):
        res = "[ "
        for a in self.args:
            res += "{ type: '" + a[ 0 ] + "', name: '" + a[ 1 ] + "' }, "
        res += "]"
        return res

    def write_ts( self ):
        mty = 0
        mar = 0
        for a in self.args:
            mty = max( mty, len( a[ 0 ] ) )
            mar = max( mar, len( a[ 1 ] ) )

        res = ""
        for a in self.args:
            res += "\n            if ( typeof args[ '" + a[ 1 ] + "'" + " " * max( 0, mar - len( a[ 1 ] ) ) + " ] == 'undefined' ) throw Error( 'Undefined arg: " + a[ 1 ] + "'" + " " * max( 0, mar - len( a[ 1 ] ) ) + " ); "
        for a in self.args:
            res += "\n            bw.write_" + a[ 0 ] + " " * max( 0, mty - len( a[ 0 ] ) )
            if a[ 0 ] in [ "UsrId" ]:
                res += "( args[ '" + a[ 1 ] + "'" + " " * max( 0, mar - len( a[ 1 ] ) ) + " ], cur_dev, cur_usr ); "
            elif a[ 0 ] in [ "PatchId", "GrpId", "Inode", "DevId" ]:
                res += "( args[ '" + a[ 1 ] + "'" + " " * max( 0, mar - len( a[ 1 ] ) ) + " ], cur_dev ); "
            else:
                res += "( args[ '" + a[ 1 ] + "'" + " " * max( 0, mar - len( a[ 1 ] ) ) + " ] ); "
        return res

    def read_ts( self, sp = "\n                " ):
        res = ""
        for a in self.args:
            res += sp + "let " + a[ 1 ] + " = "
            if a[ 0 ] in [ "UsrId" ]:
                res += "br.read_" + a[ 0 ] + "( cur_dev, cur_usr );"
            elif a[ 0 ] in [ "PatchId", "GrpId", "Inode", "DevId" ]:
                res += "br.read_" + a[ 0 ] + "( cur_dev );"
            else:
                res += "br.read_" + a[ 0 ] + "();"
        return res

    def arg_names_ts( self ):
        return jc( [ a[ 1 ] for a in self.args ] )

    def cpp_equ( self, type ):
        m = {
            "String": "CbString",
        }
        try:
            return m[ type ]
        except:
            return type

    def cpp_arg( self, type ):
        m = {
            "String"     : "const CbString &",
            "VectorClock": "const VectorClock &",
        }
        try:
            return m[ type ]
        except:
            return type + " "



    arg_names_ts


tracker_client_cmds = [
    Cmd( "error_msg              ",  0, [ ("String","msg"        )                                                       ] ), # msg begins by unsigned error code (@see TrackerCommands)
    Cmd( "corrupted_data         ",  1, [                                                                                ] ),
    Cmd( "ans_login              ",  3, [ ("DevId" ,"dev"        ), ("UsrId"      ,"usr"      ), ("String","token")      ] ), #
    Cmd( "ans_logout             ",  4, [                                                                                ] ), #
    Cmd( "bind_obj               ",  5, [ ("Inode" ,"id"         ), ("DevId"      ,"dev"      )                          ] ), # say that a peer has a given object
    Cmd( "req_inode_data         ",  6, [ ("Inode" ,"id"         ), ("DevId"      ,"dev"      )                          ] ), # request and forget
    Cmd( "req_inode_updates      ",  7, [ ("Inode" ,"id"         ), ("DevId"      ,"dev"      ), ("VectorClock","known") ] ), # request and forget
    Cmd( "req_and_bind_inode_data",  8, [ ("Inode" ,"id"         ), ("DevId"      ,"dev"      )                          ] ), #
    Cmd( "webrtc_offer           ",  9, [ ("DevId" ,"dev"        ), ("String"     ,"sdp"      )                          ] ), #
    Cmd( "webrtc_answer          ", 10, [ ("DevId" ,"dev"        ), ("String"     ,"sdp"      )                          ] ), #
    Cmd( "ice_candidate          ", 11, [ ("DevId" ,"dev"        ), ("PI8"        ,"initiator"), ("String"     ,"sdp" )  ] ), #
    Cmd( "ans_create_user        ", 12, [ ("String","res"        )                                                       ] ), #
]
tracker_server_cmds = [
    Cmd( "error_msg              ",  0, [ ("String","msg"        )                                                       ] ), # msg begins by unsigned error code (@see TrackerCommands)
    Cmd( "corrupted_data         ",  1, [                                                                                ] ),
    Cmd( "login                  ",  4, [ ("String","user"       ), ("String"     ,"pass"     )                          ] ),
    Cmd( "logout                 ",  5, [                                                                                ] ),
    Cmd( "bind_obj               ",  6, [ ("Inode" ,"id"         ), ("VectorClock","known"    )                          ] ),
    Cmd( "load_and_bind_obj      ",  7, [ ("Inode" ,"id"         )                                                       ] ),
    Cmd( "load_obj               ",  8, [ ("Inode" ,"id"         )                                                       ] ),
    Cmd( "get_dev_info           ",  9, [ ("DevId" ,"dev"        )                                                       ] ),
    Cmd( "ice_candidate          ", 10, [ ("DevId" ,"destination"), ("PI8"        ,"initiator"), ("String"     ,"jsep")  ] ),
    Cmd( "webrtc_offer           ", 11, [ ("DevId" ,"destination"), ("String"     ,"data"     )                          ] ),
    Cmd( "webrtc_answer          ", 12, [ ("DevId" ,"destination"), ("String"     ,"data"     )                          ] ),
    Cmd( "create_user            ", 13, [ ("String","user"       ), ("String"     ,"pass"     ), ("String"     ,"email") ] ),
]
db_cmds = [
    Cmd( "changes                ",  0, [ ("Inode" ,"inode"      ), ("Uint8Array" ,"data"     )                          ] ), #
    Cmd( "obj_data               ",  1, [ ("Inode" ,"inode"      ), ("Uint8Array" ,"data"     )                          ] ), #

    # //// authentication
    # //DECL_CMD( req_identity        ,  1  ); ///< request usr and dev id from the other size (for an instance capable to handle random values and to asymetric cryptography, with a stored list of public keys for trusted instances. => [ usr_id, and a random key encrypted by the known public keys ]+ 0u (the random value is the same accross the list)
    # //DECL_CMD( ans_identity        ,  2  ); ///< encrypted string for 123456789u, dev_id and usr_id (encrypted by the random value). string size = 0 means anon_usr.
    # //DECL_CMD( ack_identity        ,  3  ); ///< 0 -> bad. 1 -> ok
    #
    # //DECL_CMD( req_auth_std        ,  4  ); ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a login and to password. => login and password.
    # //DECL_CMD( req_auth_tok        ,  5  ); ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a token that may be used for authentication. => token.
    # //DECL_CMD( req_auth_yub        ,  6  ); ///< new connection from an instance relying on a PKI (typically a browser) to determine what is the user of the server, with a login, a password and a yubikey. => dev_id, login (or a zero sized string if not specified), the password and the yubikey.
    # //DECL_CMD( ans_auth            , 10  ); ///< answer to a PKI based connection. => [ dev_id, usr_id ] or 0u if bad credentials
    #
    #
    # //// content / patches
    # //DECL_CMD( req_content         , 11, Inode  ); ///< inode
    # //DECL_CMD( req_changes         , 12, Inode, VectorClock ); ///< inode, id_last_mod (SetOfPatchId)
    # //DECL_CMD( new_changes         , 13, Inode,   ); ///< inode, patch_data
    # //DECL_CMD( ack_changes         , 14  ); ///< inode, patch_id
    #
    # //// graphs
    # //DECL_CMD( graph_cmd           , 15  ); ///< msg
    # //DECL_CMD( graph_ans           , 16  ); ///< num, msg
    #
    # //// errors
    # //DECL_CMD( corrupted_data      , 20  ); ///< code
    # //DECL_CMD( corrupted_command   , 21  ); ///< code, inode
    # //DECL_CMD( incompatible_do_type, 22  ); ///< inode
    # //DECL_CMD( unknown_do_type     , 23  ); ///< inode
    # //DECL_CMD( inode_does_not_exist, 24  ); ///< inode
    #
    # //DECL_CMD( G                   , 71  ); ///< 'G', beginning of GET
    # //DECL_CMD( P                   , 80  ); ///< 'P', beginning of PUT, POST, ...
    #
    # //// helpers
    # //DECL_CMD( nop                 , 128 ); ///< a single byte (used e.g. for alignment)
    # //DECL_CMD( nnop                , 129 ); ///< n worthless bytes (used e.g. for alignment)
    # //DECL_CMD( end                 , 130 ); ///< graceful end of the connection
    # //DECL_CMD( quit                , 131 ); ///< graceful end of the db instance
]

class PatchType:
    def __init__( self ):
        pass

patch_types = [
    "OtWrapperString"
]

# generation
mytemplate = Template( filename = sys.argv[ 1 ] )
f = file( sys.argv[ 2 ], "w" )
f.write( "/// This file is generated (by bin/Method_template.py). Do not modify it directly.\n" )
f.write( mytemplate.render( methods = methods, objects = objects, self_ops = self_ops, std_ops = std_ops, jc = jc,
         string = string, decl_types_for_rw = decl_types_for_rw, known_inodes = known_inodes, itertools = itertools,
         tracker_client_cmds = tracker_client_cmds, tracker_server_cmds = tracker_server_cmds, db_cmds = db_cmds,
         patch_types = patch_types, decl_patch_types_for_rw = decl_patch_types_for_rw ) )
