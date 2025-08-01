// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

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
    virtual compute::device* get_device() const noexcept = 0;
    virtual std::size_t get_size() const noexcept = 0;
    virtual void* get_data_ptr() noexcept = 0;
    virtual const void* get_data_ptr() const noexcept = 0;

};

} // namespace multidimensional
} // namespace xmipp4
