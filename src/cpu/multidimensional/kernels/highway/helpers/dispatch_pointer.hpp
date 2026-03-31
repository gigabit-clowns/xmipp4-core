// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <hwy/highway.h>

#if XMIPP4_HWY_DYNAMIC_DISPATCH
	#define XMIPP4_HWY_FUNCTION_POINTER(Func) \
		HWY_DYNAMIC_POINTER(Func)

	#define XMIPP4_HWY_EXPORT(Func) \
		HWY_EXPORT(Func)

#else
	#define XMIPP4_HWY_FUNCTION_POINTER(Func) \
		(&HWY_STATIC_DISPATCH(Func))

	#define XMIPP4_HWY_EXPORT(Func)

#endif
