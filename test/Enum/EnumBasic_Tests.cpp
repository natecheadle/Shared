#include <Enum/Enum.hpp>

#include <gtest/gtest.h>

namespace Shared {
    enum class TestEnum
    {
        First,
        Second,
        Third,
        Fourth
    };

    class TestEnum_Basic : public Enum<TestEnum, TestEnum::First, TestEnum::Fourth, int, TestEnum_Basic> {
      public:
        TestEnum_Basic()
            : Enum()
        {
        }
        TestEnum_Basic(TestEnum value)
            : Enum(value)
        {
        }
        TestEnum_Basic(const TestEnum_Basic& other)
            : Enum(other)
        {
        }
    };

    TEST(EnumBasic_Tests, ValidateAssignmentOperator)
    {
        TestEnum_Basic basicEnum;
        basicEnum = TestEnum::Third;

        EXPECT_EQ(TestEnum_Basic(TestEnum::Third), basicEnum);
    }

    TEST(EnumBasic_Tests, ValidateIterator)
    {
        TestEnum_Basic basicEnum;
        for (auto it = basicEnum.Begin(); it != basicEnum.End(); ++it)
        {
            EXPECT_EQ(*it, basicEnum);
            ++basicEnum;
        }
    }

    TEST(EnumBasic_Tests, ValidateIncrementDecrement)
    {
        TestEnum_Basic basicEnum(TestEnum::First);
        ++basicEnum;
        EXPECT_EQ(TestEnum_Basic(TestEnum::Second), basicEnum);

        --basicEnum;
        EXPECT_EQ(TestEnum_Basic(TestEnum::First), basicEnum);

        --basicEnum;
        EXPECT_EQ(TestEnum_Basic(TestEnum::First), basicEnum);

        basicEnum = TestEnum::Fourth;

        ++basicEnum;
        EXPECT_EQ(TestEnum_Basic(TestEnum::Fourth), basicEnum);
    }

    TEST(EnumBasic_Tests, ValidateConditionalOperators)
    {
        TestEnum_Basic basicEnum1(TestEnum::First);
        TestEnum_Basic basicEnum2(TestEnum::Second);
        TestEnum_Basic basicEnum3(TestEnum::Third);
        TestEnum_Basic basicEnum1_1(TestEnum::First);

        EXPECT_TRUE(basicEnum1 < basicEnum2);
        EXPECT_FALSE(basicEnum2 < basicEnum2);

        EXPECT_TRUE(basicEnum1 <= basicEnum2);
        EXPECT_TRUE(basicEnum2 <= basicEnum2);
        EXPECT_FALSE(basicEnum3 <= basicEnum2);

        EXPECT_TRUE(basicEnum2 > basicEnum1);
        EXPECT_FALSE(basicEnum2 > basicEnum2);

        EXPECT_TRUE(basicEnum2 >= basicEnum1);
        EXPECT_TRUE(basicEnum2 >= basicEnum2);
        EXPECT_FALSE(basicEnum2 >= basicEnum3);

        EXPECT_TRUE(basicEnum1_1 == basicEnum1);
        EXPECT_FALSE(basicEnum1 == basicEnum2);

        EXPECT_FALSE(basicEnum1_1 != basicEnum1);
        EXPECT_TRUE(basicEnum1 != basicEnum2);
    }

    TEST(EnumBasic_Tests, ValidateOstream)
    {
        std::stringstream stream;
        TestEnum_Basic    basicEnum(TestEnum::First);

        stream << basicEnum;

        EXPECT_EQ("0", stream.str());
    }

    TEST(EnumBasic_Tests, ValidateAddSubOperators)
    {
        TestEnum_Basic basicEnum(TestEnum::First);
        basicEnum += 2;

        EXPECT_EQ(TestEnum_Basic(TestEnum::Third), basicEnum);
        basicEnum = TestEnum::First;

        auto newEnum = basicEnum + 1;

        EXPECT_EQ(TestEnum_Basic(TestEnum::Second), newEnum);

        basicEnum = TestEnum::Fourth;
        basicEnum -= 2;

        EXPECT_EQ(TestEnum_Basic(TestEnum::Second), basicEnum);

        basicEnum = TestEnum::Fourth;
        newEnum   = basicEnum - 1;

        EXPECT_EQ(TestEnum_Basic(TestEnum::Third), newEnum);
    }

    TEST(EnumBasic_Tests, ValidateIsValid)
    {
        TestEnum_Basic basicEnum;
        EXPECT_THROW(basicEnum.FromIntegral(5), std::invalid_argument);

        EXPECT_NO_THROW(basicEnum.FromIntegral(2));
    }
} // namespace Shared