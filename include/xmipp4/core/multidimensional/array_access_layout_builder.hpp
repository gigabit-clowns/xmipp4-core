// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_access_layout_build_flags.hpp"
#include "array_access_layout.hpp"
#include "../span.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;
class array_access_layout_implementation;

class array_access_layout_builder
{
public:
    static const array_access_layout_build_flags default_flags;

    array_access_layout_builder() noexcept;
    array_access_layout_builder(const array_access_layout_builder&) = delete;
    array_access_layout_builder(array_access_layout_builder&& other) noexcept;
    ~array_access_layout_builder();

    array_access_layout_builder& 
    operator=(const array_access_layout_builder&) = delete;
    array_access_layout_builder& 
    operator=(array_access_layout_builder&& other) noexcept;

    array_access_layout_builder& set_extents(
        std::vector<std::size_t> batch_extents
    );

    array_access_layout_builder& add_operand(
        const strided_layout &layout,
        numerical_type data_type
    );

    array_access_layout build(
        array_access_layout_build_flags flags = default_flags
    );

private:
    std::unique_ptr<array_access_layout_implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4
