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
 * @file slice.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of slice.hpp
 * @date 2023-08-13
 * 
 */

#include "slice.hpp"

namespace xmipp4 
{

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Step, Stop>::slice(start_type start, 
                                step_type step, 
                                stop_type stop ) noexcept
    : m_start(start)
    , m_step(step)
    , m_stop(stop)
{
}

template <typename Start, typename Step, typename Stop>
template <typename Start2, typename Step2, typename Stop2>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Step, Stop>::slice(const slice<Start2, Step2, Stop2>& other) noexcept
    : slice(other.get_start(), other.get_step(), other.get_stop())
{
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR 
bool slice<Start, Step, Stop>::operator==(const slice& other) const noexcept
{
    return m_start == other.m_start && 
           m_step == other.m_step && 
           m_stop == other.m_stop;
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR 
bool slice<Start, Step, Stop>::operator!=(const slice& other) const noexcept
{
    return !(*this == other);
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Step, Stop>::set_start(start_type start) noexcept
{
    m_start = start;
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Step, Stop>::start_type& 
slice<Start, Step, Stop>::get_start() const noexcept
{
    return m_start;
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Step, Stop>::set_step(step_type step) noexcept
{
    m_step = step;
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Step, Stop>::step_type&
slice<Start, Step, Stop>::get_step() const noexcept
{
    return m_step;
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Step, Stop>::set_stop(stop_type stop) noexcept
{
    m_stop = stop;
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Step, Stop>::stop_type& 
slice<Start, Step, Stop>::get_stop() const noexcept
{
    return m_stop;
}



template <typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<begin_tag, adjacent_tag, Stop> 
make_slice(Stop stop) noexcept
{
    return slice<begin_tag, adjacent_tag, Stop>(
        begin, 
        adjacent,  
        stop
    );
}

template <typename Start, typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<Start, adjacent_tag, Stop> 
make_slice(Start start, Stop stop) noexcept
{
    return slice<Start, adjacent_tag, Stop>(
        start, 
        adjacent, 
        stop
    );
}

template <typename Start, typename Step, typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<Start, Step, Stop> 
make_slice(Start start, Step step, Stop stop) noexcept
{
    return slice<Start, Step, Stop>(
        start, 
        step, 
        stop
    );
}



template <typename Start, typename Step, typename Stop>
inline std::ostream& operator<<(std::ostream& os, const slice<Start, Step, Stop> &s)
{
    return os << "slice(" << s.get_start()
              << ", " << s.get_step()
              << ", " << s.get_stop() << ")";
}

inline std::ostream& operator<<(std::ostream& os, begin_tag)
{
    return os << "begin";
}

inline std::ostream& operator<<(std::ostream& os, end_tag)
{
    return os << "end";
}

inline std::ostream& operator<<(std::ostream& os, adjacent_tag)
{
    return os << "adjacent";
}

} // namespace xmipp4