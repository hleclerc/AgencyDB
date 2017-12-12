/// This file is generated (by bin/Method_template.py). Do not modify it directly.
import Inode from "./Inode";

///
export default {
    root_home: new Inode( Inode.base_dev, 3 ),
    root_dir: new Inode( Inode.base_dev, 2 ),
    type_Number: new Inode( Inode.base_dev, 12 ),
    type_Directory: new Inode( Inode.base_dev, 10 ),
    type_String: new Inode( Inode.base_dev, 11 ),
};
