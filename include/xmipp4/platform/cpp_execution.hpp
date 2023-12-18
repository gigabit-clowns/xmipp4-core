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
 * @file cpp_execution.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Macro definitions for using C++ algorithm parallelization
 * @date 2023-11-14
 * 
 */

#include "cpp_version.hpp"

#if XMIPP4_HAS_CPP17
    #include <execution>
    #define XMIPP4_SEQ std::execution::seq,
    #define XMIPP4_PAR std::execution::par,
    #define XMIPP4_PAR_UNSEQ std::execution::par_unseq,
#else
    #define XMIPP4_SEQ
    #define XMIPP4_PAR
    #define XMIPP4_PAR_UNSEQ
#endif