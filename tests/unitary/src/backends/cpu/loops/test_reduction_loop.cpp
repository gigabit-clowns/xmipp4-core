// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <backends/cpu/loops/reduction_loop.hpp>

#include <xmipp4/core/layout/joint_layout.hpp>
#include <xmipp4/core/layout/joint_layout_builder.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/span.hpp>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::cpu;

namespace
{

/**
 * @brief Build a layout from extents and one stride vector per operand.
 */
joint_layout make_layout(
	const std::vector<std::size_t> &extents,
	const std::vector<std::vector<std::ptrdiff_t>> &strides
)
{
	joint_layout_builder builder;
	builder.set_extents(make_span(extents));
	for (const auto &operand : strides)
	{
		builder.add_operand(
			make_span(extents),
			make_span(operand),
			0
		);
	}
	return builder.build();
}

/**
 * @brief Tally of the calls a kernel received.
 */
struct call_log
{
	std::size_t seeds = 0;
	std::size_t combines = 0;
	std::size_t finalizes = 0;
	std::size_t identities = 0;
	std::vector<std::size_t> reported_counts;
	std::vector<std::size_t> positions;
};

/**
 * @brief Summing kernel that records every call it receives.
 *
 * The log is held through a shared_ptr so that the copy the loop takes
 * shares state with the handle kept by the test.
 */
class recording_kernel
{
public:
	recording_kernel()
		: m_log(std::make_shared<call_log>())
	{
	}

	template <typename Outputs, typename Inputs>
	struct accumulators
	{
		using type = type_list<int>;
	};

	void seed(int &accumulator, const int *value, std::size_t position) const
	{
		++m_log->seeds;
		m_log->positions.push_back(position);
		accumulator = *value;
	}

	void combine(
		int &accumulator,
		const int *value,
		std::size_t position
	) const
	{
		++m_log->combines;
		m_log->positions.push_back(position);
		accumulator += *value;
	}

	void merge(int &accumulator, const int &other) const
	{
		accumulator += other;
	}

	void finalize(int *result, const int &accumulator, std::size_t count) const
	{
		++m_log->finalizes;
		m_log->reported_counts.push_back(count);
		*result = accumulator;
	}

	void identity(int &accumulator) const
	{
		++m_log->identities;
		accumulator = 0;
	}

	const call_log& log() const noexcept
	{
		return *m_log;
	}

private:
	std::shared_ptr<call_log> m_log;
};

/**
 * @brief Summing kernel with no neutral element.
 */
struct seeded_only_kernel
{
	template <typename Outputs, typename Inputs>
	struct accumulators
	{
		using type = type_list<int>;
	};

	void seed(int &accumulator, const int *value, std::size_t) const
	{
		accumulator = *value;
	}

	void combine(int &accumulator, const int *value, std::size_t) const
	{
		accumulator += *value;
	}

	void merge(int &accumulator, const int &other) const
	{
		accumulator += other;
	}

	void finalize(int *result, const int &accumulator, std::size_t) const
	{
		*result = accumulator;
	}
};

/**
 * @brief Two accumulators of different types feeding one output.
 *
 * The shape a masked average has: a numerator and a denominator accumulated
 * apart and divided once the fold is over. No operation in the catalog needs
 * it yet, which is why it is exercised here.
 */
struct masked_mean_kernel
{
	template <typename Outputs, typename Inputs>
	struct accumulators
	{
		using type = type_list<double, std::size_t>;
	};

	void seed(
		double &total,
		std::size_t &count,
		const double *value,
		const bool *mask,
		std::size_t
	) const
	{
		total = *mask ? *value : 0.0;
		count = *mask ? 1u : 0u;
	}

	void combine(
		double &total,
		std::size_t &count,
		const double *value,
		const bool *mask,
		std::size_t
	) const
	{
		if (*mask)
		{
			total += *value;
			++count;
		}
	}

	void merge(
		double &total,
		std::size_t &count,
		const double &other_total,
		const std::size_t &other_count
	) const
	{
		total += other_total;
		count += other_count;
	}

	void finalize(
		double *result,
		const double &total,
		const std::size_t &count,
		std::size_t
	) const
	{
		*result = count ? total / static_cast<double>(count) : 0.0;
	}
};

/**
 * @brief Two accumulators feeding two outputs in one pass.
 */
struct minmax_kernel
{
	template <typename Outputs, typename Inputs>
	struct accumulators
	{
		using type = type_list<int, int>;
	};

	void seed(
		int &lowest,
		int &highest,
		const int *value,
		std::size_t
	) const
	{
		lowest = *value;
		highest = *value;
	}

	void combine(
		int &lowest,
		int &highest,
		const int *value,
		std::size_t
	) const
	{
		lowest = std::min(lowest, *value);
		highest = std::max(highest, *value);
	}

	void merge(
		int &lowest,
		int &highest,
		const int &other_lowest,
		const int &other_highest
	) const
	{
		lowest = std::min(lowest, other_lowest);
		highest = std::max(highest, other_highest);
	}

	void finalize(
		int *lowest_result,
		int *highest_result,
		const int &lowest,
		const int &highest,
		std::size_t
	) const
	{
		*lowest_result = lowest;
		*highest_result = highest;
	}
};

std::vector<int> iota_vector(std::size_t count)
{
	std::vector<int> result(count);
	std::iota(result.begin(), result.end(), 0);
	return result;
}

} // anonymous namespace

TEST_CASE(
	"run_reduction_loop should seed once and combine once per remaining "
	"element for each output",
	"[reduction_loop]"
)
{
	// A 4x3 matrix reduced along its outer axis, so three outputs of four
	// elements each.
	const auto input = iota_vector(12);
	std::vector<int> output(3, -1);

	const recording_kernel kernel;
	run_reduction_loop(
		kernel,
		make_layout({3}, {{1}, {1}}),
		make_layout({4}, {{3}}),
		4,
		std::make_tuple(output.data()),
		std::make_tuple(static_cast<const int*>(input.data()))
	);

	CHECK( kernel.log().seeds == 3 );
	CHECK( kernel.log().combines == 3 * 3 );
	CHECK( kernel.log().finalizes == 3 );
	CHECK( kernel.log().identities == 0 );
	CHECK( kernel.log().reported_counts ==
	       std::vector<std::size_t>(3, std::size_t(4)) );

	CHECK( output == std::vector<int>({18, 22, 26}) );
}

TEST_CASE(
	"run_reduction_loop should fold along the innermost axis",
	"[reduction_loop]"
)
{
	// The same matrix reduced along its contiguous axis instead, which is
	// the orientation that leaves one accumulator held across a run.
	const auto input = iota_vector(12);
	std::vector<int> output(4, -1);

	const recording_kernel kernel;
	run_reduction_loop(
		kernel,
		make_layout({4}, {{3}, {1}}),
		make_layout({3}, {{1}}),
		3,
		std::make_tuple(output.data()),
		std::make_tuple(static_cast<const int*>(input.data()))
	);

	CHECK( kernel.log().seeds == 4 );
	CHECK( kernel.log().combines == 4 * 2 );
	CHECK( kernel.log().finalizes == 4 );
	CHECK( output == std::vector<int>({3, 12, 21, 30}) );
}

TEST_CASE(
	"run_reduction_loop should fold every axis into a single output",
	"[reduction_loop]"
)
{
	const auto input = iota_vector(12);
	int output = -1;

	const recording_kernel kernel;
	run_reduction_loop(
		kernel,
		make_layout({}, {{}, {}}),
		make_layout({4, 3}, {{3, 1}}),
		12,
		std::make_tuple(&output),
		std::make_tuple(static_cast<const int*>(input.data()))
	);

	CHECK( kernel.log().seeds == 1 );
	CHECK( kernel.log().combines == 11 );
	CHECK( kernel.log().finalizes == 1 );
	CHECK( output == 66 );
}

TEST_CASE(
	"run_reduction_loop should copy when no axis is reduced",
	"[reduction_loop]"
)
{
	// A reduced space of rank zero holds one position, so every output is
	// seeded and finalized without a single combination.
	const auto input = iota_vector(6);
	std::vector<int> output(6, -1);

	const recording_kernel kernel;
	run_reduction_loop(
		kernel,
		make_layout({6}, {{1}, {1}}),
		make_layout({}, {{}}),
		1,
		std::make_tuple(output.data()),
		std::make_tuple(static_cast<const int*>(input.data()))
	);

	CHECK( kernel.log().seeds == 6 );
	CHECK( kernel.log().combines == 0 );
	CHECK( kernel.log().finalizes == 6 );
	CHECK( output == input );
}

TEST_CASE(
	"run_reduction_loop should complete outputs beyond one tile",
	"[reduction_loop]"
)
{
	// More surviving elements than a tile holds, and a count that is not a
	// multiple of it, so the last tile is partial.
	const std::size_t rows = 3;
	const std::size_t columns = 2500;
	REQUIRE( columns > reduction_tile_size<type_list<int>>::value );

	const auto input = iota_vector(rows * columns);
	std::vector<int> output(columns, -1);

	const recording_kernel kernel;
	run_reduction_loop(
		kernel,
		make_layout({columns}, {{1}, {1}}),
		make_layout({rows}, {{static_cast<std::ptrdiff_t>(columns)}}),
		rows,
		std::make_tuple(output.data()),
		std::make_tuple(static_cast<const int*>(input.data()))
	);

	CHECK( kernel.log().seeds == columns );
	CHECK( kernel.log().combines == columns * (rows - 1) );
	CHECK( kernel.log().finalizes == columns );

	std::vector<int> expected(columns);
	for (std::size_t c = 0; c < columns; ++c)
	{
		int total = 0;
		for (std::size_t r = 0; r < rows; ++r)
		{
			total += input[r * columns + c];
		}
		expected[c] = total;
	}
	CHECK( output == expected );
}

TEST_CASE(
	"run_reduction_loop should follow the strides of each operand",
	"[reduction_loop]"
)
{
	const auto input = iota_vector(6);

	SECTION( "a reversed input" )
	{
		int output = -1;
		const recording_kernel kernel;
		run_reduction_loop(
			kernel,
			make_layout({}, {{}, {}}),
			make_layout({6}, {{-1}}),
			6,
			std::make_tuple(&output),
			std::make_tuple(static_cast<const int*>(input.data() + 5))
		);

		CHECK( output == 15 );
	}

	SECTION( "an input broadcast along the reduced axis" )
	{
		int output = -1;
		const recording_kernel kernel;
		run_reduction_loop(
			kernel,
			make_layout({}, {{}, {}}),
			make_layout({4}, {{0}}),
			4,
			std::make_tuple(&output),
			std::make_tuple(static_cast<const int*>(input.data() + 5))
		);

		CHECK( kernel.log().combines == 3 );
		CHECK( output == 20 );
	}

	SECTION( "an output written with a stride" )
	{
		std::vector<int> output(6, -1);
		const recording_kernel kernel;
		run_reduction_loop(
			kernel,
			make_layout({3}, {{1}, {2}}),
			make_layout({2}, {{3}}),
			2,
			std::make_tuple(output.data()),
			std::make_tuple(static_cast<const int*>(input.data()))
		);

		CHECK( output == std::vector<int>({3, -1, 5, -1, 7, -1}) );
	}
}

TEST_CASE(
	"run_reduction_loop should answer a reduction over no elements with the "
	"identity",
	"[reduction_loop]"
)
{
	std::vector<int> output(3, -1);

	const recording_kernel kernel;
	run_reduction_loop(
		kernel,
		make_layout({3}, {{1}, {1}}),
		make_layout({0}, {{1}}),
		0,
		std::make_tuple(output.data()),
		std::make_tuple(static_cast<const int*>(nullptr))
	);

	CHECK( kernel.log().seeds == 0 );
	CHECK( kernel.log().combines == 0 );
	CHECK( kernel.log().identities == 3 );
	CHECK( kernel.log().finalizes == 3 );
	CHECK( output == std::vector<int>({0, 0, 0}) );
}

TEST_CASE(
	"run_reduction_loop should reject a reduction over no elements without "
	"an identity",
	"[reduction_loop]"
)
{
	STATIC_REQUIRE(
		has_reduction_identity<recording_kernel, type_list<int>>::value
	);
	STATIC_REQUIRE_FALSE(
		has_reduction_identity<seeded_only_kernel, type_list<int>>::value
	);

	std::vector<int> output(3, -1);

	CHECK_THROWS_AS(
		run_reduction_loop(
			seeded_only_kernel(),
			make_layout({3}, {{1}, {1}}),
			make_layout({0}, {{1}}),
			0,
			std::make_tuple(output.data()),
			std::make_tuple(static_cast<const int*>(nullptr))
		),
		std::invalid_argument
	);
}

TEST_CASE(
	"run_reduction_loop should do nothing when the outputs are empty",
	"[reduction_loop]"
)
{
	const auto input = iota_vector(6);

	const recording_kernel kernel;
	run_reduction_loop(
		kernel,
		make_layout({0}, {{1}, {1}}),
		make_layout({6}, {{1}}),
		6,
		std::make_tuple(static_cast<int*>(nullptr)),
		std::make_tuple(static_cast<const int*>(input.data()))
	);

	CHECK( kernel.log().seeds == 0 );
	CHECK( kernel.log().finalizes == 0 );
}

TEST_CASE(
	"run_reduction_loop should keep several accumulators apart",
	"[reduction_loop]"
)
{
	// Two accumulators of unrelated types, fed by two inputs and answering
	// through one output.
	const std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

	// A plain array rather than a vector, std::vector<bool> having no
	// elements to point at.
	const bool mask[] = {true, false, true, true, false, true};

	std::vector<double> output(3, -1.0);

	run_reduction_loop(
		masked_mean_kernel(),
		make_layout({3}, {{1}, {1}, {1}}),
		make_layout({2}, {{3}, {3}}),
		2,
		std::make_tuple(output.data()),
		std::make_tuple(
			static_cast<const double*>(values.data()),
			static_cast<const bool*>(mask)
		)
	);

	// Column 0 keeps 1 and 4, column 1 keeps neither 2 nor 5, column 2 keeps
	// both 3 and 6.
	CHECK( output[0] == 2.5 );
	CHECK( output[1] == 0.0 );
	CHECK( output[2] == 4.5 );
}

TEST_CASE(
	"run_reduction_loop should write several outputs in one pass",
	"[reduction_loop]"
)
{
	const std::vector<int> input = {5, 1, 9, 3, 7, 2};
	std::vector<int> lowest(3, -1);
	std::vector<int> highest(3, -1);

	run_reduction_loop(
		minmax_kernel(),
		make_layout({3}, {{1}, {1}, {1}}),
		make_layout({2}, {{3}}),
		2,
		std::make_tuple(lowest.data(), highest.data()),
		std::make_tuple(static_cast<const int*>(input.data()))
	);

	CHECK( lowest == std::vector<int>({3, 1, 2}) );
	CHECK( highest == std::vector<int>({5, 7, 9}) );
}

TEST_CASE(
	"reduction_tile_size should shrink as the accumulators grow",
	"[reduction_loop]"
)
{
	// A tile is sized to keep its accumulators resident, so declaring more
	// of them, or larger ones, shortens it rather than spilling.
	using one_small = type_list<int>;
	using one_large = type_list<double>;
	using two_large = type_list<double, double>;

	CHECK( accumulator_footprint<one_small>::value == sizeof(int) );
	CHECK( accumulator_footprint<two_large>::value == 2 * sizeof(double) );

	CHECK( reduction_tile_size<one_large>::value <
	       reduction_tile_size<one_small>::value );
	CHECK( reduction_tile_size<two_large>::value <
	       reduction_tile_size<one_large>::value );
	CHECK( reduction_tile_size<two_large>::value > 0 );
}
