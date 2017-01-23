import DevId from "../System/DevId"

/** */
abstract class DbConn {
    abstract send_changes( changes: Uint8Array ): void;
    abstract dst_dev_id(): DevId;
}
export default DbConn;
