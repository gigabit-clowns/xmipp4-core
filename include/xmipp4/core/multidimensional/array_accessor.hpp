// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array_accessor
{
public:


private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;
    
};

} // namespace multidimensional
} // namespace xmipp4

