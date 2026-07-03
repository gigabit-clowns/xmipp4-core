// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_program_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/multidimensional/operation_program_builder.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>

#include <core/find_most_suitable_backend.hpp>
#include <cpu/multidimensional/programs/cpu_operation_program_registrar.hpp>

#include <vector>
#include <unordered_map>

namespace xmipp4
{
namespace multidimensional
{

class operation_program_manager::implementation
{
public:
	bool register_builder(
		std::unique_ptr<operation_program_builder> builder
	)
	{
		XMIPP4_ASSERT(builder);
		const auto &op_id = builder->get_operation_id();
		m_builders[op_id].push_back(std::move(builder));
		return true;
	}

	operation_program_builder* get_most_suitable_builder(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures,
		hardware::command_queue &queue
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
			[&operation, &output_signatures, &input_signatures, &queue]
			(const auto &item)
			{
				return item->get_suitability(
					operation,
					output_signatures,
					input_signatures,
					queue
				);
			}
		);

		if (ite2 == available_backends.cend())
		{
			return nullptr;
		}

		return ite2->get();
	}

	std::shared_ptr<hardware::program> build(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures,
		hardware::command_queue &queue,
		operation_program_cache *cache
	) const
	{
		const auto *builder = get_most_suitable_builder(
			operation,
			output_signatures,
			input_signatures,
			queue
		);

		if (!builder)
		{
			throw invalid_operation_error(
				"Could not find a suitable operation program builder for the "
				"requested operation"
			);
		}

		return builder->build(
			operation,
			output_signatures,
			input_signatures,
			queue,
			cache
		);
	}

private:
	std::unordered_map<
		operation_id,
		std::vector<std::unique_ptr<operation_program_builder>>
	> m_builders;

};

operation_program_manager::operation_program_manager() noexcept = default;
operation_program_manager::~operation_program_manager() = default;

void operation_program_manager::register_builtin_backends()
{
	register_cpu_operation_programs(*this);
}

bool operation_program_manager::register_builder(
	std::unique_ptr<operation_program_builder> builder
)
{
	if (!builder)
	{
		return false;
	}

	return create_if_null().register_builder(std::move(builder));
}

std::shared_ptr<hardware::program>
operation_program_manager::build(
	const operation &operation,
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures,
	hardware::command_queue &queue,
	operation_program_cache *cache
) const
{
	return get_implementation().build(
		operation,
		output_signatures,
		input_signatures,
		queue,
		cache
	);
}

operation_program_manager::implementation&
operation_program_manager::create_if_null()
{
	if (!m_implementation)
	{
		m_implementation = std::make_unique<implementation>();
	}

	return *m_implementation;
}

const operation_program_manager::implementation&
operation_program_manager::get_implementation() const noexcept
{
	static const implementation empty_implementation;
	return m_implementation ? *m_implementation : empty_implementation;
}

} // namespace multidimensional
} // namespace xmipp4
