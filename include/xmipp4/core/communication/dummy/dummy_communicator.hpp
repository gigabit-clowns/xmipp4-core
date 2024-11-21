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
 * @file dummy_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the communication::dummy_communicator class
 * @date 2024-10-26
 * 
 */

#include "../communicator.hpp"

#include <memory>

namespace xmipp4 
{
namespace communication
{

namespace detail
{

template<typename... Ts>
class dummy_communicator_helper;

template<typename T, typename... Ts>
class dummy_communicator_helper<T, Ts...>
    : public dummy_communicator_helper<Ts...>
{
public:
    using dummy_communicator_helper<Ts...>::send;
    using dummy_communicator_helper<Ts...>::receive;
    using dummy_communicator_helper<Ts...>::send_receive;
    using dummy_communicator_helper<Ts...>::broadcast;
    using dummy_communicator_helper<Ts...>::scatter;
    using dummy_communicator_helper<Ts...>::gather;
    using dummy_communicator_helper<Ts...>::all_gather;
    using dummy_communicator_helper<Ts...>::reduce;
    using dummy_communicator_helper<Ts...>::all_reduce;
    using dummy_communicator_helper<Ts...>::all_to_all;

    void send(int destination_rank, span<const T> buf) final;

    std::size_t receive(int source_rank, span<T> buf) final;

    std::size_t send_receive(int destination_rank, span<const T> send_buf,
                             int source_rank, span<T> receive_buf ) final;

    void broadcast(int root, span<T> buf) final;

    void scatter(int root, 
                 span<const T> send_buf, span<T> recv_buf ) final;

    void gather(int root, 
                span<const T> send_buf, span<T> recv_buf ) final;

    void all_gather(span<const T> send_buf, span<T> recv_buf) final;

    void reduce(int root, reduction_operation op,
                span<const T> send_buf, span<T> recv_buf ) final;

    void all_reduce(reduction_operation op,
                    span<const T> send_buf, span<T> recv_buf ) final;

    void all_to_all(span<const T> send_buf, span<T> recv_buf) final;

private:
    void dummy_collective_operation(int root, 
                                    span<const T> send_buf, 
                                    span<T> recv_buf );
    void dummy_collective_operation(span<const T> send_buf, span<T> recv_buf);

};

template<>
class dummy_communicator_helper<>
    : public communicator
{
};

} // namespace detail





class dummy_communicator final
    : public detail::dummy_communicator_helper<memory::byte,
                                               char,
                                               signed char,
                                               unsigned char,
                                               short,
                                               unsigned short,
                                               int,
                                               unsigned int,
                                               long,
                                               unsigned long,
                                               long long,
                                               unsigned long long,
                                               float,
                                               double,
                                               long double>
{
public:
    std::size_t get_size() const override;

    int get_rank() const override;

    std::unique_ptr<communicator> split(int colour, 
                                        int rank_priority ) const override;

    std::shared_ptr<communicator> split_shared(int colour, 
                                               int rank_priority ) const override;

    void barrier() override;

};

} // namespace communication
} // namespace xmipp4
