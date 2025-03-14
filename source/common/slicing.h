// Copyright(c) 2021 Hansen Audio.

#include "hao/plugin_base/core/types.h"
#include <cstddef>
#include <numeric>

namespace hao::plugin_base::common {

//-----------------------------------------------------------------------------
template <typename T, typename Func, std::size_t N = 32>
void slice(T const _total, const Func& func)
{
    static_assert(std::is_integral<T>::value, "Must be INTEGRAL type!");

    std::size_t begin = 0;
    std::size_t total = std::size_t(_total);
    while (total > 0)
    {
        std::size_t num_samples = total < N ? total : N;
        func(begin, num_samples);

        total -= num_samples;
        begin += num_samples;
    }
}

//-----------------------------------------------------------------------------
} // namespace hao::plugin_base::common