# Xmipp4 Rust Refactor Status (Rust vs C++)

This file is the running handoff/status document for the Rust spike. It is intended to help teammates quickly understand what has been migrated, what is parity-complete, what is still pending, and what is needed before a true drop-in replacement against the C++ codebase is feasible.

## Snapshot

- Date: 2026-05-01
- Rust workspace root: `rust/`
- Main crate: `rust/core-types`
- C++ reference area: `include/xmipp4/core/multidimensional/*` and `src/core/multidimensional/*`

## What Has Been Added In Rust

### 1) Core type layer

- `NumericalType` and `NumericalTypeCategory` in `rust/core-types/src/numerical_type.rs`
- Basic helpers: `size_bytes`, `category`, `make_complex`

### 2) Array descriptor layer

- `ArrayDescriptor` in `rust/core-types/src/array_descriptor.rs`
- Helper functions: `is_initialized`, `compute_storage_requirement`
- Status vs C++: functionally aligned for currently implemented surface (constructor/default/getters/equality/storage/init checks)

### 3) Strided layout layer

- `StridedLayout` in `rust/core-types/src/strided_layout.rs`
- Implemented methods:
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

### 4) Runtime subscripts/slices for apply_subscripts

- `DynamicSubscript`, `Slice` and helpers in `rust/core-types/src/subscript.rs`
- Helpers exported from crate root:
  - `ellipsis`, `new_axis`
  - `all`, `even`, `odd`
  - `make_slice`, `make_slice_start`, `make_slice_full`, `end`
- Slice sanitization with C++-aligned validation/error messages

## Quality Gates (Current)

- CI Rust job added in `.github/workflows/build-and-test.yml`
  - `cargo fmt --all -- --check`
  - `cargo check --workspace --all-targets`
  - `cargo clippy --workspace --all-targets -- -D warnings`
  - `cargo test --workspace`

## Tests Added

- Integration tests (public API oriented):
  - `rust/core-types/tests/numerical_type.rs`
  - `rust/core-types/tests/array_descriptor.rs`
  - `rust/core-types/tests/strided_layout.rs`

Current status: tests pass locally with fmt/check/clippy/test.

## Parity Decision Legend

- `aligned`: behavior considered aligned with current C++ implementation for implemented surface.
- `manual-review`: requires team review/decision before freezing API/behavior.
- `not-started`: not implemented yet in Rust.

## Parity Matrix (Current)

### numerical_type

- `size/category/make_complex`: `aligned` for implemented cases
- Full promotion lattice parity (`promote_types` equivalent): `not-started`

### array_descriptor

- Constructor/default/getters/equality: `aligned`
- `is_initialized`: `aligned`
- `compute_storage_requirement`: `aligned`

### strided_layout

- Implemented methods listed above: `aligned` for current tested contracts
- Equality/hash nuance for extent==1 axes: now aligned with C++ equality semantics in Rust implementation

### subscript/slice

- Behavioral parity for tested apply/sanitize paths: `aligned`
- API naming ergonomics vs C++ (constructor/helper naming): `manual-review`

## Immediate Issues Found Earlier (Now Resolved)

### 1) Storage requirement semantics

- Previously differed from C++ for empty layouts and negative-stride patterns.
- Now aligned to C++-style computation in Rust `compute_storage_requirement`.

### 2) Equality/hash semantics when extent==1

- Previously strict derived equality/hash in Rust caused mismatch.
- Now Rust uses custom equality/hash policy compatible with C++ equality behavior for extent==1 axes.

## Remaining Items Requiring Manual Team Scrutiny

### 1) Error typing strategy

- Current Rust APIs return `Result<_, String>`.
- Team should decide whether to keep this during spike or migrate to typed error enums soon.

### 2) Public API ergonomics for subscripts/slices

- Behavior is covered, but naming and exact constructor shape are not strict 1:1 with C++ API style.
- Team should decide preferred long-term external API before freezing.

## What Is Still Missing For True Drop-In Replacement

Even with strong parity in `core-types`, this is not yet a drop-in replacement for the C++ repository because:

1. No C++/Rust integration boundary yet (no FFI/bridge used by existing C++ runtime).
2. Rust code is isolated under `rust/`; CMake/C++ pipeline does not consume it.
3. Many runtime-system modules remain C++-only (execution context, device/queue/event, allocators/resources, kernel dispatch/manager, plugin/backends, communication stack, etc.).
4. Packaging/deployment artifacts are still C++/Python/conda oriented.

## Practical Definition Of "Drop-In" For This Project

To be considered realistically interchangeable with current C++ state:

1. Behavioral parity for all targeted modules is validated by contract tests.
2. C++ callers can invoke Rust implementations through a stable ABI boundary without external behavior change.
3. Existing tests (or equivalent migrated tests) pass with Rust-backed implementation paths.
4. Build/CI/release flows support the selected Rust-backed path in a reproducible way.

## Recommended Next Steps

1. Keep closing parity at module boundaries with contract tests first.
2. Introduce a minimal C++/Rust bridge for one narrow vertical path once team is satisfied with `core-types` stability.
3. Decide typed-errors policy and subscript API naming before wider adoption.

## Change Log (High-Level)

- Added Rust workspace and `core-types` crate.
- Added Rust parity implementations for array descriptor and major strided layout operations.
- Added dynamic subscript/slice support to complete `apply_subscripts`.
- Added CI Rust quality checks and tests as independent track.
