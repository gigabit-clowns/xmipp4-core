// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class array::implementation
{
public:
	implementation() = default;
	implementation(
		std::shared_ptr<hardware::buffer> storage,
		strided_layout layout, 
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

	hardware::buffer* get_storage() noexcept
	{
		return m_storage.get();
	}

	const std::shared_ptr<hardware::buffer>& share_storage() noexcept
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
			share_storage(),
			get_layout().apply_subscripts(subscripts),
			get_data_type()
		);
	}

	implementation transpose()
	{
		return implementation(
			share_storage(),
			get_layout().transpose(),
			get_data_type()
		);
	}

	implementation permute(span<const std::size_t> order)
	{
		return implementation(
			share_storage(),
			get_layout().permute(order),
			get_data_type()
		);
	}

	implementation matrix_transpose(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
	{
		return implementation(
			share_storage(),
			get_layout().matrix_transpose(axis1, axis2),
			get_data_type()
		);
	}

	implementation matrix_diagonal(std::ptrdiff_t axis1, std::ptrdiff_t axis2)
	{
		return implementation(
			share_storage(),
			get_layout().matrix_diagonal(axis1, axis2),
			get_data_type()
		);
	}

	implementation squeeze()
	{
		return implementation(
			share_storage(),
			get_layout().squeeze(),
			get_data_type()
		);
	}

	implementation broadcast_to(span<const std::size_t> extents)
	{
		return implementation(
			share_storage(),
			get_layout().broadcast_to(extents),
			get_data_type()
		);
	}

	void record_queue(hardware::device_queue &queue, bool exclusive) const
	{
		if (!m_storage)
		{
			throw invalid_operation_error(
				"Can not call record_queue on an storage-less array"
			);
		}

		m_storage->record_queue(queue, exclusive);
	}

private:
	std::shared_ptr<hardware::buffer> m_storage;
	array_descriptor m_descriptor;

};

array::array() = default;
array::array(array&& other) noexcept = default;
array::~array() = default;
array& array::operator=(array&& other) noexcept = default;

array::array(
	std::shared_ptr<hardware::buffer> storage,
	strided_layout layout, 
	numerical_type data_type
)
	: array(
		std::make_shared<implementation>(
			std::move(storage), 
			std::move(layout), 
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

hardware::buffer* array::get_storage() noexcept
{
	return
		m_implementation ? 
		m_implementation->get_storage() : 
		nullptr;
}

const hardware::buffer* array::get_storage() const noexcept
{
	return
		m_implementation ? 
		m_implementation->get_storage() : 
		nullptr;
}

std::shared_ptr<const hardware::buffer> array::share_storage() const noexcept
{
	return
		m_implementation ? 
		m_implementation->share_storage() : 
		nullptr;
}

std::shared_ptr<hardware::buffer> array::share_storage() noexcept
{
	return
		m_implementation ? 
		m_implementation->share_storage() : 
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

void array::record_queue(hardware::device_queue &queue, bool exclusive) const
{
	if (!m_implementation)
	{
		throw invalid_operation_error(
			"Can not call record_queue on an uninitialized array"
		);
	}

	m_implementation->record_queue(queue, exclusive);
}

} // namespace multidimensional
} // namespace xmipp4
