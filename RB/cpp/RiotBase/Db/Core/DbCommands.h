#pragma once

#include "../../System/TypeConfig.h"

namespace RiotBase {
namespace DbCommands {

#define DECL_CMD( CMD, NUM ) constexpr PI8 CMD = NUM;
#include "DbCommandsDecl.h"
#undef DECL_CMD

} // namespace DbCommands
} // namespace RiotBase
