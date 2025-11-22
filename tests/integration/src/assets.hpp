// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/platform/operating_system.h>
#include <xmipp4/core/platform/compiler.h>

#include <string>

namespace xmipp4
{

inline std::string get_asset_root()
{
	return "assets";
}

inline std::string get_text_file_path()
{
	#if XMIPP4_WINDOWS
		return get_asset_root() + "\\" + "lorem_ipsum.txt";
	#elif XMIPP4_APPLE || XMIPP4_LINUX
		return get_asset_root() + "/" + "lorem_ipsum.txt";
	#else
		#error "Unknown platform"
	#endif
}

inline std::string get_test_plugin_directory()
{
	#if XMIPP4_WINDOWS
		return get_asset_root() + "\\plugins";
	#elif XMIPP4_APPLE || XMIPP4_LINUX
		return get_asset_root() + "/plugins";
	#else
		#error "Unknown platform"
	#endif
}

inline std::string get_mock_plugin_path(const std::string &name)
{

	#if XMIPP4_WINDOWS
		#if XMIPP4_MINGW
			return get_asset_root() + "\\plugins\\lib" + name + ".dll";
		#else
			return get_asset_root() + "\\plugins\\" + name + ".dll";
		#endif
	#elif XMIPP4_APPLE || XMIPP4_LINUX
		return get_asset_root() + "/plugins/lib" + name + ".so";
	#else
		#error "Unknown platform"
	#endif
}

} // namespace xmipp4
