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
 * @file device_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the compute::device_communicator class
 * @date 2024-10-21
 * 
 */

#include "numerical_type.hpp"
#include "../reduction_operation.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_buffer;
class queue;



/**
 * @brief Abstract class to represent interprocess and inter-device
 * communications.
 * 
 */
class XMIPP4_CORE_API device_communicator
{
public:
    device_communicator() = default;
    device_communicator(const device_communicator &other) = default;
    device_communicator(device_communicator &&other) = default;
    virtual ~device_communicator() = default;

    device_communicator& operator=(const device_communicator &other) = default;
    device_communicator& operator=(device_communicator &&other) = default;

    /**
     * @brief Get the amount of peers in the communicator.
     * 
     * @return std::size_t The number of peers.
     * 
     */
    virtual std::size_t get_size() const = 0;

    /**
     * @brief Get the rank of the device.
     * 
     * @return int The rank.
     * 
     */
    virtual int get_rank() const = 0;

    /**
     * @brief Allocate a buffer specially suited to be used for communication.
     * 
     * @param type Numerical type of the buffer.
     * @param count Number of elements in the buffer.
     * @return std::unique_ptr<device_buffer> The buffer.
     */
    virtual std::unique_ptr<device_buffer>
    create_buffer(numerical_type type, std::size_t count) = 0;

    /**
     * @brief Allocate a buffer specially suited to be used for communication.
     * 
     * @param type Numerical type of the buffer.
     * @param count Number of elements in the buffer.
     * @return std::shared_ptr<device_buffer> The buffer.
     */
    virtual std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type, std::size_t count) = 0;

    /**
     * @brief Wait until all ranks reach this point.
     * 
     * @param queue Queue where the command is enqueued.
     * 
     * @note All calling peers should call it.
     * 
     */
    virtual void barrier(queue &queue) = 0;

    /**
     * @brief Send a message.
     * 
     * @param destination_rank Rank of the receiver.
     * @param send_buffer The buffer to be sent.
     * @param queue Queue where the command is enqueued.
     * 
     * @note The destination rank should be calling receive() or send_receive().
     * 
     */
    virtual void send(int destination_rank, 
                      const device_buffer &send_buffer, queue &queue) = 0;

    /**
     * @brief Receive a message.
     * 
     * @param source_rank Rank of the sender.
     * @param buffer The buffer where the received message will be written.
     * @param queue Queue where the command is enqueued.
     * 
     * @note The source rank should be calling send() or send_receive().
     * 
     */
    virtual void receive(int source_rank, 
                         device_buffer &receive_buffer, queue &queue) = 0;

    /**
     * @brief Send and receive a message.
     * 
     * Simultaneously sends and receives a message.
     * 
     * @param destination_rank Rank of the receiver.
     * @param send_buffer The buffer to be sent.
     * @param source_rank Rank of the sender.
     * @param receive_buffer The buffer where the received message 
     * will be written.
     * @param queue Queue where the command is enqueued.
     * 
     * @note The source rank should be calling send() or send_receive()
     * @note The destination rank should be calling receive() or send_receive()
     * @see send
     * @see receive
     *  
     */
    virtual std::size_t send_receive(int destination_rank, 
                                     const device_buffer &send_buffer,
                                     int source_rank, 
                                     device_buffer &receive_buffer,
                                     queue &queue ) = 0;

    /**
     * @brief Broadcasts a message to all peers.
     * 
     * The message is broadcasted from the root rank to the rest of the ranks.
     * 
     * @param root The rank that sends the message.
     * @param buf The buffer to send or receive.
     * @param queue Queue where the command is enqueued.
     * 
     * @note All calling peers should call it with the same root.
     * 
     */
    virtual void broadcast(int root, device_buffer &buffer, queue &queue) = 0;

    /**
     * @brief Scatter a message across all peers.
     * 
     * The message is divided into equal blocks and each chunk is sent
     * to a different rank. The root rank is responsible of sending the 
     * message, whilst the rest of ranks will receive it. The root rank
     * also obtains one of the chunks.
     * 
     * @param root The rank responsible of scattering the message.
     * @param send_buffer The buffer to be sent. Ignored for all non-root ranks.
     * @param recv_buffer The buffer where the received data will be written.
     * @param queue Queue where the command is enqueued.
     * 
     * @note All ranks of the communicator need to call this method with the
     * same root.
     * 
     */
    virtual void scatter(int root, 
                         const device_buffer &send_buffer, 
                         device_buffer &recv_buffer, 
                         queue &queue ) = 0;

    /**
     * @brief Gather a message across all peers.
     * 
     * Chunks of the message are gathered from all peers and assembled together
     * in the root rank.
     * 
     * @param root The rank responsible of gathering the message.
     * @param send_buffer The buffer to be sent.
     * @param recv_buffer The buffer where the received message will be written.
     * Ignored for all non-root ranks.
     * @param queue Queue where the command is enqueued.
     * 
     * @note All ranks of the communicator need to call this method with the
     * same root.
     * 
     */
    virtual void gather(int root, 
                        const device_buffer &send_buffer, 
                        device_buffer &recv_buffer, 
                        queue &queue ) = 0;

    /**
     * @brief Gather a message across all peers.
     * 
     * All peers in the communicator gather the message from the rest of the
     * peers.
     * 
     * @param send_buffer The buffer to be sent.
     * @param recv_buffer The buffer where the received message is written.
     * @param queue Queue where the command is enqueued.
     * 
     * @note All ranks of the communicator need to call this method.
     * @see gather
     * 
     */
    virtual void all_gather(const device_buffer &send_buffer, 
                            device_buffer &recv_buffer, 
                            queue &queue ) = 0;

    /**
     * @brief Perform a reduction operation across all peers
     * 
     * All the messages are combined into a single one by the root rank.
     * 
     * @param root The rank responsible of combining the message.
     * @param operation The reduction operation to be performed.
     * @param send_buffer The buffer to be sent.
     * @param recv_buffer The buffer where the messages will be combined.
     * @param queue Queue where the command is enqueued.
     * 
     * Ignored for all non-root ranks.
     * @note All ranks of the communicator need to call this method. send_buf 
     * may be equal to recv_buf, in which case the operation
     * is performed in-place.
     * 
     */
    virtual void reduce(int root, reduction_operation operation,
                        const device_buffer &send_buffer, 
                        device_buffer &recv_buffer,
                        queue &queue ) = 0;

    /**
     * @brief Perform a reduction operation across all peers
     * 
     * All the messages are combined into a single one by all ranks.
     * 
     * @param operation The reduction operation to be performed.
     * @param send_buffer The buffer to be sent.
     * @param recv_buffer The buffer where the messages will be combined.
     * @param queue Queue where the command is enqueued.
     * 
     * @note All ranks of the communicator need to call this method. send_buf 
     * may be equal to recv_buf, in which case the operation is performed 
     * in-place.
     * @see reduce
     * 
     */
    virtual void all_reduce(reduction_operation operation,
                            const device_buffer &send_buf, 
                            device_buffer &recv_buf,
                            queue &queue ) = 0;

    /**
     * @brief Combination of scatter and gather.
     * 
     * Every peer has a buffer containing elements that will be scattered 
     * across all processes, as well as a buffer in which store elements 
     * that will be gathered from all other peers.
     * 
     * @param send_buffer The buffer to be sent.
     * @param recv_buffer The buffer where the messages will be combined.
     * @param queue Queue where the command is enqueued.
     * 
     * @note All ranks of the communicator need to call this method.
     * @see scatter
     * @see gather
     * 
     */
    virtual void all_to_all(const device_buffer &send_buffer, 
                            device_buffer &recv_buffer, 
                            queue &queue ) = 0;

};

} // namespace compute
} // namespace xmipp4
