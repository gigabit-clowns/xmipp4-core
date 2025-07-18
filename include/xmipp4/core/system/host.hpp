// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>

namespace xmipp4 
{
namespace system
{

/**
 * @brief Get the hostname of the machine.
 * 
 * @return std::string The hostname.
 */
std::string get_hostname();

/**
 * @brief Get the total memory available in the system.
 * 
 * @return std::size_t Total memory in bytes.
 */
std::size_t get_total_system_memory();

/**
 * @brief Get the memory available in the system.
 * 
 * @return std::size_t Total memory in bytes.
 */
std::size_t get_available_system_memory();

} // namespace system
} // namespace xmipp4
