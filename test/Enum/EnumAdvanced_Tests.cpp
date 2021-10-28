#include <Enum/Enum.hpp>

#include <gtest/gtest.h>

namespace Shared {
    enum class AdvancedTestEnum
    {
        First  = 0,
        Second = 2,
        Third  = 4,
        Fourth = 8
    };

    class TestEnum_Advanced
        : public Enum<AdvancedTestEnum, AdvancedTestEnum::First, AdvancedTestEnum::Fourth, int, TestEnum_Advanced> {
      public:
        TestEnum_Advanced()
            : Enum()
        {
        }
        TestEnum_Advanced(AdvancedTestEnum value)
            : Enum(value)
        {
        }
        TestEnum_Advanced(const TestEnum_Advanced& other)
            : Enum(other)
        {
        }

      protected:
        AdvancedTestEnum Increment(AdvancedTestEnum value) const override
        {
            switch (value)
            {
            case AdvancedTestEnum::First:
                return AdvancedTestEnum::Second;
            case AdvancedTestEnum::Second:
                return AdvancedTestEnum::Third;
            case AdvancedTestEnum::Third:
                return AdvancedTestEnum::Fourth;
            default:
                return AdvancedTestEnum::Fourth;
            }
        }

        AdvancedTestEnum Decrement(AdvancedTestEnum value) const override
        {
            switch (value)
            {
            case AdvancedTestEnum::Second:
                return AdvancedTestEnum::First;
            case AdvancedTestEnum::Third:
                return AdvancedTestEnum::Second;
            case AdvancedTestEnum::Fourth:
                return AdvancedTestEnum::Third;
            default:
                return AdvancedTestEnum::First;
            }
        }

        void Print(std::ostream& os, AdvancedTestEnum value) const override
        {
            switch (value)
            {
            case AdvancedTestEnum::First:
                os << "First";
                break;
            case AdvancedTestEnum::Second:
                os << "Second";
                break;
            case AdvancedTestEnum::Third:
                os << "Third";
                break;
            case AdvancedTestEnum::Fourth:
                os << "Fourth";
                break;
            }
        }

        bool IsValid(int rawValue) const override
        {
            switch (rawValue)
            {
            case static_cast<int>(AdvancedTestEnum::First):
            case static_cast<int>(AdvancedTestEnum::Second):
            case static_cast<int>(AdvancedTestEnum::Third):
            case static_cast<int>(AdvancedTestEnum::Fourth):
                return true;
            }

            return false;
        }
    };

    TEST(EnumAdvanced_Tests, ValidateAssignmentOperator)
    {
        TestEnum_Advanced advEnum;
        advEnum = AdvancedTestEnum::Third;

        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::Third), advEnum);
    }

    TEST(EnumAdvanced_Tests, ValidateIterator)
    {
        TestEnum_Advanced advEnum;
        for (auto it = advEnum.Begin(); it != advEnum.End(); ++it)
        {
            EXPECT_EQ(*it, advEnum);
            ++advEnum;
        }
    }

    TEST(EnumAdvanced_Tests, ValidateIncrementDecrement)
    {
        TestEnum_Advanced advEnum(AdvancedTestEnum::First);
        ++advEnum;
        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::Second), advEnum);

        --advEnum;
        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::First), advEnum);

        --advEnum;
        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::First), advEnum);

        advEnum = AdvancedTestEnum::Fourth;

        ++advEnum;
        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::Fourth), advEnum);
    }

    TEST(EnumAdvanced_Tests, ValidateConditionalOperators)
    {
        TestEnum_Advanced advEnum1(AdvancedTestEnum::First);
        TestEnum_Advanced advEnum2(AdvancedTestEnum::Second);
        TestEnum_Advanced advEnum3(AdvancedTestEnum::Third);
        TestEnum_Advanced advEnum1_1(AdvancedTestEnum::First);

        EXPECT_TRUE(advEnum1 < advEnum2);
        EXPECT_FALSE(advEnum2 < advEnum2);

        EXPECT_TRUE(advEnum1 <= advEnum2);
        EXPECT_TRUE(advEnum2 <= advEnum2);
        EXPECT_FALSE(advEnum3 <= advEnum2);

        EXPECT_TRUE(advEnum2 > advEnum1);
        EXPECT_FALSE(advEnum2 > advEnum2);

        EXPECT_TRUE(advEnum2 >= advEnum1);
        EXPECT_TRUE(advEnum2 >= advEnum2);
        EXPECT_FALSE(advEnum2 >= advEnum3);

        EXPECT_TRUE(advEnum1_1 == advEnum1);
        EXPECT_FALSE(advEnum1 == advEnum2);

        EXPECT_FALSE(advEnum1_1 != advEnum1);
        EXPECT_TRUE(advEnum1 != advEnum2);
    }

    TEST(EnumAdvanced_Tests, ValidateOstream)
    {
        std::stringstream stream;
        TestEnum_Advanced advEnum(AdvancedTestEnum::First);

        stream << advEnum;

        EXPECT_EQ("First", stream.str());
    }

    TEST(EnumAdvanced_Tests, ValidateAddSubOperators)
    {
        TestEnum_Advanced advEnum(AdvancedTestEnum::First);
        advEnum += 2;

        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::Third), advEnum);
        advEnum = AdvancedTestEnum::First;

        auto newEnum = advEnum + 1;

        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::Second), newEnum);

        advEnum = AdvancedTestEnum::Fourth;
        advEnum -= 2;

        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::Second), advEnum);

        advEnum = AdvancedTestEnum::Fourth;
        newEnum = advEnum - 1;

        EXPECT_EQ(TestEnum_Advanced(AdvancedTestEnum::Third), newEnum);
    }

    TEST(EnumAdvanced_Tests, ValidateIsValid)
    {
        TestEnum_Advanced advEnum;
        EXPECT_THROW(advEnum.FromIntegral(1), std::invalid_argument);

        EXPECT_NO_THROW(advEnum.FromIntegral(2));
    }
} // namespace Shared