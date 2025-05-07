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
 * @file batch_reader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::batch_reader class
 * @date 2025-05-07
 * 
 */

#include "location.hpp"
#include "../span.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace image
{

class reader_manager;

/**
 * @brief Class to efficiently read batches of images.
 * 
 * This class is used to read batches of images from potentially multiple image
 * stacks. Reads are bundled to maximize throughput. In addition, it caches
 * open readers to avoid opening and closing files repeatedly.
 * 
 */
class batch_reader
{
public:
    explicit batch_reader(const reader_manager &readers, 
                          std::size_t max_open = 64 );
    batch_reader(const batch_reader &) = delete;
    batch_reader(batch_reader &&);
    ~batch_reader();

    batch_reader &operator=(const batch_reader &) = delete;
    batch_reader &operator=(batch_reader &&);
    
    /**
     * @brief Read a batch of images.
     * 
     * @param locations Locations of the images to read.
     */
    void read_batch(span<const location> locations); // TODO return

private:

};

} // namespace image
} // namespace xmipp4
