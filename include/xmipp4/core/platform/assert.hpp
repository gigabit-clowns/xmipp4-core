// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file assert.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Custom declaration of assert
 * @date 2023-08-08
 * 
 */

#include <cassert>

/**
 * @brief Custom assert that allows optimization 
 * in release builds
 * 
 */
#define XMIPP4_ASSERT(expr) assert(expr)
