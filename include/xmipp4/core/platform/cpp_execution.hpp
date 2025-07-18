// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "cpp_features.hpp"

#if XMIPP4_HAS_LIB_EXECUTION
    #include <execution>
    #define XMIPP4_SEQ std::execution::seq,
    #define XMIPP4_PAR std::execution::par,
    #define XMIPP4_PAR_UNSEQ std::execution::par_unseq,
#else
    #define XMIPP4_SEQ
    #define XMIPP4_PAR
    #define XMIPP4_PAR_UNSEQ
#endif