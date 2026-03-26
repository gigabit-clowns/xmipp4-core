// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4
{

/**
 * @brief Helper class to pass type information to a function.
 * 
 * Eventually to be replaced with `std::type_identity` in C++20.
 * 
 * @tparam T Type passed.
 */
template <typename T>
class type_tag
{
public:
	using type = T; ///< The type passed as the template argument.
};

/**
 * @brief Helper class to pass a heterogeneous sequence of types to a function.
 * 
 * This class offers similar functionality to `std::tuple`, but explicitly
 * avoids storing the actual values.
 * 
 * @tparam Ts Sequence of types.
 */
template <typename... Ts>
struct type_list {};

/**
 * @brief Utility class to obtain the size of a `type_list`.
 * 
 * @tparam T A specialization `type_list` for which the size is obtained.
 */
template <typename T>
struct type_list_size;

template <typename... Ts>
struct type_list_size<type_list<Ts...>>
	: std::integral_constant<std::size_t, sizeof...(Ts)>
{
};

/**
 * @brief Utility class to index a type on a `type_list`.
 * 
 * @tparam T A specialization `type_list` for which a type is extracted.
 */
template<std::size_t I, typename T>
struct type_list_element;

template<std::size_t I, typename Head, typename... Tail>
struct type_list_element<I, type_list<Head, Tail...>>
    : type_list_element<I - 1, type_list<Tail...>>
{
};
 
template<class Head, typename... Tail>
struct type_list_element<0, type_list<Head, Tail...>>
{
    using type = Head;
};

} // namespace xmipp4
