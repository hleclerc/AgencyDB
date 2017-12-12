#pragma once

namespace RiotBase {
class Rp;

/// `value` will be owned by the function
///
void set_lazy_val( const Rp *symbol, Rp *value );

} // namespace RiotBase
