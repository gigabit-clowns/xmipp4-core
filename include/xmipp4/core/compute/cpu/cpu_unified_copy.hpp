// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../unified_copy.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Declaration of the unified_copy interface for the CPU to allow 
 * migrating the data with the CPU acting as a compute device. 
 * 
 */
class cpu_unified_copy final
    : public unified_copy
{
    // Intentionally empty. No overrides    

}; 

} // namespace compute
} // namespace xmipp4
