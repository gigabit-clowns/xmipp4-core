// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file mock_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Mock for backend interface.
 * @date 2025-03-27
 */

#include <xmipp4/core/backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{

class mock_backend final
    : public backend
{
public:
    MAKE_MOCK0(get_name, std::string(), const noexcept);
    MAKE_MOCK0(get_version, version(), const noexcept);
    MAKE_MOCK0(is_available, bool(), const noexcept);
    MAKE_MOCK0(get_priority, backend_priority(), const noexcept);

};

} // namespace xmipp4
