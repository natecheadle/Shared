#pragma once
#include <array>
#include <type_traits>

namespace Shared
{
    ///< summary>
    /// Helper Class for Calculating CRC Values either via a CRC Table or BitWise.
    /// T Data Type for CRC (Must be unsigned integral type.)
    ///</summary>
    template<class T, T POLYNOMIAL, T INIT_VALUE, T XOR_OUT, bool REFLECT_DATA>
    class CRCHelper
    {
      public:
        template<class INPUT_IT>
        static T CalculateCRCBitWise(INPUT_IT begin, INPUT_IT end)
        {
            static_assert(std::is_same<typename std::iterator_traits<INPUT_IT>::value_type, unsigned char>::value,
                          "INPUT_IT must be iterator over unsigned char");

            T tableValue{INIT_VALUE};

            for (auto it = begin; it != end; ++it) {
                if (REFLECT_DATA) {
                    tableValue =
                        (tableValue >> 8) ^ obtainTableValue(static_cast<unsigned char>(tableValue & 0xFF) ^ *it);
                } else {
                    tableValue = (tableValue << 8) ^
                                 obtainTableValue(static_cast<unsigned char>((tableValue >> (Width - 8)) & 0xFF) ^ *it);
                }
            }

            return tableValue ^ XOR_OUT;
        };

        template<class INPUT_IT>
        static T CalculateCRCViaTable(INPUT_IT begin, INPUT_IT end)
        {
            static_assert(std::is_same<typename std::iterator_traits<INPUT_IT>::value_type, unsigned char>::value,
                          "INPUT_IT must be iterator over unsigned char");

            T tableValue{INIT_VALUE};

            for (auto it = begin; it != end; ++it) {
                if (REFLECT_DATA) {
                    tableValue = (tableValue >> 8) ^ CRCTable[static_cast<unsigned char>(tableValue & 0xFF) ^ *it];
                } else {
                    tableValue = (tableValue << 8) ^
                                 CRCTable[(static_cast<unsigned char>(tableValue >> (Width - 8)) & 0xFF) ^ *it];
                }
            }

            return tableValue ^ XOR_OUT;
        };

      private:
        constexpr static unsigned short Width = sizeof(T) * 8;
        constexpr static size_t TableSize     = 256;

        CRCHelper() { static_assert(std::is_unsigned<T>::value, "T must be unsigned integral type."); };

        static constexpr std::array<T, TableSize> PopulateCRCTable()
        {
            std::array<T, TableSize> crcTable;
            for (size_t i = 0; i < crcTable.size(); ++i) {
                crcTable[i] = obtainTableValue(static_cast<unsigned char>(i & 0xFF));
            }

            return crcTable;
        };

        static constexpr T reflectValue(T data, unsigned short numBits)
        {
            T reflectedValue{0};
            unsigned short posBit{0};

            for (posBit = 0; posBit < numBits; posBit++) {
                if ((data & 1) == 1) {
                    reflectedValue |= static_cast<T>(1) << ((numBits - 1) - posBit);
                }

                data = (data >> 1);
            }
            return reflectedValue;
        };

        static constexpr T obtainTableValue(unsigned char tablePos)
        {
            T tableValue{0};
            unsigned char posBit = 0;

            if (REFLECT_DATA) {
                tableValue = static_cast<T>(reflectValue(tablePos, 8) & 0xFF) << (Width - 8);
            } else {
                tableValue = static_cast<T>(tablePos) << (Width - 8);
            }

            for (posBit = 0; posBit < 8; posBit++) {
                if (tableValue & (static_cast<T>(1) << (Width - 1))) {
                    tableValue = (tableValue << 1) ^ POLYNOMIAL;
                } else {
                    tableValue = (tableValue << 1);
                }
            }
            return REFLECT_DATA ? reflectValue(tableValue, Width) : tableValue;
        };

        constexpr static std::array<T, TableSize> CRCTable = PopulateCRCTable();
    };
} // namespace Shared
