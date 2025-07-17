// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
