// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
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

template <typename T, typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<
		contiguous_stride_tag,
		contiguous_stride_tag
	>, /*inner_strides*/
	type_tag<T> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			[inner_extent] (
				T *destination, 
				const Q *source
			)
			{
				for (std::size_t i = 0; i < inner_extent; ++i)
				{
					destination[i] = static_cast<T>(source[i]);
				}
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<Q>()
	);
}

template <typename T>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<
		contiguous_stride_tag,
		contiguous_stride_tag
	>, /*inner_strides*/
	type_tag<T> /*destination_type_tag*/,
	type_tag<T> /*source_type_tag*/
)
{
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			[inner_extent] (
				T *destination, 
				const T *source
			)
			{
				std::copy_n(source, inner_extent, destination);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T, typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<
		contiguous_stride_tag,
		broadcasting_stride_tag
	>, /*inner_strides*/
	type_tag<T> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			[inner_extent] (
				T *destination, 
				const Q *source
			)
			{
				const auto fill_value = static_cast<T>(*source);
				std::fill_n(destination, inner_extent, fill_value);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<Q>()
	);
}

template <typename T, typename Q, typename... Strides>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<Strides...> inner_strides,
	type_tag<T> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			[inner_extent, inner_strides] (
				T *destination, 
				const Q* source
			)
			{
				const auto destination_inner_stride = 
					std::get<copy_operation::OPERAND_DESTINATION>(inner_strides);
				const auto source_inner_stride = 
					std::get<copy_operation::OPERAND_SOURCE>(inner_strides);

				std::ptrdiff_t destination_index = 0;
				std::ptrdiff_t source_index = 0;
				for (std::size_t i = 0; i < inner_extent; ++i)
				{
					destination[destination_index] = 
						static_cast<T>(source[source_index]);

					destination_index += destination_inner_stride;
					source_index += source_inner_stride;
				}
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<Q>()
	);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> cast_handler(
	F&& callable,
	type_tag<T> destination_type_tag,
	type_tag<Q> source_type_tag,
	std::true_type
)
{
	return std::forward<F>(callable)(destination_type_tag, source_type_tag);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> cast_handler(
	F&&,
	type_tag<T>,
	type_tag<Q>,
	std::false_type
)
{
	throw std::invalid_argument(
		"Provided source array's data type is not castable to destination "
		"array's data type"
	);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> cast_handler(
	F&& callable,
	type_tag<T> destination_type_tag,
	type_tag<Q> source_type_tag
)
{
	return cast_handler(
		std::forward<F>(callable),
		destination_type_tag,
		source_type_tag,
		typename std::is_convertible<Q, T>::type()
	);
}

} // anonymous namespace

operation_id 
cpu_copy_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<copy_operation>();
}

backend_priority cpu_copy_kernel_builder::get_suitability(
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

std::shared_ptr<kernel> cpu_copy_kernel_builder::build(
	const operation& operation,
	span<const array_descriptor> descriptors,
	hardware::device& device
) const
{
	if (dynamic_cast<const copy_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected operation to be an "
			"instance of copy_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected device to be an instance "
			"of cpu_device"
		);
	}

	if (descriptors.size() != copy_operation::OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected exactly 2 "
			"array descriptors."
		);
	}

	const auto &destination_descriptor = 
		descriptors[copy_operation::OPERAND_DESTINATION];
	const auto &source_descriptor = 
		descriptors[copy_operation::OPERAND_SOURCE];

	array_access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	layout_builder.add_operand(source_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout] 
		(auto destination_type_tag, auto source_type_tag)
		{
			return cast_handler(
				[&access_layout] 
				(auto destination_type_tag, auto source_type_tag)
				{
					XMIPP4_CONST_CONSTEXPR
					std::integral_constant<std::size_t, copy_operation::OPERAND_COUNT> 
					operand_count;

					return dispatch_inner_loop(
						[&access_layout, destination_type_tag, source_type_tag]
						(std::size_t inner_extent, const auto &inner_strides)
						{
							return make_copy_kernel(
								std::move(access_layout),
								inner_extent,
								inner_strides,
								destination_type_tag,
								source_type_tag
							);
						},
						access_layout,
						operand_count
					);
				},
				destination_type_tag,
				source_type_tag
			);
		},
		destination_descriptor.get_data_type(),
		source_descriptor.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
