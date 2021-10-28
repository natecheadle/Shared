#pragma once

#include <array>
#include <cstddef>

namespace Shared {
    template <class T, size_t SIZE>
    class StaticQueue {
        std::array<T, SIZE + 1>     m_Storage;
        decltype(m_Storage.begin()) m_pHead;
        decltype(m_Storage.begin()) m_pTail;

      public:
        class Iterator {
            const decltype(m_Storage.begin()) m_StorageBegin;
            const decltype(m_Storage.begin()) m_StorageEnd;
            decltype(m_Storage.begin())       m_CurLoc;

          public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type   = std::ptrdiff_t;

            Iterator(
                decltype(m_Storage.begin()) loc,
                decltype(m_Storage.begin()) begin,
                decltype(m_Storage.begin()) end)
                : m_CurLoc(loc)
                , m_StorageBegin(begin)
                , m_StorageEnd(end)
            {
            }

            Iterator(const Iterator& other)
                : m_CurLoc(other.m_CurLoc)
                , m_StorageBegin(other.m_StorBegin)
                , m_StorageEnd(other.m_StorEnd)
            {
            }

            Iterator& operator++()
            {
                if (++m_CurLoc == m_StorageEnd)
                {
                    m_CurLoc = m_StorageBegin;
                }

                return *this;
            }

            Iterator& operator--()
            {
                if (m_CurLoc == m_StorageBegin)
                {
                    m_CurLoc = m_StorageEnd - 1;
                }
                else
                {
                    --m_CurLoc;
                }

                return *this;
            }

            const T& operator*() { return *m_CurLoc; }

            friend bool operator==(const Iterator& lhs, const Iterator& rhs) { return lhs.m_CurLoc == rhs.m_CurLoc; }

            friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return lhs.m_CurLoc != rhs.m_CurLoc; }
        };

        typedef T                   value_type;
        typedef decltype(m_Storage) container_type;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef size_t              size_type;

        StaticQueue()
            : m_pTail(m_Storage.begin())
            , m_pHead(m_Storage.begin())
        {
        }

        [[nodiscard]] bool Full() const
        {
            return m_pHead == m_Storage.begin() ? m_pTail == m_Storage.end() - 1 : m_pTail == m_pHead - 1;
        }

        [[nodiscard]] bool Empty() const { return (m_pHead == m_pTail); }

        [[nodiscard]] size_type MaxSize() const { return SIZE; }

        [[nodiscard]] size_type Size() const
        {
            return m_pTail >= m_pHead ? m_pTail - m_pHead
                                      : (m_Storage.cend() - m_pHead) + (m_pTail - m_Storage.cbegin());
        }

        bool Push(const_reference value)
        {
            if (!Full())
            {
                *m_pTail = value;
                if (++m_pTail == m_Storage.end())
                {
                    m_pTail = m_Storage.begin();
                }

                return true;
            }

            return false;
        }

        bool Pop()
        {
            if (Empty())
                return false;

            if (++m_pHead == m_Storage.end())
            {
                m_pHead = m_Storage.begin();
            }
            return true;
        }

        value_type Front() { return *m_pHead; }
        value_type Back() { return *(m_pTail != m_Storage.begin() ? m_pTail - 1 : m_Storage.end() - 1); }

        Iterator Begin() { return Iterator(m_pHead, m_Storage.begin(), m_Storage.end()); }
        Iterator End() { return Iterator(m_pTail, m_Storage.begin(), m_Storage.end()); }
    };
} // namespace Shared
