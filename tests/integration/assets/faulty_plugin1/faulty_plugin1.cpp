// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file faulty_plugin1.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief An invalid plugin which declares the hook
 * but returns null
 * @date 2024-10-28
 * 
 */

#include <xmipp4/core/plugin.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#if defined(XMIPP4_FAULTY_PLUGIN1_EXPORTING)
    #define XMIPP4_FAULTY_PLUGIN1_API XMIPP4_EXPORT
#else
    #define XMIPP4_FAULTY_PLUGIN1_API XMIPP4_IMPORT
#endif

extern "C"
{
XMIPP4_FAULTY_PLUGIN1_API const xmipp4::plugin* xmipp4_get_plugin() 
{
    return NULL;
}
}
