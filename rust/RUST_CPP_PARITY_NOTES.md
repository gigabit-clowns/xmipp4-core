# Xmipp4 Rust Refactor Status (Rust vs C++)

This document is a consolidated snapshot of the current Rust spike state versus the C++ reference. It should be updated as a current-state rewrite, not as an incremental journal.

## Snapshot

- Date: 2026-05-01
- Rust workspace root: `rust/`
- Main crate: `rust/core-types`
- C++ reference area: `include/xmipp4/core/multidimensional/*` and `src/core/multidimensional/*`

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

- `DynamicSubscript` and `Slice` in `rust/core-types/src/subscript.rs`
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

1. No C++/Rust integration boundary yet (no production FFI bridge in use).
2. Rust code is still isolated under `rust/`; CMake/C++ runtime does not consume it.
3. Major runtime-system modules remain C++-only (execution context, allocators/resources, kernel dispatch, plugins/backends, communication stack).
4. Release/packaging flows are still centered on C++/Python/conda artifacts.

## Recommended Next Steps

1. Continue parity-by-contract per module.
2. Freeze public API names listed in this document while drop-in compatibility is a target.
3. Introduce one narrow C++/Rust bridge vertical path once module-level parity is stable.
