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
 * @file logger.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines logging utilities.
 * @date 2024-11-21
 * 
 */

#define XMIPP4_LOG_LEVEL_TRACE SPDLOG_LEVEL_TRACE
#define XMIPP4_LOG_LEVEL_DEBUG SPDLOG_LEVEL_DEBUG
#define XMIPP4_LOG_LEVEL_INFO SPDLOG_LEVEL_INFO
#define XMIPP4_LOG_LEVEL_WARN SPDLOG_LEVEL_WARN
#define XMIPP4_LOG_LEVEL_ERROR SPDLOG_LEVEL_ERROR
#define XMIPP4_LOG_LEVEL_CRITICAL SPDLOG_LEVEL_CRITICAL

#ifdef XMIPP4_LOG_LEVEL
    #define SPDLOG_ACTIVE_LEVEL XMIPP4_LOG_LEVEL
#endif

#include <spdlog/spdlog.h>

#define XMIPP4_LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define XMIPP4_LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define XMIPP4_LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define XMIPP4_LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define XMIPP4_LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define XMIPP4_LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
