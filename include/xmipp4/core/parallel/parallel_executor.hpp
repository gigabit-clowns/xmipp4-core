// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <functional>

namespace xmipp4 
{
namespace parallel
{

class parallel_executor
{
public:
    using function_type = std::function<void(std::size_t, std::size_t)>;

    virtual ~parallel_executor() = default;

    virtual 
    void execute(function_type function, 
                 std::size_t work_size, 
                 std::size_t grain_size ) = 0;

};

} // namespace parallel
} // namespace xmipp4
