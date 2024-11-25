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

/**
 * @file fallback_device_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines fallback_device_communicator interface
 * @date 2024-11-25
 * 
 */

#include "../device_communicator.hpp"
#include "../../communication/communicator.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class XMIPP4_CORE_API fallback_device_communicator final
    : public device_communicator
{
public:
    fallback_device_communicator(std::shared_ptr<communication::communicator> communicator);
    fallback_device_communicator(const fallback_device_communicator &other) = default;
    fallback_device_communicator(fallback_device_communicator &&other) = default;
    virtual ~fallback_device_communicator() = default;

    fallback_device_communicator& operator=(const fallback_device_communicator &other) = default;
    fallback_device_communicator& operator=(fallback_device_communicator &&other) = default;

    std::size_t get_size() const noexcept override;
    int get_rank() const noexcept override;

private:
    std::shared_ptr<communication::communicator> m_communicator;

}; 

} // namespace compute
} // namespace xmipp4
