// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/memory/aligned_alloc.hpp>

#include <xmipp4/core/platform/operating_system.h>

#include <cstdlib>

namespace xmipp4
{
namespace memory
{

void* aligned_alloc(std::size_t size, std::size_t alignment) noexcept
{
	#if XMIPP4_WINDOWS
		return _aligned_malloc(size, alignment);
	#elif XMIPP4_POSIX
		void *result;
		
		if(posix_memalign(&result, alignment, size) != 0)
		{
			result = nullptr;
		}

		return result;
	#else
		return std::aligned_alloc(alignment, size);
	#endif
}

void aligned_free(void* ptr) noexcept
{
	#if XMIPP4_WINDOWS
		_aligned_free(ptr);
	#else
		std::free(ptr);
	#endif
}

} // namespace memory
} // namespace xmipp4
