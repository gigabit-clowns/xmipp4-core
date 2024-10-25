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
 * @file host_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the compute::host_communicator class
 * @date 2024-10-24
 * 
 */


#include "reduction_operation.hpp"
#include "../span.hpp"
#include "../memory/byte.hpp"

#include <memory>

namespace xmipp4 
{
namespace compute
{

template <typename T>
class host_communications_interface
{
public:
    host_communications_interface() = default;
    host_communications_interface(const host_communications_interface &other) = default;
    host_communications_interface(host_communications_interface &&other) = default;
    virtual ~host_communications_interface() = default;

    host_communications_interface& operator=(const host_communications_interface &other) = default;
    host_communications_interface& operator=(host_communications_interface &&other) = default;

    virtual void send(int destination_rank, span<const T> buf) = 0;


    virtual void receive(int source_rank, span<T> buf) = 0;

    virtual void broadcast(int root, span<T> buf) = 0;

    virtual void scatter(int root, 
                         span<const T> send_buf, span<T> recv_buf ) = 0;

    virtual void gather(int root, 
                        span<const T> send_buf, span<T> recv_buf ) = 0;

    virtual void all_gather(span<const T> send_buf, span<T> recv_buf) = 0;

    virtual void reduce(int root, reduction_operation op,
                        span<const T> send_buf, span<T> recv_buf ) = 0;

    virtual void all_reduce(reduction_operation op,
                            span<const T> send_buf, span<T> recv_buf ) = 0;

    virtual void all_to_all(span<const T> send_buf, span<T> recv_buf) = 0;

};

class host_communicator
    : public host_communications_interface<memory::byte>
    , public host_communications_interface<short>
    , public host_communications_interface<unsigned short>
    , public host_communications_interface<int>
    , public host_communications_interface<unsigned int>
    , public host_communications_interface<long>
    , public host_communications_interface<unsigned long>
    , public host_communications_interface<long long>
    , public host_communications_interface<unsigned long long>
    , public host_communications_interface<float>
    , public host_communications_interface<double>
    , public host_communications_interface<long double>
{
public:
    host_communicator() = default;
    host_communicator(const host_communicator &other) = default;
    host_communicator(host_communicator &&other) = default;
    virtual ~host_communicator() = default;

    host_communicator& operator=(const host_communicator &other) = default;
    host_communicator& operator=(host_communicator &&other) = default;

    virtual std::size_t get_size() const = 0;
    virtual int get_rank() const = 0;

    virtual std::unique_ptr<host_communicator> split(int colour, 
                                                     int rank_priority ) const = 0;
    virtual std::shared_ptr<host_communicator> split_shared(int colour, 
                                                            int rank_priority ) const = 0;

    virtual void barrier() = 0;

};

} // namespace compute
} // namespace xmipp4
