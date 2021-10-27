#include <cstddef>
#include <iostream>

namespace Shared
{
    template<class INPUT_IT>
    void PrintHexArray(std::ostream& os, INPUT_IT begin, INPUT_IT end)
    {
        typedef std::underlying_type<std::byte>::type byte_under;
        static_assert(std::is_same<typename std::iterator_traits<INPUT_IT>::value_type, byte_under>::value,
                      "INPUT_IT must be iterator over std::byte");
        os << "[ ";
        for (auto it = begin; it != end; ++it) {
            if (*it <= 0xF) {
                os << "0x0" << std::hex << *it << std::dec;
            } else {
                os << "0x" << std::hex << *it << std::dec;
            }

            if (it != end - 1) {
                os << ", ";
            }
        }

        os << " ]";
    }
} // namespace Shared