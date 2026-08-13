// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/dispatch/operation_descriptor.hpp>

namespace xmipp4
{

operation_descriptor::operation_descriptor(
	const char *component,
	const char *name,
	span<const char* const> output_operand_names,
	span<const char* const> input_operand_names
) noexcept
	: m_component(component)
	, m_name(name)
	, m_output_operand_names(output_operand_names)
	, m_input_operand_names(input_operand_names)
{
}

const char* operation_descriptor::get_component() const noexcept
{
	return m_component;
}

const char* operation_descriptor::get_name() const noexcept
{
	return m_name;
}

operation_arity operation_descriptor::get_arity() const noexcept
{
	return operation_arity(
		m_output_operand_names.size(),
		m_input_operand_names.size()
	);
}

span<const char* const>
operation_descriptor::get_output_operand_names() const noexcept
{
	return m_output_operand_names;
}

span<const char* const>
operation_descriptor::get_input_operand_names() const noexcept
{
	return m_input_operand_names;
}

const char* operation_descriptor::get_operand_name(
	std::size_t index,
	bool output_operand
) const noexcept
{
	const auto &names =
		output_operand ? m_output_operand_names : m_input_operand_names;

	return index < names.size() ? names[index] : nullptr;
}

std::ostream& operator<<(
	std::ostream &os,
	const operation_descriptor &descriptor
)
{
	return os << descriptor.get_component() << '.' << descriptor.get_name();
}

} // namespace xmipp4
