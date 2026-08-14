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
//
// Measured on a sum of 64 M floats: raising it from 4 KB to 16 KB gains
// between a tenth and a sixth of the time on the orientation that folds an
// outer axis, and beyond 16 KB the gain is inside the noise. The orientation
// that folds the contiguous axis is indifferent to it, holding one
// accumulator across each run whatever the tile.
#ifndef XMIPP4_REDUCTION_TILE_BUDGET
	#define XMIPP4_REDUCTION_TILE_BUDGET 16384UL
#endif

// Bounds on the resulting tile. A tile shorter than the minimum stops the
// input reads from streaming, whatever the accumulators cost; one longer than
// the maximum stops fitting.
#ifndef XMIPP4_MINIMUM_REDUCTION_TILE_SIZE
	#define XMIPP4_MINIMUM_REDUCTION_TILE_SIZE 64UL
#endif

#ifndef XMIPP4_MAXIMUM_REDUCTION_TILE_SIZE
	#define XMIPP4_MAXIMUM_REDUCTION_TILE_SIZE 4096UL
#endif
