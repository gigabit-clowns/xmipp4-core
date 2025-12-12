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
		array_descriptor descriptor
	) noexcept
		: m_storage(std::move(storage))
		, m_descriptor(std::move(descriptor))
	{
	}

	void set_descriptor(array_descriptor descriptor) noexcept
	{
		m_descriptor = std::move(descriptor);
	}

	const array_descriptor& get_descriptor() const noexcept
	{
		return m_descriptor;
	}

	void set_storage(std::shared_ptr<hardware::buffer> storage) noexcept
	{
		m_storage = std::move(storage);
	}

	hardware::buffer* get_storage() noexcept
	{
		return m_storage.get();
	}

	const std::shared_ptr<hardware::buffer>& share_storage() noexcept
	{
		return m_storage;
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
	array_descriptor descriptor
)
	: array(
		std::make_shared<implementation>(
			std::move(storage), 
			std::move(descriptor)
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

void array::set_descriptor(array_descriptor descriptor)
{
	if (!m_implementation)
	{
		m_implementation = std::make_shared<implementation>(
			nullptr, 
			std::move(descriptor)
		);
	}
	else
	{
		m_implementation->set_descriptor(std::move(descriptor));
	}
}

const array_descriptor& array::get_descriptor() const noexcept
{
	static array_descriptor empty_descriptor;
	return
		m_implementation ? 
		m_implementation->get_descriptor() : 
		empty_descriptor;
}

void array::set_storage(std::shared_ptr<hardware::buffer> storage)
{
	if (!m_implementation)
	{
		m_implementation = std::make_shared<implementation>(
			std::move(storage),
			array_descriptor()
		);
	}
	else
	{
		m_implementation->set_storage(std::move(storage));
	}
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

} // namespace multidimensional
} // namespace xmipp4
