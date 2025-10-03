// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <xmipp4/core/multidimensional/broadcast_error.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include <stdexcept>
#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

static
bool broadcast_extent(std::size_t &extent1, std::size_t &extent2)
{
    if (extent1 == extent2)
    {
        return true;
    }

    if (extent1 == 1)
    {
        extent1 = extent2;
    }
    else if (extent2 == 1)
    {
        extent2 = extent1;
    }
    else
    {
        return false;
    }

    return true;
}

static
void pad_extents(
    std::vector<std::size_t> &extents, 
    std::size_t target_size
)
{
    XMIPP4_ASSERT( extents.size() < target_size );

    const auto padding = target_size - extents.size();
    extents.insert(extents.cbegin(), padding, 1U);
}

void broadcast_extents(
    std::vector<std::size_t> &extents1, 
    std::vector<std::size_t> &extents2 
)
{
    if (extents1.size() < extents2.size())
    {
        pad_extents(extents1, extents2.size());
    }
    
    if (extents2.size() < extents1.size())
    {
        pad_extents(extents2, extents1.size());
    }

    XMIPP4_ASSERT( extents1.size() == extents2.size() );
    const auto n = extents1.size();
    for (std::size_t i = 0; i < n; ++i)
    {
        if (!broadcast_extent(extents1[i], extents2[i]))
        {
            throw broadcast_error(extents1, extents2);
        }
    }
}

} // namespace multidimensional
} // namespace xmipp4
