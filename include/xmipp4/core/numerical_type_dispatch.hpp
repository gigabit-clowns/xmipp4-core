// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"
#include "platform/constexpr.hpp"

namespace xmipp4
{

/**
 * @brief Helper class to pass type information to a function.
 * 
 * Eventually to be replaced with std::type_identity in C++20.
 * 
 * @tparam T Type passed.
 */
template <typename T>
class type_tag
{
public:
	using type = T;

};

/**
 * @brief Dispatch a function with the numerical types.
 * 
 * This function dispatches a set of numerical_type-s and calls the provided
 * visitor with the dispatched types wrapped around a type_tag. In case a type
 * fails conversion to a C++ type, type_tag<void> is passed.
 * 
 * @tparam F Functor to be dispatched.
 * @tparam Types any amount of numeric_type-s
 * @param visitor Function to be called with the dispatched types.
 * @param types Types to be dispatched.
 * @return auto The result of calling visitor(types...)
 * 
 * @see numerical_type
 * @see type_tag
 */
template <typename F, typename... Types>
XMIPP4_CONSTEXPR
auto dispatch_numerical_types(F&& visitor, Types&& ...types);

} // namespace xmipp4
