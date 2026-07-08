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

class program;
class command_queue;

} // namespace hardware

namespace ndarray
{

class array_signature;

} // namespace ndarray

namespace multidimensional
{

class operation;
class operation_program_builder;
class operation_program_cache;

/**
 * @brief Centralizes all known instances of operation program builders.
 *
 * The operation program builders are cataloged by their intended operation such
 * that it allows fast retrieval.
 */
class XMIPP4_CORE_API operation_program_manager
	: public service_manager
{
public:
	operation_program_manager() noexcept;
	operation_program_manager(const operation_program_manager &other) = delete;
	operation_program_manager(operation_program_manager &&other) = delete;
	~operation_program_manager() override;

	operation_program_manager&
	operator=(const operation_program_manager &other) = delete;
	operation_program_manager&
	operator=(operation_program_manager &&other) = delete;

	void register_builtin_backends() override;

	/**
	 * @brief Register a new operation program builder.
	 *
	 * @param builder The builder to be registered.
	 * @return true The operation program builder was successfully registered.
	 * @return false The operation program builder could not be registered.
	 */
	bool register_builder(
		std::unique_ptr<operation_program_builder> builder
	);

	/**
	 * @brief Build an executable operation program for a given launch
	 * configuration.
	 *
	 * @param operation The operation for which the program is built.
	 * @param output_signatures The output array signatures involved in the
	 * operation.
	 * @param input_signatures The input array signatures involved in the
	 * operation.
	 * @param queue The command queue where the built program is
	 * intended to be used.
	 * @param cache Optional cache for backend-private resources, forwarded
	 * to the selected builder.
	 * @return std::shared_ptr<hardware::program> The executable program suited
	 * for the requested operation and signature.
	 */
	std::shared_ptr<hardware::program> build(
		const operation &operation,
		span<const ndarray::array_signature> output_signatures,
		span<const ndarray::array_signature> input_signatures,
		hardware::command_queue &queue,
		operation_program_cache *cache = nullptr
	) const;

private:
	class implementation;
	XMIPP4_STD_MEMBER_INTERFACE 
	std::unique_ptr<implementation> m_implementation;

	implementation& create_if_null();
	const implementation& get_implementation() const noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
