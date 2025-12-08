// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
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
		: m_storage(std::move(storage))
		, m_descriptor(std::move(layout), data_type)
	{
	}

	const array_descriptor& get_descriptor() const noexcept
	{
		return m_descriptor;
	}

	numerical_type get_data_type() const noexcept
	{
		return m_descriptor.get_data_type();
	}

	const strided_layout& get_layout() const noexcept
	{
		return m_descriptor.get_layout();
	}

	storage& get_storage() noexcept
	{
		return m_storage;
	}

	std::size_t get_rank() const noexcept
	{
		return get_layout().get_rank();
	}

	void get_extents(std::vector<std::size_t> &extents) const
	{
		get_layout().get_extents(extents);
	}

	implementation apply_subscripts(span<const dynamic_subscript> subscripts)
	{
		return implementation(
			get_layout().apply_subscripts(subscripts),
			get_storage().share(),
			get_data_type()
		);
	}

	implementation transpose()
	{
		return implementation(
			get_layout().transpose(),
			get_storage().share(),
			get_data_type()
		);
	}

	implementation permute(span<const std::size_t> order)
	{
		return implementation(
			get_layout().permute(order),
			get_storage().share(),
			get_data_type()
		);
	}

	implementation matrix_transpose(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
	{
		return implementation(
			get_layout().matrix_transpose(axis1, axis2),
			get_storage().share(),
			get_data_type()
		);
	}

	implementation matrix_diagonal(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
	{
		return implementation(
			get_layout().matrix_diagonal(axis1, axis2),
			get_storage().share(),
			get_data_type()
		);
	}

	implementation squeeze()
	{
		return implementation(
			get_layout().squeeze(),
			get_storage().share(),
			get_data_type()
		);
	}

	implementation broadcast_to(span<const std::size_t> extents)
	{
		return implementation(
			get_layout().broadcast_to(extents),
			get_storage().share(),
			get_data_type()
		);
	}

private:
	storage m_storage;
	array_descriptor m_descriptor;

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

const array_descriptor& array::get_descriptor() const noexcept
{
	static array_descriptor empty_descriptor;
	return
		m_implementation ? 
		m_implementation->get_descriptor() : 
		empty_descriptor;
}

numerical_type array::get_data_type() const noexcept
{
	return get_descriptor().get_data_type();
}

const strided_layout& array::get_layout() const noexcept
{
	return get_descriptor().get_layout();
}		

storage* array::get_storage() noexcept
{
	return
		m_implementation ? 
		&(m_implementation->get_storage()) : 
		nullptr;
}

const storage* array::get_storage() const noexcept
{
	return
		m_implementation ? 
		&(m_implementation->get_storage()) : 
		nullptr;
}

XMIPP4_NODISCARD
std::size_t array::get_rank() const noexcept
{
	return get_layout().get_rank();
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
std::size_t compute_storage_requirement(
	const strided_layout &layout, 
	numerical_type data_type
) noexcept
{
	return layout.compute_storage_requirement() * get_size(data_type);
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

static
array* check_output_array(array *out, hardware::memory_allocator &allocator)
{
	const storage* storage;
    if (out && (storage = out->get_storage()))
    {
        if (storage->get_memory_resource() != &allocator.get_memory_resource())
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
storage cannibalize_output_array(
	array *out, 
	std::size_t storage_requirement
)
{
    if (out)
    {
		auto *out_storage = out->get_storage();
        if (out_storage && out_storage->get_size() >= storage_requirement)
        {
			return out_storage->share();
        }

		XMIPP4_LOG_WARN(
			"An array was provided for reuse but it does not have enough "
			"storage."
		);
    }

	return storage();
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
    const auto storage_requirement = 
		compute_storage_requirement(layout, data_type);

    out = check_output_array(out, allocator);
	auto storage = cannibalize_output_array(out, storage_requirement);
    if (storage.get_buffer() != nullptr)
    {
        const auto alignment = get_alignment_requirement(
            allocator, 
            storage_requirement
        );
        storage.rebind(
			allocator.allocate(
				storage_requirement,
				alignment,
				queue
  			)
		);
    }

    return array(
        std::move(layout),
        std::move(storage),
        data_type
    );
}

} // namespace multidimensional
} // namespace xmipp4
