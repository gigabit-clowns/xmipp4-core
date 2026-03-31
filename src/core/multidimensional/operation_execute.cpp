// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

void execute(
	const operation &operation,
	span<array> output_operands,
	span<const array_view> input_operands,
	const execution_context &context
)
{
	//       ___
    //      /   \
    //     |--o--|
    //     |  _  |
    //     |     |
    //     |     |
    //     |     |        C'mon,
    //     |     |\       do something...
    //     |     | \
    //     |     |  \
    //     |_____|   \
    //     |  |  |    \
    //     |  |  |     \
    //     |  |  |      \
    //     |  |  |       `--
    //     |  |  |
    //     |__|__|
}

} // namespace multidimensional
} // namespace xmipp4

