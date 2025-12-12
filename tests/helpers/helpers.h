#ifndef BIGNUMBERS_HELPERS_H
#define BIGNUMBERS_HELPERS_H

#include <string>

namespace arbys::bignumbers {
    class BigInt;
}

namespace arbys::bignumbers::tests::helpers {
    void expectCompare(const BigInt& a, const BigInt& b, std::strong_ordering expected);
    std::string makeIncreasing(size_t len);
    void expectBigIntEquals(const BigInt &bn, const std::string &s);
    std::string bigIntegerStringAdd(const std::string& a, const std::string& b);
} // namespace arbys::bignumbers::tests::helpers

#endif //BIGNUMBERS_HELPERS_H
