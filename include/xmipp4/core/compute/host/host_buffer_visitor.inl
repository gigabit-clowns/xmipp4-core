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
 * @file host_buffer_visitor.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_buffer_visitor.hpp
 * @date 2024-10-29
 * 
 */

#include "host_buffer_visitor.hpp"

#include "host_buffer.hpp"

#include <type_traits>
#include <utility>
#include <cstdint>
#include <complex>
#include <stdexcept>

namespace xmipp4 
{
namespace compute
{

template <typename T>
inline span<T> make_span(host_buffer &buffer) noexcept
{
    return span<T>(
        static_cast<T*>(buffer.get_data()),
        buffer.get_count()
    );
}

template <typename T>
inline span<const T> make_span(const host_buffer &buffer) noexcept
{
    return span<const T>(
        static_cast<const T*>(buffer.get_data()),
        buffer.get_count()
    );
}



namespace detail
{

template <typename F, typename T>
struct binder
{
    F f; T t;
    template <typename... Args>
    auto operator()(Args&&... args) const
        -> decltype(f(t, std::forward<Args>(args)...))
    {
        return f(t, std::forward<Args>(args)...);
    }
};

template <typename F, typename T>
binder<typename std::decay<F>::type,
       typename std::decay<T>::type> bind_first(F&& f, T&& t)
{
    return { std::forward<F>(f), std::forward<T>(t) };
}
    
template <typename T, typename Func, typename Buffer, typename... Buffers>
inline auto visit_helper(Func &&func, Buffer&& buffer, Buffers&&... rest)
{
    return visit(
        bind_first(std::forward<Func>(func), make_span<T>(buffer)),
        std::forward<Buffers>(rest)...
    );
}

template <typename Func>
auto visit(Func &&func)
{
    return std::forward<Func>(func)();
}

template <typename Func, typename Buffer, typename... Buffers>
auto visit(Func &&func, Buffer&& buffer, Buffers&&... rest)
{
    switch (buffer.get_type())
    {
    case numerical_type::int8:
        return visit_helper<std::int8_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::uint8: 
        return visit_helper<std::uint8_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::int16: 
        return visit_helper<std::int16_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::uint16: 
        return visit_helper<std::uint16_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::int32: 
        return visit_helper<std::int32_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::uint32: 
        return visit_helper<std::uint32_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::int64: 
        return visit_helper<std::int64_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::uint64: 
        return visit_helper<std::uint64_t>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::float32: 
        return visit_helper<float>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::float64: 
        return visit_helper<double>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );
    
    case numerical_type::complex_float32: 
        return visit_helper<std::complex<float>>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    case numerical_type::complex_float64: 
        return visit_helper<std::complex<double>>(
            std::forward<Func>(func), 
            std::forward<Buffer>(buffer),
            std::forward<Buffers>(rest)...
        );

    default:
        throw std::invalid_argument("Cannot convert buffer type to host type" );

    }
}

} // namespace detail


template <typename Func, typename... Buffers>
auto visit(Func &&func, Buffers&&... buffers)
{
    return detail::visit(
        std::forward<Func>(func),
        std::forward<Buffers>(buffers)...
    );
}



namespace detail
{

template <typename T, typename Func, typename... Buffers>
inline auto visit_same_helper(Func &&func, Buffers&&... buffers)
{
    return std::forward<Func>(func)(make_span<T>(buffers)...);
}

template <typename Func, typename... Buffers>
auto visit_same(Func &&func, numerical_type type, Buffers&&... buffers)
{
    switch (type)
    {
    case numerical_type::int8:
        return visit_same_helper<std::int8_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::uint8: 
        return visit_same_helper<std::uint8_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::int16: 
        return visit_same_helper<std::int16_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::uint16: 
        return visit_same_helper<std::uint16_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::int32: 
        return visit_same_helper<std::int32_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::uint32: 
        return visit_same_helper<std::uint32_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::int64: 
        return visit_same_helper<std::int64_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::uint64: 
        return visit_same_helper<std::uint64_t>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::float32: 
        return visit_same_helper<float>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::float64: 
        return visit_same_helper<double>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );
    
    case numerical_type::complex_float32: 
        return visit_same_helper<std::complex<float>>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    case numerical_type::complex_float64: 
        return visit_same_helper<std::complex<double>>(
            std::forward<Func>(func), 
            std::forward<Buffers>(buffers)...
        );

    default:
        throw std::invalid_argument("Cannot convert buffer type to host type" );

    }
}

} // namespace detail
template <typename Func, typename... Buffers>
auto visit_same(Func &&func, numerical_type type, Buffers&&... buffers)
{
    return detail::visit_same(
        std::forward<Func>(func),
        type,
        std::forward<Buffers>(buffers)...
    );
}

} // namespace compute
} // namespace xmipp4

