// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/subscript_tags.hpp>
#include <xmipp4/core/multidimensional/slice.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_subscript_callable
{
public:
    MAKE_MOCK1(function_call, void(ellipsis_tag), const);
    MAKE_MOCK1(function_call, void(new_axis_tag), const);
    MAKE_MOCK1(function_call, void(std::ptrdiff_t), const);
    MAKE_MOCK1(function_call, void(dynamic_slice), const);

    template <typename T>
    void operator()(T &&arg) const
    {
        function_call(std::forward<T>(arg));
    }

};

} // namespace multidimensional
} // namespace xmipp4
