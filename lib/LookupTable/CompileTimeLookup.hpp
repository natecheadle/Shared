#include <array>
#include <type_traits>
namespace Shared {
    template <typename KeyT, typename ValueT, size_t Size, const std::array<std::pair<KeyT, ValueT>, Size>& Values>
    class CompileTimeLookup {
      public:
        static constexpr ValueT GetValue(KeyT key) { return Find(key); }

      private:
        static constexpr std::array<std::pair<KeyT, ValueT>, Size> m_Values = Values;

        static constexpr ValueT Find(KeyT key)
        {
            for (auto& pair : m_Values)
            {
                if (pair.first == key)
                {
                    return pair.second;
                }
            }

            return m_Values[0].second;
        }
    };
} // namespace Shared