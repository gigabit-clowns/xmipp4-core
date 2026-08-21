// SPDX-License-Identifier: GPL-3.0-only

// Tuning knobs of the CPU backend. Every one of them has a default that is
// reasonable on an ordinary desktop processor and may be overridden from the
// build, which is what makes them worth defining rather than writing down at
// the point of use.

// Budget in bytes for the accumulators one reduction tile holds, whatever
// their number, so that a kernel keeping several of them takes a
// proportionally shorter tile and the footprint stays the same. Half of a
// typical first level data cache, leaving the other half to the input
// streaming past.
#ifndef XMIPP4_REDUCTION_TILE_BUDGET
	#define XMIPP4_REDUCTION_TILE_BUDGET 16384UL
#endif

// Every worker of a threaded reduction holds a tile of its own, on its own
// stack, so this is a per thread cost. At the default it is 16 KB against the
// 8 MB, 1 MB and 512 KB default thread stacks of glibc, MSVC and musl; raising
// it past about 64 KB needs a look at the last of those.
//
// Bounds on the resulting tile. A tile shorter than the minimum stops the
// input reads from streaming, whatever the accumulators cost; one longer than
// the maximum stops fitting.
#ifndef XMIPP4_MINIMUM_REDUCTION_TILE_SIZE
	#define XMIPP4_MINIMUM_REDUCTION_TILE_SIZE 64UL
#endif

#ifndef XMIPP4_MAXIMUM_REDUCTION_TILE_SIZE
	#define XMIPP4_MAXIMUM_REDUCTION_TILE_SIZE 4096UL
#endif

// Budget in bytes for the sub-accumulators one run is folded into at a time,
// whatever their number, read the same way the tile budget is: a kernel
// keeping several accumulators takes proportionally fewer lanes.
//
// A run folded into a single accumulator is a chain of dependent operations,
// one every few cycles however wide the vector registers are, because a
// floating point fold is not associative and no compiler may break it. Lanes
// are what break it: the run is dealt out over several sub-accumulators and
// they are merged once at the end. Only a kernel that says its fold may be
// reassociated is folded this way; see has_reassociable_fold.
#ifndef XMIPP4_REDUCTION_FOLD_LANE_BUDGET
	#define XMIPP4_REDUCTION_FOLD_LANE_BUDGET 256UL
#endif

// Bounds on the resulting lane count. The maximum is where the measurement
// above stops improving; past it the seeding and the merge cost more than
// the extra independence buys. The minimum keeps the arithmetic meaningful
// for a kernel whose accumulators exceed the budget on their own.
#ifndef XMIPP4_MAXIMUM_REDUCTION_FOLD_LANES
	#define XMIPP4_MAXIMUM_REDUCTION_FOLD_LANES 16UL
#endif

#ifndef XMIPP4_MINIMUM_REDUCTION_FOLD_LANES
	#define XMIPP4_MINIMUM_REDUCTION_FOLD_LANES 2UL
#endif

// pocketfft re-derives the twiddle factors of a transform every time it is
// asked for one unless it is allowed to remember them. A program is built
// once and run many times, so without this every run would pay for a plan it
// had already made. The cache is a bounded, mutex guarded set of function
// local statics inside pocketfft itself.
#ifndef XMIPP4_POCKETFFT_CACHE_SIZE
	#define XMIPP4_POCKETFFT_CACHE_SIZE 16UL
#endif

// How much elementwise work a thread has to be given for waking it to pay for
// itself: the hand-off, the cache line the two threads either side of a chunk
// boundary share, and the join. A loop below one of these runs on the calling
// thread alone.
//
// Stated in elementwise operations, so that a loop whose iteration costs more
// than one of them divides it down rather than inventing a unit of its own: a
// reduction's surviving element costs as many as it folds, and a linalg batch
// element costs a whole matrix product. See grain_for_cost.
//
// This is the default. It may be overridden at run time through the
// XMIPP4_PARALLEL_GRAIN_SIZE environment variable, which is what lets a test
// suite of small operands reach the threaded paths at all.
#ifndef XMIPP4_PARALLEL_GRAIN_SIZE
	#define XMIPP4_PARALLEL_GRAIN_SIZE 32768UL
#endif
