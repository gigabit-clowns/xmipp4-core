// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_arithmetic.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/complex/complex_from_real_imag_operation.hpp>
#include <xmipp4/core/multidimensional/operations/complex/complex_from_polar_operation.hpp>
#include <xmipp4/core/multidimensional/operations/complex/conjugate_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

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

array conj(array_view x, const execution_context &context, array *out)
{
	return execute_unary(conjugate_operation(), std::move(x), context, out);
}

} // namespace multidimensional
} // namespace xmipp4
