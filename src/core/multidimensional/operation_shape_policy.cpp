// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <stdexcept>
#include <sstream>

namespace xmipp4
{
namespace multidimensional
{

operation_shape_policy::operation_shape_policy() noexcept = default;
operation_shape_policy::~operation_shape_policy() = default;

void operation_shape_policy::accept(
	span<const shape_type> user_output_shapes,
	span<const shape_type> canonical_output_shapes,
	span<const shape_type> /*input_shapes*/
) const
{
	XMIPP4_ASSERT(user_output_shapes.size() == canonical_output_shapes.size());

	for (std::size_t i = 0; i < user_output_shapes.size(); ++i)
	{
		if (user_output_shapes[i] != canonical_output_shapes[i])
		{
			std::ostringstream oss;
			oss << "user-supplied output shape at index " << i
				<< " does not match the shape deduced from the inputs.";
			throw std::invalid_argument(oss.str());
		}
	}
}

} // namespace multidimensional
} // namespace xmipp4
