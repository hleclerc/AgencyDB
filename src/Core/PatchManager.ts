

/** */
abstract class PatchManager {
    /** called by OtWrappers to register a modification */
    abstract new_modification( mod: Uint8Array );

}
export default PatchManager; 
