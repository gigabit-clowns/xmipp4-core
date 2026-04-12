// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation.hpp>

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

operation::operation() noexcept = default;
operation::~operation() = default;

operation_id operation::get_id() const noexcept
{
	return operation_id(typeid(*this));
}

std::string operation::serialize_parameters() const
{
	return "";
}

namespace
{

template <typename Os>
Os& format_operation(
	Os& os, 
	const std::string &name, 
	const std::string &parameters
)
{

	os << name;
	if(!parameters.empty())
	{
		os << '(' << parameters << ')';
	}

	return os;
}

} // anonymous namespace

std::ostream& operator<<(std::ostream& os, const operation& op)
{
	const auto name = op.get_name();
	const auto parameters = op.serialize_parameters();
	return format_operation(os, name, parameters);
}

std::string to_string(const operation& op)
{
	const auto name = op.get_name();
	const auto parameters = op.serialize_parameters();
	if(parameters.empty())
	{
		return name; // Fast return
	}

	std::ostringstream oss;
	format_operation(oss, name, parameters);
	return oss.str();
}

} // namespace multidimensional
} // namespace xmipp4
