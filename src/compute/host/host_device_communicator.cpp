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
 * @file host_device_communicator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_device_communicator.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/host/host_device_communicator.hpp>

#include "default_host_buffer.hpp"

#include <xmipp4/core/compute/host/host_buffer_visitor.hpp>
#include <xmipp4/core/communication/communicator.hpp>

#include <complex>
#include <sstream>

namespace xmipp4
{
namespace compute
{

template <typename T>
static span<T> remove_complex(span<T> buffer)
{
    return buffer;
}

template <typename T>
static span<T> remove_complex(span<std::complex<T>> buffer)
{
    return span<T>(
        reinterpret_cast<T*>(buffer.data()),
        2*buffer.size()
    );
}

template <typename T>
static span<const T> remove_complex(span<const std::complex<T>> buffer)
{
    return span<const T>(
        reinterpret_cast<const T*>(buffer.data()),
        2*buffer.size()
    );
}

static numerical_type check_buffer_types(const device_buffer &send_buffer,
                                         const device_buffer &recv_buffer )
{
    numerical_type result;

    const auto send_type = send_buffer.get_type();
    const auto recv_type = recv_buffer.get_type();



    if (send_type == recv_type)
    {
        result = send_type;
    }
    else
    {
        std::ostringstream oss;
        oss << "send (" << send_type << ") and receive (" << recv_type 
            << ") types mismatch";

        throw std::invalid_argument(oss.str());
    }

    return result;
}

void check_reduction_operation(numerical_type type, reduction_operation operation)
{
    if (is_complex(type))
    {
        switch (operation)
        {
        case reduction_operation::min:
            throw std::invalid_argument(
                "min reduction cannot performed on complex numerical types"
            );

        case reduction_operation::max:
            throw std::invalid_argument(
                "maxreduction cannot performed on complex numerical types"
            );

        default:
            break;

        }
    }
}



host_device_communicator::host_device_communicator(std::shared_ptr<host_communicator> comm)
    : m_communicator(std::move(comm))
{
}

std::size_t host_device_communicator::get_size() const
{
    return m_communicator->get_size();
}

int host_device_communicator::get_rank() const
{
    return m_communicator->get_rank();
}

std::unique_ptr<device_buffer>
host_device_communicator::create_buffer(numerical_type type,
                                        std::size_t count )
{
    return std::make_unique<default_host_buffer>(type, count);
}

std::shared_ptr<device_buffer> 
host_device_communicator::create_buffer_shared(numerical_type type,
                                               std::size_t count )
{
    return std::make_shared<default_host_buffer>(type, count);
}

void host_device_communicator::barrier(queue&)
{
    m_communicator->barrier();
}





void host_device_communicator::send(int destination_rank, 
                                    const device_buffer &send_buf, 
                                    queue& )
{
    visit_buffers(
        [this, destination_rank] (auto send_buf) -> void
        {
            m_communicator->send(destination_rank, remove_complex(send_buf));
        },
        dynamic_cast<const host_buffer&>(send_buf)
    );
}

void host_device_communicator::receive(int source_rank, 
                                       device_buffer &recv_buf, 
                                       queue& )
{
    visit_buffers(
        [this, source_rank] (auto recv_buf) -> std::size_t
        {
            return m_communicator->receive(
                source_rank, remove_complex(recv_buf)
            );
        },
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

void host_device_communicator::send_receive(int destination_rank, 
                                            const device_buffer &send_buf,
                                            int source_rank,
                                            device_buffer &recv_buf, 
                                            queue& )
{
    const auto type = check_buffer_types(send_buf, recv_buf);
    visit_homogeneous_buffers(
        [this, destination_rank, source_rank] (auto send_buf, auto recv_buf)
        {
            m_communicator->send_receive(
                destination_rank,
                remove_complex(send_buf),
                source_rank,
                remove_complex(recv_buf)
            );
        },
        type,
        dynamic_cast<const host_buffer&>(send_buf),
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

void host_device_communicator::broadcast(int root, device_buffer &buf, queue&)
{
    visit_buffers(
        [this, root] (auto buf) -> void
        {
            m_communicator->broadcast(
                root, remove_complex(buf)
            );
        },
        dynamic_cast<host_buffer&>(buf)
    );

}

void host_device_communicator::scatter(int root, 
                                       const device_buffer &send_buf, 
                                       device_buffer &recv_buf, 
                                       queue& )
{
    const auto type = check_buffer_types(send_buf, recv_buf);
    visit_homogeneous_buffers(
        [this, root] (auto send_buf, auto recv_buf) -> void
        {
            m_communicator->scatter(
                root, 
                remove_complex(send_buf),
                remove_complex(recv_buf)
            );
        },
        type,
        dynamic_cast<const host_buffer&>(send_buf),
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

void host_device_communicator::gather(int root, 
                                      const device_buffer &send_buf, 
                                      device_buffer &recv_buf, 
                                      queue& )
{
    const auto type = check_buffer_types(send_buf, recv_buf);
    visit_homogeneous_buffers(
        [this, root] (auto send_buf, auto recv_buf) -> void
        {
            m_communicator->gather(
                root, 
                remove_complex(send_buf),
                remove_complex(recv_buf)
            );
        },
        type,
        dynamic_cast<const host_buffer&>(send_buf),
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

void host_device_communicator::all_gather(const device_buffer &send_buf, 
                                          device_buffer &recv_buf, 
                                          queue& )
{
    const auto type = check_buffer_types(send_buf, recv_buf);
    visit_homogeneous_buffers(
        [this] (auto send_buf, auto recv_buf) -> void
        {
            m_communicator->all_gather(
                remove_complex(send_buf),
                remove_complex(recv_buf)
            );
        },
        type,
        dynamic_cast<const host_buffer&>(send_buf),
        dynamic_cast<host_buffer&>(recv_buf)
    );

}

void host_device_communicator::reduce(int root, 
                                      reduction_operation operation,
                                      const device_buffer &send_buf, 
                                      device_buffer &recv_buf,
                                      queue& )
{
    const auto type = check_buffer_types(send_buf, recv_buf);
    check_reduction_operation(type, operation);

    visit_homogeneous_buffers(
        [this, root, operation] (auto send_buf, auto recv_buf) -> void
        {
            m_communicator->reduce(
                root, operation,
                remove_complex(send_buf),
                remove_complex(recv_buf)
            );
        },
        type,
        dynamic_cast<const host_buffer&>(send_buf),
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

void host_device_communicator::all_reduce(reduction_operation operation,
                                          const device_buffer &send_buf, 
                                          device_buffer &recv_buf,
                                          queue& )
{
    const auto type = check_buffer_types(send_buf, recv_buf);
    check_reduction_operation(type, operation);

    visit_homogeneous_buffers(
        [this, operation] (auto send_buf, auto recv_buf) -> void
        {
            m_communicator->all_reduce(
                operation,
                remove_complex(send_buf),
                remove_complex(recv_buf)
            );
        },
        type,
        dynamic_cast<const host_buffer&>(send_buf),
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

void host_device_communicator::all_to_all(const device_buffer &send_buf, 
                                          device_buffer &recv_buf, 
                                          queue& )
{
    const auto type = check_buffer_types(send_buf, recv_buf);
    visit_homogeneous_buffers(
        [this] (auto send_buf, auto recv_buf) -> void
        {
            m_communicator->all_to_all(
                remove_complex(send_buf),
                remove_complex(recv_buf)
            );
        },
        type,
        dynamic_cast<const host_buffer&>(send_buf),
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

} // namespace system
} // namespace xmipp4
