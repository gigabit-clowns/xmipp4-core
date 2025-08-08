// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <xmipp4/core/multidimensional/broadcast_error.hpp>
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
    bool result = true;

    if (extent1 != extent2)
    {
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
            result = false;
        }
    }

    return result;
}

void broadcast_extents(std::vector<std::size_t> &extents1, 
                       std::vector<std::size_t> &extents2 )
{
    if (extents1.size() < extents2.size())
    {
        const auto padding = extents2.size() - extents1.size();
        extents1.insert(extents1.cbegin(), padding, 1U);
    }
    
    if (extents2.size() < extents1.size())
    {
        const auto padding = extents1.size() - extents2.size();
        extents2.insert(extents2.cbegin(), padding, 1U);
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
