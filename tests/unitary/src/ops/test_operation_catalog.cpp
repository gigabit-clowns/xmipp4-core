// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/dispatch/operation_id.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>
#include <xmipp4/ops/policies/axis_list.hpp>
#include <xmipp4/ops/policies/inverse_real_fourier_transform_shape_policy.hpp>

#include <xmipp4/ops/arithmetic/abs_operation.hpp>
#include <xmipp4/ops/arithmetic/add_operation.hpp>
#include <xmipp4/ops/arithmetic/divide_operation.hpp>
#include <xmipp4/ops/arithmetic/divmod_operation.hpp>
#include <xmipp4/ops/arithmetic/floor_divide_operation.hpp>
#include <xmipp4/ops/arithmetic/modulo_operation.hpp>
#include <xmipp4/ops/arithmetic/multiply_operation.hpp>
#include <xmipp4/ops/arithmetic/negate_operation.hpp>
#include <xmipp4/ops/arithmetic/sign_operation.hpp>
#include <xmipp4/ops/arithmetic/subtract_operation.hpp>
#include <xmipp4/ops/assignment/copy_operation.hpp>
#include <xmipp4/ops/assignment/fill_operation.hpp>
#include <xmipp4/ops/bitwise/bitwise_and_operation.hpp>
#include <xmipp4/ops/bitwise/bitwise_not_operation.hpp>
#include <xmipp4/ops/bitwise/bitwise_or_operation.hpp>
#include <xmipp4/ops/bitwise/bitwise_xor_operation.hpp>
#include <xmipp4/ops/bitwise/left_shift_operation.hpp>
#include <xmipp4/ops/bitwise/right_shift_operation.hpp>
#include <xmipp4/ops/compare/equal_operation.hpp>
#include <xmipp4/ops/compare/greater_equal_operation.hpp>
#include <xmipp4/ops/compare/greater_operation.hpp>
#include <xmipp4/ops/compare/less_equal_operation.hpp>
#include <xmipp4/ops/compare/less_operation.hpp>
#include <xmipp4/ops/compare/maximum_operation.hpp>
#include <xmipp4/ops/compare/minimum_operation.hpp>
#include <xmipp4/ops/compare/not_equal_operation.hpp>
#include <xmipp4/ops/complex/angle_operation.hpp>
#include <xmipp4/ops/complex/conjugate_operation.hpp>
#include <xmipp4/ops/fourier/fft_operation.hpp>
#include <xmipp4/ops/fourier/fftshift_operation.hpp>
#include <xmipp4/ops/fourier/ifft_operation.hpp>
#include <xmipp4/ops/fourier/ifftshift_operation.hpp>
#include <xmipp4/ops/fourier/irfft_operation.hpp>
#include <xmipp4/ops/fourier/rfft_operation.hpp>
#include <xmipp4/ops/linalg/cross_operation.hpp>
#include <xmipp4/ops/linalg/dot_operation.hpp>
#include <xmipp4/ops/linalg/matmul_operation.hpp>
#include <xmipp4/ops/linalg/vecdot_operation.hpp>
#include <xmipp4/ops/logical/logical_and_operation.hpp>
#include <xmipp4/ops/logical/logical_not_operation.hpp>
#include <xmipp4/ops/logical/logical_or_operation.hpp>
#include <xmipp4/ops/logical/logical_xor_operation.hpp>
#include <xmipp4/ops/numeric/copysign_operation.hpp>
#include <xmipp4/ops/numeric/is_finite_operation.hpp>
#include <xmipp4/ops/numeric/is_inf_operation.hpp>
#include <xmipp4/ops/numeric/is_nan_operation.hpp>
#include <xmipp4/ops/numeric/sign_bit_operation.hpp>
#include <xmipp4/ops/power/cbrt_operation.hpp>
#include <xmipp4/ops/power/exp2_operation.hpp>
#include <xmipp4/ops/power/exp_operation.hpp>
#include <xmipp4/ops/power/expm1_operation.hpp>
#include <xmipp4/ops/power/log10_operation.hpp>
#include <xmipp4/ops/power/log1p_operation.hpp>
#include <xmipp4/ops/power/log2_operation.hpp>
#include <xmipp4/ops/power/log_operation.hpp>
#include <xmipp4/ops/power/power_operation.hpp>
#include <xmipp4/ops/power/reciprocal_operation.hpp>
#include <xmipp4/ops/power/sqrt_operation.hpp>
#include <xmipp4/ops/power/square_operation.hpp>
#include <xmipp4/ops/reduction/all_operation.hpp>
#include <xmipp4/ops/reduction/amax_operation.hpp>
#include <xmipp4/ops/reduction/amin_operation.hpp>
#include <xmipp4/ops/reduction/any_operation.hpp>
#include <xmipp4/ops/reduction/argmax_operation.hpp>
#include <xmipp4/ops/reduction/argmin_operation.hpp>
#include <xmipp4/ops/reduction/count_nonzero_operation.hpp>
#include <xmipp4/ops/reduction/mean_operation.hpp>
#include <xmipp4/ops/reduction/product_operation.hpp>
#include <xmipp4/ops/reduction/sum_operation.hpp>
#include <xmipp4/ops/rounding/ceil_operation.hpp>
#include <xmipp4/ops/rounding/floor_operation.hpp>
#include <xmipp4/ops/rounding/round_operation.hpp>
#include <xmipp4/ops/rounding/trunc_operation.hpp>
#include <xmipp4/ops/selection/clip_operation.hpp>
#include <xmipp4/ops/selection/where_operation.hpp>
#include <xmipp4/ops/trigonometric/acos_operation.hpp>
#include <xmipp4/ops/trigonometric/acosh_operation.hpp>
#include <xmipp4/ops/trigonometric/asin_operation.hpp>
#include <xmipp4/ops/trigonometric/asinh_operation.hpp>
#include <xmipp4/ops/trigonometric/atan2_operation.hpp>
#include <xmipp4/ops/trigonometric/atan_operation.hpp>
#include <xmipp4/ops/trigonometric/atanh_operation.hpp>
#include <xmipp4/ops/trigonometric/cos_operation.hpp>
#include <xmipp4/ops/trigonometric/cosh_operation.hpp>
#include <xmipp4/ops/trigonometric/degrees_operation.hpp>
#include <xmipp4/ops/trigonometric/hypot_operation.hpp>
#include <xmipp4/ops/trigonometric/radians_operation.hpp>
#include <xmipp4/ops/trigonometric/sin_operation.hpp>
#include <xmipp4/ops/trigonometric/sincos_operation.hpp>
#include <xmipp4/ops/trigonometric/sinh_operation.hpp>
#include <xmipp4/ops/trigonometric/tan_operation.hpp>
#include <xmipp4/ops/trigonometric/tanh_operation.hpp>

#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::ops;

namespace
{

// What the catalog is checked for. The operation is kept alive because
// nearly everything worth asking is a virtual call on it.
struct catalog_entry
{
	std::shared_ptr<const operation> instance;
	operation_id id;
	std::string name;
	std::string component;
	std::size_t declared_outputs;
	std::size_t declared_inputs;
};

// Constructed in place rather than taken by value: an operation is
// neither copyable nor movable, which is what keeps one from being
// duplicated out from under the dispatcher holding it.
template <typename Op, typename... Args>
void add(std::vector<catalog_entry> &catalog, Args&&... args)
{
	using operation_type = Op;

	auto held = std::make_shared<const operation_type>(
		std::forward<Args>(args)...
	);
	const auto &descriptor = operation_type::get_static_descriptor();
	const auto id = held->get_id();

	// Built in one expression because an operation_id has no value that
	// would mean "not yet known", and rightly so.
	catalog.push_back(catalog_entry{
		std::move(held),
		id,
		descriptor.get_name(),
		descriptor.get_component(),
		operation_type::output_operand_count,
		operation_type::input_operand_count
	});
}

/**
 * @brief Every operation the catalog declares.
 *
 * A new operation is added here, and the checks below then cover it. That
 * is deliberate: the alternative is a registry the declarations write
 * themselves into, which would make an operation that failed to register
 * indistinguishable from one that does not exist.
 */
std::vector<catalog_entry> make_catalog()
{
	std::vector<catalog_entry> catalog;

	add<abs_operation>(catalog);
	add<acos_operation>(catalog);
	add<acosh_operation>(catalog);
	add<add_operation>(catalog);
	add<all_operation>(catalog, axis_list{ 0 });
	add<amax_operation>(catalog, axis_list{ 0 });
	add<amin_operation>(catalog, axis_list{ 0 });
	add<angle_operation>(catalog);
	add<any_operation>(catalog, axis_list{ 0 });
	add<argmax_operation>(catalog, axis_list{ 0 });
	add<argmin_operation>(catalog, axis_list{ 0 });
	add<asin_operation>(catalog);
	add<asinh_operation>(catalog);
	add<atan_operation>(catalog);
	add<atan2_operation>(catalog);
	add<atanh_operation>(catalog);
	add<bitwise_and_operation>(catalog);
	add<bitwise_not_operation>(catalog);
	add<bitwise_or_operation>(catalog);
	add<bitwise_xor_operation>(catalog);
	add<cbrt_operation>(catalog);
	add<ceil_operation>(catalog);
	add<clip_operation>(catalog);
	add<conjugate_operation>(catalog);
	add<copy_operation>(catalog);
	add<copysign_operation>(catalog);
	add<cos_operation>(catalog);
	add<cosh_operation>(catalog);
	add<count_nonzero_operation>(catalog, axis_list{ 0 });
	add<cross_operation>(catalog, 0);
	add<degrees_operation>(catalog);
	add<divide_operation>(catalog);
	add<divmod_operation>(catalog);
	add<dot_operation>(catalog);
	add<equal_operation>(catalog);
	add<exp_operation>(catalog);
	add<exp2_operation>(catalog);
	add<expm1_operation>(catalog);
	add<fft_operation>(catalog, axis_list{ 0 });
	add<fftshift_operation>(catalog, axis_list{ 0 });
	add<fill_operation>(catalog, scalar_value(0));
	add<floor_operation>(catalog);
	add<floor_divide_operation>(catalog);
	add<greater_operation>(catalog);
	add<greater_equal_operation>(catalog);
	add<hypot_operation>(catalog);
	add<ifft_operation>(catalog, axis_list{ 0 });
	add<ifftshift_operation>(catalog, axis_list{ 0 });
	add<irfft_operation>(catalog, axis_list{ 0 }, signal_parity::even);
	add<is_finite_operation>(catalog);
	add<is_inf_operation>(catalog);
	add<is_nan_operation>(catalog);
	add<left_shift_operation>(catalog);
	add<less_operation>(catalog);
	add<less_equal_operation>(catalog);
	add<log_operation>(catalog);
	add<log10_operation>(catalog);
	add<log1p_operation>(catalog);
	add<log2_operation>(catalog);
	add<logical_and_operation>(catalog);
	add<logical_not_operation>(catalog);
	add<logical_or_operation>(catalog);
	add<logical_xor_operation>(catalog);
	add<matmul_operation>(catalog);
	add<maximum_operation>(catalog);
	add<mean_operation>(catalog, axis_list{ 0 });
	add<minimum_operation>(catalog);
	add<modulo_operation>(catalog);
	add<multiply_operation>(catalog);
	add<negate_operation>(catalog);
	add<not_equal_operation>(catalog);
	add<power_operation>(catalog);
	add<product_operation>(catalog, axis_list{ 0 });
	add<radians_operation>(catalog);
	add<reciprocal_operation>(catalog);
	add<rfft_operation>(catalog, axis_list{ 0 });
	add<right_shift_operation>(catalog);
	add<round_operation>(catalog);
	add<sign_operation>(catalog);
	add<sign_bit_operation>(catalog);
	add<sin_operation>(catalog);
	add<sincos_operation>(catalog);
	add<sinh_operation>(catalog);
	add<sqrt_operation>(catalog);
	add<square_operation>(catalog);
	add<subtract_operation>(catalog);
	add<sum_operation>(catalog, axis_list{ 0 });
	add<tan_operation>(catalog);
	add<tanh_operation>(catalog);
	add<trunc_operation>(catalog);
	add<vecdot_operation>(catalog, axis_list{ 0 });
	add<where_operation>(catalog);

	return catalog;
}

} // anonymous namespace

TEST_CASE(
	"every declared operation should report a name and a component",
	"[operation_catalog]"
)
{
	for (const auto &entry : make_catalog())
	{
		INFO( "operation " << entry.name );
		CHECK( !entry.name.empty() );
		CHECK( entry.component == "xmipp4.ops" );

		// The name a diagnostic prints and the one the declaration gives
		// come from the same place, so they cannot drift.
		CHECK( entry.instance->get_name() == entry.name );
	}
}

TEST_CASE(
	"every declared operation should agree with its own arity",
	"[operation_catalog]"
)
{
	// The operand counts come from the typing rule and the names from the
	// declaration, and basic_operation static asserts that those two
	// match. This checks that the arity reported at runtime matches them.
	for (const auto &entry : make_catalog())
	{
		INFO( "operation " << entry.name );
		const auto arity = entry.instance->get_arity();
		CHECK( arity.get_output_count() == entry.declared_outputs );
		CHECK( arity.get_input_count() == entry.declared_inputs );

		// An operation with no output would compute nothing.
		CHECK( arity.get_output_count() >= 1 );
	}
}

TEST_CASE(
	"every declared operation should name each of its operands",
	"[operation_catalog]"
)
{
	// The names are what a diagnostic uses to say which operand it
	// objected to, so a missing one is only noticed when something has
	// already gone wrong.
	for (const auto &entry : make_catalog())
	{
		INFO( "operation " << entry.name );
		const auto &descriptor = entry.instance->get_descriptor();

		const auto outputs = descriptor.get_output_operand_names();
		REQUIRE( outputs.size() == entry.declared_outputs );
		for (std::size_t i = 0; i < outputs.size(); ++i)
		{
			INFO( "output operand " << i );
			REQUIRE( outputs[i] != nullptr );
			CHECK( !std::string(outputs[i]).empty() );
		}

		const auto inputs = descriptor.get_input_operand_names();
		REQUIRE( inputs.size() == entry.declared_inputs );
		for (std::size_t i = 0; i < inputs.size(); ++i)
		{
			INFO( "input operand " << i );
			REQUIRE( inputs[i] != nullptr );
			CHECK( !std::string(inputs[i]).empty() );
		}
	}
}

TEST_CASE(
	"no two declared operations should share a name",
	"[operation_catalog]"
)
{
	// Every operation lives in one namespace whatever directory it is
	// filed under, so a name belongs to the catalog rather than to a
	// family. This is what stands between an elementwise minimum and a
	// reduction wanting to be called the same thing.
	const auto catalog = make_catalog();

	std::set<std::string> seen;
	for (const auto &entry : catalog)
	{
		INFO( "operation " << entry.name );
		CHECK( seen.insert(entry.name).second );
	}

	CHECK( seen.size() == catalog.size() );
}

TEST_CASE(
	"no two declared operations should share an identifier",
	"[operation_catalog]"
)
{
	// The identifier is what binds an operation to the builders
	// implementing it, so a collision would route one operation to
	// another's backend rather than failing to find one.
	const auto catalog = make_catalog();

	for (std::size_t i = 0; i < catalog.size(); ++i)
	{
		for (std::size_t j = i + 1; j < catalog.size(); ++j)
		{
			INFO( catalog[i].name << " against " << catalog[j].name );
			CHECK( !(catalog[i].id == catalog[j].id) );
		}
	}
}

TEST_CASE(
	"every declared operation should be reachable through the base "
	"interface",
	"[operation_catalog]"
)
{
	// A verb holds an operation only as the base class, so everything the
	// dispatcher needs has to be answerable through it.
	for (const auto &entry : make_catalog())
	{
		INFO( "operation " << entry.name );

		const operation &op = *entry.instance;
		CHECK( op.get_id() == entry.id );
		CHECK( std::string(op.get_descriptor().get_name()) == entry.name );

		// Both policies are supplied rather than left pure, which is the
		// difference between an operation that can be dispatched and one
		// that merely compiles.
		const auto &shape_policy = op.get_operation_shape_policy();
		const auto &type_policy = op.get_operation_data_type_policy();
		CHECK( static_cast<const void*>(&shape_policy) !=
		       static_cast<const void*>(&type_policy) );
	}
}
