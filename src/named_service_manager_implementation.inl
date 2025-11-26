// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "named_service_manager_implementation.hpp"

namespace xmipp4
{

template <typename T>
inline
bool named_service_manager_implementation<T>::register_backend(
	std::unique_ptr<backend_type> backend
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

template <typename T>
inline
void named_service_manager_implementation<T>::enumerate_backends(
	std::vector<std::string> &names
) const
{
	names.clear();
	names.reserve(m_backends.size());
	for(const auto &item : m_backends)
	{
		names.emplace_back(item.first);
	}
}

template <typename T>
inline
typename named_service_manager_implementation<T>::backend_type* 
named_service_manager_implementation<T>::get_backend(
	const std::string &name
) const
{
	const auto ite = m_backends.find(name);
	if (ite == m_backends.end())
	{
		return nullptr;
	}

	return ite->second.get();
}

template <typename T>
inline
const typename named_service_manager_implementation<T>::backend_map& 
named_service_manager_implementation<T>::get_backend_map() const noexcept
{
	return m_backends;
}

} // namespace xmipp4
