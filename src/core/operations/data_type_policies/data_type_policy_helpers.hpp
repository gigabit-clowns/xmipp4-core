// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>

#include <initializer_list>

namespace xmipp4
{
namespace operations
{

XMIPP4_NORETURN
void throw_mismatch(
	numerical_type expected,
	numerical_type got,
	const char *context
);

XMIPP4_NORETURN
void throw_category(
	numerical_type got,
	const char *category,
	const char *context
);

void require_valid(
	numerical_type type,
	const char *context
);

void require_category(
	numerical_type type,
	std::initializer_list<numerical_type_category> allowed,
	const char *category_name,
	const char *context
);

numerical_type require_same(
	span<const numerical_type> types,
	const char *context
);

void require_exact(
	span<const numerical_type> types,
	numerical_type value,
	const char *context
);

void fill(span<numerical_type> types, numerical_type value);

} // namespace operations
} // namespace xmipp4

#include "data_type_policy_helpers.inl"
