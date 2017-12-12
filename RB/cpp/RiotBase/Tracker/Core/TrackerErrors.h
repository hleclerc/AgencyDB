#pragma once

#include "../../System/TypeConfig.h"

namespace RiotBase {
namespace TrackerErrors {

#define DECL_ERR( CMD, NUM ) constexpr PI8 CMD = NUM;
#include "TrackerErrorsDecl.h"
#undef DECL_ERR

} // namespace TrackerErrors
} // namespace RiotBase
