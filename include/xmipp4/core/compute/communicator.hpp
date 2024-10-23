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
 * @brief Definition of the compute::communicator class
 * @date 2024-10-21
 * 
 */

#include <memory>

namespace xmipp4 
{
namespace compute
{

class buffer;
class queue;

class communicator
{
public:
    communicator() = default;
    communicator(const communicator &other) = default;
    communicator(communicator &&other) = default;
    virtual ~communicator() = default;

    communicator& operator=(const communicator &other) = default;
    communicator& operator=(communicator &&other) = default;

    virtual std::size_t get_size() const = 0;
    virtual int get_rank() const = 0;

    virtual std::unique_ptr<communicator> split(int group_id, 
                                                int rank_priority ) const = 0;
    virtual std::shared_ptr<communicator> split_shared(int group_id, 
                                                       int rank_priority ) const = 0;

    virtual void barrier() = 0;

    virtual void send(int destination_rank, 
                      const buffer &buf, queue &q) = 0;

    virtual void receive(int source_rank, buffer &buf, queue &q) = 0;

    virtual void broadcast(int root, buffer &buf, queue &q) = 0;

    virtual void scatter(int root, 
                         const buffer &send_buf, buffer &recv_buf, 
                         queue &q ) = 0;

    virtual void gather(int root, 
                        const buffer &send_buf, buffer &recv_buf, 
                        queue &q ) = 0;

    virtual void all_gather(const buffer &send_buf, buffer &recv_buf, 
                            queue &q) = 0;

    virtual void all_to_all(const buffer &send_buf, buffer &recv_buf, 
                            queue &q) = 0;

};

} // namespace compute
} // namespace xmipp4
