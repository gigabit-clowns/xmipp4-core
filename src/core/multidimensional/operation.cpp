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

std::ostream& operator<<(std::ostream& os, const operation& op)
{
	const auto name = op.get_name();
	os << name;

	const auto parameters = op.serialize_parameters();
	if(!parameters.empty())
	{
		os << '(' << parameters << ')';
	}

	return os;
}

std::string to_string(const operation& op)
{
	std::ostringstream oss;
	oss << op;
	return oss.str();
}

} // namespace multidimensional
} // namespace xmipp4
