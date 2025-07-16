// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file cpu_device_queue.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines cpu_device_queue interface
 * @date 2024-11-27
 * 
 */

#include "../device_queue_pool.hpp"

#include "cpu_device_queue.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Implementation of the device_queue_pool interface to be 
 * able to obtain cpu_device_queue-s.
 * 
 * As cpu_device_queue-s are synchronous, it does not make sense to
 * instantiate multiple of them. Thus, this pool always has a size of
 * one.
 * 
 */
class cpu_device_queue_pool final
    : public device_queue_pool
{
public:
    std::size_t get_size() const noexcept override;
    cpu_device_queue& get_queue(std::size_t index) override;

private:
    cpu_device_queue m_queue;

}; 

} // namespace compute
} // namespace xmipp4
