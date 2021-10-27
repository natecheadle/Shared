
#include <Serialize/PrintHexArray.hpp>
#include <Serialize/SerializeDeserializeNum.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <variant>

namespace Shared {
    struct SerializeDeserialize_TestInputs
    {
        enum class Type
        {
            Int16  = 0,
            UInt16 = 1,
            Int32  = 2,
            UInt32 = 3,
            Int64  = 4,
            UInt64 = 5,
            Float  = 6,
            Double = 7
        };

        const Type DataType;
        const std::variant<short, unsigned short, int, unsigned int, long long, unsigned long long, float, double>
                                         Value;
        const size_t                     ValueSize;
        const std::vector<unsigned char> ExpectedLEResult;
        const std::vector<unsigned char> ExpectedBEResult;

        friend std::ostream& operator<<(std::ostream& os, const SerializeDeserialize_TestInputs& input)
        {
            os << "{ DataType: " << int(input.DataType) << ", Value: ";
            switch (input.DataType)
            {
            case Type::Int16:
                os << std::get<short>(input.Value);
                break;
            case Type::UInt16:
                os << std::get<unsigned short>(input.Value);
                break;
            case Type::Int32:
                os << std::get<int>(input.Value);
                break;
            case Type::UInt32:
                os << std::get<unsigned int>(input.Value);
                break;
            case Type::Int64:
                os << std::get<long long>(input.Value);
                break;
            case Type::UInt64:
                os << std::get<unsigned long long>(input.Value);
                break;
            case Type::Float:
                os << std::get<float>(input.Value);
                break;
            case Type::Double:
                os << std::get<double>(input.Value);
                break;
            }
            os << ", ValueSize: " << input.ValueSize << ", ExpectedLEResult: ";

            Shared::PrintHexArray(os, input.ExpectedLEResult.begin(), input.ExpectedLEResult.end());
            os << ", ExpectedBEResult: ";
            Shared::PrintHexArray(os, input.ExpectedBEResult.begin(), input.ExpectedBEResult.end());

            return os;
        }

        template <class T>
        static void TestSerialize(SerializeDeserialize_TestInputs inputs)
        {
            static_assert(std::is_arithmetic<T>::value, "T must be unsigned integral type.");

            unsigned char* pRawData = new unsigned char[sizeof(T)];

            Shared::SerializeArithmaticType(std::get<T>(inputs.Value), pRawData, pRawData + sizeof(T));

            EXPECT_EQ(inputs.ExpectedLEResult, std::vector<unsigned char>(pRawData, pRawData + sizeof(T)));

            Shared::SerializeArithmaticType<T, decltype(pRawData), Shared::Endianess::BigEndian>(
                std::get<T>(inputs.Value),
                pRawData,
                pRawData + sizeof(T));

            EXPECT_EQ(inputs.ExpectedBEResult, std::vector<unsigned char>(pRawData, pRawData + sizeof(T)));

            delete[] pRawData;
        }

        template <class T>
        static void TestDeserialize(SerializeDeserialize_TestInputs inputs)
        {
            T integralValue =
                Shared::DeSerializeArithmaticType<T>(inputs.ExpectedLEResult.begin(), inputs.ExpectedLEResult.end());

            EXPECT_EQ(std::get<T>(inputs.Value), integralValue);

            integralValue = Shared::
                DeSerializeArithmaticType<T, decltype(inputs.ExpectedBEResult.begin()), Shared::Endianess::BigEndian>(
                    inputs.ExpectedBEResult.begin(),
                    inputs.ExpectedBEResult.end());

            EXPECT_EQ(std::get<T>(inputs.Value), integralValue);
        }
    };

    class SerializeDeserialize_UnitTests : public testing::TestWithParam<SerializeDeserialize_TestInputs> {
    };

    TEST_P(SerializeDeserialize_UnitTests, ValidateSerializeIntegralValue)
    {
        SerializeDeserialize_TestInputs inputs = GetParam();

        switch (inputs.DataType)
        {
        case SerializeDeserialize_TestInputs::Type::Int16:
            SerializeDeserialize_TestInputs::TestSerialize<short>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Int32:
            SerializeDeserialize_TestInputs::TestSerialize<int>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Int64:
            SerializeDeserialize_TestInputs::TestSerialize<long long>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::UInt16:
            SerializeDeserialize_TestInputs::TestSerialize<unsigned short>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::UInt32:
            SerializeDeserialize_TestInputs::TestSerialize<unsigned int>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::UInt64:
            SerializeDeserialize_TestInputs::TestSerialize<unsigned long long>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Float:
            SerializeDeserialize_TestInputs::TestSerialize<float>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Double:
            SerializeDeserialize_TestInputs::TestSerialize<double>(GetParam());
            break;
        }
    }

    TEST_P(SerializeDeserialize_UnitTests, ValidateDeserializeIntegralValue)
    {
        SerializeDeserialize_TestInputs inputs = GetParam();

        switch (inputs.DataType)
        {
        case SerializeDeserialize_TestInputs::Type::Int16:
            SerializeDeserialize_TestInputs::TestDeserialize<short>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Int32:
            SerializeDeserialize_TestInputs::TestDeserialize<int>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Int64:
            SerializeDeserialize_TestInputs::TestDeserialize<long long>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::UInt16:
            SerializeDeserialize_TestInputs::TestDeserialize<unsigned short>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::UInt32:
            SerializeDeserialize_TestInputs::TestDeserialize<unsigned int>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::UInt64:
            SerializeDeserialize_TestInputs::TestDeserialize<unsigned long long>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Float:
            SerializeDeserialize_TestInputs::TestDeserialize<float>(GetParam());
            break;
        case SerializeDeserialize_TestInputs::Type::Double:
            SerializeDeserialize_TestInputs::TestDeserialize<double>(GetParam());
            break;
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        CRCTypes,
        SerializeDeserialize_UnitTests,
        testing::Values(
            SerializeDeserialize_TestInputs({
                SerializeDeserialize_TestInputs::Type::Int16,
                short(1000),
                sizeof(short),
                {0xE8, 0x03},
                {0x03, 0xE8}
    }),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Int16,
                 short(-1000),
                 sizeof(short),
                 {0x18, 0xFC},
                 {0xFC, 0x18}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::UInt16,
                 (unsigned short)2000,
                 sizeof(unsigned short),
                 {0xD0, 0x07},
                 {0x07, 0xD0}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Int32,
                 int(2222222),
                 sizeof(int),
                 {0x8E, 0xE8, 0x21, 0x00},
                 {0x00, 0x21, 0xE8, 0x8E}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Int32,
                 int(-2222222),
                 sizeof(int),
                 {0x72, 0x17, 0xDE, 0xFF},
                 {0xFF, 0xDE, 0x17, 0x72}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::UInt32,
                 (unsigned int)222222200,
                 sizeof(unsigned int),
                 {0x78, 0xD7, 0x3E, 0x0D},
                 {0x0D, 0x3E, 0xD7, 0x78}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Int64,
                 (long long)22222222153325,
                 sizeof(long long),
                 {0x6D, 0x56, 0xA8, 0x03, 0x36, 0x14, 0x00, 0x00},
                 {0x00, 0x00, 0x14, 0x36, 0x03, 0xA8, 0x56, 0x6D}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Int64,
                 (long long)-22222222153325,
                 sizeof(long long),
                 {0x93, 0xA9, 0x57, 0xFC, 0xC9, 0xEB, 0xFF, 0xFF},
                 {0xFF, 0xFF, 0xEB, 0xC9, 0xFC, 0x57, 0xA9, 0x93}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::UInt64,
                 (unsigned long long)22222222153325,
                 sizeof(unsigned long long),
                 {0x6D, 0x56, 0xA8, 0x03, 0x36, 0x14, 0x00, 0x00},
                 {0x00, 0x00, 0x14, 0x36, 0x03, 0xA8, 0x56, 0x6D}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Float,
                 (float)1.5,
                 sizeof(float),
                 {0x00, 0x00, 0xC0, 0x3F},
                 {0x3F, 0xC0, 0x00, 0x00}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Float,
                 (float)-.002,
                 sizeof(float),
                 {0x6F, 0x12, 0x03, 0xBB},
                 {0xBB, 0x03, 0x12, 0x6F}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Double,
                 (double)-.002,
                 sizeof(double),
                 {0xFC, 0xA9, 0xF1, 0xD2, 0x4D, 0x62, 0x60, 0xBF},
                 {0xBF, 0x60, 0x62, 0x4D, 0xD2, 0xF1, 0xA9, 0xFC}}),
            SerializeDeserialize_TestInputs(
                {SerializeDeserialize_TestInputs::Type::Double,
                 (double)1.5,
                 sizeof(double),
                 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x3F},
                 {0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}})));

} // namespace Shared