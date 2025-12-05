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
		span<const strided_layout> layouts,
		span<const numerical_type> data_types,
		hardware::device &device
	) const
	{
		const auto &op_id = operation.get_operation_id();
		const auto ite1 = m_builders.find(op_id);
		if (ite1 == m_builders.end())
		{
			return nullptr;
		}

		const auto &available_backends = ite1->second;
		const auto ite2 = find_most_suitable_backend(
			available_backends.begin(),
			available_backends.end(),
			[&] (const auto &item)
			{
				return item->get_suitability(
					operation, 
					layouts, 
					data_types, 
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
		span<const strided_layout> layouts,
		span<const numerical_type> data_types,
		hardware::device &device
	)
	{
		const auto *builder = get_most_suitable_builder(
			operation, 
			layouts, 
			data_types, 
			device
		);

		if (!builder)
		{
			throw invalid_operation_error(
				"Could not find a suitable kernel for the requested operation"
			);
		}

		return builder->build(operation, layouts, data_types, device);
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
	// No-op
}

bool kernel_manager::register_kernel(std::unique_ptr<kernel_builder> builder)
{
	if (!builder)
	{
		return false;
	}

	create_if_null();
	return m_implementation->register_kernel(std::move(builder));
}

std::shared_ptr<kernel> kernel_manager::build_kernel(
	const operation &operation,
	span<const strided_layout> layouts,
	span<const numerical_type> data_types,
	hardware::device &device
)
{
	if (!m_implementation)
	{
		return implementation().build_kernel(
			operation, 
			layouts, 
			data_types, 
			device
		);
	}

	return m_implementation->build_kernel(
		operation, 
		layouts, 
		data_types, 
		device
	);
}

void kernel_manager::create_if_null()
{
	if (!m_implementation)
	{
		m_implementation = std::make_unique<implementation>();
	}
}

} // namespace multidimensional
} // namespace xmipp4
