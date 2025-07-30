// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_array_interface.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class array
    : public strided_array_interface<array>
{
public:
    using view_type = array;
    using const_view_type = const array;

    array() = default;
    array(const array& other) = default;
    array(array&& other) = default;
    virtual ~array() = default;

    array& operator=(const array& other) = default;
    array& operator=(array&& other) = default;

    virtual void* data() noexcept = 0;
    virtual const void* data() const noexcept = 0;

};

} // namespace multidimensional
} // namespace xmipp4
