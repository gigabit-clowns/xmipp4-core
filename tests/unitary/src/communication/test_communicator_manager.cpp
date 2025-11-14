// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>

#include "mock/mock_communicator_backend.hpp"

using namespace xmipp4;
using namespace xmipp4::communication;

// TODO
