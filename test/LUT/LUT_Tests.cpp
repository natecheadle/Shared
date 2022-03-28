#include <LUT/CompileTimeLookup.hpp>

#include <gtest/gtest.h>

#include <string>

namespace Shared {
    constexpr std::array<std::pair<int, std::string_view>, 3> TestTable = {
        {{1, "one"}, {2, "two"}, {3, "three"}}
    };
    typedef CompileTimeLookup<int, std::string_view, 3, TestTable> TestLookup;

    TEST(LUT_Tests, TestBasicLookup)
    {
        ASSERT_EQ(std::string("one"), TestLookup::GetValue(1));
        ASSERT_EQ(std::string("two"), TestLookup::GetValue(2));
        ASSERT_EQ(std::string("three"), TestLookup::GetValue(3));
    }
} // namespace Shared