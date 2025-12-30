#pragma once

#include "big_int.h"

namespace arbys::bignum {

struct DivisionResult {
    big_int quotient;
    big_int remainder;
};

} // namespace arbys::bignum