# Unreleased

- Restructured the repository into components: core (plugin SDK), ops
(operation catalog), functional (user-facing verbs), backends/cpu (reference
backend) and an em placeholder for the CryoEM domain.
- Adopted one namespace per component: the core subsystem namespaces
(hardware, dispatch, layout, ndarray, memory, binary, system) were flattened
into the root xmipp4 namespace; concrete operations moved to xmipp4::ops and
the CPU backend to xmipp4::cpu (dropping the cpu_ class-name prefixes).
- Grouped the loose core-root headers into core/numerical/ and core/meta/,
and moved access_flags next to its only user in core/system/.
- Promoted the CPU program builder base class into the public backend SDK
(include/xmipp4/backends/cpu/) so plugins can provide optimized CPU builders.
- The shared library is now assembled from per-component CMake object
libraries; installed headers are filtered to *.h/*.hpp/*.inl.

# Release v0.1.0
Initial xmipp4-core release. Still work in progress.

- Defined and implemented plugin architecture.
- Defined communication API for executing multi-process and multi-node
environments.
- Defined basic hardware API for discovering devices and performing basic 
operations.
- Wrappers and implementations of basic and advanced math functions.
- Cross platform utilities such as dynamic library loading and memory-mapped files
- Foundations for multidimensional arrays.
