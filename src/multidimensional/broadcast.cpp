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


namespace detail
{

template <typename T>
void compute_consensus_extents(span<const T> operands,
                               std::vector<std::size_t> &consensus_extents )
{
    std::vector<std::size_t> operand_extents;
    for (const auto &operand : operands)
    {
        operand.get_extents(operand_extents);
        broadcast_extents(consensus_extents, operand_extents);
    }
}

void broadcast_to(span<strided_layout> operands, 
                  span<const std::size_t> consensus_extents )
{
    for (std::size_t i = 0; i < operands.size(); ++i)
    {
        operands[i] = operands[i].broadcast_to(consensus_extents);
    }
}

template <typename T>
void broadcast(span<T> operands,
               std::vector<std::size_t> &consensus_extents )
{
    compute_consensus_extents(
        span<const T>(operands.data(), operands.size()), 
        consensus_extents
    );
    broadcast_to(
        operands, 
        make_span(consensus_extents)
    );
}

} // namespace detail

void broadcast(span<strided_layout> operands, 
               std::vector<std::size_t> &consensus_extents )
{
    detail::broadcast(operands, consensus_extents);
}

} // namespace multidimensional
} // namespace xmipp4
