// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "stringfy.h"

/**
 * @def XMIPP4_PRAGMA
 * @brief Provide a directive to the compiler.
 * 
 */
#define XMIPP4_PRAGMA(x) _Pragma(XMIPP4_STRINGFY(x))
