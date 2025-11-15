// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../backend_priority.hpp"
#include "../version.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <memory>

namespace xmipp4 
{
namespace communication
{

class communicator;

class XMIPP4_CORE_API communicator_backend
{
public:
    communicator_backend() = default;
    communicator_backend(const communicator_backend &other) = default;
    communicator_backend(communicator_backend &&other) = default;
    virtual ~communicator_backend() = default;

    communicator_backend& operator=(const communicator_backend &other) = default;
    communicator_backend& operator=(communicator_backend &&other) = default;

    /**
     * @brief Get the name of the backend.
     * 
     * @return std::string The name
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Get the backend version.
     * 
     * @return version The version.
     */
    virtual version get_version() const = 0;

    /**
     * @brief Get the suitability of the backend.
     * 
     * @return backend_priority The suitability.
     */
    virtual backend_priority get_suitability() const = 0;

    /**
     * @brief Get the world communicator.
     * 
     * The world communicator connects all known peers together.
     * 
     * @return std::shared_ptr<communicator> Reference to the world
     * communicator.
     */
    virtual std::shared_ptr<communicator> create_world_communicator() const = 0;

}; 

} // namespace communication
} // namespace xmipp4
