// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_id_builder.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Str>
static void nest_impl(std::ostringstream &oss, Str&& namespace_name)
{
    const char *separator = "::";
    oss << std::forward<Str>(namespace_name) << separator;
}

operation_id_builder& operation_id_builder::nest(
    const std::string &namespace_name
)
{
    nest_impl(m_oss, namespace_name);
    return *this;
}

operation_id_builder& operation_id_builder::nest(const char *namespace_name)
{
    nest_impl(m_oss, namespace_name);
    return *this;
}

template <typename Str>
static operation_id build_impl(std::ostringstream &oss, Str&& namespace_name)
{
    oss << namespace_name;
    operation_id result(oss.str());
    oss.clear();
    return result;
}

operation_id operation_id_builder::build(const std::string &operation_name)
{
    return build_impl(m_oss, operation_name);
}

operation_id operation_id_builder::build(const char *operation_name)
{
    return build_impl(m_oss, operation_name);
}

} // namespace multidimensional
} // namespace xmipp4

