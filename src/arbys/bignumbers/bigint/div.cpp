//
// Created by rares on 12/20/25.
//

#include <stdexcept>

#include "arbys/bignumbers/bigint.h"

namespace arbys::bignumbers {
    BigInt BigInt::div(long long other) const {
        if (other == 0)
            throw std::invalid_argument("Division by zero");

        bool resultIsNegative = is_negative_;

        long long divisor = other;
        if (divisor < 0) {
            divisor = std::abs(divisor);
            resultIsNegative = !resultIsNegative;
        }

        std::vector<limb_t> quotient(length_, 0);

        long long remainder = 0;

        // Traverse from most significant digit → least significant
        for (size_t i = 0; i < length_; i++) {
            const size_t j = length_ - 1 - i;

            // New value to divide: remainder * 10 + next_digit
            const long long value = remainder * 10 + digits_[j];

            const long long qDigit = value / divisor;
            const long long r      = value % divisor;

            quotient[j] = static_cast<limb_t>(qDigit);
            remainder   = r;
        }

        size_t newLength = quotient.size();
        while (newLength > 1 && quotient[newLength - 1] == 0)
            newLength--;

        quotient.resize(newLength);

        resultIsNegative = resultIsNegative && !(newLength == 1 && quotient[0] == 0);
        BigInt result(resultIsNegative, quotient, newLength);

        return result;
    }

    BigInt BigInt::operator/(long long other) const {
        return div(other);
    }
}
