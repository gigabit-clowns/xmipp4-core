// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/communication/device_communicator_manager.hpp>

#include <xmipp4/core/communication/device_transaction.hpp>
#include <xmipp4/core/communication/host_communicator.hpp>
#include <xmipp4/core/communication/host_operation.hpp>
#include <xmipp4/core/communication/host_duplex_region.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include "../find_most_suitable_backend.hpp"
#include "../named_service_manager_implementation.hpp"

#include <array>
#include <unordered_map>
#include <cstring>

namespace xmipp4
{
namespace communication
{

class device_communicator_manager::implementation
	: public named_service_manager_implementation<device_communicator_backend>
{
public:
	std::shared_ptr<device_transaction> create_world_communicators(
		const std::shared_ptr<host_communicator> &node_communicator,
		span<hardware::device*> devices,
		span<std::shared_ptr<device_communicator>> out
	) const
	{
		device_communicator_backend *backend;
		if (node_communicator)
		{
			backend = negotiate_backend(*node_communicator, devices);
		}
		else
		{
			// TODO
		}

		if (!backend)
		{
			throw invalid_operation_error(
				"There is no available device_communicator_backend"
			);
		}

		XMIPP4_ASSERT( devices.size() == out.size() );
		return backend->create_world_communicators(
			node_communicator,
			devices,
			out
		);
	}

private:
	device_communicator_backend* negotiate_backend(
		host_communicator &node_communicator,
		span<hardware::device*> devices
	) const
	{
		const std::size_t root_rank = 0;
		if (node_communicator.get_rank() == root_rank)
		{
			return negotiate_backend_leader(
				node_communicator, 
				devices, 
				root_rank
			);
		}
		else
		{
			return negotiate_backend_follower(
				node_communicator, 
				devices, 
				root_rank
			);
		}

	}

	device_communicator_backend* negotiate_backend_leader(
		host_communicator &node_communicator,
		span<hardware::device*> devices,
		std::size_t root_rank
	) const
	{
		const auto &backends = get_backend_map();
		
		// Set up exchange areas
		std::array<char, 1024> name_buffer;
		const auto name_broadcast = node_communicator.create_broadcast(
			host_duplex_region(name_buffer.data(), name_buffer.size()),
			root_rank
		);

		int count;
		const auto count_reduce = node_communicator.create_reduce(
			host_duplex_region(&count, 1),
			reduction_operation::sum,
			root_rank
		);

		// Evaluate with peers
		std::vector<device_communicator_backend*> supported_backends;
		for (const auto &item : backends)
		{
			XMIPP4_ASSERT(item.second);
			if (!is_backend_available(*(item.second), devices))
			{
				continue;
			}

			const auto &name = item.first;
			name_buffer = {};
			name.copy(name_buffer.data(), name_buffer.size()-1);
			name_broadcast->execute();

			count = 1;
			count_reduce->execute();

			if (count == node_communicator.get_size())
			{
				supported_backends.emplace_back(item.second.get());
			}
		}
		
		// Signal end of the loop
		name_buffer = {};
		name_broadcast->execute();

		// Select best
		auto selected_backend = find_most_suitable_backend(
			supported_backends.cbegin(), 
			supported_backends.cend(),
			[devices] (const auto &item)
			{
				XMIPP4_ASSERT(item);
				return item->get_suitability(devices);
			}
		);

		// Share the result with peers
		name_buffer = {};
		if (selected_backend != supported_backends.cend())
		{
			const auto name = (*selected_backend)->get_name();
			name.copy(name_buffer.data(), name_buffer.size()-1);
		}
		name_broadcast->execute();

		return *selected_backend;
	}

	device_communicator_backend* negotiate_backend_follower(
		host_communicator &node_communicator,
		span<hardware::device*> devices,
		std::size_t root_rank
	) const
	{
		// Set up exchange areas
		std::array<char, 1024> name_buffer;
		const auto name_broadcast = node_communicator.create_broadcast(
			host_duplex_region(
				static_cast<char*>(nullptr), 
				name_buffer.data(), 
				name_buffer.size()
			),
			root_rank
		);

		int count;
		const auto count_reduce = node_communicator.create_reduce(
			host_duplex_region(&count, static_cast<int*>(nullptr), 1),
			reduction_operation::sum,
			root_rank
		);

		name_broadcast->execute();
		while(name_buffer.front())
		{
			const std::string name(name_buffer.data());

			count = is_backend_available(name, devices) ? 1: 0;
			count_reduce->execute();

			// Receive next
			name_broadcast->execute();
		}

		// Receive the result
		name_broadcast->execute();
		const std::string name(name_buffer.data());

		return get_backend(name);
	}

	bool is_backend_available(
		const std::string &name,
		span<hardware::device*> devices
	) const
	{
		const auto *backend = get_backend(name);
		if (!backend)
		{
			return false;
		}

		return is_backend_available(*backend, devices);
	}
	
	static
	bool is_backend_available(
		const device_communicator_backend &backend,
		span<hardware::device*> devices
	)
	{
		const auto suitability = backend.get_suitability(devices);
		return suitability != backend_priority::unsupported;
	}
};



device_communicator_manager::device_communicator_manager() = default;

device_communicator_manager::~device_communicator_manager() = default;

void device_communicator_manager::register_builtin_backends()
{
	// Currently there are no builtin backends
}

void device_communicator_manager::enumerate_backends(
	std::vector<std::string> &names
) const
{
	names.clear();
	if (m_implementation)
	{
		m_implementation->enumerate_backends(names);
	}
}

device_communicator_backend* 
device_communicator_manager::get_backend(const std::string &name) const
{
	if (!m_implementation)
	{
		return nullptr;
	}

	return m_implementation->get_backend(name);
}

bool device_communicator_manager::register_backend(
	std::unique_ptr<device_communicator_backend> backend
)
{
	create_implementation_if_null();
	return m_implementation->register_backend(std::move(backend));
}

std::shared_ptr<device_transaction> 
device_communicator_manager::create_world_communicators(
	const std::shared_ptr<host_communicator> &node_communicator,
	span<hardware::device*> devices,
	span<std::shared_ptr<device_communicator>> out
) const
{
	if (devices.size() != out.size())
	{
		throw std::invalid_argument(
			"Device array and output array must have matching sizes"
		);
	}

	if (!m_implementation)
	{
		// Throws
		return implementation().create_world_communicators(
			node_communicator,
			devices,
			out
		);
	}

	return m_implementation->create_world_communicators(
		node_communicator,
		devices,
		out
	);
}

void device_communicator_manager::create_implementation_if_null()
{
	if (!m_implementation)
	{
		m_implementation = std::make_unique<implementation>();
	}
}

} // namespace communication
} // namespace xmipp4
