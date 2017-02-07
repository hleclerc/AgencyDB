//// nsmake global alias ./RpDirectoryOperations.ts ./(GenRpDirectoryOperations.ts).ts
import OtWrapperWithRightFlags from "../Core/OtWrapperWithRightFlags"
import BinaryWriter            from "../System/BinaryWriter"
import BinaryReader            from "../System/BinaryReader"
import Inode                   from "../System/Inode"
import UsrId                   from "../System/UsrId"
import DevId                   from "../System/DevId"
import methods                 from "../Core/methods"
import Rp                      from "../Core/Rp"
import LvDirectory             from "../LvDirectory"
import LvString                from "../LvString"
import RpDirectoryOperations   from "./RpDirectoryOperations"

export default 
class RpDirectory extends OtWrapperWithRightFlags {
    constructor( usr_id = new UsrId ) {
        super( RpDirectory, usr_id );
    }

    variable_type__b() {
        return LvDirectory;
    }

    to_String__b(): string {
        return `{${ this.data.map( x => x.name.toString() + ":" + x.inode.toString() ).join( "," ) }}`;
    }

    copy__b( usr_id = new UsrId ): Rp {
        let res = new RpDirectory( usr_id );
        res.data.push( ...this.data );
        return res;
    }

    new_patch( res: BinaryWriter, msg: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
        RpDirectoryOperations.new_patch( this, res, msg, as_usr, cq_unk, null, src_dev, src_usr, cur_dev, cur_usr );
    }

    undo_patch( data: BinaryReader, as_usr: UsrId ) {
        RpDirectoryOperations.undo_patch( this, data, as_usr );
    }

    get_possible_rights__b(): Array<string> {
        return RpDirectoryOperations.get_possible_rights__b();
    }

    add_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr = new UsrId ) {
        this.sig_change( RpDirectoryOperations.tar.AddUsrRight( this, as_usr, usr, this._right_flags_from_right_type( right_types ) ) );
        return this;
    }

    rem_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr = new UsrId ) {
        this.sig_change( RpDirectoryOperations.tar.RemUsrRight( this, as_usr, usr, this._right_flags_from_right_type( right_types ) ) );
        return this;
    }

    set( name: string, inode: Inode, _as_usr = new UsrId ): void {
        const ind = this.data.findIndex( x => x.name == name );
        if ( ind >= 0 )
            this.data[ ind ].inode = inode;
        else
            this.data.push( { name, inode } );
    }

    delete( name: string ): void {
        const ind = this.data.findIndex( x => x.name == name );
        if ( ind >= 0 )
            this.data.splice( ind, 1 );
    }

    data = new Array<{name:string,inode:Inode}>();
} 
Rp.make_templ( RpDirectory );

// for now, we assume that inode is of type RpInode
methods["set__sbb"].add_surdef( 3, [ RpDirectory, type => ! type.symbolic_value, type => ! type.symbolic_value ], function( dir: RpDirectory, name: Rp, inode ) {
    dir.sig_change( RpDirectoryOperations.tar.SetVal( dir, new UsrId, methods[ "to_String__b" ].call_1( name ), inode.data ) );
    return dir;
} );

methods["sub__sb"].add_surdef( 3, [ RpDirectory, type => ! type.symbolic_value ], function( dir: RpDirectory, name: Rp ) {
    dir.sig_change( RpDirectoryOperations.tar.Delete( dir, new UsrId, methods[ "to_String__b" ].call_1( name ) ) );
    return dir;
} );

