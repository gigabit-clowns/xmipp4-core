// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "cpp_features.hpp"

#if XMIPP4_HAS_CONDITIONAL_EXPLICIT
    #define XMIPP4_NO_EXPLICIT explicit(false)
#else
    #define XMIPP4_NO_EXPLICIT
#endif
