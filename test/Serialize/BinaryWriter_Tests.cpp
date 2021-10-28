
#include <Serialize/BinaryWriter.hpp>

#include <gtest/gtest.h>

namespace Shared {
    TEST(BinaryWriter_UnitTests, ValidateBasicReadWrite)
    {
        const int              initVal1{10};
        const int              initVal2{20};
        const float            initVal3{30.0};
        const double           initVal4{40.0};
        const std::vector<int> initVal5({1, 2, 3, 4, 5});
        const size_t           expectedSize{
            sizeof(initVal1) + sizeof(initVal2) + sizeof(initVal3) + sizeof(initVal4) +
            initVal5.size() * sizeof(*initVal5.begin()) + sizeof(size_t)};

        BinaryWriter writer;

        writer.WriteNum(initVal1);
        writer.WriteNum(initVal2);
        writer.WriteNum(initVal3);
        writer.WriteNum(initVal4);
        writer.WriteArrayNum(initVal5.begin(), initVal5.end());

        writer.Reset();

        int              val1{writer.ReadNum<int>()};
        int              val2{writer.ReadNum<int>()};
        float            val3{writer.ReadNum<float>()};
        double           val4{writer.ReadNum<double>()};
        std::vector<int> val5(writer.ReadArrayNum<int>());
        EXPECT_EQ(initVal1, val1);
        EXPECT_EQ(initVal2, val2);
        EXPECT_EQ(initVal3, val3);
        EXPECT_EQ(initVal4, val4);

        EXPECT_EQ(expectedSize, writer.GetSize());

        writer.Clear();
        EXPECT_EQ(0, writer.GetSize());
        EXPECT_EQ(writer.GetBegin(), writer.GetEnd());
    }

    TEST(BinaryWriter_UnitTests, ValidateExternalStorage)
    {
        std::vector<unsigned char> memBuffer(16, 0x00);

        const int initVal1{10};
        const int initVal2{20};

        BinaryWriter writer(memBuffer.begin(), memBuffer.end());

        writer.WriteNum(initVal1);
        writer.WriteNum(initVal2);
        writer.WriteNum(initVal1);
        writer.WriteNum(initVal2);
        ASSERT_THROW(writer.WriteNum(initVal1), std::runtime_error);

        writer.Reset();

        int val1{writer.ReadNum<int>()};
        int val2{writer.ReadNum<int>()};
        int val3{writer.ReadNum<int>()};
        int val4{writer.ReadNum<int>()};
        ASSERT_THROW(int val5{writer.ReadNum<int>()}, std::runtime_error);

        EXPECT_EQ(initVal1, val1);
        EXPECT_EQ(initVal2, val2);
        EXPECT_EQ(initVal1, val3);
        EXPECT_EQ(initVal2, val4);

        writer.Clear();
        EXPECT_EQ(memBuffer.size(), writer.GetSize());
    }
} // namespace Shared