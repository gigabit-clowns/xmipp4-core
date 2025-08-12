// SPDX-License-Identifier: GPL-3.0-only

#include "dummy_parallel_executor.hpp"

#include <stdexcept>

namespace xmipp4 
{
namespace parallel
{

void dummy_parallel_executor::execute(function_type function, 
                                      std::size_t work_size,
                                      std::size_t grain_size )
{
    if (function && work_size)
    {
        if (grain_size == 0)
        {
            throw std::invalid_argument("grain_size must be greater than 0");
        }

        std::size_t start = 0;
        while (start < work_size)
        {
            const auto end = std::min(start + grain_size, work_size);
            function(start, end);
            start = end;
        }
    }
}

} // namespace parallel
} // namespace xmipp4
