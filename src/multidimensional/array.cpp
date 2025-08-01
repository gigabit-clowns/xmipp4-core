// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/storage.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename ForwardIt>
static
void check_axis_permutation(ForwardIt first, ForwardIt last, std::size_t count)
{
    // Based on:
    // https://en.cppreference.com/w/cpp/algorithm/is_permutation
    // The former function could be re-used, provided that something
    // like boost::counting_iterator is available.

    if (std::distance(first, last) != static_cast<std::ptrdiff_t>(count))
    {
        throw std::invalid_argument(
            "permutation's length does not match the required count"
        );
    }

    // Skip common prefix
    std::size_t i = 0;
    while (first != last && *first == i)
    {
        ++first;
        ++i;
    }

    // For the rest, check that it is a permutation
    while (i < count)
    {
        // Ensure the current value appears in the range.
        auto ite = std::find(first, last, i);
        if (ite == last)
        {
            std::ostringstream oss;
            oss << "value " << i << " is missing in the permutation";
            throw std::invalid_argument(oss.str());
        }

        ++i;
    }
}



class array::implementation
{
public:
    implementation() = default;
    implementation(strided_layout layout, 
                   std::shared_ptr<storage> storage ) noexcept
        : m_layout(std::move(layout))
        , m_storage(std::move(storage))
    {
    }

    numerical_type get_data_type() const noexcept
    {
        return numerical_type::unknown; // TODO
    }

    const strided_layout& get_layout() const noexcept
    {
        return m_layout;
    }

    storage* get_storage() noexcept
    {
        return m_storage.get();
    }

    std::shared_ptr<storage> share_storage() noexcept
    {
        return m_storage;
    }

    implementation apply_subscripts(span<const dynamic_subscript> subscripts)
    {
        return implementation(
            get_layout().apply_subscripts(subscripts),
            share_storage()
        );
    }

    implementation transpose()
    {
        return implementation(
            get_layout().transpose(),
            share_storage()
        );
    }

    implementation permute(span<const std::size_t> order)
    {
        return implementation(
            get_layout().permute(order),
            share_storage()
        );
    }

    implementation swap_axes(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
    {
        return implementation(
            get_layout().swap_axes(axis1, axis2),
            share_storage()
        );
    }

    implementation squeeze()
    {
        return implementation(
            get_layout().squeeze(),
            share_storage()
        );
    }

    implementation broadcast_to(span<const std::size_t> extents)
    {
        return implementation(
            get_layout().broadcast_to(extents),
            share_storage()
        );
    }

private:
    strided_layout m_layout;
    std::shared_ptr<storage> m_storage;

};





array::array() = default;
array::array(array&& other) noexcept = default;
array::~array() = default;
array& array::operator=(array&& other) noexcept = default;



numerical_type array::get_data_type() const noexcept
{
    return m_implementation ? 
           m_implementation->get_data_type() : 
           numerical_type::unknown ;
}

strided_layout array::get_layout() const noexcept
{
    return m_implementation ? 
           m_implementation->get_layout() : 
           strided_layout() ;
}

storage* array::get_storage() noexcept
{
    return m_implementation ? 
           m_implementation->get_storage() : 
           nullptr ;
}

XMIPP4_NODISCARD
std::shared_ptr<storage> array::share_storage() noexcept
{
    return m_implementation ? 
           m_implementation->share_storage() : 
           nullptr ;
}

XMIPP4_NODISCARD
std::shared_ptr<const storage> array::share_storage() const noexcept
{
    return m_implementation ? 
           m_implementation->share_storage() : 
           nullptr ;
}

XMIPP4_NODISCARD
array array::view() noexcept
{
    return array(m_implementation);
}

XMIPP4_NODISCARD
array array::apply_subscripts(span<const dynamic_subscript> subscripts)
{
    if (m_implementation)
    {
        return array(m_implementation->apply_subscripts(subscripts));
    }
    else
    {
        return array(implementation().apply_subscripts(subscripts));
    }
}

XMIPP4_NODISCARD
array array::transpose()
{
    if (m_implementation)
    {
        return array(m_implementation->transpose());
    }
    else
    {
        return array(implementation().transpose());
    }
}

XMIPP4_NODISCARD
array array::permute(span<const std::size_t> order)
{
    if (m_implementation)
    {
        return array(m_implementation->permute(order));
    }
    else
    {
        return array(implementation().permute(order));
    }
}

XMIPP4_NODISCARD
array array::swap_axes(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
{
    if (m_implementation)
    {
        return array(m_implementation->swap_axes(axis1, axis2));
    }
    else
    {
        return array(implementation().swap_axes(axis1, axis2));
    }
}

XMIPP4_NODISCARD
array array::squeeze()
{
    if (m_implementation)
    {
        return array(m_implementation->squeeze());
    }
    else
    {
        return array(implementation().squeeze());
    }
}

XMIPP4_NODISCARD 
array array::broadcast_to(span<const std::size_t> extents)
{
    if (m_implementation)
    {
        return array(m_implementation->broadcast_to(extents));
    }
    else
    {
        return array(implementation().broadcast_to(extents));
    }
}

} // namespace multidimensional
} // namespace xmipp4
