// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/kernel_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>
#include "../find_most_suitable_backend.hpp"

#include <vector>
#include <unordered_map>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_manager::implementation
{
public:
	bool register_kernel(std::unique_ptr<kernel_builder> builder)
	{
		XMIPP4_ASSERT(builder);
		const auto &op_id = builder->get_operation_id();
		m_builders[op_id].push_back(std::move(builder));
		return true;
	}

	kernel_builder* get_most_suitable_builder(
		const operation &operation,
		span<const array_descriptor> descriptors,
		hardware::device &device
	) const
	{
		const auto op_id = operation.get_id();
		const auto ite1 = m_builders.find(op_id);
		if (ite1 == m_builders.end())
		{
			return nullptr;
		}

		const auto &available_backends = ite1->second;
		const auto ite2 = find_most_suitable_backend(
			available_backends.begin(),
			available_backends.end(),
			[&operation, &descriptors, &device] (const auto &item)
			{
				return item->get_suitability(
					operation, 
					descriptors,
					device
				);
			}
		);

		if (ite2 == available_backends.cend())
		{
			return nullptr;
		}

		return ite2->get();
	}

	std::shared_ptr<kernel> build_kernel(
		const operation &operation,
		span<const array_descriptor> descriptors,
		hardware::device &device
	) const
	{
		const auto *builder = get_most_suitable_builder(
			operation, 
			descriptors,
			device
		);

		if (!builder)
		{
			throw invalid_operation_error(
				"Could not find a suitable kernel builder for the requested "
				"operation"
			);
		}

		return builder->build(operation, descriptors, device);
	}

private:
	std::unordered_map<
		operation_id, 
		std::vector<std::unique_ptr<kernel_builder>>
	> m_builders;

};

kernel_manager::kernel_manager() noexcept = default;
kernel_manager::~kernel_manager() = default;

void kernel_manager::register_builtin_backends()
{
	// Add operations here.
}

bool kernel_manager::register_kernel(std::unique_ptr<kernel_builder> builder)
{
	if (!builder)
	{
		return false;
	}

	return create_if_null().register_kernel(std::move(builder));
}

std::shared_ptr<kernel> kernel_manager::build_kernel(
	const operation &operation,
	span<const array_descriptor> descriptors,
	hardware::device &device
) const
{
	return get_implementation().build_kernel(operation, descriptors, device);
}

kernel_manager::implementation& kernel_manager::create_if_null()
{
	if (!m_implementation)
	{
		m_implementation = std::make_unique<implementation>();
	}

	return *m_implementation;
}

const kernel_manager::implementation& 
kernel_manager::get_implementation() const noexcept
{
	static const implementation empty_implementation;
	return m_implementation ? *m_implementation : empty_implementation;
}

} // namespace multidimensional
} // namespace xmipp4
