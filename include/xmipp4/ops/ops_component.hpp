// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4
{
namespace ops
{

/**
 * @brief Component tag for the generic operation catalog.
 *
 * Every operation declaration names the component it belongs to, which
 * qualifies it in diagnostics and keeps the names of different components
 * apart. The generic catalog uses this one; the CryoEM catalog and each
 * plugin provide their own.
 *
 * @see XMIPP4_DECLARE_OPERATION
 */
struct ops_component
{
	static const char* get_component() noexcept
	{
		return "xmipp4.ops";
	}
};

} // namespace ops
} // namespace xmipp4
