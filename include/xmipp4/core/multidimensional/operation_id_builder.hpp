// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_id.hpp"

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

class operation_id_builder
{
public:
    operation_id_builder& nest(const std::string &namespace_name);
    operation_id_builder& nest(const char *namespace_name);
    operation_id build(const std::string &operation_name);
    operation_id build(const char *operation_name);

private:
    std::ostringstream m_oss;

};

} // namespace multidimensional
} // namespace xmipp4
