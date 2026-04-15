// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/policies/same_data_type_policy.hpp>

#include <sstream>
#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

// TODO undo vibe coding.

namespace
{

XMIPP4_NORETURN 
void throw_mismatch(
	numerical_type expected, 
	numerical_type got,
	const char *context
)
{
	std::ostringstream oss;
	oss << context << ": expected all operands to have data type "
		<< expected << ", but got " << got << ".";
	throw std::invalid_argument(oss.str());
}

XMIPP4_NORETURN 
void throw_category(
	numerical_type got, 
	const char *category,
	const char *context
)
{
	std::ostringstream oss;
	oss << context << ": expected all operands to have a " << category
		<< " data type, but got " << got << ".";
	throw std::invalid_argument(oss.str());
}

numerical_type require_same(
	span<const numerical_type> types,
	const char *context
)
{
	if (types.empty())
	{
		throw std::invalid_argument(
			std::string(context) + ": at least one operand is required."
		);
	}

	const auto reference = types[0];
	for (std::size_t i = 1; i < types.size(); ++i)
	{
		const auto &type = types[i];
		if (type != reference)
		{
			throw_mismatch(reference, type, context);
		}
	}

	return reference;
}

void require_exact(
	span<const numerical_type> types,
	numerical_type value,
	const char *context
)
{
	for (auto type : types)
	{
		if (type != value)
		{
			throw_mismatch(value, type, context);
		}
	}
}

void fill(span<numerical_type> types, numerical_type value)
{
	std::fill(types.begin(), types.end(), value);
}

} // anonymous namespace



// same_data_type_policy

void same_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types, 
		"same_data_type_policy::infer_output"
	);
	check(reference);
	fill(output_types, reference);
}

void same_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types, 
		"same_data_type_policy::validate"
	);
	check(reference);
	require_exact(input_types, reference, "same_data_type_policy::validate");
}

void same_data_type_policy::check(numerical_type type)
{
	if (get_size(type) == 0)
	{
		throw std::invalid_argument("invalid numerical type");
	}
}

const same_data_type_policy& same_data_type_policy::get() noexcept
{
	static const same_data_type_policy instance;
	return instance;
}



// same_floating_data_type_policy

void same_floating_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types,
		"same_floating_data_type_policy::infer_output"
	);
	check(reference);
	fill(output_types, reference);
}

void same_floating_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types,
		"same_floating_data_type_policy::validate"
	);
	check(reference);
	require_exact(
		input_types, reference, "same_floating_data_type_policy::validate"
	);
}

void same_floating_data_type_policy::check(numerical_type type)
{
	if (get_category(type) != numerical_type_category::floating_point)
	{
		throw_category(
			type, "floating point", "same_floating_data_type_policy"
		);
	}
}

const same_floating_data_type_policy&
same_floating_data_type_policy::get() noexcept
{
	static const same_floating_data_type_policy instance;
	return instance;
}



// same_complex_data_type_policy

void same_complex_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types,
		"same_complex_data_type_policy::infer_output"
	);
	check(reference);
	fill(output_types, reference);
}

void same_complex_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types,
		"same_complex_data_type_policy::validate"
	);
	check(reference);
	require_exact(
		input_types, reference, "same_complex_data_type_policy::validate"
	);
}

void same_complex_data_type_policy::check(numerical_type type)
{
	if (get_category(type) != numerical_type_category::complex)
	{
		throw_category(type, "complex", "same_complex_data_type_policy");
	}
}

const same_complex_data_type_policy&
same_complex_data_type_policy::get() noexcept
{
	static const same_complex_data_type_policy instance;
	return instance;
}



// same_arithmetic_data_type_policy

void same_arithmetic_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types,
		"same_arithmetic_data_type_policy::infer_output"
	);
	check(reference);
	fill(output_types, reference);
}

void same_arithmetic_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types,
		"same_arithmetic_data_type_policy::validate"
	);
	check(reference);
	require_exact(
		input_types, reference, "same_arithmetic_data_type_policy::validate"
	);
}

void same_arithmetic_data_type_policy::check(numerical_type type)
{
	switch (get_category(type))
	{
	case numerical_type_category::signed_integer:
	case numerical_type_category::unsigned_integer:
	case numerical_type_category::floating_point:
	case numerical_type_category::complex:
		break;
	default:
		throw_category(
			type, "arithmetic", "same_arithmetic_data_type_policy"
		);
	}
}

const same_arithmetic_data_type_policy&
same_arithmetic_data_type_policy::get() noexcept
{
	static const same_arithmetic_data_type_policy instance;
	return instance;
}



// same_integer_data_type_policy

void same_integer_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types,
		"same_integer_data_type_policy::infer_output"
	);
	check(reference);
	fill(output_types, reference);
}

void same_integer_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types,
		"same_integer_data_type_policy::validate"
	);
	check(reference);
	require_exact(
		input_types, reference, "same_integer_data_type_policy::validate"
	);
}

void same_integer_data_type_policy::check(numerical_type type)
{
	switch (get_category(type))
	{
	case numerical_type_category::signed_integer:
	case numerical_type_category::unsigned_integer:
		break;
	default:
		throw_category(type, "integer", "same_integer_data_type_policy");
	}
}

const same_integer_data_type_policy&
same_integer_data_type_policy::get() noexcept
{
	static const same_integer_data_type_policy instance;
	return instance;
}



// boolean_data_type_policy

void boolean_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_exact(
		input_types,
		numerical_type::boolean,
		"boolean_data_type_policy::infer_output"
	);
	fill(output_types, numerical_type::boolean);
}

void boolean_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_exact(
		output_types,
		numerical_type::boolean,
		"boolean_data_type_policy::validate"
	);
	require_exact(
		input_types,
		numerical_type::boolean,
		"boolean_data_type_policy::validate"
	);
}

const boolean_data_type_policy& boolean_data_type_policy::get() noexcept
{
	static const boolean_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
