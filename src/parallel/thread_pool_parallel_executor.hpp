// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/parallel/parallel_executor.hpp>

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

namespace xmipp4 
{
namespace parallel
{

class thread_pool_parallel_executor
    : public parallel_executor
{
public:
    thread_pool_parallel_executor(std::size_t num_workers);
    thread_pool_parallel_executor(const thread_pool_parallel_executor&) = delete;
    thread_pool_parallel_executor(thread_pool_parallel_executor&&) = delete;
    ~thread_pool_parallel_executor() override;

    thread_pool_parallel_executor& 
    operator=(const thread_pool_parallel_executor&) = delete;
    thread_pool_parallel_executor& 
    operator=(thread_pool_parallel_executor&&) = delete;

    void execute(function_type function, 
                 std::size_t work_size, 
                 std::size_t grain_size ) override;

private:
    std::vector<std::thread> m_workers;
    std::mutex m_mutex;
    std::condition_variable m_arouse_condition_variable;
    std::condition_variable m_completed_condition_variable;
    std::atomic<std::size_t> m_progress;
    function_type m_current_function;
    std::size_t m_current_work_size;
    std::size_t m_current_grain_size;
    std::size_t m_active_threads;
    bool m_exit;

    void worker_function();
    void parallel_work();
    bool acquire_chunk(std::size_t &start, std::size_t &end);
    void signal_exit();

};

} // namespace parallel
} // namespace xmipp4
