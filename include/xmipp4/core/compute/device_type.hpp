#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

#include "../platform/constexpr.hpp"

#include <string_view>

namespace xmipp4
{
namespace compute
{

/**
 * @brief Device categories.
 * 
 */
enum class device_type
{
    unknown = -1, ///< The device does fall into any of the following categories
    cpu, ///< CPU of the host.
    gpu, ///< A discrete Graphics Processing Unit (GPU)
    integrated_gpu, ///< An integrated Graphics Processing Unit (iGPU)
    fpga, ///< A Field Programmable Grid Array (FPGA)
};

XMIPP4_CONSTEXPR const char* to_string(device_type type) noexcept;
bool from_string(std::string_view str, device_type& type) noexcept;

} // namespace compute
} // namespace xmipp4

#include "device_type.inl"
