// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/communication/host_communicator_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include "../find_most_suitable_backend.hpp"
#include "../named_service_manager_implementation.hpp"
#include "dummy/dummy_host_communicator_backend.hpp"

#include <unordered_map>

namespace xmipp4
{
namespace communication
{

class host_communicator_manager::implementation
	: public named_service_manager_implementation<host_communicator_backend>
{
public:
	std::shared_ptr<host_communicator> create_world_communicator() const
	{
		const auto &backends = get_backend_map();
		const auto backend = find_most_suitable_backend(
			backends.cbegin(), 
			backends.cend(),
			[] (const auto &item)
			{
				XMIPP4_ASSERT(item.second);
				return item.second->get_suitability();
			}
		);

		if (backend == backends.cend())
		{
			throw invalid_operation_error(
				"There is no available host_communicator_backend"
			);
		}

		return backend->second->create_world_communicator();
	}
};



host_communicator_manager::host_communicator_manager() = default;

host_communicator_manager::~host_communicator_manager() = default;

void host_communicator_manager::register_builtin_backends()
{
	dummy_host_communicator_backend::register_at(*this);
}

void host_communicator_manager::enumerate_backends(
	std::vector<std::string> &names
) const
{
	names.clear();
	if (m_implementation)
	{
		m_implementation->enumerate_backends(names);
	}
}

host_communicator_backend* 
host_communicator_manager::get_backend(const std::string &name) const
{
	if (!m_implementation)
	{
		return nullptr;
	}

	return m_implementation->get_backend(name);
}

bool host_communicator_manager::register_backend(
	std::unique_ptr<host_communicator_backend> backend
)
{
	create_implementation_if_null();
	return m_implementation->register_backend(std::move(backend));
}

std::shared_ptr<host_communicator> 
host_communicator_manager::create_world_communicator() const
{
	if (!m_implementation)
	{
		return implementation().create_world_communicator(); // Throws
	}

	return m_implementation->create_world_communicator();
}

void host_communicator_manager::create_implementation_if_null()
{
	if (!m_implementation)
	{
		m_implementation = std::make_unique<implementation>();
	}
}

} // namespace communication
} // namespace xmipp4
