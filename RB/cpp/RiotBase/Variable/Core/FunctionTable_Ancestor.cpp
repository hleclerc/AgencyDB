#include "FunctionTable_Ancestor.h"

namespace RiotBase {

FunctionTable_Ancestor *FunctionTable_Ancestor::last_function_table = 0;

FunctionTable_Ancestor::FunctionTable_Ancestor() {
    prev_function_table = last_function_table;
    last_function_table = this;
}

FunctionTable_Ancestor::~FunctionTable_Ancestor() {
}

} // namespace RiotBase
