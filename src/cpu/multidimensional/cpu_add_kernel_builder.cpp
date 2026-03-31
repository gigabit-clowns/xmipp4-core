// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_add_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/add_operation.hpp>
#include <xmipp4/core/multidimensional/multi_array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/cpu/hardware/cpu_device.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>

#include "cpu_kernel.hpp"
#include "cpu_inner_loop_dispatch.hpp"
#include "cpu_elementwise_outer_loop.hpp"
#include "kernels/generic/arithmetic.hpp"
#include "kernels/highway/add_kernel.hpp"
#include "kernels/highway/add_constant_kernel.hpp"
#include "kernels/highway/fill_constant_kernel.hpp"
#include "kernels/highway/helpers/convert_data_type.hpp"

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

using add_operand_count_tag =
	std::integral_constant<std::size_t, add_operation::OPERAND_COUNT>;

template <typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
std::shared_ptr<kernel> make_add_kernel(
	multi_array_access_layout access_layout,
	std::tuple<
		contiguous_stride_tag,
		contiguous_stride_tag,
		contiguous_stride_tag
	> /*inner_strides*/,
	type_tag<T> /*type_tag*/
)
{
	add_kernel<typename to_hwy_data_type<T>::type> add;
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[add] (T *result, const T *lhs, const T *rhs, std::size_t count)
			{
				add(to_hwy(result), to_hwy(lhs), to_hwy(rhs), count);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T, T>()
	);
}

template <typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
std::shared_ptr<kernel> make_add_kernel(
	multi_array_access_layout access_layout,
	std::tuple<
		contiguous_stride_tag,
		broadcasting_stride_tag,
		contiguous_stride_tag
	> /*inner_strides*/,
	type_tag<T> /*type_tag*/
)
{
	add_constant_kernel<typename to_hwy_data_type<T>::type> add;
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[add] (T *result, const T *lhs, const T *rhs, std::size_t count)
			{
				add(to_hwy(result), to_hwy(rhs), count, *to_hwy(lhs));
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T, T>()
	);
}

template <typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
std::shared_ptr<kernel> make_add_kernel(
	multi_array_access_layout access_layout,
	std::tuple<
		contiguous_stride_tag,
		contiguous_stride_tag,
		broadcasting_stride_tag
	> /*inner_strides*/,
	type_tag<T> /*type_tag*/
)
{
	add_constant_kernel<typename to_hwy_data_type<T>::type> add;
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[add] (T *result, const T *lhs, const T *rhs, std::size_t count)
			{
				add(to_hwy(result), to_hwy(lhs), count, *to_hwy(rhs));
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T, T>()
	);
}

template <typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
std::shared_ptr<kernel> make_add_kernel(
	multi_array_access_layout access_layout,
	std::tuple<
		contiguous_stride_tag,
		broadcasting_stride_tag,
		broadcasting_stride_tag
	> /*inner_strides*/,
	type_tag<T> /*type_tag*/
)
{
	fill_constant_kernel<typename to_hwy_data_type<T>::type> fill;
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[fill] (T *result, const T *lhs, const T *rhs, std::size_t count)
			{
				const auto fill_value = *lhs + *rhs;
				fill(to_hwy(result), count, *to_hwy(&fill_value));
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T, T>()
	);
}

template <typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
std::shared_ptr<kernel> make_add_kernel(
	multi_array_access_layout access_layout,
	const std::tuple<ptrdiff_t, ptrdiff_t, ptrdiff_t>& inner_strides,
	type_tag<T> /*type_tag*/
)
{
	const auto result_inner_stride = 
		std::get<add_operation::OPERAND_RESULT>(inner_strides);
	const auto lhs_inner_stride = 
		std::get<add_operation::OPERAND_LHS>(inner_strides);
	const auto rhs_inner_stride = 
		std::get<add_operation::OPERAND_RHS>(inner_strides);

	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[result_inner_stride, lhs_inner_stride, rhs_inner_stride] 
			(T *result, const T *lhs, const T *rhs, std::size_t count)
			{
				add_strided(
					result, 
					lhs, 
					rhs, 
					count, 
					result_inner_stride, 
					lhs_inner_stride, 
					rhs_inner_stride
				);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T, T>()
	);
}

template <typename Stride1, typename Stride2, typename Stride3>
std::shared_ptr<kernel> make_add_kernel(
	multi_array_access_layout /*access_layout*/,
	const std::tuple<Stride1, Stride2, Stride3>& /*inner_strides*/,
	type_tag<void> /*type_tag*/
)
{
	throw std::invalid_argument(
		"cpu_add_kernel_builder::build: Expected an arithmetic type."
	);
}

} // anonymous namespace

operation_id 
cpu_add_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<add_operation>();
}

backend_priority cpu_add_kernel_builder::get_suitability(
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

std::shared_ptr<kernel> cpu_add_kernel_builder::build(
	const operation& operation,
	span<const array_descriptor> descriptors,
	hardware::device& device
) const
{
	if (dynamic_cast<const add_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_add_kernel_builder::build: Expected operation to be an "
			"instance of add_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_add_kernel_builder::build: Expected device to be an instance "
			"of cpu_device"
		);
	}

	if (descriptors.size() != add_operation::OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_add_kernel_builder::build: Expected exactly 3 "
			"array descriptors."
		);
	}

	const auto data_type = descriptors[0].get_data_type();
	for (std::size_t i = 1; i < add_operation::OPERAND_COUNT; ++i)
	{
		if (descriptors[i].get_data_type() != data_type)
		{
			throw std::invalid_argument(
				"cpu_add_kernel_builder::build: Expected all operands to have "
				"the same data type"
			);
		}
	}

	multi_array_access_layout_builder layout_builder;
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
					return make_add_kernel(
						std::move(access_layout),
						inner_strides,
						type_tag
					);
				},
				access_layout,
				add_operand_count_tag()
			);
		},
		native_arithmetic_type_map(),
		data_type
	);
}

} // namespace multidimensional
} // namespace xmipp4
