// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{

class service_catalog;

namespace multidimensional
{

class dispatcher;

class operation_execution_context
{
public:
	XMIPP4_CORE_API
	operation_execution_context() noexcept;

	XMIPP4_CORE_API
	explicit operation_execution_context(service_catalog &catalog);

	operation_execution_context(
		const operation_execution_context &other
	) = delete;
	XMIPP4_CORE_API
	operation_execution_context(operation_execution_context &&other) noexcept;
	XMIPP4_CORE_API
	~operation_execution_context();

	operation_execution_context& 
	operator=(const operation_execution_context &other) = delete;
	XMIPP4_CORE_API
	operation_execution_context& 
	operator=(operation_execution_context &&other) noexcept;

	XMIPP4_CORE_API
	const dispatcher& get_dispatcher() const;

	

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	implementation& get_implementation();
	const implementation& get_implementation() const;
};

} // namespace multidimensional
} // namespace xmipp4
