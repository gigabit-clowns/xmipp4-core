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

bool is_floating(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::float16:
	case numerical_type::float32:
	case numerical_type::float64:
		return true;
	default:
		return false;
	}
}

bool is_complex(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::complex_float16:
	case numerical_type::complex_float32:
	case numerical_type::complex_float64:
		return true;
	default:
		return false;
	}
}

bool is_integer(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::int8:
	case numerical_type::uint8:
	case numerical_type::int16:
	case numerical_type::uint16:
	case numerical_type::int32:
	case numerical_type::uint32:
	case numerical_type::int64:
	case numerical_type::uint64:
		return true;
	default:
		return false;
	}
}

bool is_arithmetic(numerical_type type) noexcept
{
	return is_integer(type) || is_floating(type) || is_complex(type);
}

bool is_boolean(numerical_type type) noexcept
{
	return type == numerical_type::boolean;
}

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

void require_same(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types,
	const char *context
)
{
	if (output_types.empty())
	{
		throw std::invalid_argument(
			std::string(context) + ": at least one input operand is required."
		);
	}

	const numerical_type reference = output_types[0];
	for (std::size_t i = 1; i < output_types.size(); ++i)
	{
		const auto &type = output_types[i];
		if (type != reference)
		{
			throw_mismatch(reference, type, context);
		}
	}

	for (const auto &type : input_types)
	{
		if (type != reference)
		{
			throw_mismatch(reference, type, context);
		}
	}
}

template <typename Predicate>
void require_category(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types,
	Predicate predicate,
	const char *category,
	const char *context
)
{
	require_same(output_types, input_types, context);
	const numerical_type reference = input_types[0];
	if (!predicate(reference))
	{
		throw_category(reference, category, context);
	}
}

void infer_same(
	span<numerical_type> output_types,
	span<const numerical_type> input_types,
	const char *context
)
{
	if (input_types.empty())
	{
		throw std::invalid_argument(
			std::string(context) + ": at least one input operand is required."
		);
	}

	const numerical_type reference = input_types[0];
	for (const auto &type : input_types)
	{
		if (type != reference)
		{
			throw_mismatch(reference, type, context);
		}
	}

	for (auto &type : output_types)
	{
		type = reference;
	}
}

template <typename Predicate>
void infer_category(
	span<numerical_type> output_types,
	span<const numerical_type> input_types,
	Predicate predicate,
	const char *category,
	const char *context
)
{
	infer_same(output_types, input_types, context);
	const numerical_type reference = input_types[0];
	if (!predicate(reference))
	{
		throw_category(reference, category, context);
	}
}

} // anonymous namespace



// same_data_type_policy

void same_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	infer_same(output_types, input_types, "same_data_type_policy");
}

void same_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_same(output_types, input_types, "same_data_type_policy");
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
	infer_category(
		output_types, input_types, is_floating,
		"floating point", "same_floating_data_type_policy"
	);
}

void same_floating_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_category(
		output_types, input_types, is_floating,
		"floating point", "same_floating_data_type_policy"
	);
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
	infer_category(
		output_types, input_types, is_complex,
		"complex", "same_complex_data_type_policy"
	);
}

void same_complex_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_category(
		output_types, input_types, is_complex,
		"complex", "same_complex_data_type_policy"
	);
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
	infer_category(
		output_types, input_types, is_arithmetic,
		"arithmetic", "same_arithmetic_data_type_policy"
	);
}

void same_arithmetic_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_category(
		output_types, input_types, is_arithmetic,
		"arithmetic", "same_arithmetic_data_type_policy"
	);
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
	infer_category(
		output_types, input_types, is_integer,
		"integer", "same_integer_data_type_policy"
	);
}

void same_integer_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_category(
		output_types, input_types, is_integer,
		"integer", "same_integer_data_type_policy"
	);
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
	infer_category(
		output_types, input_types, is_boolean,
		"boolean", "boolean_data_type_policy"
	);
}

void boolean_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_category(
		output_types, input_types, is_boolean,
		"boolean", "boolean_data_type_policy"
	);
}

const boolean_data_type_policy& boolean_data_type_policy::get() noexcept
{
	static const boolean_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
