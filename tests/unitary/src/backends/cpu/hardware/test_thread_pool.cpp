// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/backends/cpu/parallel_for.hpp>
#include <xmipp4/backends/cpu/thread_pool.hpp>

#include <xmipp4/core/platform/constexpr.hpp>
#include <xmipp4/core/platform/operating_system.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#if XMIPP4_POSIX
	#include <signal.h>
	#include <sys/wait.h>
	#include <unistd.h>
#endif

using namespace xmipp4;
using namespace xmipp4::cpu;

namespace
{

using range = std::pair<std::size_t, std::size_t>;

/**
 * @brief Record the range of every chunk a job was cut into.
 *
 * The pool invokes bodies concurrently, so the record is guarded. The order
 * the ranges arrive in is a matter of timing; they are sorted before being
 * asserted on.
 */
class range_recorder
{
public:
	void operator()(std::size_t begin, std::size_t end) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_ranges.emplace_back(begin, end);
	}

	std::vector<range> take_sorted() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto result = m_ranges;
		std::sort(result.begin(), result.end());
		return result;
	}

private:
	mutable std::mutex m_mutex;
	mutable std::vector<range> m_ranges;
};

/**
 * @brief Whether the ranges tile [0, count) exactly, in order and without gaps.
 */
bool covers(const std::vector<range> &ranges, std::size_t count)
{
	std::size_t expected = 0;
	for (const auto &r : ranges)
	{
		if (r.first != expected || r.second <= r.first)
		{
			return false;
		}
		expected = r.second;
	}

	return expected == count;
}

} // anonymous namespace


TEST_CASE(
	"thread_pool should count the calling thread among its participants",
	"[thread_pool]"
)
{
	// A pool of no worker still has one participant, so that a caller never
	// has to special case it.
	CHECK( thread_pool(0).get_size() == 1 );
	CHECK( thread_pool(3).get_size() == 4 );
}

TEST_CASE(
	"parallel_for should run the whole range on the calling thread when the "
	"pool has no worker",
	"[thread_pool]"
)
{
	thread_pool pool(0);

	range_recorder recorder;
	const auto caller = std::this_thread::get_id();
	std::atomic<bool> same_thread(true);
	const auto body =
		[&recorder, &caller, &same_thread]
		(std::size_t begin, std::size_t end)
		{
			if (std::this_thread::get_id() != caller)
			{
				same_thread.store(false);
			}
			recorder(begin, end);
		};

	parallel_for(pool, 1000, 1, body);

	CHECK( recorder.take_sorted() == std::vector<range>{ { 0, 1000 } } );
	CHECK( same_thread.load() );
}

TEST_CASE(
	"parallel_for should cut the range into one chunk per participant",
	"[thread_pool]"
)
{
	thread_pool pool(3);

	range_recorder recorder;
	parallel_for(pool, 100, 1, std::ref(recorder));

	const auto ranges = recorder.take_sorted();
	CHECK( ranges.size() == 4 );
	CHECK( covers(ranges, 100) );
}

TEST_CASE(
	"parallel_for should balance a range that does not divide evenly",
	"[thread_pool]"
)
{
	// 10 over 4 participants is two chunks of 3 and two of 2, the longer ones
	// first. The partition depends only on the count, the grain and the pool
	// size, never on timing, so it can be asserted exactly.
	thread_pool pool(3);

	range_recorder recorder;
	parallel_for(pool, 10, 1, std::ref(recorder));

	CHECK( recorder.take_sorted() ==
	       std::vector<range>{ { 0, 3 }, { 3, 6 }, { 6, 8 }, { 8, 10 } } );
}

TEST_CASE(
	"parallel_for should not split work below one grain",
	"[thread_pool]"
)
{
	thread_pool pool(3);

	range_recorder recorder;
	parallel_for(pool, 100, 1000, std::ref(recorder));

	CHECK( recorder.take_sorted() == std::vector<range>{ { 0, 100 } } );
}

TEST_CASE(
	"parallel_for should give every chunk at least one grain",
	"[thread_pool]"
)
{
	// 100 iterations at a grain of 40 affords two chunks, not the four the
	// pool could otherwise fill.
	thread_pool pool(3);

	range_recorder recorder;
	parallel_for(pool, 100, 40, std::ref(recorder));

	const auto ranges = recorder.take_sorted();
	CHECK( ranges.size() == 2 );
	CHECK( covers(ranges, 100) );
}

TEST_CASE(
	"parallel_for should not invoke the body for an empty range",
	"[thread_pool]"
)
{
	thread_pool pool(3);

	range_recorder recorder;
	parallel_for(pool, 0, 1, std::ref(recorder));

	CHECK( recorder.take_sorted().empty() );
}

TEST_CASE(
	"parallel_for should visit every iteration exactly once",
	"[thread_pool]"
)
{
	XMIPP4_CONST_CONSTEXPR std::size_t count = 10000;
	thread_pool pool(4);

	std::vector<int> visits(count, 0);
	const auto body =
		[&visits] (std::size_t begin, std::size_t end)
		{
			for (std::size_t i = begin; i < end; ++i)
			{
				++visits[i];
			}
		};

	parallel_for(pool, count, 1, body);

	CHECK( std::count(visits.cbegin(), visits.cend(), 1) ==
	       static_cast<std::ptrdiff_t>(count) );
}

TEST_CASE(
	"parallel_for should be reusable for one job after another",
	"[thread_pool]"
)
{
	// The workers sleep between jobs, so this is what catches a wake-up lost
	// to a worker that was still draining the previous one.
	thread_pool pool(4);

	std::atomic<std::size_t> total(0);
	const auto body =
		[&total] (std::size_t begin, std::size_t end)
		{
			total.fetch_add(end - begin);
		};

	for (std::size_t i = 0; i < 1000; ++i)
	{
		parallel_for(pool, 100, 1, body);
	}

	CHECK( total.load() == 100*1000 );
}

TEST_CASE(
	"parallel_for should run a nested loop inline rather than deadlock",
	"[thread_pool]"
)
{
	// A job holds the pool for the whole of its duration, so the inner loop
	// finds it taken and is handed its whole range on the thread that asked
	// for it. Waiting instead would be waiting on the very threads that are
	// running this body. Repeated, because the failure it guards against is
	// a race rather than a misreading.
	thread_pool pool(3);

	std::atomic<std::size_t> inner_calls(0);
	std::atomic<std::size_t> inner_total(0);
	const auto body =
		[&pool, &inner_calls, &inner_total] (std::size_t, std::size_t)
		{
			const auto inner =
				[&inner_calls, &inner_total]
				(std::size_t begin, std::size_t end)
				{
					inner_calls.fetch_add(1);
					inner_total.fetch_add(end - begin);
				};

			parallel_for(pool, 100, 1, inner);
		};

	for (std::size_t i = 0; i < 100; ++i)
	{
		parallel_for(pool, 4, 1, body);
	}

	CHECK( inner_calls.load() == 4*100 );
	CHECK( inner_total.load() == 100*4*100 );
}

TEST_CASE(
	"parallel_for should rethrow what the body threw on the calling thread",
	"[thread_pool]"
)
{
	thread_pool pool(3);

	const auto body =
		[] (std::size_t, std::size_t)
		{
			throw std::runtime_error("from a chunk");
		};

	REQUIRE_THROWS_AS( parallel_for(pool, 100, 1, body), std::runtime_error );

	SECTION("and the pool is usable afterwards")
	{
		std::atomic<std::size_t> total(0);
		const auto counter =
			[&total] (std::size_t begin, std::size_t end)
			{
				total.fetch_add(end - begin);
			};

		REQUIRE_NOTHROW( parallel_for(pool, 100, 1, counter) );
		CHECK( total.load() == 100 );
	}
}

TEST_CASE(
	"parallel_for should report the lowest numbered chunk that threw",
	"[thread_pool]"
)
{
	// Every chunk throws, so which exception survives is decided by the rule
	// rather than by which core got there first. The lowest one wins, which
	// is what makes the message a user sees reproducible.
	thread_pool pool(3);

	const auto body =
		[] (std::size_t begin, std::size_t)
		{
			throw std::logic_error(std::to_string(begin));
		};

	for (std::size_t i = 0; i < 100; ++i)
	{
		try
		{
			parallel_for(pool, 100, 1, body);
			FAIL( "Expected the body's exception to propagate." );
		}
		catch (const std::logic_error &e)
		{
			CHECK( std::string(e.what()) == "0" );
		}
	}
}

TEST_CASE(
	"thread_pool::get_default should always answer the same pool",
	"[thread_pool]"
)
{
	const auto &first = thread_pool::get_default();
	const auto &second = thread_pool::get_default();

	REQUIRE( first != nullptr );
	CHECK( first == second );
	CHECK( first->get_size() >= 1 );
}

#if XMIPP4_POSIX

namespace
{

/**
 * @brief Run a body in a forked child and report how it ended.
 *
 * The child never returns to the caller: it reports through its exit status
 * and leaves through exit, so that its static destructors run. That is
 * deliberate and is most of the point. A child leaving through _exit would
 * take none of its threads down and would never destroy the pool it
 * inherited, which is the very thing that hangs.
 *
 * Buffered output is flushed before the fork, so that the child does not
 * inherit a copy of it and print it a second time on its way out.
 *
 * The parent polls rather than blocking, because the whole point of these
 * cases is a child that hangs, and waiting for one of those would hang the
 * suite instead of failing it.
 *
 * @param body Run in the child. Returns the status to exit with.
 * @param timeout How long to give the child before giving up on it.
 * @return int The child's exit status, or -1 if it had to be killed.
 */
template <typename Body>
int run_in_forked_child(
	const Body &body,
	std::chrono::milliseconds timeout = std::chrono::seconds(10)
)
{
	std::fflush(nullptr);

	const auto child = ::fork();
	REQUIRE( child >= 0 );

	if (child == 0)
	{
		std::exit(body());
	}

	const auto deadline = std::chrono::steady_clock::now() + timeout;
	for (;;)
	{
		int status = 0;
		const auto waited = ::waitpid(child, &status, WNOHANG);
		REQUIRE( waited >= 0 );

		if (waited == child)
		{
			return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
		}

		if (std::chrono::steady_clock::now() >= deadline)
		{
			::kill(child, SIGKILL);
			::waitpid(child, &status, 0);
			return -1;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

/**
 * @brief Sum [0, count) through a pool, as a check that it still works.
 *
 * @return bool Whether every iteration was visited exactly once.
 */
bool pool_still_works(thread_pool &pool, std::size_t count)
{
	std::atomic<std::size_t> total(0);
	const auto body =
		[&total] (std::size_t begin, std::size_t end)
		{
			for (std::size_t i = begin; i < end; ++i)
			{
				total.fetch_add(i);
			}
		};

	parallel_for(pool, count, 1, body);

	return total.load() == count*(count - 1)/2;
}

} // anonymous namespace

TEST_CASE(
	"a thread pool should still work in a forked child",
	"[thread_pool]"
)
{
	// Without the fork handlers the child inherits handles naming threads that
	// no longer exist and mutexes no surviving thread can unlock. It does not
	// fail, it hangs, which is why the child is given a deadline.
	// Held by pointer so that the child can let go of it before leaving.
	// exit() does not unwind, so a pool living in this frame would keep its
	// respawned workers running right through the child's exit.
	auto pool = std::unique_ptr<thread_pool>(new thread_pool(4));

	// Repeated, because the deadlock this targets needs a worker to be holding
	// the mutex at the instant of the fork, and one fork may well miss it.
	// Running a loop just before each fork is what puts the workers in that
	// window, on their way back to sleep.
	for (int attempt = 0; attempt < 16; ++attempt)
	{
		REQUIRE( pool_still_works(*pool, 1000) );

		const auto status = run_in_forked_child(
			[&pool] ()
			{
				if (!pool_still_works(*pool, 1000))
				{
					return 1;
				}

				// The workers were put back, so the child is as parallel as
				// the parent was rather than quietly serial.
				const auto parallel = pool->get_size() == 5;
				pool.reset();
				return parallel ? 0 : 2;
			}
		);

		INFO( "attempt " << attempt );
		REQUIRE( status == 0 );
	}
}

TEST_CASE(
	"a thread pool inherited by a forked child should be destructible there",
	"[thread_pool]"
)
{
	// The one failure that needs no luck at all. Every handle the child
	// inherits still reports itself joinable, so joining one waits on a thread
	// that no longer exists and will never exit. A child destroying an
	// inherited pool hangs every time, which is what a child exiting through
	// the normal path does to the static one.
	auto pool = std::unique_ptr<thread_pool>(new thread_pool(4));
	REQUIRE( pool_still_works(*pool, 1000) );

	const auto status = run_in_forked_child(
		[&pool] ()
		{
			pool.reset();
			return 0;
		}
	);

	CHECK( status == 0 );
}

TEST_CASE(
	"a thread pool should let a forked child build one of its own",
	"[thread_pool]"
)
{
	auto pool = std::unique_ptr<thread_pool>(new thread_pool(4));
	REQUIRE( pool_still_works(*pool, 1000) );

	const auto status = run_in_forked_child(
		[&pool] ()
		{
			int result;
			{
				thread_pool child_pool(2);
				result = pool_still_works(child_pool, 1000) ? 0 : 1;
			}

			pool.reset();
			return result;
		}
	);

	CHECK( status == 0 );
}

TEST_CASE(
	"a thread pool should survive being forked more than once",
	"[thread_pool]"
)
{
	// A child inherits the handlers, so its own forks have to work too.
	auto pool = std::unique_ptr<thread_pool>(new thread_pool(4));
	REQUIRE( pool_still_works(*pool, 1000) );

	const auto status = run_in_forked_child(
		[&pool] ()
		{
			const auto grandchild = run_in_forked_child(
				[&pool] ()
				{
					const auto works = pool_still_works(*pool, 1000);
					pool.reset();
					return works ? 0 : 1;
				}
			);

			const auto works = pool_still_works(*pool, 1000);
			pool.reset();

			if (grandchild != 0)
			{
				return 3;
			}
			return works ? 0 : 1;
		}
	);

	CHECK( status == 0 );
}

TEST_CASE(
	"a thread pool of no workers should survive a fork",
	"[thread_pool]"
)
{
	// Nothing to bring down and nothing to put back, so this pins that the
	// handlers cope with the degenerate pool rather than assuming a worker.
	auto pool = std::unique_ptr<thread_pool>(new thread_pool(0));

	const auto status = run_in_forked_child(
		[&pool] ()
		{
			if (!pool_still_works(*pool, 1000))
			{
				return 1;
			}

			const auto serial = pool->get_size() == 1;
			pool.reset();
			return serial ? 0 : 2;
		}
	);

	CHECK( status == 0 );
}

TEST_CASE(
	"the default thread pool should survive a fork",
	"[thread_pool]"
)
{
	// The one that matters in practice: it is a static, so a child that exits
	// normally destroys it, and it is built as soon as anything opens the CPU
	// backend.
	const auto &pool = thread_pool::get_default();
	REQUIRE( pool != nullptr );
	REQUIRE( pool_still_works(*pool, 1000) );

	const auto status = run_in_forked_child(
		[&pool] ()
		{
			return pool_still_works(*pool, 1000) ? 0 : 1;
		}
	);

	CHECK( status == 0 );
}

#endif // XMIPP4_POSIX
