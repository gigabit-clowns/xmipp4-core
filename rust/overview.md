# Xmipp4 Rust Refactor Handover

## Purpose

This file is intended to be pasted at the beginning of a new Copilot session with zero prior context.

It documents:

- C++ architecture essentials that must be preserved
- Migration strategy and rationale
- What has already been implemented in Rust
- What remains before a realistic drop-in replacement

## Starting Point

The Rust refactor started from a repository state functionally equivalent to C++ `main`.

## C++ Architecture Essentials (Must Preserve)

The current C++ design is a runtime system, not only a tensor container.

Core patterns:

- Array model: storage + descriptor
- Descriptor model: `strided_layout` + `numerical_type`
- Read-only views as first-class (`array_view`)
- Execution path includes:
	- operation arity checks
	- shape/type policy deduction and validation
	- output/input storage resolution
	- `array_signature` construction
	- kernel builder selection by suitability
	- execution on active queue
- Explicit queue/event abstractions
- Memory resource kinds (host/device/unified/mapped/staging)
- Transfer helpers with accessibility checks and aliasing when possible

This architecture should remain the contract while migrating.

## Strategic Migration Proposal

1. Avoid big-bang rewrite.
2. Migrate incrementally with parity checkpoints.
3. Keep Rust isolated first; no immediate C++ replacement.
4. Validate behavior against C++ module contracts before moving upward.

Practical note on `ndarray`:

- Useful as internal CPU implementation aid.
- Not sufficient as top-level runtime architecture contract for future GPU/backend/plugin goals.

## Repositories Considered During Analysis

1. `xmipp4-core`
- Primary architectural reference.

2. `xmipp4-cuda`
- Conceptual guidance for stream/event/pinned-memory lifecycle.
- Outdated for direct API parity.

3. `wiener-poc` (historical)
- Demonstrated focused Rust CPU/GPU ideas; not used as direct template.

## What Is Already Implemented In Rust

Workspace:

- `rust/Cargo.toml`
- `rust/core-types` crate

Current Rust modules:

1. `numerical_type`
- `NumericalType`, `NumericalTypeCategory`
- `size_bytes`, `category`, `make_complex`, `promote_types`

2. `array_descriptor`
- `ArrayDescriptor`
- `is_initialized`
- `compute_storage_requirement`

3. `strided_layout`
- `new`, `make_contiguous_layout`
- `rank`, `extents`, `strides`, `offset`, `extents_equal`
- `compute_element_count`, `compute_storage_requirement`
- `transpose`, `permute`, `matrix_transpose`, `matrix_diagonal`, `squeeze`
- `broadcast_to`, `apply_subscripts`

4. `dynamic_subscript` + `slice`
- `DynamicSubscript` in `rust/core-types/src/dynamic_subscript.rs`
- `Slice` and sanitization helpers in `rust/core-types/src/slice.rs`
- compatibility facade in `rust/core-types/src/subscript.rs`
- helpers remain available through `xmipp4_core_types::subscript::*`:
	- `ellipsis`, `new_axis`
	- `all`, `even`, `odd`
	- `make_slice`, `make_slice_start`, `make_slice_full`, `end`
	- `sanitize_slice`

5. C ABI bridge (first vertical integration)
- `rust/core-types/src/c_api.rs`
- exported symbols:
	- `xmipp4_rust_promote_types`
	- `xmipp4_rust_compute_storage_requirement`

## CI And Quality Status

Rust checks integrated in `.github/workflows/build-and-test.yml` as independent job:

- `cargo fmt --all -- --check`
- `cargo check --workspace --all-targets`
- `cargo clippy --workspace --all-targets -- -D warnings`
- `cargo test --workspace`

Additional integration CI job in `.github/workflows/build-and-test.yml`:

- docs generation (`XMIPP4_CORE_BUILD_DOC=ON`)
- CMake build with Rust bridge (`XMIPP4_CORE_ENABLE_RUST_BRIDGE=ON`)
- full C++ `ctest` run
- explicit `[rust_bridge]` comparison test run

Current local status: passing.

## Current Parity Status (High-Level)

Implemented module parity (for implemented surface) is high for:

- `array_descriptor`
- major `strided_layout` operations
- dynamic subscripting behavior in tested scenarios

Still pending for full C++ interchangeability:

- remaining higher-level runtime modules (execution context, allocators/resources, queue/event/device model, kernel dispatch/manager, plugin/backends, communication)
- broader C++ <-> Rust integration boundary (current bridge is intentionally narrow)

## What "Drop-In Replacement" Requires

For this project, drop-in replacement means:

1. Module-level behavior parity validated by contract tests.
2. Existing C++ call paths can invoke Rust equivalents without externally visible behavior changes.
3. Build and CI support stable mixed or switched execution paths.
4. Remaining runtime-system modules are migrated or safely bridged.

At current status, Rust is a validated isolated implementation track, not yet a drop-in replacement for the C++ runtime.

## Immediate vs Manual Review Items

Immediate (already addressed in current branch):

- `strided_layout` storage requirement semantics alignment
- equality/hash semantics alignment for extent-1 axes
- `promote_types` parity path + optional C++/Rust bridge route

Manual team review still recommended:

- slice sanitization overflow hardening for extreme values
- C ABI preconditions/guard rails documentation for pointer+rank inputs
- long-term policy for `subscript.rs` compatibility facade (keep/deprecate/internalize)
- final public API ergonomics for subscript/slice helpers before freezing API

## Suggested Next Steps

1. Keep closing parity module by module with public API tests.
2. Keep bridge-on and bridge-off C++ test paths green in CI.
3. Expand C++/Rust bridge incrementally beyond the current narrow vertical path.
4. Reassess drop-in readiness after additional integrated vertical paths pass existing behavior checks.
