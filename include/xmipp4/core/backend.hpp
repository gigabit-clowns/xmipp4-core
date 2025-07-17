// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines basic backend interface
 * @date 2024-10-29
 * 
 */

#include "version.hpp"
#include "backend_priority.hpp"
#include "platform/dynamic_shared_object.h"

#include <string>

namespace xmipp4
{

/**
 * @brief Abstract class representing a backend.
 * 
 * A concrete instance of this class may be used to create objects and query
 * an specific backend.
 * 
 */
class XMIPP4_CORE_API backend
{
public:
    backend() = default;
    backend(const backend& other) = default;
    backend(backend&& other) = default;
    virtual ~backend() = default;

    backend& operator=(const backend& other) = default;
    backend& operator=(backend&& other) = default;

    /**
     * @brief Get the name of the backend.
     * 
     * @return std::string The name.
     */
    virtual std::string get_name() const noexcept = 0;

    /**
     * @brief Get the version of the backend.
     * 
     * @return version The version.
     */
    virtual version get_version() const noexcept = 0;

    /**
     * @brief Check if a backend is usable.
     * 
     * @return true Backend is usable.
     * @return false Backend is not usable.
     */
    virtual bool is_available() const noexcept = 0;

    /**
     * @brief Get the priority of this backend.
     * 
     * This attribute can be used to break a tie when multiple backends
     * can be used. In general prefer using backend_priority::normal unless 
     * there is a strong reason.
     * 
     * @return backend_priority The priority of this backend.
     */
    virtual backend_priority get_priority() const noexcept = 0;

};

} // namespace xmipp4
