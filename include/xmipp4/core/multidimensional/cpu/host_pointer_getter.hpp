// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../../hardware/buffer.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class host_pointer_getter
{
public:
	template <typename T>
	T* operator()(hardware::buffer &buffer) const;

	template <typename T>
	const T* operator()(const hardware::buffer &buffer) const;
};

} // namespace multidimensional
} // namespace xmipp4
