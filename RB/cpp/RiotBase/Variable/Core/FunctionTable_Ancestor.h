#pragma once

namespace RiotBase {

///
class FunctionTable_Ancestor {
public:
    FunctionTable_Ancestor();
    virtual ~FunctionTable_Ancestor();
    virtual void resize_table() = 0;

    static FunctionTable_Ancestor *last_function_table;
    FunctionTable_Ancestor        *prev_function_table;
    const char                    *name;
};

} // namespace RiotBase
