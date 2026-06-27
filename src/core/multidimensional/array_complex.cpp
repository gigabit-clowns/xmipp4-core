// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_arithmetic.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/complex/complex_from_real_imag_operation.hpp>
#include <xmipp4/core/multidimensional/operations/complex/complex_from_polar_operation.hpp>
#include <xmipp4/core/multidimensional/operations/complex/conjugate_operation.hpp>
#include <xmipp4/core/multidimensional/operations/complex/complex_angle_operation.hpp>

#include <core/multidimensional/strided_layout_implementation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

array get_complex_piece(array a, std::ptrdiff_t offset)
{
	auto storage = a.share_storage();
	const auto &descriptor = a.get_descriptor();
	const auto complex_data_type = descriptor.get_data_type();
	const auto real_data_type = make_real(complex_data_type);
	
	if (
		real_data_type == numerical_type::unknown || 
		real_data_type == complex_data_type
	)
	{
		throw std::invalid_argument("expected complex input");
	}

	const auto &layout_impl = descriptor.get_layout().get_implementation();
	auto axes = layout_impl.get_axes();
	for (auto &axis : axes)
	{
		axis.set_stride(2*axis.get_stride());
	}
	
	return array(
		std::move(storage),
		array_descriptor(
			strided_layout(
				strided_layout_implementation(
					std::move(axes),
					layout_impl.get_offset() + offset
				)
			),
			real_data_type
		)
	);
}

} // anonymous namespace

array complex(
	array_view real,
	array_view imag,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		complex_from_real_imag_operation(),
		std::move(real),
		std::move(imag),
		context,
		out
	);
}

array polar(
	array_view abs,
	array_view angle,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		complex_from_polar_operation(),
		std::move(abs),
		std::move(angle),
		context,
		out
	);
}

array real(array value)
{
	return get_complex_piece(std::move(value), 0);
}

array imag(array value)
{
	return get_complex_piece(std::move(value), 1);
}

array conj(array_view x, const execution_context &context, array *out)
{
	return execute_unary(conjugate_operation(), std::move(x), context, out);
}

array angle(array_view x, const execution_context &context, array *out)
{
	return execute_unary(complex_angle_operation(), std::move(x), context, out);
}

} // namespace multidimensional
} // namespace xmipp4
