// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cassert>

/**
 * @brief Custom assert that allows optimization 
 * in release builds
 * 
 */
#define XMIPP4_ASSERT(expr) assert(expr)
