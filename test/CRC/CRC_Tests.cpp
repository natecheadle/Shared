#include <CRC/CRCTypes.h>

#include <gtest/gtest.h>

#include <map>

namespace Shared {
enum class CRC_Type { XModem, DNP };
static const std::map<CRC_Type,
                      std::map<unsigned short, std::vector<unsigned char>>>
    defaultTestData = {
        {CRC_Type::XModem,
         {{0x31C3, {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}},
          {0x56BE,
           {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
            0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03,
            0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
            0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
            0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02,
            0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
            0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
            0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05,
            0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}}}},
        {CRC_Type::DNP,
         {{0xEA82, {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}},
          {0x17CF,
           {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
            0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03,
            0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
            0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
            0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02,
            0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
            0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
            0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x02, 0x03, 0x04, 0x05,
            0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}}}}};

TEST(CRC_UnitTests, ValidateTableCalculation) {
  for (const std::pair<CRC_Type,
                       std::map<unsigned short, std::vector<unsigned char>>>
           &CrcType : defaultTestData) {
    for (const std::pair<unsigned short, std::vector<unsigned char>>
             &testValue : CrcType.second) {
      unsigned short result{0};
      if (CrcType.first == CRC_Type::XModem) {
        result = XModem16BitCRC::CalculateCRCViaTable(testValue.second.begin(),
                                                      testValue.second.end());
      } else {
        result = DNP16BitCRC ::CalculateCRCViaTable(testValue.second.begin(),
                                                    testValue.second.end());
      }

      EXPECT_EQ(testValue.first, result);
    }
  }
}

TEST(CRC_UnitTests, ValidateBitWiseCalculation) {
  for (const std::pair<CRC_Type,
                       std::map<unsigned short, std::vector<unsigned char>>>
           &CrcType : defaultTestData) {
    for (const std::pair<unsigned short, std::vector<unsigned char>>
             &testValue : CrcType.second) {
      unsigned short result{0};
      if (CrcType.first == CRC_Type::XModem) {
        result = XModem16BitCRC::CalculateCRCBitWise(testValue.second.begin(),
                                                     testValue.second.end());
      } else {
        result = DNP16BitCRC::CalculateCRCBitWise(testValue.second.begin(),
                                                  testValue.second.end());
      }

      EXPECT_EQ(testValue.first, result);
    }
  }
}
} // namespace Shared