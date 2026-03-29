// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{

/**
 * @brief Functor to reduce an array into the sum of all of its elements.
 * 
 * @tparam T Type of the elements in the arrays.
 */
template <typename T>
class sum_kernel
{
public:
	using value_type = T;

	sum_kernel() noexcept;
	sum_kernel(const sum_kernel& other) = default;
	sum_kernel(sum_kernel&& other) = default;
	~sum_kernel() = default;

	sum_kernel& operator=(const sum_kernel& other) = default;
	sum_kernel& operator=(sum_kernel&& other) = default;

	value_type operator()(
		const value_type* values, 
		std::size_t count
	) const noexcept;

private:
	using handle_type = value_type (*)(
		const value_type*, 
		std::size_t
	);
	
	handle_type m_handle;
};

} // namespace xmipp4
