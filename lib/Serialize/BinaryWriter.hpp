#include "SerializeDeserializeNum.hpp"

#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

namespace Shared {
    /**
     * Binary Writer class for conveniantly reading/writing data to a buffer.
     * The user can supply a buffer or the class can manage it for the user.
     * The goal of the class is to mimic the .NET Binary Writer API design.
     * The class is not thread safe. Do not try to read/write from multiple threads.
     * @tparam INPUT_IT: Iterator/pointer type for the underlying buffer
     * @tparam UseExceptions: If true code uses assert() instead of throwing exceptions for invalid operations
     */
    template <class INPUT_IT = std::vector<unsigned char>::iterator, bool UseExceptions = true>
    class BinaryWriter {
      public:
        /**
         * Construct class and have class manage buffer.
         * @param initialSize: Specific size to initialize buffer to.
         */
        BinaryWriter(size_t initialSize = 0)
            : m_UseExternalMem(false)
            , m_MemBuffer(initialSize, 0x00)
            , m_Begin(m_MemBuffer.begin())
            , m_End(m_MemBuffer.end())
            , m_CurrentLoc(m_Begin)
        {
        }
        /**
         * Construct class and have it write data to external buffer.
         * @param begin: Iterator/pointer to first element in buffer.
         * @param end: Iterator/pointer to end element in buffer.
         */
        BinaryWriter(INPUT_IT begin, INPUT_IT end)
            : m_UseExternalMem(true)
            , m_Begin(begin)
            , m_End(end)
            , m_CurrentLoc(begin)
        {
        }
        /**
         * Reset current location to beginning of buffer.
         */
        void Reset() { m_CurrentLoc = m_Begin; }

        /**
         * Clear all data in buffer.
         */
        void Clear()
        {
            if (m_UseExternalMem)
            {
                std::fill(m_Begin, m_End, 0x00);
            }
            else
            {
                m_MemBuffer.resize(0);
                m_Begin      = m_MemBuffer.begin();
                m_End        = m_MemBuffer.end();
                m_CurrentLoc = m_Begin;
            }
        }
        /**
         * Set current location in buffer
         * @param loc: Location to advanced iterator from begin.
         */
        void SetLoc(size_t loc)
        {
            size_t totalSize = std::distance(m_Begin, m_End);
            if (loc > totalSize)
            {
                if constexpr (UseExceptions)
                {
                    throw std::invalid_argument("Cannot set location to past end of storage.");
                }
                else
                {
                    assert(loc <= totalSize);
                }
            }
        }

        /**
         * @return Iterator/Pointer to beginning of the buffer.
         */
        INPUT_IT GetBegin() const { return m_Begin; }

        /**
         * @return Iterator/Pointer to location of next write/read operation.
         */
        INPUT_IT GetCurLoc() const { return m_CurrentLoc; }

        /**
         * @return Iterator/Pointer to end location of the buffer.
         */
        INPUT_IT GetEnd() const { return m_End; }

        /**
         * @return Current buffer size.
         */
        size_t GetSize() const { return std::distance(m_Begin, m_End); }

        /**
         * Write an array of numeric values to the buffer.
         * @tparam w_INPUT_IT: Iterator/Pointer type for the inputs.
         * @param begin: Iterator/Pointer to first item in array.
         * @param end: Iterator/Pointer to last item in array.
         */
        template <class w_INPUT_IT>
        void WriteArrayNum(w_INPUT_IT begin, w_INPUT_IT end)
        {
            size_t arraySize = std::distance(begin, end);
            size_t totalSize{sizeof(size_t) + arraySize * sizeof(*begin)};

            prepareMem(totalSize);

            SerializeArithmaticType(arraySize, m_CurrentLoc, m_CurrentLoc + sizeof(arraySize));
            m_CurrentLoc += sizeof(arraySize);

            for (auto it = begin; it != end; ++it)
            {
                SerializeArithmaticType(*begin, m_CurrentLoc, m_CurrentLoc + sizeof(*begin));
                m_CurrentLoc += sizeof(*begin);
            }
        }

        /**
         * Read an array of numeric values from the buffer.
         * @tparam T: Type of the numeric value to read into array.
         * @return Vector of T values read from buffer.
         */
        template <class T>
        std::vector<T> ReadArrayNum()
        {
            canRead(sizeof(size_t));
            size_t arraySize = ReadNum<size_t>();
            size_t totalSize{arraySize * sizeof(T)};

            canRead(totalSize);

            std::vector<T> returnVal(arraySize, 0);
            for (size_t i = 0; i < arraySize; ++i)
            {
                returnVal.at(i) = DeSerializeArithmaticType<T>(m_CurrentLoc, m_CurrentLoc + sizeof(T));
                m_CurrentLoc += sizeof(T);
            }

            return std::move(returnVal);
        }

        /**
         * Write numeric value to the buffer.
         * @tparam T: Type of value to write to the buffer.
         * @param value: Value of T to write to the buffer.
         */
        template <class T>
        void WriteNum(T value)
        {
            prepareMem(sizeof(T));
            SerializeArithmaticType(value, m_CurrentLoc, m_CurrentLoc + sizeof(T));
            m_CurrentLoc += sizeof(T);
        }

        /**
         * Read numeric value from buffer.
         * @tparam T: Type of numeric value to read from buffer.
         * @return Value of type T read from buffer.
         */
        template <class T>
        T ReadNum()
        {
            canRead(sizeof(T));
            T value = DeSerializeArithmaticType<T>(m_CurrentLoc, m_CurrentLoc + sizeof(T));
            m_CurrentLoc += sizeof(T);

            return value;
        }

      protected:
        /**
         * Validate that a value of a given size can be read from the buffer.
         * Throw or assert if not possible.
         * @param size: Size of value to read from buffer and validate.
         */
        virtual void canRead(size_t size)
        {
            size_t availableSize = std::distance(m_CurrentLoc, m_End);
            if (availableSize < size)
            {
                if constexpr (UseExceptions)
                {
                    throw std::runtime_error("Insufficient space to write value.");
                }
                else
                {
                    assert(availableSize >= size);
                }
            }
        }

        /**
         * Prepare memory buffer for writing. If the class owns
         * the buffer resize appropriately so that insert will succeed.
         * @param size: Size of data to be prepared for insertion.
         */
        virtual void prepareMem(size_t size)
        {
            size_t availableSize = std::distance(m_CurrentLoc, m_End);
            if (availableSize < size)
            {
                auto f_resizeVec = [&]() {
                    size_t loc = std::distance(m_Begin, m_CurrentLoc);

                    size_t resizeLen = (size - availableSize) + m_MemBuffer.size();
                    m_MemBuffer.resize(resizeLen);

                    m_Begin      = m_MemBuffer.begin();
                    m_End        = m_MemBuffer.end();
                    m_CurrentLoc = m_Begin + loc;
                };

                if constexpr (UseExceptions)
                {
                    if (m_UseExternalMem)
                    {
                        throw std::runtime_error("Insufficient space to write value.");
                    }
                    else
                    {
                        f_resizeVec();
                    }
                }
                else
                {
                    if (m_UseExternalMem)
                    {
                        assert(availableSize >= size);
                    }
                    else
                    {
                        f_resizeVec();
                    }
                }
            }
        }

      private:
        bool                       m_UseExternalMem;
        std::vector<unsigned char> m_MemBuffer;
        INPUT_IT                    m_Begin;
        INPUT_IT                    m_End;
        INPUT_IT                    m_CurrentLoc;
    };

} // namespace Shared