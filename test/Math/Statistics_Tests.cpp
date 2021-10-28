#include <Math/Statistics.hpp>

#include <gtest/gtest.h>

#include <array>
#include <functional>

namespace Shared {
    struct ValueWithError
    {
        double Value;
        double Error;
    };

    constexpr std::array<ValueWithError, 10> InputValues1 = {
        ValueWithError({1.0, 0.0}),
        ValueWithError({2.0, 0.0}),
        ValueWithError({3.0, 0.0}),
        ValueWithError({4.0, 0.0}),
        ValueWithError({5.0, 0.0}),
        ValueWithError({6.0, 0.0}),
        ValueWithError({7.0, 0.0}),
        ValueWithError({8.0, 0.0}),
        ValueWithError({9.0, 0.0}),
        ValueWithError({10.0, 0.0}),
    };

    constexpr std::array<ValueWithError, 11> InputValues2 = {
        ValueWithError({1.0, 0.0}),
        ValueWithError({2.0, 0.0}),
        ValueWithError({3.0, 0.0}),
        ValueWithError({4.0, 0.0}),
        ValueWithError({5.0, 0.0}),
        ValueWithError({6.0, 0.0}),
        ValueWithError({7.0, 0.0}),
        ValueWithError({8.0, 0.0}),
        ValueWithError({9.0, 0.0}),
        ValueWithError({10.0, 0.0}),
        ValueWithError({11.0, 0.0}),
    };

    constexpr std::array<ValueWithError, 12> InputValues3 = {
        ValueWithError({1.0, 0.0}),
        ValueWithError({2.0, 0.0}),
        ValueWithError({3.0, 0.0}),
        ValueWithError({4.0, 0.0}),
        ValueWithError({5.0, 0.0}),
        ValueWithError({6.0, 0.0}),
        ValueWithError({7.0, 0.0}),
        ValueWithError({8.0, 0.0}),
        ValueWithError({9.0, 0.0}),
        ValueWithError({10.0, 0.0}),
        ValueWithError({11.0, 0.0}),
        ValueWithError({12.0, 0.0}),
    };

    constexpr std::array<double, 10> GetValues1()
    {
        std::array<double, 10> input;
        size_t                 i{0};

        for (const ValueWithError& value : InputValues1)
            input.at(i++) = value.Value;

        return input;
    }

    constexpr std::array<double, 11> GetValues2()
    {
        std::array<double, 11> input;
        size_t                 i{0};

        for (const ValueWithError& value : InputValues2)
            input.at(i++) = value.Value;

        return input;
    }

    constexpr std::array<double, 12> GetValues3()
    {
        std::array<double, 12> input;
        size_t                 i{0};

        for (const ValueWithError& value : InputValues3)
            input.at(i++) = value.Value;

        return input;
    }

    constexpr std::array<double, 10> dInputValues  = GetValues1();
    constexpr std::array<double, 11> dInputValues1 = GetValues2();
    constexpr std::array<double, 12> dInputValues2 = GetValues3();

    TEST(Statistics_Tests, ValidateMean)
    {
        double mean = Statistics::Mean(dInputValues.begin(), dInputValues.end());

        EXPECT_DOUBLE_EQ(5.5, mean);
    }

    TEST(Statistics_Tests, ValidateStructMean)
    {
        std::function<double(decltype(*InputValues1.begin()))> getX = [](decltype(*InputValues1.begin()) it) {
            return it.Value;
        };
        double mean = Statistics::Mean(InputValues1.begin(), InputValues1.end(), getX);

        EXPECT_DOUBLE_EQ(5.5, mean);
    }

    TEST(Statistics_Tests, ValidateStdDev)
    {
        double stdDev = Statistics::StdDeviation(dInputValues.begin(), dInputValues.end());

        EXPECT_TRUE(std::abs(2.872281323 - stdDev) < .000001);
    }

    TEST(Statistics_Tests, ValidateStructStdDev)
    {
        std::function<double(decltype(*InputValues1.begin()))> getX = [](decltype(*InputValues1.begin()) it) {
            return it.Value;
        };
        double stdDev = Statistics::StdDeviation(InputValues1.begin(), InputValues1.end(), getX);

        EXPECT_TRUE(std::abs(2.872281323 - stdDev) < .000001);
    }

    TEST(Statistics_Tests, ValidateMedian)
    {
        double median = Statistics::Median(dInputValues.begin(), dInputValues.end());
        EXPECT_DOUBLE_EQ(5.5, median);

        median = Statistics::Median(dInputValues1.begin(), dInputValues1.end());
        EXPECT_DOUBLE_EQ(6.0, median);
    }

    TEST(Statistics_Tests, ValidateStructMedian)
    {
        std::function<double(decltype(*InputValues1.begin()))> getX = [](decltype(*InputValues1.begin()) it) {
            return it.Value;
        };
        double median = Statistics::Median(InputValues1.begin(), InputValues1.end(), getX);

        EXPECT_DOUBLE_EQ(5.5, median);
    }

    TEST(Statistics_Tests, ValidateCalculateQuartiles)
    {
        const Statistics::Quartiles expectedValue1{-12, -4.5, 3, 5.5, 8, 15.5, 23};
        const Statistics::Quartiles expectedValue2{-15, -6, 3, 6, 9, 18, 27};
        const Statistics::Quartiles expectedValue3{-14.5, -5.5, 3.5, 6.5, 9.5, 18.5, 27.5};

        Statistics::Quartiles quartiles = Statistics::CalculateQuartiles(dInputValues.begin(), dInputValues.end());

        EXPECT_DOUBLE_EQ(expectedValue1.Median, quartiles.Median);
        EXPECT_DOUBLE_EQ(expectedValue1.Q1, quartiles.Q1);
        EXPECT_DOUBLE_EQ(expectedValue1.Q3, quartiles.Q3);
        EXPECT_DOUBLE_EQ(expectedValue1.LowerInnerFence, quartiles.LowerInnerFence);
        EXPECT_DOUBLE_EQ(expectedValue1.LowerOuterFence, quartiles.LowerOuterFence);
        EXPECT_DOUBLE_EQ(expectedValue1.UpperInnerFence, quartiles.UpperInnerFence);
        EXPECT_DOUBLE_EQ(expectedValue1.UpperOuterFence, quartiles.UpperOuterFence);

        Statistics::Quartiles quartiles2 = Statistics::CalculateQuartiles(dInputValues1.begin(), dInputValues1.end());

        EXPECT_DOUBLE_EQ(expectedValue2.Median, quartiles2.Median);
        EXPECT_DOUBLE_EQ(expectedValue2.Q1, quartiles2.Q1);
        EXPECT_DOUBLE_EQ(expectedValue2.Q3, quartiles2.Q3);
        EXPECT_DOUBLE_EQ(expectedValue2.LowerInnerFence, quartiles2.LowerInnerFence);
        EXPECT_DOUBLE_EQ(expectedValue2.LowerOuterFence, quartiles2.LowerOuterFence);
        EXPECT_DOUBLE_EQ(expectedValue2.UpperInnerFence, quartiles2.UpperInnerFence);
        EXPECT_DOUBLE_EQ(expectedValue2.UpperOuterFence, quartiles2.UpperOuterFence);

        Statistics::Quartiles quartiles3 = Statistics::CalculateQuartiles(dInputValues2.begin(), dInputValues2.end());

        EXPECT_DOUBLE_EQ(expectedValue3.Median, quartiles3.Median);
        EXPECT_DOUBLE_EQ(expectedValue3.Q1, quartiles3.Q1);
        EXPECT_DOUBLE_EQ(expectedValue3.Q3, quartiles3.Q3);
        EXPECT_DOUBLE_EQ(expectedValue3.LowerInnerFence, quartiles3.LowerInnerFence);
        EXPECT_DOUBLE_EQ(expectedValue3.LowerOuterFence, quartiles3.LowerOuterFence);
        EXPECT_DOUBLE_EQ(expectedValue3.UpperInnerFence, quartiles3.UpperInnerFence);
        EXPECT_DOUBLE_EQ(expectedValue3.UpperOuterFence, quartiles3.UpperOuterFence);
    }

    TEST(Statistics_Tests, ValidateStructCalculateQuartiles)
    {
        const Statistics::Quartiles expectedValue{-12, -4.5, 3, 5.5, 8, 15.5, 23};

        std::function<double(decltype(*InputValues1.begin()))> getX = [](decltype(*InputValues1.begin()) it) {
            return it.Value;
        };
        Statistics::Quartiles quartiles = Statistics::CalculateQuartiles(InputValues1.begin(), InputValues1.end(), getX);

        ASSERT_DOUBLE_EQ(expectedValue.Median, quartiles.Median);
        ASSERT_DOUBLE_EQ(expectedValue.Q1, quartiles.Q1);
        ASSERT_DOUBLE_EQ(expectedValue.Q3, quartiles.Q3);
        ASSERT_DOUBLE_EQ(expectedValue.LowerInnerFence, quartiles.LowerInnerFence);
        ASSERT_DOUBLE_EQ(expectedValue.LowerOuterFence, quartiles.LowerOuterFence);
        ASSERT_DOUBLE_EQ(expectedValue.UpperInnerFence, quartiles.UpperInnerFence);
        ASSERT_DOUBLE_EQ(expectedValue.UpperOuterFence, quartiles.UpperOuterFence);
    }
} // namespace Shared
