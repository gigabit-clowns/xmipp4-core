// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/communication/host_communicator_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include "../find_most_suitable_backend.hpp"
#include "dummy/dummy_host_communicator_backend.hpp"

#include <unordered_map>

namespace xmipp4
{
namespace communication
{

class host_communicator_manager::implementation
{
public:
	bool register_backend(
		std::unique_ptr<host_communicator_backend> backend
	)
	{
		if (!backend)
		{
			return false;
		}

		auto name = backend->get_name();
		bool inserted;
		std::tie(std::ignore, inserted) = m_backends.emplace(
			std::move(name),
			std::move(backend)
		);

		return inserted;
	}

	std::shared_ptr<host_communicator> create_world_communicator() const
	{
		const auto backend = find_most_suitable_backend(
			m_backends.cbegin(), m_backends.cend(),
			[] (const auto &item)
			{
				XMIPP4_ASSERT(item.second);
				return item.second->get_suitability();
			}
		);

		if (backend == m_backends.cend())
		{
			throw invalid_operation_error(
				"There is no available host_communicator_backend"
			);
		}

		return backend->second->create_world_communicator();
	}

private:
	using backend_map = std::unordered_map<
		std::string, 
		std::unique_ptr<host_communicator_backend>
	>;

	backend_map m_backends;

};



host_communicator_manager::host_communicator_manager() = default;

host_communicator_manager::~host_communicator_manager() = default;

void host_communicator_manager::register_builtin_backends()
{
	dummy_host_communicator_backend::register_at(*this);
}

void host_communicator_manager::enumerate_backends(
	std::vector<std::string> &backends
) const
{
	// TODO
}

host_communicator_backend* 
host_communicator_manager::get_backend(const std::string &name) const
{
	// TODO
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
		throw invalid_operation_error(
			"No backends were registered."
		);
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
