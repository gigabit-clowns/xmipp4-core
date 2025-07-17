// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file pragma.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides a cross-platform macro for compiler pragma directives.
 * @date 2024-10-25
 * 
 */

#include "stringfy.h"

/**
 * @def XMIPP4_PRAGMA
 * @brief Provide a directive to the compiler.
 * 
 */
#define XMIPP4_PRAGMA(x) _Pragma(XMIPP4_STRINGFY(x))
