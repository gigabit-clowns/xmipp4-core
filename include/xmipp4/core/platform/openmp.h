#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

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
