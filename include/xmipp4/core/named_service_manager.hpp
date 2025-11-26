// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/dynamic_shared_object.h"

#include "service_manager.hpp"

#include <string>
#include <vector>

namespace xmipp4
{

class named_backend;

/**
 * @brief Subclass of service_manager where its backends can be indexed by
 * name.
 * 
 */
class XMIPP4_CORE_API named_service_manager
	: public service_manager
{
public:
	named_service_manager() = default;
	named_service_manager(const named_service_manager &other) = delete;
	named_service_manager(named_service_manager &&other) = delete;
	~named_service_manager() override = default;

	named_service_manager&
	operator=(const named_service_manager &other) = delete;
	named_service_manager&
	operator=(named_service_manager &&other) = delete;

	/**
	 * @brief Get the a backend contained in this manager by its name
	 * 
	 * @param name The name of the backend.
	 * @return named_backend* The retrieved backend. nullptr if none is found.
	 * 
	 */
	virtual named_backend* get_backend(const std::string &name) const = 0;

	/**
	 * @brief Enumerate available backends by their names.
	 * 
	 * @param names Output parameter with the names of the contained backends.
	 */
	virtual void enumerate_backends(std::vector<std::string> &names) const = 0;
};

} // namespace xmipp4

