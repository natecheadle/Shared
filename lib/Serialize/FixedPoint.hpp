#pragma once

#include <cmath>
#include <stdexcept>
#include <string>

namespace Shared
{

    template<class BASE_TYPE, size_t INT_BITS, size_t FRAC_BITS>
    class FixedPoint
    {
      public:
        FixedPoint()
            : m_FixedPointValue(0.0)
        {
            static_assert((sizeof(BASE_TYPE) * 8) >= (INT_BITS + FRAC_BITS),
                          "FRACT_BITS + INT_BITS must be less than or equal to sizeof(BASE_TYPE) * 8");
            static_assert(sizeof(BASE_TYPE) < 8, "64 bit integers are not supported as base types.");
        }

        FixedPoint(double value)
            : m_FixedPointValue(Round(value))
        {
            if (value > GetMax()) {
                throw std::out_of_range(std::to_string(value) + " is greater than max value of " +
                                        std::to_string(GetMax()));
            } else if (value < GetMin()) {
                throw std::out_of_range(std::to_string(value) + " is less than min value of " +
                                        std::to_string(GetMax()));
            }
        }

        FixedPoint(BASE_TYPE value)
            : m_FixedPointValue((double)value * GetStepSize())
        {
        }

        ~FixedPoint() {}

        double ToDouble() const { return m_FixedPointValue; }
        BASE_TYPE ToIntegral() const
        {
            return static_cast<BASE_TYPE>(m_FixedPointValue * ((unsigned long long)1 << FRAC_BITS));
        }

        static bool IsSigned() { return std::is_signed<BASE_TYPE>::value; }

        static size_t GetSize() { return INT_BITS + FRAC_BITS; }

        static double GetMax()
        {
            unsigned long long maxIntegralValue;

            if (IsSigned()) {
                maxIntegralValue = ((unsigned long long)1 << (GetSize() - 1)) - 1;
            } else {
                maxIntegralValue = ((unsigned long long)1 << GetSize()) - 1;
            }

            return (double)maxIntegralValue * GetStepSize();
        }

        static double GetMin()
        {
            if (IsSigned()) {
                return GetMax() * -1;
            } else {
                return 0;
            }
        }

        static double GetStepSize() { return double(1.0) / double((unsigned long long)1 << FRAC_BITS); }

        static size_t GetFractionalBits() { return FRAC_BITS; }
        static size_t GetIntegerBits() { return INT_BITS; }

        static double Round(double value)
        {
            double remainder = std::fmod(value, GetStepSize());

            if (remainder >= (GetStepSize() / 2)) {
                return value + (GetStepSize() - remainder);
            } else {
                return value - remainder;
            }
        }

        static double Truncate(double value)
        {
            double remainder = std::fmod(value, GetStepSize());

            return value - remainder;
        }

      private:
        double m_FixedPointValue;
    };
} // namespace Shared
