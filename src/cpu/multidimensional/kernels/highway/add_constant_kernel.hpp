// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Functor to add a constant to an array.
 * 
 * @tparam T Type of the elements stored in the array.
 */
template <typename T>
class add_constant_kernel
{
public:
	using value_type = T;

	add_constant_kernel() noexcept;
	add_constant_kernel(const add_constant_kernel& other) = default;
	add_constant_kernel(add_constant_kernel&& other) = default;
	~add_constant_kernel() = default;

	add_constant_kernel& operator=(const add_constant_kernel& other) = default;
	add_constant_kernel& operator=(add_constant_kernel&& other) = default;

	void operator()(
		value_type* result, 
		const value_type* x, 
		std::size_t count,
		const value_type &value
	) const noexcept;

private:
	using handle_type = void (*)(
		value_type*, 
		const value_type*, 
		std::size_t,
		const value_type&
	);
	
	handle_type m_handle;
};

} // namespace multidimensional
} // namespace xmipp4
