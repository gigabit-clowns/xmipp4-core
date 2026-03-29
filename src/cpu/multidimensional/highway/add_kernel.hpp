// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{

/**
 * @brief Functor to add two arrays into a result array.
 * 
 * @tparam T Type of the items in the arrays.
 */
template <typename T>
class add_kernel
{
public:
	using value_type = T;

	add_kernel() noexcept;
	add_kernel(const add_kernel& other) = default;
	add_kernel(add_kernel&& other) = default;
	~add_kernel() = default;

	add_kernel& operator=(const add_kernel& other) = default;
	add_kernel& operator=(add_kernel&& other) = default;

	void operator()(
		value_type* result, 
		const value_type* x, 
		const value_type* y, 
		std::size_t count
	) const noexcept;

private:
	using handle_type = void (*)(
		value_type*, 
		const value_type*, 
		const value_type*, 
		std::size_t
	);
	
	handle_type m_handle;
};

} // namespace xmipp4
