// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <stdexcept>
#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

static
void broadcast_to_extent(std::size_t &from, std::size_t to)
{
    if (from != to)
    {
        if (from == 1)
        {
            from = to;
        }
        else
        {
            std::ostringstream oss;
            oss << "Cannot broadcast extent of " << from << " items into to an "
                << " extent of " << to << " items.";
            throw std::logic_error(oss.str());
        }
    }
}

void broadcast_to(std::vector<std::size_t> &from, span<const std::size_t> to)
{
    if (from.size() > to.size())
    {
        std::ostringstream oss;
        oss << "Cannot broadcast " << from.size() << "-dimensional extents into"
            << to.size() << "-dimensions.";
        throw std::logic_error(oss.str());
    }

    const auto padding = to.size() - from.size();
    from.insert(from.cbegin(), padding, 1U);

    XMIPP4_ASSERT( from.size() == to.size() );
    const auto n = to.size();
    for (std::size_t i = 0; i < n; ++i)
    {
        broadcast_to_extent(from[i], to[i]);
    }
}

} // namespace multidimensional
} // namespace xmipp4


