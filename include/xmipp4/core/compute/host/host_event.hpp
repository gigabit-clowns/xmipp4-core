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
 * @file host_event.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_event class.
 * @date 2024-11-13
 * 
 */

#include "../device_event.hpp"
#include "../device_to_host_event.hpp"

namespace xmipp4 
{
namespace compute
{

class host_event final
    : public device_event
    , public device_to_host_event
{
public:
    void record(device_queue &queue) override;

    void wait() const override;
    void wait(device_queue &queue) const override;

    bool is_signaled() const override;

}; 

} // namespace compute
} // namespace xmipp4
