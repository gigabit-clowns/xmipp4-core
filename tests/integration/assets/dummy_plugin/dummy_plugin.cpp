// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file dummy_plugin.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Exports a dummy plugin
 * @date 2024-10-28
 * 
 */

#include <xmipp4/core/plugin.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#if defined(XMIPP4_DUMMY_PLUGIN_EXPORTING)
    #define XMIPP4_DUMMY_PLUGIN_API XMIPP4_EXPORT
#else
    #define XMIPP4_DUMMY_PLUGIN_API XMIPP4_IMPORT
#endif

namespace xmipp4
{

static const std::string name = "dummy-plugin";

class dummy_plugin final
    : public xmipp4::plugin
{
    const std::string& get_name() const noexcept final
    {
        return name;
    }

    version get_version() const noexcept final
    {
        return version(1, 2, 3);
    }

    void register_at(interface_catalog&) const
    {
        // NO-OP
    }
};

} // namespace xmipp4

static const xmipp4::dummy_plugin instance;

extern "C"
{
XMIPP4_DUMMY_PLUGIN_API const xmipp4::plugin* xmipp4_get_plugin() 
{
    return &instance;
}
}
