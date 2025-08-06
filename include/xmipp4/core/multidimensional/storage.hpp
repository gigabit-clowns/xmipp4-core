// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>
#include <typeinfo>

namespace xmipp4 
{
namespace compute
{

class device;
    
} // namespace compute

namespace multidimensional
{

class storage
{
public:
    /**
     * @brief Get the size in bytes for this buffer.
     * 
     * @return std::size_t Size in bytes.
     */
    virtual std::size_t get_size() const noexcept = 0;

    /**
     * @brief Get the backend key for dispatching operations with this buffer.
     * 
     * The backend key is usually obtained by declaring a stateless tag
     * and returning typeid(my_backend_tag)
     * 
     * @return const std::type_info& The backend key.
     */
    virtual const std::type_info& get_backend_key() const noexcept;

    /**
     * @brief Get the device where this buffer is stored. 
     * 
     * @return compute::device& The device where the buffer is stored.
     */
    virtual compute::device& get_device() const noexcept = 0;

};

} // namespace multidimensional
} // namespace xmipp4
