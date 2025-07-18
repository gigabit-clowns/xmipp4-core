// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/dynamic_shared_object.h"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace xmipp4
{

/**
 * @brief Abstract class defining the interface of a backend manager.
 * 
 * An backend manager keeps track of all known implementations of a given
 * interface and provides utilities to access the most relevant implementation
 * for a given context. This is a base class for collecting them on an
 * interface_catalog.
 * 
 * @see interface_catalog
 * 
 */
class XMIPP4_CORE_API backend_manager
{
public:
    backend_manager() = default;
    backend_manager(const backend_manager& other) = default;
    backend_manager(backend_manager&& other) = default;
    virtual ~backend_manager() = default;

    backend_manager& operator=(const backend_manager& other) = default;
    backend_manager& operator=(backend_manager&& other) = default;

    /**
     * @brief Register backends bundled with the core.access_flag_bits
     * 
     * This usually loads fallback implementations that are always 
     * available although they may not be the most efficient or performant.
     * The loaded backend(s) depend on the implementation of the specific
     * interface.
     * 
     */
    virtual void register_builtin_backends() { /*By default this is a no-op*/ };

};

template <typename B>
class basic_backend_manager
    : public backend_manager
{
public:
    /**
     * @brief Alias of the backend template parameter provided
     * when instantiating this class.
     * 
     */
    using backend_type = B;

    basic_backend_manager() = default;
    basic_backend_manager(const basic_backend_manager &other) = delete;
    basic_backend_manager(basic_backend_manager &&other) = default;
    ~basic_backend_manager() override = default;

    basic_backend_manager& operator=(const basic_backend_manager &other) = delete;
    basic_backend_manager& operator=(basic_backend_manager &&other) = default;

    /**
     * @brief Register a new implementation.
     * 
     * @param backend The backend to be registered.
     * @return true Successfully registered.
     * @return false Failed to register. I.e. an homonym implementation 
     * already exists or nullptr was provided.
     * 
     */
    bool register_backend(std::unique_ptr<backend_type> backend);

    /**
     * @brief Enumerate all available backends.
     * 
     * @param backends Output list with the names of the backends.
     * @note The list is cleared before appending the backend names.
     */
    void enumerate_backends(std::vector<std::string> &backends) const;

    /**
     * @brief Find a backend by its name.
     * 
     * @param name The name of the backend.
     * @return backend_type* The backend. Nullptr if not found.
     */
    backend_type* get_backend(const std::string &name) const;

    /**
     * @brief Get all the backends known by this manager.
     * 
     * @param backends Output parameter where backends are written.
     * 
     * @note Caller must not manage the lifetime of the returned objects.
     * 
     */
    void get_backends(std::vector<backend_type*> &backends) const;

    /**
     * @brief Get all available backends known by this manager.
     * 
     * @param backends Output parameter where backends are written.
     * 
     * @note Caller must not manage the lifetime of the returned objects.
     * 
     */
    void get_available_backends(std::vector<backend_type*> &backends) const;

private:
    std::unordered_map<std::string, std::unique_ptr<backend_type>> m_backends;

};

} // namespace xmipp4

#include "backend_manager.inl"
