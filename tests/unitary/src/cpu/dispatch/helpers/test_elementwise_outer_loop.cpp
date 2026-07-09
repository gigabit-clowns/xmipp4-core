// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <cpu/dispatch/helpers/elementwise_outer_loop.hpp>

#include <xmipp4/core/layout/access_layout.hpp>
#include <xmipp4/core/layout/access_layout_builder.hpp>
#include <xmipp4/core/span.hpp>

#include <cstddef>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::dispatch;
using namespace xmipp4::layout;

namespace
{

/**
 * @brief Record of a single inner-loop invocation.
 */
struct outer_loop_call
{
	std::vector<const int*> pointers;
	std::size_t count;
};

/**
 * @brief Inner-loop functor that records every invocation.
 *
 * The recorded log is held through a shared_ptr so that the copy stored inside
 * the elementwise_outer_loop shares state with the handle kept by the test.
 */
class recording_inner_loop
{
public:
	recording_inner_loop()
		: m_calls(std::make_shared<std::vector<outer_loop_call>>())
	{
	}

	// The outer loop invokes the handler as (pointers..., count). The count is
	// the last argument, so it is captured together with the pointers and split
	// out afterwards (a trailing std::size_t after a pack is a non-deduced
	// context).
	template <typename... Args>
	void operator()(Args... args) const
	{
		constexpr std::size_t count_index = sizeof...(Args) - 1;
		const auto packed = std::make_tuple(args...);

		outer_loop_call call;
		call.count = static_cast<std::size_t>(std::get<count_index>(packed));
		collect_pointers(
			call.pointers,
			packed,
			std::make_index_sequence<count_index>()
		);
		m_calls->push_back(std::move(call));
	}

	const std::vector<outer_loop_call>& calls() const noexcept
	{
		return *m_calls;
	}

private:
	std::shared_ptr<std::vector<outer_loop_call>> m_calls;

	template <typename Tuple, std::size_t... Is>
	static void collect_pointers(
		std::vector<const int*> &out,
		const Tuple &packed,
		std::index_sequence<Is...>
	)
	{
		(void) std::initializer_list<int> {
			(out.push_back(std::get<Is>(packed)), 0)...
		};
	}
};

/**
 * @brief Description of one operand for make_layout.
 */
struct operand_spec
{
	std::vector<std::ptrdiff_t> strides;
	std::ptrdiff_t offset;
};

/**
 * @brief Build a layout with the provided iteration extents and operands.
 *
 * Optimizations are disabled so that the axis order (dim 0 being the inner
 * loop) and the strides are preserved verbatim.
 */
access_layout make_layout(
	const std::vector<std::size_t> &extents,
	const std::vector<operand_spec> &operands
)
{
	access_layout_builder builder;
	builder.set_extents(make_span(extents));
	for (const auto &operand : operands)
	{
		builder.add_operand(
			make_span(extents),
			make_span(operand.strides),
			operand.offset
		);
	}
	return builder.build({});
}

// Backing storage the operand pointers point into. Large enough that all the
// offsets produced by the tests stay in bounds.
std::vector<int> make_buffer()
{
	return std::vector<int>(2048);
}

// Offsets of a call's pointers relative to the common base pointer.
std::vector<std::ptrdiff_t> offsets_of(
	const outer_loop_call &call,
	const int *base
)
{
	std::vector<std::ptrdiff_t> result;
	for (const auto *pointer : call.pointers)
	{
		result.push_back(pointer - base);
	}
	return result;
}

} // namespace



TEST_CASE(
	"elementwise_outer_loop invokes the handler once for a 1D layout",
	"[elementwise_outer_loop]"
)
{
	auto buffer = make_buffer();
	recording_inner_loop recorder;
	run_elementwise_outer_loop(
		recorder,
		make_layout({5}, {{{1}, 0}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 1 );
	CHECK( calls[0].count == 5 );
	CHECK( offsets_of(calls[0], buffer.data()) == std::vector<std::ptrdiff_t>{0} );
}

TEST_CASE(
	"elementwise_outer_loop iterates over the outer dimension of a 2D "
	"layout",
	"[elementwise_outer_loop]"
)
{
	auto buffer = make_buffer();
	recording_inner_loop recorder;
	// Inner extent 3 (dim 0), outer extent 4 (dim 1) with outer stride 100.
	run_elementwise_outer_loop(
		recorder,
		make_layout({3, 4}, {{{1, 100}, 0}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 4 );
	const std::vector<std::ptrdiff_t> expected_offsets = {0, 100, 200, 300};
	for (std::size_t i = 0; i < calls.size(); ++i)
	{
		CHECK( calls[i].count == 3 );
		CHECK(
			offsets_of(calls[i], buffer.data()) ==
			std::vector<std::ptrdiff_t>{expected_offsets[i]}
		);
	}
}

TEST_CASE(
	"elementwise_outer_loop varies the innermost outer dimension fastest",
	"[elementwise_outer_loop]"
)
{
	auto buffer = make_buffer();
	recording_inner_loop recorder;
	// Extents {2, 2, 2}: dim 1 (stride 10) varies before dim 2 (stride 100).
	run_elementwise_outer_loop(
		recorder,
		make_layout({2, 2, 2}, {{{1, 10, 100}, 0}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 4 );
	const std::vector<std::ptrdiff_t> expected_offsets = {0, 10, 100, 110};
	for (std::size_t i = 0; i < calls.size(); ++i)
	{
		CHECK( calls[i].count == 2 );
		CHECK(
			offsets_of(calls[i], buffer.data()) ==
			std::vector<std::ptrdiff_t>{expected_offsets[i]}
		);
	}
}

TEST_CASE(
	"elementwise_outer_loop advances each operand by its own stride and "
	"base offset",
	"[elementwise_outer_loop]"
)
{
	auto buffer = make_buffer();
	recording_inner_loop recorder;
	// Two operands sharing extents {2, 3}. The second one has a base offset.
	run_elementwise_outer_loop(
		recorder,
		make_layout(
			{2, 3},
			{
				{{1, 10}, 0},
				{{1, 20}, 1000}
			}
		),
		buffer.data(),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 3 );
	const std::vector<std::vector<std::ptrdiff_t>> expected = {
		{0, 1000},
		{10, 1020},
		{20, 1040}
	};
	for (std::size_t i = 0; i < calls.size(); ++i)
	{
		CHECK( calls[i].count == 2 );
		CHECK( offsets_of(calls[i], buffer.data()) == expected[i] );
	}
}

TEST_CASE(
	"elementwise_outer_loop invokes the handler once with count 1 for a "
	"rank-0 layout",
	"[elementwise_outer_loop]"
)
{
	auto buffer = make_buffer();
	recording_inner_loop recorder;
	// Empty extents, single operand with a base offset.
	run_elementwise_outer_loop(
		recorder,
		make_layout({}, {{{}, 7}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 1 );
	CHECK( calls[0].count == 1 );
	CHECK( offsets_of(calls[0], buffer.data()) == std::vector<std::ptrdiff_t>{7} );
}

TEST_CASE(
	"elementwise_outer_loop does not invoke the handler for an empty axis",
	"[elementwise_outer_loop]"
)
{
	auto buffer = make_buffer();
	recording_inner_loop recorder;
	run_elementwise_outer_loop(
		recorder,
		make_layout({0}, {{{1}, 0}}),
		buffer.data()
	);

	CHECK( recorder.calls().empty() );
}

TEST_CASE(
	"run_elementwise_outer_loop accepts an rvalue inner loop",
	"[elementwise_outer_loop]"
)
{
	auto buffer = make_buffer();
	recording_inner_loop recorder;
	// Pass a temporary inner loop (sharing the recorder's log) to exercise the
	// forwarding reference.
	run_elementwise_outer_loop(
		recording_inner_loop(recorder),
		make_layout({3}, {{{1}, 5}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 1 );
	CHECK( calls[0].count == 3 );
	CHECK( offsets_of(calls[0], buffer.data()) == std::vector<std::ptrdiff_t>{5} );
}
