// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "version.hpp"
#include "platform/dynamic_shared_object.h"

#include <string>
#include <vector>

namespace xmipp4
{

/**
 * @brief Base class for backends that can be indexed by their name.
 * 
 */
class XMIPP4_CORE_API named_backend
{
public:
	named_backend() = default;
	named_backend(const named_backend &other) = delete;
	named_backend(named_backend &&other) = delete;
	virtual ~named_backend() = default;

	named_backend& operator=(const named_backend &other) = delete;
	named_backend& operator=(named_backend &&other) = delete;

	/**
	 * @brief Get the name of the backend.
	 * 
	 * @return std::string The name. 
	 */
	virtual std::string get_name() const = 0;

	/**
	 * @brief Get the version of the backend.
	 * 
	 * @return version The version.
	 */
	virtual version get_version() const = 0;

};

} // namespace xmipp4

