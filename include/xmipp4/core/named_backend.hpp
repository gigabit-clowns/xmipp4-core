// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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

	virtual std::string get_name() const = 0;

};

} // namespace xmipp4

