#pragma once

#include <iostream>

namespace Shared
{
    class TwoDigitVersion
    {
      public:
        TwoDigitVersion(size_t majorVer, size_t minorVer)
            : m_MajorVer(majorVer)
            , m_MinorVer(minorVer)
        {
        }

        TwoDigitVersion()
            : m_MajorVer(0)
            , m_MinorVer(0)
        {
        }

        TwoDigitVersion(const TwoDigitVersion& other) = default;
        TwoDigitVersion& operator=(const TwoDigitVersion& other) = default;

        size_t GetMajorVersion() const { return m_MajorVer; }
        size_t GetMinorVersion() const { return m_MinorVer; }

        friend std::ostream& operator<<(std::ostream& os, const TwoDigitVersion& version)
        {
            os << version.m_MajorVer << "." << version.m_MinorVer;
        }

      private:
        size_t m_MajorVer;
        size_t m_MinorVer;
    };
} // namespace Shared
