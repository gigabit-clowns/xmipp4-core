// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace multidimensional
{

class operation_id;
class operation_schema;

class operation
{
public:
	XMIPP4_CORE_API
	operation() noexcept;
	operation(const operation &other) = delete;
	operation(operation &&other) = delete;
	XMIPP4_CORE_API
    virtual ~operation();

	operation& operator=(const operation &other) = delete;
	operation& operator=(operation &&other) = delete;

	virtual const operation_id& get_operation_id() const noexcept = 0;

	virtual const operation_schema& get_operation_schema() const noexcept = 0;
};

} // namespace multidimensional
} // namespace xmipp4
