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

namespace xmipp4
{
namespace metadata
{

class labeled_base
{
public:
    labeled_base() = default;
    explicit labeled_base(const std::string& label);
    explicit labeled_base(std::string&& label) noexcept;
    labeled_base(const labeled_base& other) = default;
    labeled_base(labeled_base&& other) = default;
    virtual ~labeled_base() = default;

    labeled_base& operator=(const labeled_base& other) = default;
    labeled_base& operator=(labeled_base&& other) = default;

    void set_label(const std::string& label);
    void set_label(std::string&& label) noexcept;
    const std::string& get_label() const noexcept;

private:
    std::string m_label;

};

template<typename ForwardIt, typename Map>
ForwardIt compute_label_to_index_map(ForwardIt first, ForwardIt last, Map& map);

} // namespace metadata
} // namespace xmipp4
