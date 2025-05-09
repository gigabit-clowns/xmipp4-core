#pragma once

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
 * @file settings.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines settings class
 * @date 2025-05-09
 * 
 */

#include <string>
#include <unordered_map>

#include "setting_source.hpp"

namespace xmipp4
{

/**
 * @brief Class holding a single setting value.
 * 
 */
class setting_value
{
public:
    /**
     * @brief Construct a setting_value in its unset state.
     * 
     */
    setting_value();

    /**
     * @brief Construct a setting_value from its value and source.
     * 
     * @tparam Str String type.
     * @param value Value contained by the object.
     * @param source Source of the value.
     */
    template <typename Str>
    explicit setting_value(Str &&value, 
                           setting_source source = setting_source::preset);
    setting_value(const setting_value &other) = default;
    setting_value(setting_value &&other) = default;
    ~setting_value() = default;

    setting_value& operator=(const setting_value &other) = default;
    setting_value& operator=(setting_value &&other) = default;
    
    /**
     * @brief Get the source of the value.
     * 
     * @return setting_source The source.
     */
    setting_source get_source() const noexcept;

    /**
     * @brief Get the value held by this object..
     * 
     * @return const std::string& 
     */
    const std::string& get_value() const noexcept;
    
    /**
     * @brief Update the value held by this object.
     * 
     * The value held by this object is only written when the 
     * source evaluates to greater than the current source.
     * 
     * @tparam Str String type
     * @param value Value to be set.
     * @param source The source of the value.
     * @return true Value was updated.
     * @return false Value was not updated.
     */
    template <typename Str>
    bool update(Str &&value, setting_source source);
    
    /**
     * @brief Update the value held by this object from another setting
     * value.
     * 
     * The value held by this object is only written when the 
     * source evaluates to greater than the current source.
     * 
     * @param other The setting object from which parameters are obtained.
     * @return true Value was updated.
     * @return false Value was not updated.
     */
    bool update(const setting_value &other);

private:
    setting_source m_source;
    std::string m_value;

};

} // namespace xmipp4

#include "setting_value.inl"
