// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>
#include <xmipp4/core/multidimensional/operation_program_cache.hpp>

#include <memory>

namespace xmipp4
{
namespace multidimensional
{

class operation_program_manager;

/**
 * @brief Eager implementation of @ref operation_dispatcher.
 *
 * Each call to @ref dispatch immediately builds the executable program for the
 * requested operation and submits it to the provided queue. Backend-private
 * resources produced while building programs (compiled kernels, FFT plans,
 * ...) are kept in an internal @ref operation_program_cache so that they are
 * reused across builds with matching launch configurations.
 */
class eager_operation_dispatcher final
	: public operation_dispatcher
{
public:
	/**
	 * @brief Construct an eager dispatcher.
	 *
	 * @param program_manager The manager queried to build the program for each
	 * operation. Must not be null.
	 * @param cache_capacity Maximum number of backend-private resources kept in
	 * the internal cache.
	 * @throws std::invalid_argument When @p program_manager is null.
	 */
	eager_operation_dispatcher(
		std::shared_ptr<const operation_program_manager> program_manager,
		std::size_t cache_capacity
	);
	~eager_operation_dispatcher() override;

	void dispatch(
		const operation &operation,
		span<array> output_operands,
		span<const array_view> input_operands,
		const hardware::device_context &device_context
	) override;

private:
	std::shared_ptr<const operation_program_manager> m_program_manager;
	operation_program_cache m_program_cache;
};

} // namespace multidimensional
} // namespace xmipp4
