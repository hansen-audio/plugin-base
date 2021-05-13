// Copyright(c) 2021 Hansen Audio.

#include <cstddef>
#include <numeric>

namespace ha {
namespace plugin_base {
namespace common {

//-----------------------------------------------------------------------------
template <typename T, typename Func, std::size_t N = 32>
void slice(T total, const Func& func)
{
    static_assert(std::is_signed<T>::value, "Must be SIGNED type!");
    static_assert(std::is_integral<T>::value, "Must be INTEGRAL type!");
    T begin = 0;
    while (total > 0)
    {
        func(begin, total < N ? total : N);
        total -= N;
        begin += N;
    }
}

//-----------------------------------------------------------------------------
} // namespace common
} // namespace plugin_base
} // namespace ha