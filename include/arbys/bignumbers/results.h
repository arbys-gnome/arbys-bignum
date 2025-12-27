#ifndef BIGNUMBERS_RESULTS_H
#define BIGNUMBERS_RESULTS_H
#include "big_int.h"

namespace arbys::bignumbers {

struct DivisionResult {
    big_int quotient;
    big_int remainder;
};

} // namespace arbys::bignumbers

#endif // BIGNUMBERS_RESULTS_H