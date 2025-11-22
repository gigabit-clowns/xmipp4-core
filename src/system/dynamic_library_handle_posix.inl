// SPDX-License-Identifier: GPL-3.0-only

#include "dynamic_library_handle.hpp"

#include <xmipp4/core/platform/constexpr.hpp>

#include <dlfcn.h>

#include <system_error>

namespace xmipp4
{
namespace system
{

inline void* dynamic_library_open(const char* filename)
{
	XMIPP4_CONST_CONSTEXPR int flags = RTLD_LAZY;
	auto *const result = ::dlopen(filename, flags);
	if (result == NULL)
	{
		throw std::system_error(std::error_code(), dlerror());
	}
	return result;
}

inline void dynamic_library_close(void* handle) noexcept
{
	::dlclose(handle);
}

inline void* dynamic_library_get_symbol(void* handle, const char* name) noexcept
{
	return ::dlsym(handle, name);
}

inline std::string dynamic_library_symbol_filename_lookup(const void* symbol)
{
	std::string result;

	Dl_info info;
	if (dladdr(symbol, &info))
	{
		result = std::string(info.dli_fname);
	}

	return result;
}

} // namespace system
} // namespace xmipp4
