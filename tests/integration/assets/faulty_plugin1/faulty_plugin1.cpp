// SPDX-License-Identifier: GPL-3.0-only

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
