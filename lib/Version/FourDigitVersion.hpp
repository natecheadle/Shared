#pragma once

#include <iostream>

namespace Shared
{
    class FourDigitVersion
    {
      public:
        FourDigitVersion(size_t majorVer, size_t minorVer, size_t subMinorVer, size_t subSubMinorVer)
            : m_MajorVer(majorVer)
            , m_MinorVer(minorVer)
            , m_SubMinorVer(subMinorVer)
            , m_SubSubMinorVer(subSubMinorVer)
        {
        }

        FourDigitVersion()
            : m_MajorVer(0)
            , m_MinorVer(0)
            , m_SubMinorVer(0)
            , m_SubSubMinorVer(0)
        {
        }

        FourDigitVersion(const FourDigitVersion& other) = default;
        FourDigitVersion& operator=(const FourDigitVersion& other) = default;

        size_t GetMajorVersion() const { return m_MajorVer; }
        size_t GetMinorVersion() const { return m_MinorVer; }
        size_t GetSubMinorVersion() const { return m_SubMinorVer; }
        size_t GetSubSubMinorVersion() const { return m_SubSubMinorVer; }

        friend std::ostream& operator<<(std::ostream& os, const FourDigitVersion& version)
        {
            os << version.m_MajorVer << "." << version.m_MinorVer << "." << version.m_SubMinorVer << "."
               << version.m_SubSubMinorVer;
            return os;
        }

      private:
        size_t m_MajorVer;
        size_t m_MinorVer;
        size_t m_SubMinorVer;
        size_t m_SubSubMinorVer;
    };
} // namespace Shared
