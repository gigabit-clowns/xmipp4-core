// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array_implementation
{
public:
	array_implementation() = default;
	array_implementation(
		std::shared_ptr<hardware::buffer> storage,
		array_descriptor descriptor
	) noexcept;
	array_implementation(const array_implementation &other) = default;
	array_implementation(array_implementation &&other) = default;
	~array_implementation() = default;

	array_implementation&
	operator=(const array_implementation &other) = default;
	array_implementation&
	operator=(array_implementation &&other) = default;

	const array_descriptor& get_descriptor() const noexcept;

	hardware::buffer* get_storage() const noexcept;

	const std::shared_ptr<hardware::buffer>& share_storage() const noexcept;

private:
	std::shared_ptr<hardware::buffer> m_storage;
	array_descriptor m_descriptor;

};

} // namespace multidimensional
} // namespace xmipp4

#include "array_implementation.inl"
