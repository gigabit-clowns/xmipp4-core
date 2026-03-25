// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/service_catalog.hpp>

#include <xmipp4/core/plugin.hpp>

#include "plugin_loader.hpp"

#include <vector>
#include <cstdlib>
#include <unordered_map>

namespace xmipp4
{

class service_catalog::implementation
{
public:
	explicit implementation(bool register_builtin_backends)
		: m_register_builtin_backends(register_builtin_backends)
	{
	}

	~implementation() = default;

	service_manager* get_service_manager(std::type_index type)
	{
		service_manager* result = nullptr;

		const auto ite = m_interfaces.find(type);
		if(ite != m_interfaces.end())
		{
			result = ite->second.get();
		}

		return result;
	}

	void create_service_manager(
		std::type_index type,
		std::unique_ptr<service_manager> manager
	)
	{
		if (m_register_builtin_backends)
		{
			manager->register_builtin_backends();
		}

		m_interfaces.emplace(
			type, std::move(manager)
		);
	}

private:
	using catalog_type = 
		std::unordered_map<std::type_index, std::unique_ptr<service_manager>>;

	bool m_register_builtin_backends;
	catalog_type m_interfaces;
};

service_catalog::service_catalog(bool register_builtin_backends)
	: m_implementation(std::make_unique<implementation>(register_builtin_backends))
{
}

service_catalog::service_catalog(service_catalog&& other) noexcept = default;

service_catalog::~service_catalog() = default;

service_catalog& 
service_catalog::operator=(service_catalog&& other) noexcept = default;

service_manager* 
service_catalog::get_service_manager(std::type_index type)
{
	return m_implementation->get_service_manager(type);
}
		
void service_catalog::create_service_manager(
	std::type_index type,
	std::unique_ptr<service_manager> manager
)
{
	m_implementation->create_service_manager(type, std::move(manager));
}

} // namespace xmipp4
