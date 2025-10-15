// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/compute/buffer.hpp>

namespace xmipp4 
{
namespace multidimensional
{


class array::implementation
{
public:
    implementation() = default;
    implementation(strided_layout layout, 
                   std::shared_ptr<storage_type> storage,
                   numerical_type data_type ) noexcept
        : m_layout(std::move(layout))
        , m_storage(std::move(storage))
        , m_data_type(data_type)
    {
    }

    numerical_type get_data_type() const noexcept
    {
        return m_data_type;
    }

    const strided_layout& get_layout() const noexcept
    {
        return m_layout;
    }

    storage_type* get_storage() noexcept
    {
        return m_storage.get();
    }

    std::shared_ptr<storage_type> share_storage() noexcept
    {
        return m_storage;
    }

    std::size_t get_rank() const noexcept
    {
        return m_layout.get_rank();
    }

    void get_extents(std::vector<std::size_t> &extents) const
    {
        m_layout.get_extents(extents);
    }

    implementation apply_subscripts(span<const dynamic_subscript> subscripts)
    {
        return implementation(
            get_layout().apply_subscripts(subscripts),
            share_storage(),
            get_data_type()
        );
    }

    implementation transpose()
    {
        return implementation(
            get_layout().transpose(),
            share_storage(),
            get_data_type()
        );
    }

    implementation permute(span<const std::size_t> order)
    {
        return implementation(
            get_layout().permute(order),
            share_storage(),
            get_data_type()
        );
    }

    implementation matrix_transpose(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
    {
        return implementation(
            get_layout().matrix_transpose(axis1, axis2),
            share_storage(),
            get_data_type()
        );
    }

    implementation matrix_diagonal(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
    {
        return implementation(
            get_layout().matrix_diagonal(axis1, axis2),
            share_storage(),
            get_data_type()
        );
    }

    implementation squeeze()
    {
        return implementation(
            get_layout().squeeze(),
            share_storage(),
            get_data_type()
        );
    }

    implementation broadcast_to(span<const std::size_t> extents)
    {
        return implementation(
            get_layout().broadcast_to(extents),
            share_storage(),
            get_data_type()
        );
    }

private:
    strided_layout m_layout;
    std::shared_ptr<storage_type> m_storage;
    numerical_type m_data_type;

};





array::array() = default;
array::array(array&& other) noexcept = default;
array::~array() = default;
array& array::operator=(array&& other) noexcept = default;

array::array(strided_layout layout, 
             std::shared_ptr<storage_type> storage, 
             numerical_type data_type )
    : array(
        std::make_shared<implementation>(
            std::move(layout), 
            std::move(storage), 
            data_type
        )
    )
{
}

array::array(std::shared_ptr<implementation> impl) noexcept
    : m_implementation(std::move(impl))
{
}

array::array(implementation &&impl)
    : array(std::make_shared<implementation>(std::move(impl)))
{
}

numerical_type array::get_data_type() const noexcept
{
    return m_implementation ? 
           m_implementation->get_data_type() : 
           numerical_type::unknown ;
}

strided_layout array::get_layout() const noexcept
{
    return m_implementation ? 
           m_implementation->get_layout() : 
           strided_layout() ;
}

array::storage_type* array::get_storage() noexcept
{
    return m_implementation ? 
           m_implementation->get_storage() : 
           nullptr ;
}

const array::storage_type* array::get_storage() const noexcept
{
    return m_implementation ? 
           m_implementation->get_storage() : 
           nullptr ;
}

XMIPP4_NODISCARD
std::shared_ptr<array::storage_type> array::share_storage() noexcept
{
    return m_implementation ? 
           m_implementation->share_storage() : 
           nullptr ;
}

XMIPP4_NODISCARD
std::shared_ptr<const array::storage_type> array::share_storage() const noexcept
{
    return m_implementation ? 
           m_implementation->share_storage() : 
           nullptr ;
}

XMIPP4_NODISCARD
std::size_t array::get_rank() const noexcept
{
    return m_implementation ? 
           m_implementation->get_rank() : 
           0U ;
}

void array::get_extents(std::vector<std::size_t> &extents) const
{
    extents.clear();
    if (m_implementation)
    {
        m_implementation->get_extents(extents);
    }
}

XMIPP4_NODISCARD
array array::view() noexcept
{
    return array(m_implementation);
}

XMIPP4_NODISCARD
array array::apply_subscripts(span<const dynamic_subscript> subscripts)
{
    if (m_implementation)
    {
        return array(m_implementation->apply_subscripts(subscripts));
    }
    else
    {
        return array(implementation().apply_subscripts(subscripts));
    }
}

XMIPP4_NODISCARD
array array::transpose()
{
    if (m_implementation)
    {
        return array(m_implementation->transpose());
    }
    else
    {
        return array(implementation().transpose());
    }
}

XMIPP4_NODISCARD
array array::permute(span<const std::size_t> order)
{
    if (m_implementation)
    {
        return array(m_implementation->permute(order));
    }
    else
    {
        return array(implementation().permute(order));
    }
}

XMIPP4_NODISCARD
array array::matrix_transpose(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
{
    if (m_implementation)
    {
        return array(m_implementation->matrix_transpose(axis1, axis2));
    }
    else
    {
        return array(implementation().matrix_transpose(axis1, axis2));
    }
}

XMIPP4_NODISCARD
array array::matrix_diagonal(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
{
    if (m_implementation)
    {
        return array(m_implementation->matrix_diagonal(axis1, axis2));
    }
    else
    {
        return array(implementation().matrix_diagonal(axis1, axis2));
    }
}

XMIPP4_NODISCARD
array array::squeeze()
{
    if (m_implementation)
    {
        return array(m_implementation->squeeze());
    }
    else
    {
        return array(implementation().squeeze());
    }
}

XMIPP4_NODISCARD 
array array::broadcast_to(span<const std::size_t> extents)
{
    if (m_implementation)
    {
        return array(m_implementation->broadcast_to(extents));
    }
    else
    {
        return array(implementation().broadcast_to(extents));
    }
}

} // namespace multidimensional
} // namespace xmipp4
