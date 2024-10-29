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
    visit(
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
    visit(
        [this, source_rank] (auto recv_buf) -> std::size_t
        {
            return m_communicator->receive(
                source_rank, remove_complex(recv_buf)
            );
        },
        dynamic_cast<host_buffer&>(recv_buf)
    );
}

void host_device_communicator::broadcast(int root, device_buffer &buf, queue&)
{
    visit(
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
    const auto type = send_buf.get_type();
    if (type == recv_buf.get_type())
    {
        visit_same(
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
    else
    {
        throw std::invalid_argument(
            "Send and receive buffers don't have matching types"
        );
    }
}

void host_device_communicator::gather(int root, 
                                      const device_buffer &send_buf, 
                                      device_buffer &recv_buf, 
                                      queue& )
{
    const auto type = send_buf.get_type();
    if (type == recv_buf.get_type())
    {
        visit_same(
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
    else
    {
        throw std::invalid_argument(
            "Send and receive buffers don't have matching types"
        );
    }
}

void host_device_communicator::all_gather(const device_buffer &send_buf, 
                                          device_buffer &recv_buf, 
                                          queue& )
{
    const auto type = send_buf.get_type();
    if (type == recv_buf.get_type())
    {
        visit_same(
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
    else
    {
        throw std::invalid_argument(
            "Send and receive buffers don't have matching types"
        );
    }
}

void host_device_communicator::reduce(int root, 
                                      reduction_operation operation,
                                      const device_buffer &send_buf, 
                                      device_buffer &recv_buf,
                                      queue& )
{
    const auto type = send_buf.get_type();
    if (type == recv_buf.get_type())
    {
        visit_same(
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
    else
    {
        throw std::invalid_argument(
            "Send and receive buffers don't have matching types"
        );
    }
}

void host_device_communicator::all_reduce(reduction_operation operation,
                                          const device_buffer &send_buf, 
                                          device_buffer &recv_buf,
                                          queue& )
{
    const auto type = send_buf.get_type();
    if (type == recv_buf.get_type())
    {
        visit_same(
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
    else
    {
        throw std::invalid_argument(
            "Send and receive buffers don't have matching types"
        );
    }
}

void host_device_communicator::all_to_all(const device_buffer &send_buf, 
                                          device_buffer &recv_buf, 
                                          queue& )
{
    const auto type = send_buf.get_type();
    if (type == recv_buf.get_type())
    {
        visit_same(
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
    else
    {
        throw std::invalid_argument(
            "Send and receive buffers don't have matching types"
        );
    }
}

} // namespace system
} // namespace xmipp4
