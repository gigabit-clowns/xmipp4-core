// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file stringfy.h
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides macro for stringfying literals.
 * @date 2024-10-25
 * 
 */

/**
 * @brief Stringfy the literal x
 * 
 */
#define XMIPP4_STRINGFY(x) #x

/**
 * @brief Stringfy the value of x
 * 
 */
#define XMIPP4_STRINGFY_VALUE(x) XMIPP4_STRINGFY(x)
