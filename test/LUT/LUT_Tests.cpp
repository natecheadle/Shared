#include <LUT/CompileTimeLookup.hpp>

#include <gtest/gtest.h>

#include <string>

namespace Shared {
    constexpr std::array<std::pair<int, std::string_view>, 3> SimpleTestTable = {
        {{1, "one"}, {2, "two"}, {3, "three"}}
    };
    typedef CompileTimeLookup<int, std::string_view, 3, SimpleTestTable> SimpleTestLookup;

    enum class MyType
    {
        Test_Type,
        Another_Type,
        Last_type,
    };

    constexpr std::array<std::pair<MyType, std::string_view>, 3> ComplexTestTable = {
        {{MyType::Test_Type, "Test Type"}, {MyType::Another_Type, "Another Type"}, {MyType::Last_type, "Last Type"}}
    };
    typedef CompileTimeLookup<MyType, std::string_view, 3, ComplexTestTable> ComplexTestLookup;

    TEST(LUT_Tests, TestBasicLookup)
    {
        ASSERT_EQ(std::string("one"), SimpleTestLookup::GetValue(1));
        ASSERT_EQ(std::string("two"), SimpleTestLookup::GetValue(2));
        ASSERT_EQ(std::string("three"), SimpleTestLookup::GetValue(3));
    }

    TEST(LUT_Tests, TestComplexLookup)
    {
        ASSERT_EQ(std::string("Test Type"), ComplexTestLookup::GetValue(MyType::Test_Type));
        ASSERT_EQ(std::string("Another Type"), ComplexTestLookup::GetValue(MyType::Another_Type));
        ASSERT_EQ(std::string("Last Type"), ComplexTestLookup::GetValue(MyType::Last_type));
    }
} // namespace Shared