// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/logger.hpp>

namespace xmipp4 
{
namespace multidimensional
{


class array::implementation
{
public:
	implementation() = default;
	implementation(
		strided_layout layout, 
		storage storage,
		numerical_type data_type
	) noexcept
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

	storage& get_storage() noexcept
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
			m_storage.view(),
			get_data_type()
		);
	}

	implementation transpose()
	{
		return implementation(
			get_layout().transpose(),
			m_storage.view(),
			get_data_type()
		);
	}

	implementation permute(span<const std::size_t> order)
	{
		return implementation(
			get_layout().permute(order),
			m_storage.view(),
			get_data_type()
		);
	}

	implementation matrix_transpose(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
	{
		return implementation(
			get_layout().matrix_transpose(axis1, axis2),
			m_storage.view(),
			get_data_type()
		);
	}

	implementation matrix_diagonal(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
	{
		return implementation(
			get_layout().matrix_diagonal(axis1, axis2),
			m_storage.view(),
			get_data_type()
		);
	}

	implementation squeeze()
	{
		return implementation(
			get_layout().squeeze(),
			m_storage.view(),
			get_data_type()
		);
	}

	implementation broadcast_to(span<const std::size_t> extents)
	{
		return implementation(
			get_layout().broadcast_to(extents),
			m_storage.view(),
			get_data_type()
		);
	}

private:
	strided_layout m_layout;
	storage m_storage;
	numerical_type m_data_type;
};

array::array() = default;
array::array(array&& other) noexcept = default;
array::~array() = default;
array& array::operator=(array&& other) noexcept = default;

array::array(
	strided_layout layout, 
	storage storage, 
	numerical_type data_type
)
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
	return
		m_implementation ? 
		m_implementation->get_data_type() : 
		numerical_type::unknown;
}

const strided_layout& array::get_layout() const noexcept
{
	// TODO
}

storage& array::get_storage() noexcept
{
	// TODO
}

const storage& array::get_storage() const noexcept
{
	// TODO
}

XMIPP4_NODISCARD
std::size_t array::get_rank() const noexcept
{
	return m_implementation ? m_implementation->get_rank() : 0U;
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

static
array* check_output_array(array *out, hardware::memory_allocator &allocator)
{
    if (out)
    {
        const auto &storage = out->get_storage();
        if (&storage.get_memory_resource() != &allocator.get_memory_resource())
        {
            XMIPP4_LOG_WARN(
                "An array was provided for reuse but it uses a different "
                "memory resource than the allocator."
            );
            return nullptr;
        }
    }

    return out;
}

static 
std::size_t get_alignment_requirement(
    const hardware::memory_allocator &allocator,
    std::size_t size
)
{
    size = binary::bit_ceil(size);
    const std::size_t max_alignment = allocator.get_max_alignment();
    const std::size_t preferred_alignment = 256; // TODO: query device for preferred alignment
    return std::min(std::min(max_alignment, preferred_alignment), size);
}

XMIPP4_NODISCARD 
array array::empty(
    strided_layout layout, 
    numerical_type data_type,
    hardware::memory_allocator &allocator,
    hardware::device_queue *queue,
    array *out
)
{
	/*
    out = check_output_array(out, allocator);

    // Check if we can reuse the output array as-is.
    if (out && out->get_data_type() == data_type && out->get_layout() == layout)
    {
        return out->view();
    }

    const auto storage_requirement = 
        layout.compute_storage_requirement() * get_size(data_type);
    std::shared_ptr<hardware::buffer> storage;

    // Check if we can reuse the output array's storage.
    if (out)
    {
        storage = out->share_storage();
        XMIPP4_ASSERT( storage ); // Checked in check_output_array
        if (storage->get_size() < storage_requirement)
        {
            XMIPP4_LOG_WARN(
                "An array was provided for reuse but it does not have enough "
                "storage."
            );
            storage.reset();
        }
    }
    
    // Allocate new storage if needed.
    if (!storage)
    {
        const auto alignment = get_alignment_requirement(
            allocator, 
            storage_requirement
        );
        storage = allocator.allocate(
            storage_requirement,
            alignment,
            queue
        );
    }

    XMIPP4_ASSERT( storage );
    return array(
        std::move(layout),
        std::move(storage),
        data_type
    );
	*/
}

} // namespace multidimensional
} // namespace xmipp4
