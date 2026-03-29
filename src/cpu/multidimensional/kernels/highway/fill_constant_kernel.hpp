// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Functor to fill an array with a constant value.
 * 
 * @tparam T Type of the array to be filled.
 */
template <typename T>
class fill_constant_kernel
{
public:
	using value_type = T;

	fill_constant_kernel() noexcept;
	fill_constant_kernel(const fill_constant_kernel& other) = default;
	fill_constant_kernel(fill_constant_kernel&& other) = default;
	~fill_constant_kernel() = default;

	fill_constant_kernel& operator=(const fill_constant_kernel& other) = default;
	fill_constant_kernel& operator=(fill_constant_kernel&& other) = default;

	void operator()(
		value_type* result, 
		std::size_t count,
		const value_type &value
	) const noexcept;

private:
	using handle_type = void (*)(
		value_type*, 
		std::size_t,
		const value_type &value
	);
	
	handle_type m_handle;
};

} // namespace multidimensional
} // namespace xmipp4
