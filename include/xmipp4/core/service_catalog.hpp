// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "service_manager.hpp"
#include "platform/dynamic_shared_object.h"

#include <memory>
#include <typeindex>
#include <type_traits>

namespace xmipp4
{

/**
 * @brief Stores a single instance of each type service_manager.
 *
 * The interface catalog allows to centralize interface managers.
 *
 * @see service_manager
 *
 */
class service_catalog
{
public:
	/**
	 * @brief Construct a new interface catalog object.
	 *
	 * @param register_builtin_backends This parameters controls whether the
	 * interface catalog should load the backends bundled with the
	 * core.
	 *
	 */
	XMIPP4_CORE_API
	explicit service_catalog(bool register_builtin_backends = true);
	service_catalog(const service_catalog &other) = delete;
	XMIPP4_CORE_API service_catalog(service_catalog &&other) noexcept;
	XMIPP4_CORE_API ~service_catalog();

	service_catalog &operator=(const service_catalog &other) = delete;
	XMIPP4_CORE_API
	service_catalog &operator=(service_catalog &&other) noexcept;

	/**
	 * @brief Get a concrete interface manager.
	 *
	 * When the method is called for the first time in a given instance with
	 * a given type, it constructs a new concrete interface manager of that
	 * type. If called previously, it returns the same instance.
	 *
	 * @tparam T interface manager. Must be child class of service_manager.
	 * @return T& the requested interface manager.
	 */
	template <typename T>
	typename std::enable_if<std::is_convertible<T *, service_manager *>::value, T &>::type
	get_service_manager();

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	XMIPP4_CORE_API
	service_manager *get_service_manager(std::type_index type);

	XMIPP4_CORE_API
	void create_service_manager(
		std::type_index type,
		std::unique_ptr<service_manager> manager
	);
};

} // namespace xmipp4

#include "service_catalog.inl"
