// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file mock_backend_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Mock for backend_manager interface.
 * @date 2025-03-27
 */

#include <xmipp4/core/backend_manager.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{

class mock_backend_manager final
    : public backend_manager
{
public:
    MAKE_MOCK0(register_builtin_backends, void(), override);

};

} // namespace xmipp4
