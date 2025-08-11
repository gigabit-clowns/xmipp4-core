// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/parallel_executor.hpp>

namespace xmipp4 
{

class dummy_parallel_executor
    : public parallel_executor
{
public:
    dummy_parallel_executor() = default;
    dummy_parallel_executor(const dummy_parallel_executor&) = default;
    dummy_parallel_executor(dummy_parallel_executor&&) = default;
    ~dummy_parallel_executor() override = default;

    dummy_parallel_executor& 
    operator=(const dummy_parallel_executor&) = default;
    dummy_parallel_executor& 
    operator=(dummy_parallel_executor&&) = default;

    void execute(function_type function, 
                 std::size_t work_size, 
                 std::size_t grain_size ) override;

};

} // namespace xmipp4
