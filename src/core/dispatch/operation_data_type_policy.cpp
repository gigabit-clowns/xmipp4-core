// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/dispatch/operation_data_type_policy.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <stdexcept>
#include <sstream>

namespace xmipp4
{
namespace dispatch
{

operation_data_type_policy::operation_data_type_policy() noexcept = default;
operation_data_type_policy::~operation_data_type_policy() = default;

void operation_data_type_policy::accept(
	span<const numerical_type> user_output_types,
	span<const numerical_type> canonical_output_types,
	span<const numerical_type> /*input_types*/
) const
{
	XMIPP4_ASSERT(user_output_types.size() == canonical_output_types.size());

	for (std::size_t i = 0; i < user_output_types.size(); ++i)
	{
		if (user_output_types[i] != canonical_output_types[i])
		{
			std::ostringstream oss;
			oss << "user-supplied output data type at index " << i
				<< " (" << user_output_types[i]
				<< ") does not match the data type deduced from the inputs"
				<< " (" << canonical_output_types[i] << ").";
			throw std::invalid_argument(oss.str());
		}
	}
}

} // namespace dispatch
} // namespace xmipp4
