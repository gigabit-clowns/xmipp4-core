// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../service_manager.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class command;

} // namespace hardware

namespace multidimensional
{

class operation;
class array_signature;
class operation_command_builder;

/**
 * @brief Centralizes all known instances of operation command builders.
 *
 * The operation command builders are cataloged by their intended operation such
 * that it allows fast retrieval.
 */
class operation_command_manager
	: public service_manager
{
public:
	XMIPP4_CORE_API
	operation_command_manager() noexcept;
	operation_command_manager(const operation_command_manager &other) = delete;
	operation_command_manager(operation_command_manager &&other) = delete;
	XMIPP4_CORE_API
	~operation_command_manager() override;

	operation_command_manager& 
	operator=(const operation_command_manager &other) = delete;
	operation_command_manager& 
	operator=(operation_command_manager &&other) = delete;

	XMIPP4_CORE_API
	void register_builtin_backends() override;

	/**
	 * @brief Register a new operation command.
	 *
	 * @param builder The builder to be registered.
	 * @return true The operation command builder was successfully registered.
	 * @return false The operation command builder could not be registered.
	 */
	XMIPP4_CORE_API
	bool register_builder(
		std::unique_ptr<operation_command_builder> builder
	);

	/**
	 * @brief Build an executable operation command for a given launch 
	 * configuration.
	 *
	 * @param operation The operation for which the command is built.
	 * @param output_signatures The output array signatures involved in the
	 * operation.
	 * @param input_signatures The output array signatures involved in the
	 * operation.
	 * @return std::shared_ptr<hardware::command> The executable command suited 
	 * for the requested operation and signature.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<hardware::command> build(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures
	) const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	implementation& create_if_null();
	const implementation& get_implementation() const noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
