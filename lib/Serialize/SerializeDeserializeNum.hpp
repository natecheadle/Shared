#pragma once

#include <cstdint>
#include <iterator>
#include <stddef.h>
#include <stdexcept>
#include <type_traits>

namespace Shared
{
    enum class Endianess
    {
        BigEndian,
        LittleEndian
    };

    template<class T, class INPUT_IT, Endianess endianess = Endianess::LittleEndian, bool UseExceptions = true>
    static void SerializeArithmaticType(T value, INPUT_IT begin, INPUT_IT end)
    {
        static_assert(std::is_same<typename std::iterator_traits<INPUT_IT>::value_type, std::byte>::value ||
                          std::is_same<typename std::iterator_traits<INPUT_IT>::value_type, unsigned char>::value,
                      "INPUT_IT must be iterator over std::byte/unsigned char");

        typename std::iterator_traits<INPUT_IT>::difference_type numElements = std::distance(begin, end);

        static_assert(std::is_arithmetic<T>::value, "T must be arithmatic type.");

        if constexpr (UseExceptions) {
            if (numElements != sizeof(value)) {
                throw std::invalid_argument("Sizeof(T) must equal distance between begin and end.");
            }
        } else {
            assert(numElements == sizeof(value));
        }

        if constexpr (std::is_integral<T>::value) {
            if constexpr (endianess == Endianess::LittleEndian) {
                for (int i = 0; i < numElements; ++i) {
                    *(begin + i) =
                        static_cast<typename std::iterator_traits<INPUT_IT>::value_type>((value >> (8 * i)) & 0xFF);
                }
            } else {
                for (int i = 0; i < numElements; ++i) {
                    *(begin + ((numElements - 1) - i)) =
                        static_cast<typename std::iterator_traits<INPUT_IT>::value_type>((value >> (8 * i)) & 0xFF);
                }
            }
        } else {
            typedef typename std::conditional<sizeof(T) == sizeof(float), uint32_t, uint64_t>::type storageType;
            storageType* tmpValue{reinterpret_cast<storageType*>(&value)};

            if constexpr (endianess == Endianess::LittleEndian) {
                for (int i = 0; i < numElements; ++i) {
                    *(begin + i) =
                        static_cast<typename std::iterator_traits<INPUT_IT>::value_type>((*tmpValue >> (8 * i)) & 0xFF);
                }
            } else {
                for (int i = 0; i < numElements; ++i) {
                    *(begin + ((numElements - 1) - i)) =
                        static_cast<typename std::iterator_traits<INPUT_IT>::value_type>((*tmpValue >> (8 * i)) & 0xFF);
                }
            }
        }
    };

    template<class T, class INPUT_IT, Endianess endianess = Endianess::LittleEndian, bool UseExceptions = true>
    static T DeSerializeArithmaticType(INPUT_IT begin, INPUT_IT end)
    {
        static_assert(std::is_same<typename std::iterator_traits<INPUT_IT>::value_type, std::byte>::value ||
                          std::is_same<typename std::iterator_traits<INPUT_IT>::value_type, unsigned char>::value,
                      "INPUT_IT must be iterator over std::byte/unsigned char");

        typename std::iterator_traits<INPUT_IT>::difference_type numElements = std::distance(begin, end);

        static_assert(std::is_arithmetic<T>::value, "T must be arithmatic type.");

        if constexpr (UseExceptions) {
            if (numElements != sizeof(T)) {
                throw std::invalid_argument("Sizeof(T) must equal distance between begin and end.");
            }
        } else {
            assert(numElements == sizeof(T));
        }

        T value{0};

        if constexpr (std::is_integral<T>::value) {
            if constexpr (endianess == Endianess::LittleEndian) {
                for (int i = 0; i < numElements; ++i) {
                    value |= static_cast<T>(*(begin + i)) << (8 * i);
                }
            } else {
                for (int i = 0; i < numElements; ++i) {
                    value |= static_cast<T>(*(begin + ((numElements - 1) - i))) << (8 * i);
                }
            }
        } else {
            typedef typename std::conditional<sizeof(T) == sizeof(float), uint32_t, uint64_t>::type storageType;
            storageType* tmpValue{reinterpret_cast<storageType*>(&value)};
            if constexpr (endianess == Endianess::LittleEndian) {
                for (int i = 0; i < numElements; ++i) {
                    *tmpValue |= static_cast<storageType>(*(begin + i)) << (8 * i);
                }
            } else {
                for (int i = 0; i < numElements; ++i) {
                    *tmpValue |= static_cast<storageType>(*(begin + ((numElements - 1) - i))) << (8 * i);
                }
            }
        }

        return value;
    };

    template<class T, class INPUT_IT, Endianess endianess = Endianess::LittleEndian>
    static void SerializeIntegralType(T integralValue, INPUT_IT begin, INPUT_IT end)
    {
        static_assert(std::is_integral<T>::value, "T must be integral type.");
        return SerializeArithmaticType<T, INPUT_IT, endianess>(integralValue, begin, end);
    };

    template<class T, class INPUT_IT, Endianess endianess = Endianess::LittleEndian>
    static T DeSerializeIntegralType(const INPUT_IT begin, const INPUT_IT end)
    {
        static_assert(std::is_integral<T>::value, "T must be integral type.");

        return DeSerializeArithmaticType<T, INPUT_IT, endianess>(begin, end);
    };
} // namespace Shared
