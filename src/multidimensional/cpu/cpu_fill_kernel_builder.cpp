// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>
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

template <typename T>
class cpu_fill
{
public:
	explicit cpu_fill(const T &fill_value)
		: m_fill_value(fill_value)
	{
	}

	void operator()(
		T *destination, 
		std::size_t count,
		std::ptrdiff_t destination_stride
	) const
	{
		std::ptrdiff_t destination_index = 0;
		for (std::size_t i = 0; i < count; ++i)
		{
			destination[destination_index] = m_fill_value;

			destination_index += destination_stride;
		}
	}

	void operator()(
		T *destination, 
		std::size_t count,
		std::integral_constant<std::ptrdiff_t, 1>
	) const
	{
		std::fill(destination, destination + count, m_fill_value);
	}

private:
	T m_fill_value;
};

template <typename T, typename... Strides>
std::shared_ptr<kernel> make_fill_kernel(
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<Strides...> inner_strides,
	const T &fill_value
)
{
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			cpu_fill<T>(fill_value),
			std::move(access_layout),
			inner_extent,
			inner_strides
		),
		type_list<T>(),
		type_list<>()
	);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> fill_value_cast_handler(
	F&& callable,
	const T& fill_value,
	type_tag<Q>,
	std::true_type
)
{
	return std::forward<F>(callable)(static_cast<Q>(fill_value));
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> fill_value_cast_handler(
	F&&,
	const T&,
	type_tag<Q>,
	std::false_type
)
{
	throw std::invalid_argument(
		"Provided fill_value is not castable to destination array's type"
	);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> fill_value_cast_handler(
	F&& callable,
	const T& fill_value,
	type_tag<Q> destination_type_tag
)
{
	return fill_value_cast_handler(
		std::forward<F>(callable),
		fill_value,
		destination_type_tag,
		typename std::is_convertible<T, Q>::type()
	);
}

template <typename F, typename T>
std::shared_ptr<kernel> fill_value_cast_handler(
	F&& callable,
	const T& fill_value,
	type_tag<T>
)
{
	return std::forward<F>(callable)(fill_value);
}

} // anonymous namespace

operation_id 
cpu_fill_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<fill_operation>();
}

backend_priority cpu_fill_kernel_builder::get_suitability(
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

std::shared_ptr<kernel> cpu_fill_kernel_builder::build(
	const operation& operation,
	span<const array_descriptor> descriptors,
	hardware::device& device
) const
{
	const auto *fill_op = 
		dynamic_cast<const fill_operation*>(&operation);
	if (fill_op == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel_builder::build: Expected operation to be an "
			"instance of fill_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel_builder::build: Expected device to be an instance "
			"of cpu_device"
		);
	}

	if (descriptors.size() != fill_operation::OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel_builder::build: Expected exactly 1 "
			"array descriptors."
		);
	}

	const auto &destination_descriptor = 
		descriptors[fill_operation::OPERAND_DESTINATION];

	const auto data_type = destination_descriptor.get_data_type();
	const auto fill_value = fill_op->get_fill_value();

	array_access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout, &fill_value] 
		(auto destination_type_tag, auto fill_value_type_tag)
		{
			return fill_value_cast_handler(
				[&access_layout] (const auto &fill_value)
				{
					XMIPP4_CONST_CONSTEXPR
					std::integral_constant<std::size_t, fill_operation::OPERAND_COUNT> 
					operand_count;

					return dispatch_inner_loop(
						[&access_layout, &fill_value]
						(std::size_t inner_extent, const auto &inner_strides)
						{
							return make_fill_kernel(
								std::move(access_layout),
								inner_extent,
								inner_strides,
								fill_value
							);
						},
						access_layout,
						operand_count
					);
				},
				fill_value.get<typename decltype(fill_value_type_tag)::type>(),
				destination_type_tag
			);
		},
		data_type,
		fill_value.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
