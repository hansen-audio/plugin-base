// Copyright(c) 2021 Hansen Audio.

#include "ha/plugin_base/core/types.h"
#include <cstddef>
#include <numeric>

namespace ha::plugin_base::common {

//-----------------------------------------------------------------------------
template <typename T, typename Func, std::size_t N = 32>
void slice(T total, const Func& func)
{
    static_assert(std::is_signed<T>::value, "Must be SIGNED type!");
    static_assert(std::is_integral<T>::value, "Must be INTEGRAL type!");

    T begin = 0;
    while (total > 0)
    {
        i32 num_samples = total < N ? total : N;
        func(begin, num_samples);

        total -= num_samples;
        begin += num_samples;
    }
}

//-----------------------------------------------------------------------------
} // namespace ha::plugin_base::common