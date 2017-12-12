#pragma once

#include "../Core/TypeListInCategory.h"
#include "../Number.h"

namespace RiotBase {
template<class T> class Bn;

//
template<>
struct TypeListInCategory<Number> {
    using T = std::tuple<Bn<SI32>,Bn<SI64>,Bn<FP32>,Bn<FP64>>;
};


}
