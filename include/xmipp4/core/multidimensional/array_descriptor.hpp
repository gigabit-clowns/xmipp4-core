// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"
#include "../numerical_type.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace multidimensional
{

class array_descriptor
{
public:
	XMIPP4_CORE_API
	array_descriptor() noexcept;
	XMIPP4_CORE_API
	array_descriptor(strided_layout layout, numerical_type data_type) noexcept;
	XMIPP4_CORE_API
	array_descriptor(const array_descriptor &other);
	XMIPP4_CORE_API
	array_descriptor(array_descriptor &&other) noexcept;
	XMIPP4_CORE_API
	~array_descriptor();

	XMIPP4_CORE_API
	array_descriptor& operator=(const array_descriptor &other);
	XMIPP4_CORE_API
	array_descriptor& operator=(array_descriptor &&other) noexcept;

	XMIPP4_CORE_API
	const strided_layout& get_layout() const noexcept;

	XMIPP4_CORE_API
	numerical_type get_data_type() const noexcept;

private:
	strided_layout m_layout;
	numerical_type m_data_type;
};

XMIPP4_CORE_API
bool operator==(
	const array_descriptor &lhs, 
	const array_descriptor &rhs
) noexcept;

XMIPP4_CORE_API
bool operator!=(
	const array_descriptor &lhs, 
	const array_descriptor &rhs
) noexcept;

} // namespace multidimensional
} // namespace xmipp4
