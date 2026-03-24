// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_sum_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/sum_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>

#include "cpu_kernel.hpp"
#include "cpu_inner_loop_dispatch.hpp"
#include "cpu_outer_loop.hpp"

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

using sum_operand_count_tag =
	std::integral_constant<std::size_t, sum_operation::OPERAND_COUNT>;

template <typename T>
std::shared_ptr<kernel> make_sum_kernel(
	array_access_layout access_layout,
	const std::tuple<
		contiguous_stride_tag,
		contiguous_stride_tag
	>, /*inner_strides*/
	type_tag<T> /*type_tag*/
)
{
	return make_typed_kernel_shared(
		// TODO fill with zeros before accumulating
		make_cpu_outer_loop(
			[] (T *result, const T *x, std::size_t count)
			{
				// TODO vectorize
				for (std::size_t i = 0; i < count; ++i)
				{
					result[i] += x[i];
				}
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T>
std::shared_ptr<kernel> make_sum_kernel(
	array_access_layout access_layout,
	const std::tuple<
		broadcasting_stride_tag,
		contiguous_stride_tag
	>, /*inner_strides*/
	type_tag<T> /*type_tag*/
)
{
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			[] (T *result, const T *x, std::size_t count)
			{
				// TODO vectorize
				T sum = {};
				for (std::size_t i = 0; i < count; ++i)
				{
					sum += x[i];
				}

				*result = sum;
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T>
T scalar_cast(std::size_t value, type_tag<T>)
{
	return T(value);
}

template <typename T>
T scalar_cast(std::size_t value, type_tag<std::complex<T>>)
{
	return T(value);
}

template <typename T>
std::shared_ptr<kernel> make_sum_kernel(
	array_access_layout access_layout,
	const std::tuple<
		broadcasting_stride_tag,
		broadcasting_stride_tag
	> /*inner_strides*/,
	type_tag<T> /*type_tag*/
)
{
	// Rare case. Explicitly declared to avoid ambiguous call error. Getting the 
	// optimized path for "free".
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			[] (T *result, const T *x, std::size_t count)
			{
				*result = scalar_cast(count, type_tag<T>()) * (*x);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T>
std::shared_ptr<kernel> make_sum_kernel(
	array_access_layout access_layout,
	const std::tuple<
		broadcasting_stride_tag,
		std::ptrdiff_t
	> inner_strides,
	type_tag<T> /*type_tag*/
)
{
	const auto x_inner_stride = 
		std::get<sum_operation::OPERAND_X>(inner_strides);

	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			[x_inner_stride] (T *result, const T *x, std::size_t count)
			{
				T sum = {};
				std::ptrdiff_t x_index = 0;
				for (std::size_t i = 0; i < count; ++i)
				{
					sum += x[x_index];

					x_index += x_inner_stride;
				}

				*result = sum;
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T>
std::shared_ptr<kernel> make_sum_kernel(
	array_access_layout access_layout,
	const std::tuple<
		std::ptrdiff_t,
		std::ptrdiff_t
	> inner_strides,
	type_tag<T> /*type_tag*/
)
{
	const auto result_inner_stride = 
		std::get<sum_operation::OPERAND_RESULT>(inner_strides);
	const auto x_inner_stride = 
		std::get<sum_operation::OPERAND_X>(inner_strides);

	XMIPP4_ASSERT(result_inner_stride != 0);

	return make_typed_kernel_shared(
		// TODO fill with zeros before accumulating
		make_cpu_outer_loop(
			[result_inner_stride, x_inner_stride] 
			(T *result, const T *x, std::size_t count)
			{
				std::ptrdiff_t result_index = 0;
				std::ptrdiff_t x_index = 0;

				for (std::size_t i = 0; i < count; ++i)
				{
					result[result_index] += x[x_index];

					result_index += result_inner_stride;
					x_index += x_inner_stride;
				}
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T>()
	);
}

} // anonymous namespace

operation_id 
cpu_sum_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<sum_operation>();
}

backend_priority cpu_sum_kernel_builder::get_suitability(
	const operation&,
	span<const array_descriptor>,
	hardware::device &device
) const
{
	if (dynamic_cast<const hardware::cpu_device*>(&device) != nullptr)
	{
		return backend_priority::normal;
	}

	return backend_priority::unsupported;
}

std::shared_ptr<kernel> cpu_sum_kernel_builder::build(
	const operation& operation,
	span<const array_descriptor> descriptors,
	hardware::device& device
) const
{
	if (dynamic_cast<const sum_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_sum_kernel_builder::build: Expected operation to be an "
			"instance of sum_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_sum_kernel_builder::build: Expected device to be an instance "
			"of cpu_device"
		);
	}

	if (descriptors.size() != sum_operation::OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_sum_kernel_builder::build: Expected exactly 2 "
			"array descriptors."
		);
	}

	const auto data_type = descriptors[0].get_data_type();
	for (std::size_t i = 1; i < sum_operation::OPERAND_COUNT; ++i)
	{
		if (descriptors[i].get_data_type() != data_type)
		{
			throw std::invalid_argument(
				"cpu_sum_kernel_builder::build: Expected all operands to have "
				"the same data type"
			);
		}
	}

	array_access_layout_builder layout_builder;
	for (const auto &descriptor : descriptors)
	{
		layout_builder.add_operand(descriptor.get_layout());
	}
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout] 
		(auto type_tag)
		{
			return dispatch_inner_loop(
				[&access_layout, type_tag]
				(const auto &inner_strides)
				{
					return make_sum_kernel(
						std::move(access_layout),
						inner_strides,
						type_tag
					);
				},
				access_layout,
				sum_operand_count_tag()
			);
		},
		data_type
	);
}

} // namespace multidimensional
} // namespace xmipp4
