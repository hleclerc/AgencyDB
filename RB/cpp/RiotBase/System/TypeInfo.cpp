#include "TypeConfig.h"
#include "TypeInfo.h"

namespace RiotBase {

template<> std::string TypeInfo<Bool>::name() { return "Bool"; }

template<> std::string TypeInfo<SI8 >::name() { return "SI8 "; }
template<> std::string TypeInfo<SI16>::name() { return "SI16"; }
template<> std::string TypeInfo<SI32>::name() { return "SI32"; }
template<> std::string TypeInfo<SI64>::name() { return "SI64"; }

template<> std::string TypeInfo<PI8 >::name() { return "PI8 "; }
template<> std::string TypeInfo<PI16>::name() { return "PI16"; }
template<> std::string TypeInfo<PI32>::name() { return "PI32"; }
template<> std::string TypeInfo<PI64>::name() { return "PI64"; }

template<> std::string TypeInfo<FP32>::name() { return "FP32"; }
template<> std::string TypeInfo<FP64>::name() { return "FP64"; }

template<> std::string TypeInfo<SI32,SI32>::name() { return "SI32,SI32"; }

template<> unsigned TypeInfo<Bool>::cat_id() { return  1; }

template<> unsigned TypeInfo<SI8 >::cat_id() { return  2; }
template<> unsigned TypeInfo<SI16>::cat_id() { return  3; }
template<> unsigned TypeInfo<SI32>::cat_id() { return  4; }
template<> unsigned TypeInfo<SI64>::cat_id() { return  5; }

template<> unsigned TypeInfo<PI8 >::cat_id() { return  6; }
template<> unsigned TypeInfo<PI16>::cat_id() { return  7; }
template<> unsigned TypeInfo<PI32>::cat_id() { return  8; }
template<> unsigned TypeInfo<PI64>::cat_id() { return  9; }

template<> unsigned TypeInfo<FP32>::cat_id() { return 10; }
template<> unsigned TypeInfo<FP64>::cat_id() { return 11; }

template<> unsigned TypeInfo<SI32,SI32>::cat_id() { return  12; } // HUM

} // namespace RiotBase
