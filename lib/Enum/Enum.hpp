#pragma once

#include "Serialize/SerializeDeserializeNum.hpp"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace Shared
{
    template<class T_ENUM, T_ENUM ENUM_BEGIN, T_ENUM ENUM_END, class T_INT, class Derived>
    class Enum
    {
        T_ENUM m_EnumValue;

      public:
        class Iterator
        {
            Derived m_FancyValue;
            T_INT m_IntVal;

          public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;

            Iterator()
                : m_FancyValue(Derived(ENUM_BEGIN))
                , m_IntVal(static_cast<T_INT>(ENUM_BEGIN))
            {
            }

            Iterator(T_ENUM enumValue)
                : m_FancyValue(Derived(enumValue))
                , m_IntVal(static_cast<T_INT>(enumValue))
            {
            }

            Iterator(Derived f)
                : m_FancyValue(f)
                , m_IntVal(f.ToIntegral())
            {
            }

            Iterator(const Iterator& other)
                : m_FancyValue(other.m_FancyValue)
                , m_IntVal(other.m_IntVal)
            {
            }

            Iterator(Iterator&& other) noexcept
                : m_FancyValue(other.m_FancyValue)
                , m_IntVal(other.m_IntVal)
            {
                other.m_FancyValue = Derived(ENUM_BEGIN);
                other.m_IntVal     = static_cast<T_INT>(ENUM_BEGIN);
            }

            Iterator& operator++()
            {
                auto oldValue = m_FancyValue;
                if (++m_FancyValue == oldValue)
                    m_IntVal = m_FancyValue.ToIntegral() + 1;
                else
                    m_IntVal = m_FancyValue.ToIntegral();

                return *this;
            }

            const Enum& operator*() { return m_FancyValue; }
            static Iterator Begin() { return Iterator(); }
            static Iterator End()
            {
                static const Iterator endIter = ++Iterator(ENUM_END); // cache end iterator
                return endIter;
            }

            bool operator!=(const Iterator& i) { return m_FancyValue != i.m_FancyValue || m_IntVal != i.m_IntVal; }
        };

        Enum()
            : m_EnumValue(ENUM_BEGIN)
        {
            static_assert(std::is_integral<T_INT>::value, "T_INT must be integral type.");
            static_assert(std::is_enum<T_ENUM>::value, "T_ENUM must be enum type.");
        }

        Enum(T_ENUM value)
            : m_EnumValue(value)
        {
        }

        Enum(T_INT intValue)
            : m_EnumValue(ENUM_BEGIN)
        {
            FromIntegral(intValue);
        }

        Enum(const Enum& value)
            : m_EnumValue(value.m_EnumValue)
        {
        }

        virtual ~Enum() = default;

        Enum& operator++()
        {
            m_EnumValue = Increment(m_EnumValue);

            return *this;
        }

        Enum& operator--()
        {
            m_EnumValue = Decrement(m_EnumValue);

            return *this;
        }

        Enum& operator=(const Enum& other)
        {
            m_EnumValue = other.m_EnumValue;
            return *this;
        }

        Enum operator+(T_INT integralValue)
        {
            T_ENUM newEnumValue = m_EnumValue;

            for (T_INT i = 0; i < integralValue; ++i) {
                newEnumValue = Increment(newEnumValue);
            }

            return newEnumValue;
        }

        Enum& operator+=(T_INT integralValue)
        {
            for (T_INT i = 0; i < integralValue; ++i) {
                m_EnumValue = Increment(m_EnumValue);
            }

            return *this;
        }

        Enum operator-(T_INT integralValue)
        {
            T_ENUM newEnumValue = m_EnumValue;

            for (T_INT i = integralValue; i > 0; --i) {
                newEnumValue = Decrement(newEnumValue);
            }

            return newEnumValue;
        }

        Enum& operator-=(T_INT integralValue)
        {
            for (T_INT i = integralValue; i > 0; --i) {
                m_EnumValue = Decrement(m_EnumValue);
            }

            return *this;
        }

        friend bool operator==(const Enum& lhs, const Enum& rhs) { return lhs.m_EnumValue == rhs.m_EnumValue; }

        friend bool operator!=(const Enum& lhs, const Enum& rhs) { return lhs.m_EnumValue != rhs.m_EnumValue; }

        bool operator<=(const Enum& rhs)
        {
            if (*this == rhs)
                return true;

            return *this < rhs;
        }

        bool operator<(const Enum& rhs) { return static_cast<Derived*>(this)->LessThan(m_EnumValue, rhs.m_EnumValue); }

        bool operator>(const Enum& rhs)
        {
            return static_cast<Derived*>(this)->GreatherThan(m_EnumValue, rhs.m_EnumValue);
        }

        bool operator>=(const Enum& rhs)
        {
            if (*this == rhs)
                return true;

            return *this > rhs;
        }

        friend std::ostream& operator<<(std::ostream& os, const Enum& value)
        {
            value.Print(os, value.m_EnumValue);
            return os;
        }

        static Iterator Begin() { return Iterator::Begin(); }

        static Iterator End() { return Iterator::End(); }

        static constexpr T_ENUM FIRST = ENUM_BEGIN;
        static constexpr T_ENUM LAST  = ENUM_END;

        T_ENUM ToEnum() const { return m_EnumValue; }
        T_INT ToIntegral() const { return static_cast<T_INT>(m_EnumValue); }
        std::string ToString() const
        {
            std::stringstream sStream;
            static_cast<Derived*>(this)->Print(sStream, m_EnumValue);
            return sStream.str();
        }

        void FromIntegral(T_INT rawValue)
        {
            if (!IsValid(rawValue))
                throw std::invalid_argument("Invalid Enumeration Value: " + std::to_string(rawValue));

            m_EnumValue = static_cast<T_ENUM>(rawValue);
        }

        virtual bool IsValid(T_INT rawValue) const
        {
            if (rawValue < static_cast<T_INT>(FIRST) || rawValue > static_cast<T_INT>(LAST)) {
                return false;
            }

            return true;
        }

        template<class INPUT_IT>
        void Serialize(INPUT_IT begin, INPUT_IT end, Endianess endianess = Endianess::LittleEndian) const
        {
            static_assert(std::is_integral<T_INT>::value, "T_INT must be integral type.");

            SerializeIntegralType<T_INT>(static_cast<T_INT>(m_EnumValue), begin, end, endianess);
        }

        template<class INPUT_IT>
        void Deserialize(const INPUT_IT begin, const INPUT_IT end, Endianess endianess = Endianess::LittleEndian)
        {
            T_INT integralValue = DeSerializeIntegralType<T_INT>(begin, end, endianess);

            FromIntegral(integralValue);
        }

      protected:
        virtual bool GreatherThan(T_ENUM lhs, T_ENUM rhs) const
        {
            return static_cast<T_INT>(lhs) > static_cast<T_INT>(rhs);
        }

        virtual bool LessThan(T_ENUM lhs, T_ENUM rhs) const
        {
            return static_cast<T_INT>(lhs) < static_cast<T_INT>(rhs);
        }

        virtual T_ENUM Increment(T_ENUM value) const
        {
            if (value == LAST)
                return LAST;

            return static_cast<T_ENUM>(static_cast<int>(value) + 1);
        }

        virtual T_ENUM Decrement(T_ENUM value) const
        {
            if (value == FIRST)
                return FIRST;

            return static_cast<T_ENUM>(static_cast<int>(value) - 1);
        }

        virtual void Print(std::ostream& os, T_ENUM value) const { os << static_cast<int>(value); }
    };
} // namespace Shared
