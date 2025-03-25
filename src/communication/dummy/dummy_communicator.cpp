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
 * @file dummy_communicator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of dummy_communicator.hpp
 * @date 2024-11-20
 * 
 */

#include <xmipp4/core/communication/dummy/dummy_communicator.hpp>

#include <stdexcept>
#include <algorithm>

namespace xmipp4 
{
namespace communication
{
namespace detail
{

static void require_root_0(int root)
{
    if (root != 0)
    {
        throw std::invalid_argument(
            "invalid root specified"
        );
    }
}

static std::size_t require_same_count(std::size_t send_count, 
                                      std::size_t recv_count )
{
    if (send_count != recv_count)
    {
        throw std::invalid_argument(
            "send and receive buffer sizes must match"
        );
    }

    return send_count;
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::send(int, span<const T>)
{
    throw std::domain_error("send cannot be called with a single rank");
}

template<typename T, typename... Ts>
std::size_t 
dummy_communicator_helper<T, Ts...>::receive(int, span<T>)
{
    throw std::domain_error("receive cannot be called with a single rank");
}

template<typename T, typename... Ts>
std::size_t 
dummy_communicator_helper<T, Ts...>::send_receive(int,
                                                  span<const T>,
                                                  int, 
                                                  span<T> )
{
    throw std::domain_error("send_receive cannot be called with a single rank");
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::broadcast(int root, span<T>)
{
    require_root_0(root);
    // No-op
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::scatter(int root, 
                                                  span<const T> send_buf, 
                                                  span<T> recv_buf )
{
    dummy_collective_operation(root, send_buf, recv_buf);
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::gather(int root, 
                                                 span<const T> send_buf, 
                                                 span<T> recv_buf )
{
    require_root_0(root);
    dummy_collective_operation(root, send_buf, recv_buf);
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::all_gather(span<const T> send_buf, 
                                                     span<T> recv_buf )
{
    dummy_collective_operation(send_buf, recv_buf);
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::reduce(int root, 
                                                 reduction_operation,
                                                 span<const T> send_buf, 
                                                 span<T> recv_buf )
{
    require_root_0(root);
    dummy_collective_operation(send_buf, recv_buf);
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::all_reduce(reduction_operation,
                                                     span<const T> send_buf, 
                                                     span<T> recv_buf )
{
    dummy_collective_operation(send_buf, recv_buf);
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::all_to_all(span<const T> send_buf, 
                                                     span<T> recv_buf )
{
    dummy_collective_operation(send_buf, recv_buf);
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::dummy_collective_operation(int root,
                                                                     span<const T> send_buf, 
                                                                     span<T> recv_buf )
{
    require_root_0(root);
    dummy_collective_operation(send_buf, recv_buf);
}

template<typename T, typename... Ts>
void dummy_communicator_helper<T, Ts...>::dummy_collective_operation(span<const T> send_buf, 
                                                                     span<T> recv_buf )
{
    const auto count = require_same_count(send_buf.size(), recv_buf.size());

    // In case it is not an alias, perform a plain copy.
    if(send_buf.data() != recv_buf.data())
    {
        std::copy_n(
            send_buf.data(),
            count,
            recv_buf.data()
        );
    }
}

} // namespace detail





std::size_t dummy_communicator::get_size() const
{
    return 1;
}

int dummy_communicator::get_rank() const
{
    return 0;
}

std::shared_ptr<communicator> 
dummy_communicator::split(int, int) const
{
    return std::make_shared<dummy_communicator>();
}


void dummy_communicator::barrier()
{
    // No-op
}

} // namespace communication
} // namespace xmipp4
