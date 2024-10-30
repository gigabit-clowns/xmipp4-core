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
 * @file communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the communication::communicator class
 * @date 2024-10-24
 * 
 */


#include "../reduction_operation.hpp"
#include "../span.hpp"
#include "../memory/byte.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Interface for communication operations with typed buffers.
 * 
 * @note This is a helper class to build communicator. It should not be
 * used separately.
 * 
 * @tparam T Type of the elements to be sent/received.
 */
template <typename T>
class XMIPP4_CORE_API communications_interface
{
public:
    communications_interface() = default;
    communications_interface(const communications_interface &other) = default;
    communications_interface(communications_interface &&other) = default;
    virtual ~communications_interface() = default;

    communications_interface& operator=(const communications_interface &other) = default;
    communications_interface& operator=(communications_interface &&other) = default;

    /**
     * @brief Send a message.
     * 
     * @param destination_rank Rank of the receiver.
     * @param buf The buffer to be sent.
     * 
     * @note The destination rank should be calling receive(). This routine may 
     * block until the message is received by the destination process. 
     * 
     */
    virtual void send(int destination_rank, span<const T> buf) = 0;

    /**
     * @brief Receive a message.
     * 
     * @param source_rank Rank of the sender.
     * @param buf The buffer where the received message will be written.
     * @return std::size_t Number of elements received.
     * @note The size of the buffer argument indicates the maximum 
     * length of a message; the actual length of the received message
     * is determined by the return value.
     * 
     */
    virtual std::size_t receive(int source_rank, span<T> buf) = 0;

    /**
     * @brief Send and receive a message.
     * 
     * @param destination_rank Rank of the receiver.
     * @param send_buf The buffer to be sent.
     * @param source_rank Rank of the sender.
     * @param receive_buf The buffer where the received message will be written.
     * @return std::size_t std::size_t Number of elements received.
     * @note The size of the reception buffer argument indicates the maximum 
     * length of a message; the actual length of the received message
     * is determined by the return value.
     * @note The destination rank should be calling receive(). This routine may 
     * block until the message is received by the destination process. 
     * @see send
     * @see receive
     *  
     */
    virtual std::size_t send_receive(int destination_rank, span<const T> send_buf,
                                     int source_rank, span<T> receive_buf ) = 0;

    /**
     * @brief Broadcasts a message to all peers.
     * 
     * The message is broadcasted from the root rank to the rest of the ranks.
     * 
     * @param root The rank that sends the message.
     * @param buf The buffer to send or receive.
     * @note All calling processes should call it with the same root.
     * 
     */
    virtual void broadcast(int root, span<T> buf) = 0;

    /**
     * @brief Scatter a message across all peers.
     * 
     * The message is divided into equal blocks and each chunk is sent
     * to a different rank. The root rank is responsible of sending the 
     * message, whilst the rest of ranks will receive it. The root rank
     * also obtains one of the chunks.
     * 
     * @param root The rank responsible of scattering the message.
     * @param send_buf The buffer to be sent. Ignored for all non-root ranks.
     * @param recv_buf The buffer where the received data will be written.
     * @note All ranks of the communicator need to call this method with the
     * same root.
     * 
     */
    virtual void scatter(int root, 
                         span<const T> send_buf, span<T> recv_buf ) = 0;

    /**
     * @brief Gather a message across all peers.
     * 
     * Chunks of the message are gathered from all peers and assembled together
     * in the root rank.
     * 
     * @param root The rank responsible of gathering the message.
     * @param send_buf The buffer to be sent.
     * @param recv_buf The buffer where the received message will be written.
     * Ignored for all non-root ranks.
     * @note All ranks of the communicator need to call this method with the
     * same root.
     * 
     */
    virtual void gather(int root, 
                        span<const T> send_buf, span<T> recv_buf ) = 0;

    /**
     * @brief Gather a message across all peers.
     * 
     * All peers in the communicator gather the message from the rest of the
     * peers.
     * 
     * @param send_buf The buffer to be sent.
     * @param recv_buf The buffer where the received message is written.
     * @note All ranks of the communicator need to call this method.
     * @see gather
     * 
     */
    virtual void all_gather(span<const T> send_buf, span<T> recv_buf) = 0;

    /**
     * @brief Perform a reduction operation across all peers
     * 
     * All the messages are combined into a single one by the root rank.
     * 
     * @param root The rank responsible of combining the message.
     * @param op The reduction operation to be performed.
     * @param send_buf The buffer to be sent.
     * @param recv_buf The buffer where the messages will be combined.
     * Ignored for all non-root ranks.
     * @note All ranks of the communicator need to call this method. send_buf 
     * may be equal to recv_buf, in which case the operation
     * is performed in-place.
     * 
     */
    virtual void reduce(int root, reduction_operation op,
                        span<const T> send_buf, span<T> recv_buf ) = 0;

    /**
     * @brief Perform a reduction operation across all peers
     * 
     * All the messages are combined into a single one by all ranks.
     * 
     * @param op The reduction operation to be performed.
     * @param send_buf The buffer to be sent.
     * @param recv_buf The buffer where the messages will be combined.
     * @note All ranks of the communicator need to call this method. send_buf 
     * may be equal to recv_buf, in which case the operation is performed 
     * in-place.
     * @see reduce
     * 
     */
    virtual void all_reduce(reduction_operation op,
                            span<const T> send_buf, span<T> recv_buf ) = 0;

    /**
     * @brief Combination of scatter and gather.
     * 
     * Every peer has a buffer containing elements that will be scattered 
     * across all processes, as well as a buffer in which store elements 
     * that will be gathered from all other peers.
     * 
     * @param send_buf The buffer to be sent.
     * @param recv_buf The buffer where the messages will be combined.
     * @note All ranks of the communicator need to call this method.
     * @see scatter
     * @see gather
     * 
     */
    virtual void all_to_all(span<const T> send_buf, span<T> recv_buf) = 0;

};



/**
 * @brief Gather multiple specializations of communications_interface
 * in a single interface.
 * 
 * @tparam Ts Types to be used in the interface.
 *  
 */
template <typename... Ts>
class XMIPP4_CORE_API multitype_communications_interface
    : public communications_interface<Ts>...
{
public:
    multitype_communications_interface() = default;
    multitype_communications_interface(const multitype_communications_interface &other) = default;
    multitype_communications_interface(multitype_communications_interface &&other) = default;
    virtual ~multitype_communications_interface() = default;

    multitype_communications_interface& 
    operator=(const multitype_communications_interface &other) = default;
    multitype_communications_interface& 
    operator=(multitype_communications_interface &&other) = default;
    
    using communications_interface<Ts>::send...;
    using communications_interface<Ts>::receive...;
    using communications_interface<Ts>::send_receive...;
    using communications_interface<Ts>::broadcast...;
    using communications_interface<Ts>::scatter...;
    using communications_interface<Ts>::gather...;
    using communications_interface<Ts>::all_gather...;
    using communications_interface<Ts>::reduce...;
    using communications_interface<Ts>::all_reduce...;
    using communications_interface<Ts>::all_to_all...;

};



/**
 * @brief Abstract class to represent interprocess and inter-node 
 * communications.
 * 
 */
class communicator
    : public multitype_communications_interface<memory::byte,
                                                     char,
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
    communicator() = default;
    communicator(const communicator &other) = default;
    communicator(communicator &&other) = default;
    virtual ~communicator() = default;

    communicator& operator=(const communicator &other) = default;
    communicator& operator=(communicator &&other) = default;

    /**
     * @brief Get the amount of peers in the communicator.
     * 
     * @return std::size_t The number of peers.
     * 
     */
    virtual std::size_t get_size() const = 0;

    /**
     * @brief Get the rank of the local process.
     * 
     * @return int The rank.
     * 
     */
    virtual int get_rank() const = 0;

     /**
     * @brief Split the current communicator.
     * 
     * Split a communicator into multiple communicators containing.
     * 
     * @param colour The group where the current rank will be assigned to.
     * @param rank_priority Hint to assign the rank in the new communicator.
     * @return std::unique_ptr<communicator> The communicator where
     * the current rank has been assigned to.
     * 
     */
    virtual std::unique_ptr<communicator> split(int colour, 
                                                int rank_priority ) const = 0;

    /**
     * @brief Split the current communicator.
     * 
     * Split a communicator into multiple communicators containing.
     * 
     * @param colour The group where the current rank will be assigned to.
     * @param rank_priority Hint to assign the rank in the new communicator.
     * @return std::shared_ptr<communicator> The communicator where
     * the current rank has been assigned to.
     * 
     */
    virtual std::shared_ptr<communicator> split_shared(int colour, 
                                                       int rank_priority ) const = 0;

    /**
     * @brief Synchronize all peers.
     * 
     * Wait until all peers have reached this call.
     * 
     * @note All ranks of the communicator need to call this method.
     * 
     */
    virtual void barrier() = 0;

};

} // namespace communication
} // namespace xmipp4
