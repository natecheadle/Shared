#pragma once

#include "SerializeDeserializeHelpers.hpp"

#include <bitset>
#include <memory>
#include <stdexcept>
#include <string>

namespace Shared
{

    template<class BIT_ID_TYPE, class BASE_TYPE, BASE_TYPE VALIDITY_MASK>
    class BitSetTemplate
    {
      public:
        BitSetTemplate(BASE_TYPE rawValue)
            : m_BitSetValues(rawValue)
        {
            if (!IsValueValid(rawValue)) {
                throw std::invalid_argument("Invalid Raw Value based on VALIDITY_MASK: " +
                                            std::to_string(VALIDITY_MASK));
            }

            static_assert(std::is_integral<BASE_TYPE>::value, "BASE_TYPE must be integral type.");
            static_assert(std::is_enum<BIT_ID_TYPE>::value, "BIT_ID_TYPE must be enum type.");
        }

        bool GetViaID(BIT_ID_TYPE id) const { return m_BitSetValues[static_cast<size_t>(id)]; }
        void SetViaID(BIT_ID_TYPE id, bool value) { m_BitSetValues[static_cast<size_t>(id)] = value; }

        BASE_TYPE GetRawValue() const { return static_cast<BASE_TYPE>(m_BitSetValues.to_ullong()); }
        void SetAllHigh() { m_BitSetValues = std::bitset<BASE_SIZE>(VALIDITY_MASK); }
        void SetAllLow() { m_BitSetValues.reset(); }
        unsigned short GetSize() const { return sizeof(BASE_TYPE); }
        static BASE_TYPE GetValididityMask() { return VALIDITY_MASK; }
        static bool IsValueValid(BASE_TYPE value) { return (value | VALIDITY_MASK) == VALIDITY_MASK; }

        void Serialize(unsigned char* pRawData) const
        {
            SerializeIntegralType<BASE_TYPE>(GetRawValue(), pRawData, pRawData + GetSize());
        }

        void Deserialize(unsigned char* data)
        {
            BASE_TYPE rawValue = DeSerializeIntegralType<BASE_TYPE>(data, data + GetSize());

            if (!IsValueValid(rawValue))
                throw std::invalid_argument("RawValue " + std::to_string(rawValue) +
                                            " is invalid based on validity mask " + std::to_string(VALIDITY_MASK));

            m_BitSetValues = std::bitset<BASE_SIZE>(rawValue);
        }

      private:
        static constexpr size_t BASE_SIZE = sizeof(BASE_TYPE) * 8;
        std::bitset<BASE_SIZE> m_BitSetValues;
    };
} // namespace Shared