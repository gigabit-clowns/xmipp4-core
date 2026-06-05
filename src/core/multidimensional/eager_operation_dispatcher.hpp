// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>
#include <xmipp4/core/multidimensional/operation_command_cache.hpp>

#include <memory>

namespace xmipp4
{
namespace multidimensional
{

class operation_command_manager;

/**
 * @brief Eager implementation of @ref operation_dispatcher.
 *
 * Each call to @ref execute immediately builds the executable command for the
 * requested operation and submits it to the provided queue. Backend-private
 * resources produced while building commands (compiled kernels, FFT plans,
 * ...) are kept in an internal @ref operation_command_cache so that they are
 * reused across builds with matching launch configurations.
 */
class eager_operation_dispatcher final
	: public operation_dispatcher
{
public:
	/**
	 * @brief Construct an eager dispatcher.
	 *
	 * @param command_manager The manager queried to build the command for each
	 * operation. Must not be null.
	 * @param cache_capacity Maximum number of backend-private resources kept in
	 * the internal cache. Must be greater than zero.
	 * @throws std::invalid_argument When @p command_manager is null.
	 */
	eager_operation_dispatcher(
		std::shared_ptr<const operation_command_manager> command_manager,
		std::size_t cache_capacity
	);
	~eager_operation_dispatcher() override;

	void dispatch(
		const operation &operation,
		span<array> output_operands,
		span<const array_view> input_operands,
		/*TBD*/
		hardware::command_queue &queue
	) override;

private:
	std::shared_ptr<const operation_command_manager> m_command_manager;
	operation_command_cache m_command_cache;
};

} // namespace multidimensional
} // namespace xmipp4
