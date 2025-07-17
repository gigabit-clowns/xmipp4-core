// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "communicator_backend.hpp"
#include "../backend_manager.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace communication
{

class communicator;

/**
 * @brief Centralizes all known implementations of the communicator_backend
 * interface.
 * 
 */
class communicator_manager final
    : public basic_backend_manager<communicator_backend>
{
public:
    XMIPP4_CORE_API communicator_manager();
    communicator_manager(const communicator_manager &other) = delete;
    XMIPP4_CORE_API communicator_manager(communicator_manager &&other) noexcept;
    XMIPP4_CORE_API ~communicator_manager() override;

    communicator_manager& 
    operator=(const communicator_manager &other) = delete;
    XMIPP4_CORE_API 
    communicator_manager& operator=(communicator_manager &&other) noexcept;

    XMIPP4_CORE_API
    void register_builtin_backends() override;
    
    /**
     * @brief Find the most suitable backend
     *
     * The most suitable backend is an available backend with the highest
     * priority.
     *  
     * @return const communicator_backend* The backend. Nullptr if none is available.
     */
    XMIPP4_CORE_API
    communicator_backend* get_preferred_backend() const;

    /**
     * @brief Get the world communicator of a backend.
     * 
     * The world communicator connects all known peers together.
     * 
     * @param name The name of the backend.
     * @return std::shared_ptr<communicator> Reference to the world
     * communicator.
     */
    XMIPP4_CORE_API
    std::shared_ptr<communicator> 
    create_world_communicator(const std::string &name) const;

    /**
     * @brief Get the world communicator from the preferred backend.
     * 
     * @return std::shared_ptr<communicator> Reference to the world
     * communicator.
     * @see get_preferred_backend
     */
    XMIPP4_CORE_API
    std::shared_ptr<communicator> create_preferred_world_communicator() const;

}; 

} // namespace communication
} // namespace xmipp4
