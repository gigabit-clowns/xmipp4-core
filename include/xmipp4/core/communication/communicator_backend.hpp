// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file communicator_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines communicator_backend interface
 * @date 2024-10-24
 * 
 */

#include "../backend.hpp"

#include "../platform/dynamic_shared_object.h"

#include <string>
#include <memory>

namespace xmipp4 
{
namespace communication
{

class communicator;

class XMIPP4_CORE_API communicator_backend
    : public backend
{
public:
    communicator_backend() = default;
    communicator_backend(const communicator_backend &other) = default;
    communicator_backend(communicator_backend &&other) = default;
    virtual ~communicator_backend() = default;

    communicator_backend& operator=(const communicator_backend &other) = default;
    communicator_backend& operator=(communicator_backend &&other) = default;

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
