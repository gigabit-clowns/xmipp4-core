// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;

class array_accessor
{
public:
    array_accessor() noexcept;
    explicit array_accessor(std::vector<std::size_t> extents);
    array_accessor(const array_accessor &other) = delete;
    array_accessor(array_accessor &&other) noexcept;
    ~array_accessor();

    array_accessor& 
    operator=(const array_accessor &other) = delete;
    array_accessor& 
    operator=(array_accessor &&other) noexcept;

    void set_extents(std::vector<std::size_t> extents);
    void add_output(const strided_layout &layout);
    void add_input(const strided_layout &layout);
    void sort_axes_by_locality();
    void coalesce_contiguous_axes();

    std::size_t get_number_of_dimensions() const noexcept;
    std::size_t get_number_of_operands() const noexcept;
    std::size_t get_number_of_outputs() const noexcept;
    std::size_t get_number_of_inputs() const noexcept;

private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;
    
};

} // namespace multidimensional
} // namespace xmipp4
