// SPDX-License-Identifier: GPL-3.0-only

#include <rexlib/em/image/image_batch_source.hpp>

#include <rexlib/core/concurrency/completion.hpp>
#include <rexlib/core/concurrency/counting_completion.hpp>
#include <rexlib/core/ndarray/array.hpp>
#include <rexlib/em/image/image_source.hpp>
#include <rexlib/em/image/image_location.hpp>
#include <rexlib/em/image/image_transaction_plan.hpp>

namespace rexlib
{
namespace em
{

image_batch_source::image_batch_source(
	std::shared_ptr<const image_source> source
)
	: m_source(std::move(source))
{
	if (!m_source)
	{
		throw std::invalid_argument(
			"image_batch_source: The downstream image source must not be null."
		);
	}
}

image_batch_source::~image_batch_source() = default;

std::shared_ptr<completion> image_batch_source::read(
	array destination,
	span<const image_location> locations
) const
{
	std::vector<std::size_t> array_extents;
	destination.get_descriptor().get_layout().get_extents(array_extents);
	
	if (array_extents.empty())
	{
		throw std::invalid_argument(
			"image_batch_source::read: destination array has no extents. "
			"Expected core_rank+1 extents"
		);
	}

	const auto batch_size = locations.size();
	if (array_extents.front() != batch_size)
	{
		throw std::invalid_argument(
			"image_batch_source::read: destination array's first dimension "
			"(batch size) must match the location count."
		);
	}

	if (batch_size == 0)
	{
		return std::make_shared<counting_completion>(0);
	}

	const auto stack_indexing = 
		locations.front().get_position_in_stack() != image_location::no_position;
	const auto array_rank = array_extents.size();
	const auto core_rank = array_rank - 1;
	const auto file_rank = stack_indexing ? array_rank : core_rank;
	const span<const std::size_t> core_extents(
		array_extents.data() + 1, 
		array_extents.size() - 1
	);

	image_transaction_plan transaction(
		core_extents,
		file_rank,
		array_rank
	);

	transaction.reserve(batch_size, batch_size);
	std::vector<std::size_t> file_offset(file_rank, 0UL);
	std::vector<std::size_t> array_offset(array_rank, 0UL);
	for (std::size_t i = 0; i < batch_size; ++i)
	{
		const auto &location = locations[i];
		const auto &file_path = location.get_path();	
		const auto position_in_stack = location.get_position_in_stack();
		const auto has_index = 
			position_in_stack != image_location::no_position;

		if (stack_indexing != has_index)
		{
			throw std::invalid_argument(
				"image_batch_source::read: mixing indexed locations and "
				"unindexed (no_position) locations is not allowed."
			);
		}

		array_offset[0] = i;
		if (stack_indexing)
		{
			file_offset[0] = position_in_stack;
		}

		transaction.add(
			transaction.add_file(file_path),
			make_span(file_offset),
			make_span(array_offset)
		);
	}

	REXLIB_ASSERT(m_source);
	return m_source->read(std::move(destination), transaction);
}

} // namespace em
} // namespace rexlib
