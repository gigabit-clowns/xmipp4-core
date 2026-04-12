// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class ofr comparing two arrays value by value.
 */
class XMIPP4_CORE_API compare_operation
	: public binary_elementwise_operation
{
public:
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Computes a < b.
 */
class XMIPP4_CORE_API less_operation final
	: public compare_operation
{
public:
	std::string get_name() const;
};

/**
 * @brief Computes a <= b.
 */
class XMIPP4_CORE_API less_equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const;
};

/**
 * @brief Computes a > b.
 */
class XMIPP4_CORE_API greater_operation final
	: public compare_operation
{
public:
	std::string get_name() const;
};

/**
 * @brief Computes a >= b.
 */
class XMIPP4_CORE_API greater_equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const;
};

/**
 * @brief Computes a == b.
 */
class XMIPP4_CORE_API equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const;
};

/**
 * @brief Computes a != b.
 */
class XMIPP4_CORE_API inequal_operation final
	: public compare_operation
{
public:
	std::string get_name() const;
};
} // namespace multidimensional
} // namespace xmipp4
