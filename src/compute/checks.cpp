// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/checks.hpp>

#include <stdexcept>

namespace xmipp4 
{
namespace compute
{

std::size_t require_same_buffer_size(std::size_t size1, std::size_t size2)
{
    if (size1 != size2)
    {
        throw std::invalid_argument(
            "Both buffers must have the same size"
        );
    }
    return size1;
}

void require_valid_source_region(const copy_region &region, std::size_t count)
{
    if ((region.get_source_offset() + region.get_count()) > count)
    {
        throw std::out_of_range(
            "Source region must be within buffer bounds"
        );
    }
}

void require_valid_destination_region(const copy_region &region, std::size_t count)
{
    if ((region.get_destination_offset() + region.get_count()) > count)
    {
        throw std::out_of_range(
            "Destination region must be within buffer bounds"
        );
    }
}

void require_valid_region(const copy_region &region, 
                          std::size_t src_count, 
                          std::size_t dst_count )
{
    require_valid_source_region(region, src_count);
    require_valid_destination_region(region, dst_count);
}

} // namespace compute
} // namespace xmipp4
