# xmipp4-core
Core library for the xmipp4 framework

[![Build and Test](https://github.com/gigabit-clowns/xmipp4-core/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/gigabit-clowns/xmipp4-core/actions/workflows/build-and-test.yml)
[![Run tests with memcheck](https://github.com/gigabit-clowns/xmipp4-core/actions/workflows/test-with-memcheck.yml/badge.svg)](https://github.com/gigabit-clowns/xmipp4-core/actions/workflows/test-with-memcheck.yml)

## SonarCloud status
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)

### Ratings
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)
[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)
[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)
[![Technical Debt](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=sqale_index)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)

### Specific metrics
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=bugs)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)
[![Vulnerabilities](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=vulnerabilities)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=gigabit-clowns_xmipp4-core&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=gigabit-clowns_xmipp4-core)

## Platform support
On Windows, the published binaries (wheels and conda packages) are built
with MSVC. This is a hard requirement, not just the default: the C++ ABI
(symbol mangling, vtable layout) is not compatible across compilers on
Windows, so any code that links against xmipp4-core there must also be built 
with MSVC. Linking against these binaries with MinGW/gcc will fail with 
undefined-reference errors on exported C++ symbols (e.g. class vtables).

This only affects linking on Windows. xmipp4-core itself is still built
and tested with gcc (via MinGW) in CI, since that only involves compiling
xmipp4-core from source with a single, consistent toolchain — the
incompatibility only shows up when mixing toolchains across the binary
boundary of a prebuilt artifact.
