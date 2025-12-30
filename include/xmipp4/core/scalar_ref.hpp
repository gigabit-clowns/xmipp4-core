// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"
#include "platform/constexpr.hpp"

#include <stdexcept>

namespace xmipp4
{

/**
 * @brief Exception thrown when accessing a scalar_ref with an incorrect type.
 */
class bad_scalar_ref_access
	: public std::exception
{
public:
	using exception::exception;
};

/**
 * @brief Const reference to a dynamicly typed scalar value.
 * 
 * The scalar value must be representable by one of the numerical_type types.
 */
class scalar_ref
{
public:
	/**
	 * @brief Construct a new scalar ref object from the referenced object.
	 * 
	 * @tparam T Type of the referenced object.
	 * @param value The referenced object.
	 */
	template <typename T>
	XMIPP4_CONSTEXPR
	scalar_ref(const T &value) noexcept;
	scalar_ref(const scalar_ref &other) = default;
	scalar_ref(scalar_ref &&other) = default;
	~scalar_ref() = default;

	scalar_ref& operator=(const scalar_ref &other) = default;
	scalar_ref& operator=(scalar_ref &&other) = default;

	/**
	 * @brief Type-safe access to the referenced object.
	 * 
	 * @tparam T Type to be casted to.
	 * @return const T& Reference to the casted object. 
	 */
	template <typename T>
	XMIPP4_CONSTEXPR
	const T& get() const;

	/**
	 * @brief Get the numerical type of the referenced object.
	 * 
	 * @return numerical_type The numerical type.
	 */
	XMIPP4_CONSTEXPR
	numerical_type get_data_type() const noexcept;

private:
	const void *m_data;
	numerical_type m_data_type;
};

} // namespace xmipp4

#include "scalar_ref.inl"
