# Xmipp4 Rust Refactor Status (Rust vs C++)

This document is a consolidated snapshot of the current Rust spike state versus the C++ reference. It should be updated as a current-state rewrite, not as an incremental journal.

## Snapshot

- Date: 2026-05-01
- Rust workspace root: `rust/`
- Main crate: `rust/core-types`
- C++ reference area: `include/xmipp4/core/multidimensional/*` and `src/core/multidimensional/*`
- Optional integration flag: `XMIPP4_CORE_ENABLE_RUST_BRIDGE`

## Implemented Surface In Rust

### numerical_type

- `NumericalType` and `NumericalTypeCategory` in `rust/core-types/src/numerical_type.rs`
- Implemented public API:
  - `size_bytes`
  - `category`
  - `make_complex`
  - `promote_types`

### array_descriptor

- `ArrayDescriptor` in `rust/core-types/src/array_descriptor.rs`
- Implemented public API:
  - `ArrayDescriptor::new`, getters, `Default`
  - `is_initialized`
  - `compute_storage_requirement`

### strided_layout

- `StridedLayout` in `rust/core-types/src/strided_layout.rs`
- Implemented public API:
  - `new`
  - `make_contiguous_layout`
  - `rank`, `extents`, `strides`, `offset`
  - `extents_equal`
  - `compute_element_count`
  - `compute_storage_requirement`
  - `transpose`
  - `permute`
  - `matrix_transpose`
  - `matrix_diagonal`
  - `squeeze`
  - `broadcast_to`
  - `apply_subscripts`

### subscript/slice

- `DynamicSubscript` in `rust/core-types/src/dynamic_subscript.rs`
- `Slice` and sanitization utilities in `rust/core-types/src/slice.rs`
- Compatibility facade in `rust/core-types/src/subscript.rs`
- Public constructors/helpers remain available through module path (`xmipp4_core_types::subscript::*`):
  - `ellipsis`, `new_axis`
  - `all`, `even`, `odd`
  - `make_slice`, `make_slice_start`, `make_slice_full`, `end`
  - `sanitize_slice`

### typed errors

- `SliceError` in `rust/core-types/src/slice_error.rs`
- `StridedLayoutError` in `rust/core-types/src/strided_layout_error.rs`

## Public API Parity Contract (C++-Aligned Names)

Only public API names are listed here. Internal helpers are intentionally excluded.

### Crate-root exports (`rust/core-types/src/lib.rs`)

- `ArrayDescriptor`
- `compute_storage_requirement`
- `is_initialized`
- `NumericalType`
- `NumericalTypeCategory`
- `promote_types`
- `SliceError`
- `StridedLayout`
- `StridedLayoutError`
- `DynamicSubscript`
- `Slice`

### Public names constrained by drop-in parity

- Keep these names stable while parity/drop-in is a goal:
  - `compute_storage_requirement` (both descriptor helper and layout method)
  - `make_contiguous_layout`
  - `compute_element_count`
  - `matrix_transpose`
  - `matrix_diagonal`
  - `broadcast_to`
  - `apply_subscripts`
  - `promote_types`

## Quality Gates

- CI Rust checks in `.github/workflows/build-and-test.yml`:
  - `cargo fmt --all -- --check`
  - `cargo check --workspace --all-targets`
  - `cargo clippy --workspace --all-targets -- -D warnings`
  - `cargo test --workspace`

## First C++/Rust Vertical Integration

An initial optional bridge is now implemented behind `XMIPP4_CORE_ENABLE_RUST_BRIDGE`.

- Rust exports C ABI symbols from `rust/core-types/src/c_api.rs`:
  - `xmipp4_rust_promote_types`
  - `xmipp4_rust_compute_storage_requirement`
- C++ routes enabled by flag:
  - `xmipp4::promote_types` in `src/core/numerical_type.cpp`
  - `xmipp4::multidimensional::compute_storage_requirement(const array_descriptor&)`
    in `src/core/multidimensional/array_descriptor.cpp`
- Build integration:
  - `src/CMakeLists.txt` triggers `cargo build --release` for `rust/core-types`
  - links generated static library into `xmipp4-core`
  - defines `XMIPP4_CORE_ENABLE_RUST_BRIDGE=1` for consumers when enabled
- C++ comparison tests:
  - `tests/unitary/src/core/test_rust_bridge.cpp`

## Documentation And SPDX Requirements

As part of drop-in readiness, parity work is not considered complete with only
functional/API alignment.

- Public API must include rustdoc documentation sufficient to generate browsable
  reference docs.
- Rust source and test files must include SPDX identifiers matching repository
  policy (`GPL-3.0-only`).
- Documentation/state files should be maintained as consolidated current-state
  snapshots, not incremental journals.

## Tests

- Public-API integration tests:
  - `rust/core-types/tests/numerical_type.rs`
  - `rust/core-types/tests/array_descriptor.rs`
  - `rust/core-types/tests/strided_layout.rs`
  - `rust/core-types/tests/subscript.rs`
- Current status: green under fmt/check/clippy/test.

## Parity Matrix

### numerical_type

- `size/category/make_complex`: `aligned` for implemented cases
- `promote_types`: `aligned` for implemented lattice and tested contract cases

### array_descriptor

- Constructor/default/getters/equality: `aligned`
- `is_initialized`: `aligned`
- `compute_storage_requirement`: `aligned`

### strided_layout

- Implemented methods listed above: `aligned` for current tested contracts
- Equality/hash nuance for extent==1 axes: `aligned`

### subscript/slice

- Sanitization and apply-related runtime behavior for tested paths: `aligned`
- Public naming ergonomics vs C++ style: `manual-review`

## Known Gaps Before True Drop-In Replacement

1. Bridge is intentionally narrow (promotion + storage requirement route only).
2. Integration is opt-in; default build path remains fully C++.
3. Major runtime-system modules remain C++-only (execution context, allocators/resources, kernel dispatch, plugins/backends, communication stack).
4. Release/packaging flows are still centered on C++/Python/conda artifacts.

## Upstreaming Considerations (Backport To Original Repo)

These are quality/integration points to revisit explicitly before proposing this
Rust work for upstream inclusion.

1. Slice arithmetic overflow hardening.
   - In `rust/core-types/src/slice.rs`, sanitization currently uses arithmetic
     expressions such as `start + step * (count - 1)` and variants.
   - For extreme values, debug/release behavior can diverge if intermediate
     arithmetic overflows.
   - Recommendation: switch critical intermediate math to checked operations or
     widened intermediate types (`i128`) before final comparisons.

2. FFI contract strictness for C ABI entrypoints.
   - `rust/core-types/src/c_api.rs` uses raw pointers and `from_raw_parts`.
   - Null pointers are checked, but pointer validity/rank sanity still depends
     on caller guarantees.
   - Recommendation: document ABI preconditions in detail and consider guard
     rails (for example, maximum accepted rank) to reduce accidental UB surface.

3. Transitional module compatibility cleanup.
   - The split into `dynamic_subscript.rs` and `slice.rs` keeps a legacy facade
     at `subscript.rs` for compatibility.
   - Recommendation: decide an upstream policy (keep facade permanently,
     deprecate it, or make it crate-internal) to avoid long-term API drift.

4. CI parity signal should stay explicit for bridge OFF and bridge ON.
   - Keep validating that original C++ `ctest` suite passes with default C++
     path (bridge OFF).
   - Keep validating that the same C++ `ctest` suite passes with
     `XMIPP4_CORE_ENABLE_RUST_BRIDGE=ON`.
   - Bridge-specific comparison tests are useful, but they are not a substitute
     for full-suite parity checks.

## Recommended Next Steps

1. Keep this bridge path green in C++ unitary CI with the flag enabled.
2. Continue parity-by-contract per module.
3. Freeze public API names listed in this document while drop-in compatibility is a target.
4. Expand the bridge gradually, preserving optional/flagged rollout until confidence is high.
