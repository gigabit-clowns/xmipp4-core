// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/masked_reduction_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Masked variant of @ref stddev_operation. The first input is a
 * boolean mask.
 */
class XMIPP4_CORE_API stddev_masked_operation final
    : public masked_reduction_operation
{
public:
    explicit stddev_masked_operation(
        std::vector<std::size_t> axes,
        std::size_t ddof = 0,
        bool keep_dim = false
    );
    std::string get_name() const override;
    std::size_t get_output_count() const noexcept override;
    std::size_t get_input_count() const noexcept override;
    std::size_t get_degrees_of_freedom() const noexcept;

private:
    std::size_t m_ddof;
};

} // namespace multidimensional
} // namespace xmipp4
