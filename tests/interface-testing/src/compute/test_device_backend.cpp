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
 * @file test_device_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of tests for device_backend
 * @date 2024-11-12
 * 
 */

#include <compute/test_device_backend.hpp>

#include <xmipp4/core/compute/device_backend.hpp>

#include <catch2/catch_test_macros.hpp>

namespace xmipp4
{
namespace compute
{
namespace testing
{

void test_device_backend(device_backend &backend)
{
    REQUIRE_NOTHROW( backend.is_available() );
}

} // namespace testing
} // namespace compute
} // namespace xmipp4

