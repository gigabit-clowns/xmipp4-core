// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <backends/cpu/loops/elementwise_loop.hpp>
#include <backends/cpu/loops/inner_loop_stride_dispatch.hpp>

#include <xmipp4/core/layout/joint_layout.hpp>
#include <xmipp4/core/layout/joint_layout_builder.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include <cstddef>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::cpu;

namespace
{

// Exact tag type each stride was resolved to. `other` catches any tag outside
// the documented vocabulary (contiguous_stride_tag, broadcasting_stride_tag,
// std::ptrdiff_t), so a regression to a bare integral_constant is detected.
enum class stride_kind
{
	contiguous,
	broadcasting,
	runtime,
	other,
};

/**
 * @brief Description of a single stride tag received by the kernel.
 */
struct stride_info
{
	std::ptrdiff_t value;
	stride_kind kind;

	bool operator==(const stride_info &other) const noexcept
	{
		return value == other.value && kind == other.kind;
	}
};

// A runtime stride is forwarded as a plain std::ptrdiff_t.
std::ptrdiff_t stride_value(std::ptrdiff_t s) noexcept
{
	return s;
}

// Statically typed strides are integral_constant-like and expose a
// compile-time value.
template <typename T>
std::ptrdiff_t stride_value(T) noexcept
{
	return T::value;
}

// Classify by exact tag type. The non-template overloads match only the
// documented tags; anything else falls through to the template overload.
stride_kind stride_classify(contiguous_stride_tag) noexcept
{
	return stride_kind::contiguous;
}
stride_kind stride_classify(broadcasting_stride_tag) noexcept
{
	return stride_kind::broadcasting;
}
stride_kind stride_classify(std::ptrdiff_t) noexcept
{
	return stride_kind::runtime;
}
template <typename T>
stride_kind stride_classify(T) noexcept
{
	return stride_kind::other;
}

/**
 * @brief Record of a single kernel invocation.
 */
struct elementwise_call
{
	std::vector<const int*> pointers;
	std::size_t count;
	std::vector<stride_info> strides;
};

/**
 * @brief Kernel that records every invocation.
 *
 * run_elementwise_loop invokes the kernel as
 * `(pointers..., count, stride_tags...)`, i.e. `n` operand pointers, the element
 * count, then `n` resolved stride tags. Those three groups are split apart and
 * stored for inspection. The log is held through a shared_ptr so that the copy
 * stored inside the loop shares state with the handle kept by the test.
 */
class recording_kernel
{
public:
	recording_kernel()
		: m_calls(std::make_shared<std::vector<elementwise_call>>())
	{
	}

	template <typename... Args>
	void operator()(Args... args) const
	{
		// Layout of the argument pack: n pointers, 1 count, n stride tags.
		XMIPP4_CONST_CONSTEXPR std::size_t n = (sizeof...(Args) - 1) / 2;
		const auto packed = std::make_tuple(args...);

		elementwise_call call;
		collect_pointers(call.pointers, packed, std::make_index_sequence<n>());
		call.count = static_cast<std::size_t>(std::get<n>(packed));
		collect_strides<n + 1>(
			call.strides,
			packed,
			std::make_index_sequence<n>()
		);
		m_calls->push_back(std::move(call));
	}

	const std::vector<elementwise_call>& calls() const noexcept
	{
		return *m_calls;
	}

private:
	std::shared_ptr<std::vector<elementwise_call>> m_calls;

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

	template <std::size_t Base, typename Tuple, std::size_t... Is>
	static void collect_strides(
		std::vector<stride_info> &out,
		const Tuple &packed,
		std::index_sequence<Is...>
	)
	{
		(void) std::initializer_list<int> {
			(
				out.push_back(
					stride_info {
						stride_value(std::get<Base + Is>(packed)),
						stride_classify(std::get<Base + Is>(packed))
					}
				),
				0
			)...
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
joint_layout make_layout(
	const std::vector<std::size_t> &extents,
	const std::vector<operand_spec> &operands
)
{
	joint_layout_builder builder;
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
	const elementwise_call &call,
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
	"run_elementwise_loop invokes the kernel once for a contiguous 1D layout",
	"[elementwise_loop]"
)
{
	auto buffer = make_buffer();
	recording_kernel recorder;
	run_elementwise_loop(
		recorder,
		make_layout({5}, {{{1}, 0}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 1 );
	CHECK( calls[0].count == 5 );
	CHECK(
		offsets_of(calls[0], buffer.data()) == std::vector<std::ptrdiff_t>{0}
	);
	REQUIRE( calls[0].strides.size() == 1 );
	CHECK( calls[0].strides[0] == stride_info{1, stride_kind::contiguous} );
}

TEST_CASE(
	"run_elementwise_loop resolves each operand's inner stride independently",
	"[elementwise_loop]"
)
{
	auto buffer = make_buffer();
	recording_kernel recorder;
	// Three operands sharing extents {4} with contiguous, broadcasting and
	// generic inner strides respectively.
	run_elementwise_loop(
		recorder,
		make_layout(
			{4},
			{
				{{1}, 0},
				{{0}, 1000},
				{{7}, 500}
			}
		),
		buffer.data(),
		buffer.data(),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 1 );
	CHECK( calls[0].count == 4 );
	const std::vector<stride_info> expected_strides = {
		{1, stride_kind::contiguous},
		{0, stride_kind::broadcasting},
		{7, stride_kind::runtime}
	};
	CHECK( calls[0].strides == expected_strides );
}

TEST_CASE(
	"run_elementwise_loop iterates the outer dimensions and passes the stride "
	"tags on every call",
	"[elementwise_loop]"
)
{
	auto buffer = make_buffer();
	recording_kernel recorder;
	// Inner extent 3 (dim 0, stride 1), outer extent 4 (dim 1, stride 100).
	run_elementwise_loop(
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
		REQUIRE( calls[i].strides.size() == 1 );
		CHECK( calls[i].strides[0] == stride_info{1, stride_kind::contiguous} );
	}
}

TEST_CASE(
	"run_elementwise_loop advances each operand by its own stride and base "
	"offset",
	"[elementwise_loop]"
)
{
	auto buffer = make_buffer();
	recording_kernel recorder;
	// Two operands sharing extents {2, 3}; the second one has a base offset.
	run_elementwise_loop(
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
		const std::vector<stride_info> expected_strides = {
			{1, stride_kind::contiguous},
			{1, stride_kind::contiguous}
		};
		CHECK( calls[i].strides == expected_strides );
	}
}

TEST_CASE(
	"run_elementwise_loop invokes the kernel once with count 1 and broadcasting "
	"tags for a rank-0 layout",
	"[elementwise_loop]"
)
{
	auto buffer = make_buffer();
	recording_kernel recorder;
	// Empty extents: a single element and no stride to inspect, so the operand
	// collapses to a broadcasting tag.
	run_elementwise_loop(
		recorder,
		make_layout({}, {{{}, 7}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 1 );
	CHECK( calls[0].count == 1 );
	CHECK(
		offsets_of(calls[0], buffer.data()) == std::vector<std::ptrdiff_t>{7}
	);
	REQUIRE( calls[0].strides.size() == 1 );
	CHECK( calls[0].strides[0] == stride_info{0, stride_kind::broadcasting} );
}

TEST_CASE(
	"run_elementwise_loop does not invoke the kernel for an empty axis",
	"[elementwise_loop]"
)
{
	auto buffer = make_buffer();
	recording_kernel recorder;
	run_elementwise_loop(
		recorder,
		make_layout({0}, {{{1}, 0}}),
		buffer.data()
	);

	CHECK( recorder.calls().empty() );
}

TEST_CASE(
	"run_elementwise_loop accepts an rvalue kernel",
	"[elementwise_loop]"
)
{
	auto buffer = make_buffer();
	recording_kernel recorder;
	// Pass a temporary kernel (sharing the recorder's log) to exercise the
	// forwarding reference.
	run_elementwise_loop(
		recording_kernel(recorder),
		make_layout({3}, {{{1}, 5}}),
		buffer.data()
	);

	const auto &calls = recorder.calls();
	REQUIRE( calls.size() == 1 );
	CHECK( calls[0].count == 3 );
	CHECK(
		offsets_of(calls[0], buffer.data()) == std::vector<std::ptrdiff_t>{5}
	);
	REQUIRE( calls[0].strides.size() == 1 );
	CHECK( calls[0].strides[0] == stride_info{1, stride_kind::contiguous} );
}
