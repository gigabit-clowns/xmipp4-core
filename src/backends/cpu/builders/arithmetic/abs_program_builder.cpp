// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/arithmetic/abs_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/load_store.hpp>

#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>

#include <stdexcept>
#include <numeric>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct abs_kernel
{
	template <typename T>
	typename std::enable_if<std::is_unsigned<T>::value, void>::type
	operator()(T *result, const T *x) const noexcept
	{
		store(result, load(x));
	}

	template <typename T>
	typename std::enable_if<!std::is_unsigned<T>::value, void>::type
	operator()(T *result, const T *x) const noexcept
	{
		using std::abs;
		store(result, abs(load(x)));
	}

	template <typename T>
	void operator()(T *result, const std::complex<T> *x) const noexcept
	{
		store(result, std::abs(load(x)));
	}
};

struct abs_type_dispatcher
{
	template <typename T>
	struct real_type
	{
		using type = T;
	};

	template <typename T>
	struct real_type<std::complex<T>>
	{
		using type = T;
	};

	template <typename F, std::size_t OutputCount, std::size_t InputCount>
	static std::shared_ptr<xmipp4::program> dispatch(
		F &&factory,
		const std::array<numerical_type, OutputCount> &output_types,
		const std::array<numerical_type, InputCount> &input_types
	)
	{
		if (output_types.size() != 1)
		{
			throw std::invalid_argument(
				"abs_type_dispatcher: expected one output"
			);
		}

		if (input_types.size() != 1)
		{
			throw std::invalid_argument(
				"abs_type_dispatcher: expected one input"
			);
		}

		const auto output_type = output_types[0];
		const auto input_type = input_types[0];
		if (output_type != make_real(input_type))
		{
			throw std::invalid_argument(
				"abs_type_dispatcher: expected output to be input's real type"
			);
		}

		return dispatch_numerical_types(
			[&factory] (auto tag) -> std::shared_ptr<xmipp4::program>
			{
				using type = typename decltype(tag)::type;
				return std::forward<F>(factory)(
					type_list<typename real_type<type>::type>(),
					type_list<type>()
				);
			},
			input_type
		);
	}

};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	abs,
	ops::abs_operation,
	default_kernel_factory<abs_kernel>,
	abs_type_dispatcher
);

} // namespace cpu
} // namespace xmipp4
