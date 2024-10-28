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
 * @file assets.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Functions to get the path to test assets
 * @date 2024-10-28
 * 
 */

#include <xmipp4/core/platform/operating_system.h>

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

inline std::string get_mock_plugin_path(const std::string &name)
{

    #if XMIPP4_WINDOWS
        return get_asset_root() + "\\" + name + ".dll";
    #elif XMIPP4_APPLE || XMIPP4_LINUX
        return get_asset_root() + "/lib" + name + ".so";
    #else
        #error "Unknown platform"
    #endif
}

} // namespace xmipp4
