// SPDX-License-Identifier: GPL-3.0-only

#include "thread_pool_parallel_executor.hpp"

#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{
namespace parallel
{

thread_pool_parallel_executor::thread_pool_parallel_executor(std::size_t num_workers)
    : m_exit(false)
{
    m_workers.reserve(num_workers);
    for (std::size_t i = 0; i < num_workers; ++i)
    {
        m_workers.emplace_back(
            std::mem_fn(&thread_pool_parallel_executor::worker_function),
            std::ref(*this)
        );
    }
}

thread_pool_parallel_executor::~thread_pool_parallel_executor()
{
    signal_exit();
    for (auto &worker : m_workers)
    {
        worker.join();
    }
}

void thread_pool_parallel_executor::execute(function_type function, 
                                            std::size_t work_size,
                                            std::size_t grain_size )
{
    if (function && work_size)
    {
        if (grain_size == 0)
        {
            throw std::invalid_argument("grain_size must be greater than 0");
        }

        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_current_function)
        {
            throw std::logic_error("Cannot concurrently call execute()");
        }

        // Set up work
        m_current_function = std::move(function);
        m_current_work_size = work_size;
        m_current_grain_size = grain_size;
        m_progress = 0;
        m_active_threads = m_workers.size();
        m_arouse_condition_variable.notify_all();
        
        // Wait for completion
        while (m_active_threads)
        {
            m_completed_condition_variable.wait(lock);
        }
        
        // Mark as completed
        m_current_function = function_type();
    }
}

void thread_pool_parallel_executor::worker_function()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    while (!m_exit)
    {
        m_arouse_condition_variable.wait(lock);

        if (m_current_function)
        {
            lock.unlock();
            parallel_work();
            lock.lock();

            --m_active_threads;
            m_completed_condition_variable.notify_all();
        }
    }
}

void thread_pool_parallel_executor::parallel_work()
{
    std::size_t start;
    std::size_t end;
    while (acquire_chunk(start, end))
    {
        m_current_function(start, end);
    }
}

bool thread_pool_parallel_executor::acquire_chunk(std::size_t &start, 
                                                  std::size_t &end )
{
    start = m_progress.load();
    do
    {
        end = std::min(start + m_current_grain_size, m_current_work_size);
    } 
    while (!m_progress.compare_exchange_weak(start, end));

    return start != end;
}

void thread_pool_parallel_executor::signal_exit()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_exit = true;
    m_arouse_condition_variable.notify_all();
}

} // namespace parallel
} // namespace xmipp4
