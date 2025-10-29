// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

namespace xmipp4 
{
namespace compute
{

class memory_resource;
class memory_transfer;

class memory_transfer_backend
{
public:
    memory_transfer_backend() = default;
    memory_transfer_backend(const memory_transfer_backend &other) = default;
    memory_transfer_backend(memory_transfer_backend &&other) = default;
    virtual ~memory_transfer_backend() = default;

    memory_transfer_backend&
    operator=(const memory_transfer_backend &other) = default;
    memory_transfer_backend&
    operator=(memory_transfer_backend &&other) = default;

    /**
     * @brief Create a transfer object to move data between two memory 
     * resources.
     * 
     * @param src Source memory resource.
     * @param dst Destination memory resource.
     * @return std::shared_ptr<memory_transfer> Memory transfer object. nullptr
     * if the transfer between the two memory resources is not supported.
     * 
     */
    virtual std::shared_ptr<memory_transfer> create_transfer(
        const memory_resource& src,
        const memory_resource& dst
    ) const = 0;

};  


} // namespace compute
} // namespace xmipp4
