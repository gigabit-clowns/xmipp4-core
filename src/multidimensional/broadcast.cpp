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
void broadcast_step(std::size_t &from, std::size_t to)
{
    if (from != to)
    {
        if (from == 1)
        {
            from = to;
        }
        else if(to != 1)
        {
            std::ostringstream oss;
            oss << "Cannot broadcast extent of " << from << " items into to an "
                << " extent of " << to << " items.";
            throw std::logic_error(oss.str());
        }
    }
}

void broadcast_step(std::vector<std::size_t> &consensus, 
                    span<const std::size_t> to)
{
    if (consensus.size() > to.size())
    {
        std::ostringstream oss;
        oss << "Cannot broadcast " << consensus.size() << "-dimensional extents"
            << " into" << to.size() << "-dimensions.";
        throw std::logic_error(oss.str());
    }

    const auto padding = to.size() - consensus.size();
    consensus.insert(consensus.cbegin(), padding, 1U);

    XMIPP4_ASSERT( consensus.size() == to.size() );
    const auto n = to.size();
    for (std::size_t i = 0; i < n; ++i)
    {
        broadcast_step(consensus[i], to[i]);
    }
}

} // namespace multidimensional
} // namespace xmipp4


