// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_command_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/multidimensional/operation_command_builder.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>

#include <core/find_most_suitable_backend.hpp>

#include <vector>
#include <unordered_map>

namespace xmipp4 
{
namespace multidimensional
{

class operation_command_manager::implementation
{
public:
	bool register_builder(
		std::unique_ptr<operation_command_builder> builder
	)
	{
		XMIPP4_ASSERT(builder);
		const auto &op_id = builder->get_operation_id();
		m_builders[op_id].push_back(std::move(builder));
		return true;
	}

	operation_command_builder* get_most_suitable_builder(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures
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
			[&operation, &output_signatures, &input_signatures] 
			(const auto &item)
			{
				return item->get_suitability(
					operation, 
					output_signatures,
					input_signatures
				);
			}
		);

		if (ite2 == available_backends.cend())
		{
			return nullptr;
		}

		return ite2->get();
	}

	std::shared_ptr<hardware::command> build(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures,
		operation_command_cache *cache
	) const
	{
		const auto *builder = get_most_suitable_builder(
			operation,
			output_signatures,
			input_signatures
		);

		if (!builder)
		{
			throw invalid_operation_error(
				"Could not find a suitable operation command builder for the "
				"requested operation"
			);
		}

		return builder->build(
			operation, output_signatures, input_signatures, cache
		);
	}

private:
	std::unordered_map<
		operation_id, 
		std::vector<std::unique_ptr<operation_command_builder>>
	> m_builders;

};

operation_command_manager::operation_command_manager() noexcept = default;
operation_command_manager::~operation_command_manager() = default;

void operation_command_manager::register_builtin_backends()
{
	// Add operations here.
}

bool operation_command_manager::register_builder(
	std::unique_ptr<operation_command_builder> builder
)
{
	if (!builder)
	{
		return false;
	}

	return create_if_null().register_builder(std::move(builder));
}

std::shared_ptr<hardware::command>
operation_command_manager::build(
	const operation &operation,
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures,
	operation_command_cache *cache
) const
{
	return get_implementation().build(
		operation,
		output_signatures,
		input_signatures,
		cache
	);
}

operation_command_manager::implementation& 
operation_command_manager::create_if_null()
{
	if (!m_implementation)
	{
		m_implementation = std::make_unique<implementation>();
	}

	return *m_implementation;
}

const operation_command_manager::implementation& 
operation_command_manager::get_implementation() const noexcept
{
	static const implementation empty_implementation;
	return m_implementation ? *m_implementation : empty_implementation;
}

} // namespace multidimensional
} // namespace xmipp4
