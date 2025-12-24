// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/kernel.hpp>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for CPU-based kernels.
 */
class cpu_kernel
	: public kernel
{
public:
	cpu_kernel() noexcept = default;
	~cpu_kernel() override = default;
};

} // namespace multidimensional
} // namespace xmipp4
