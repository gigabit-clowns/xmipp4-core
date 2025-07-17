// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file openmp.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides a cross-platform macro for accessing OpenMP functionality
 * when supported
 * @date 2024-10-25
 * 
 */

#include "pragma.h"

/**
 * @def XMIPP4_OMP
 * @brief Provide a OpenMP directive to the compiler.
 * 
 */
#if _OPENMP
    #define XMIPP4_OMP(x) XMIPP4_PRAGMA(omp x)
#else
    #define XMIPP4_OMP(x)
#endif
