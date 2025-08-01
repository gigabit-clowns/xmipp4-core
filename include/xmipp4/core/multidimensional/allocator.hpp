// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

namespace xmipp4 
{
namespace compute
{

class device_queue;
    
} // namespace compute
    
namespace multidimensional
{

class storage;

class allocator
{
public:
    virtual 
    std::shared_ptr<storage> allocate(std::size_t size, 
                                      std::size_t alignment, 
                                      compute::device_queue *queue ) = 0;

};

} // namespace multidimensional
} // namespace xmipp4
