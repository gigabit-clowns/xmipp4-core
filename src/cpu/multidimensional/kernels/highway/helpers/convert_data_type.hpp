// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/fixed_float.hpp>

#include <hwy/highway.h>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
struct to_hwy_data_type
{
	using type = T;
};

template <>
struct to_hwy_data_type<xmipp4::float16_t>
{
	using type = hwy::float16_t;
};

} // namespace multidimensional
} // namespace xmipp4