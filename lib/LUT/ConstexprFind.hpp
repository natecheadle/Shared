#include <type_traits>

namespace Shared {

    template <typename Output, typename InputIT>
    constexpr Output Find(InputIT begin, InputIT end, Output value)
    {
        if constexpr (std::is_pointer_v<Output>)
        {
            for (auto it = begin; it != end; it++)
            {
                if (**it == *value)
                    return *it;
            }
        }
        else
        {
            for (auto it = begin; it != end; it++)
            {
                if (*it == value)
                    return *it;
            }
        }
    }
} // namespace Shared
