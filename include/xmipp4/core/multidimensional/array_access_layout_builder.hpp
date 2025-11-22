// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_access_layout_build_flags.hpp"
#include "array_access_layout.hpp"
#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;
class array_access_layout_implementation;

/**
 * @brief Factory class to obtain instances of array_access_layout.
 * 
 * @see array_access_layout
 * 
 */
class array_access_layout_builder
{
public:
    static XMIPP4_CONST_CONSTEXPR 
    array_access_layout_build_flags default_flags = {
        array_access_layout_build_flag_bits::enable_reordering,
        array_access_layout_build_flag_bits::enable_coalescing
    };

    array_access_layout_builder() noexcept;
    array_access_layout_builder(const array_access_layout_builder&) = delete;
    array_access_layout_builder(array_access_layout_builder&& other) noexcept;
    ~array_access_layout_builder();

    array_access_layout_builder& 
    operator=(const array_access_layout_builder&) = delete;
    array_access_layout_builder& 
    operator=(array_access_layout_builder&& other) noexcept;

    /**
     * @brief Set the extents of the iteration space.
     * 
     * @param extents Extents of the iteration space.
     * @return array_access_layout_builder& A reference to *this.
     * 
     * @note This method can only be called once at most for each layout creation
     * and it must be called before any call to add_operand.
     */
    array_access_layout_builder& set_extents(span<const std::size_t> extents);

    /**
     * @brief Add an operand to the layout.
     *
     * If set_extents has not been called, the first call to this method will
     * set the extents of the operand as the extents of the iteration space.
     * 
     * When the iteration space extents are defined, operands that do not
     * match these extents are broadcasted to be consistent. Note that
     * iteration space is never broadcasted.
     *  
     * @param layout Layout of the operand.
     * @return array_access_layout_builder& A reference to *this.
     */
    array_access_layout_builder& add_operand(const strided_layout &layout);

    /**
     * @brief Build the array_access_layout from the operands contained in 
     * this builder.
     * 
     * The building process can apply certain optimizations to the joint
     * layout which can be controlled though the flags parameter.
     * 
     * @param flags Flags controlling the optimizations applied to the joint
     * layout. By default, all optimizations are enabled.
     * @return array_access_layout The newly built array_access_layout.
     */
    array_access_layout build(
        array_access_layout_build_flags flags = default_flags
    );

    /**
     * @brief Get a pointer to the implementation.
     * 
     * @return const array_access_layout_implementation* Pointer to the
     * implementation.
     * 
     * @note This method is meant to be used for testing purposes, as the 
     * array_access_layout_implementation is not publicly available.
     */
    const array_access_layout_implementation* 
    get_implementation() const noexcept;

private:
    std::unique_ptr<array_access_layout_implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4
