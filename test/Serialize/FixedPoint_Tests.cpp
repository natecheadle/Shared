#include <Serialize/FixedPoint.hpp>

#include <gtest/gtest.h>

#include <variant>

namespace Shared {
    TEST(FixedPoint_UnitTests, FixedPointConstructionFromDouble)
    {
        Shared::FixedPoint<unsigned short, 8, 8> U16_8Value((double)1.0);

        EXPECT_DOUBLE_EQ(1.0, U16_8Value.ToDouble());
        EXPECT_EQ((unsigned short)1 << 8, U16_8Value.ToIntegral());

        U16_8Value = (double)2.0;

        EXPECT_DOUBLE_EQ(2.0, U16_8Value.ToDouble());
        EXPECT_EQ((unsigned short)2 << 8, U16_8Value.ToIntegral());

        U16_8Value = (unsigned short)1;

        EXPECT_DOUBLE_EQ(U16_8Value.GetStepSize(), U16_8Value.ToDouble());
        EXPECT_EQ(1, U16_8Value.ToIntegral());

        EXPECT_THROW(U16_8Value = (double)-1.0, std::out_of_range);
        EXPECT_THROW(U16_8Value = (double)1000.0, std::out_of_range);
    }

    TEST(FixedPoint_UnitTests, StaticMethods)
    {
        {
            Shared::FixedPoint<unsigned short, 8, 8> U16_8Value;

            size_t fractionalBits = U16_8Value.GetFractionalBits();
            size_t integerBits    = U16_8Value.GetIntegerBits();
            EXPECT_EQ(8, fractionalBits);
            EXPECT_EQ(8, integerBits);

            double stepSize = U16_8Value.GetStepSize();
            EXPECT_DOUBLE_EQ((1.0 / (double)((unsigned short)1 << 8)), stepSize);

            double max         = U16_8Value.GetMax();
            double expectedMax = (double)((unsigned short)1 << 8) - U16_8Value.GetStepSize();
            EXPECT_DOUBLE_EQ(expectedMax, max);

            double min = U16_8Value.GetMin();
            EXPECT_DOUBLE_EQ(0, min);
        }
        {
            Shared::FixedPoint<short, 8, 8> S16_8Value;

            size_t fractionalBits = S16_8Value.GetFractionalBits();
            size_t integerBits    = S16_8Value.GetIntegerBits();
            EXPECT_EQ(8, fractionalBits);
            EXPECT_EQ(8, integerBits);

            double stepSize = S16_8Value.GetStepSize();
            EXPECT_DOUBLE_EQ((1.0 / (double)((unsigned short)1 << 8)), stepSize);

            double max         = S16_8Value.GetMax();
            double expectedMax = (double)((unsigned short)1 << 7) - S16_8Value.GetStepSize();
            EXPECT_DOUBLE_EQ(expectedMax, max);

            double min = S16_8Value.GetMin();
            EXPECT_DOUBLE_EQ(-expectedMax, min);
        }
    }

    TEST(FixedPoint_UnitTests, ValidateNonStandardSizes)
    {
        Shared::FixedPoint<unsigned short, 1, 8> U9_8Value;

        size_t fractionalBits = U9_8Value.GetFractionalBits();
        size_t integerBits    = U9_8Value.GetIntegerBits();
        EXPECT_EQ(8, fractionalBits);
        EXPECT_EQ(1, integerBits);

        double stepSize = U9_8Value.GetStepSize();
        EXPECT_DOUBLE_EQ((1.0 / (double)((unsigned short)1 << 8)), stepSize);

        double max         = U9_8Value.GetMax();
        double expectedMax = double(2) - U9_8Value.GetStepSize();
        EXPECT_DOUBLE_EQ(expectedMax, max);

        double min = U9_8Value.GetMin();
        EXPECT_DOUBLE_EQ(0, min);
    }

    TEST(FixedPoint_UnitTests, ValidateRound)
    {
        double roundedValue = Shared::FixedPoint<short, 16, 0>::Round(1.5);
        EXPECT_DOUBLE_EQ(2.0, roundedValue);
        roundedValue = Shared::FixedPoint<short, 16, 0>::Round(1.45);
        EXPECT_DOUBLE_EQ(1.0, roundedValue);

        double truncatedValue = Shared::FixedPoint<short, 16, 0>::Truncate(1.5);
        EXPECT_DOUBLE_EQ(1.0, truncatedValue);
        truncatedValue = Shared::FixedPoint<short, 16, 0>::Truncate(1.45);
        EXPECT_DOUBLE_EQ(1.0, truncatedValue);
    }
} // namespace Shared