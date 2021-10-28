#pragma once

#include <cstddef>
#include <vector>

namespace Shared {
    template <class T>
    class TwoDVector {
      public:
        typedef T                value_type;
        typedef decltype(m_Data) container_type;
        typedef T&               reference;
        typedef const T&         const_reference;
        typedef size_t           size_type;

        TwoDVector()
            : m_Rows(0)
            , m_Columns(0)
        {
        }

        TwoDVector(size_t initialRows, size_t initialColumns, T& initialValue = T())
            : m_Data(size_t(initialRows * initialColumns), initialValue)
            , m_Rows(initialRows)
            , m_Columns(initialColumns)
        {
        }

        TwoDVector(const TwoDVector<T>& other)
            : m_Data(other.m_Data)
            , m_Rows(other.m_Rows)
            , m_Columns(other.m_Columns)
        {
        }

        TwoDVector(TwoDVector<T>&& other)
            : m_Data(std::move(other))
            , m_Rows(other.m_Rows)
            , m_Columns(other.m_Columns)
        {
            other.m_Rows    = 0;
            other.m_Columns = 0;
        }

        bool IsEmpty() const { return m_Data.empty(); }
        void ClearContents() { m_Data.clear(); }

        reference GetValue(size_t row, size_t column) const
        {
            if (row > m_Rows)
            {
                throw std::out_of_range("row exceeds allocated rows.");
            }
            else if (column > m_Columns)
            {
                throw std::out_of_range("column exceeds allocated columns.");
            }
            return m_Data.at(row * m_Rows + column);
        }

        void Resize(size_t rowSize, size_t columnSize)
        {
            m_Rows    = rowSize;
            m_Columns = columnSize;
            m_Data.resize(m_Rows * m_Columns);
        }

      private:
        size_t         m_Rows;
        size_t         m_Columns;
        std::vector<T> m_Data;
    };
} // namespace Shared
