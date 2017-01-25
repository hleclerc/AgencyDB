import BinaryWriter from "../System/BinaryWriter"

/** */
abstract class PatchManager {
    /** called by OtWrappers to register a modification */
    abstract new_modification( wr: ( bw: BinaryWriter ) => void );

}
export default PatchManager; 
