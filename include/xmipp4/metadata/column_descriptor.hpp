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

#include <string>
#include <typeindex>
#include <type_traits>

namespace xmipp4
{
namespace metadata
{

class column_descriptor
{
public:
    column_descriptor() = default;
    column_descriptor(const column_descriptor& other) = default;
    column_descriptor(column_descriptor&& other) = default;
    ~column_descriptor() = default;

    column_descriptor& operator=(const column_descriptor& other) = default;
    column_descriptor& operator=(column_descriptor&& other) = default;

    void set_label(const std::string& name);
    void set_label(std::string&& name) noexcept;
    const std::string& get_label() const noexcept;

    void set_category(const std::string& category);
    void set_category(std::string&& category) noexcept;
    const std::string& get_category() const noexcept;

    void set_type(std::type_index type) noexcept;
    std::type_index get_type() const noexcept;

private:
    std::string m_label;
    std::string m_category;
    std::type_index m_type;

};

} // namespace metadata
} // namespace xmipp4

#include "column_descriptor.inl"
