// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"

namespace xmipp4 
{
namespace hardware
{

class memory_allocator;
class device_queue;

} // namespace hardware

namespace multidimensional
{

class array;
class strided_layout;

array empty(
    const strided_layout &layout, 
    numerical_type data_type,
    hardware::memory_allocator &allocator,
    hardware::device_queue *queue,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
