// SPDX-License-Identifier: GPL-3.0-only

#include "sequence_loop.hpp"

#include "inner_loop_stride_dispatch.hpp"
#include "strided_pointer_iterator.hpp"

namespace xmipp4
{
namespace cpu
{
namespace detail
{

template <typename Generator, typename T, typename Stride>
inline
void run_sequence_loop_impl(
	const Generator &generator,
	T *pointer,
	std::size_t count,
	Stride stride
)
{
	auto ite = make_strided_pointer_iterator(pointer, stride);
	for (std::size_t i = 0; i < count; ++i)
	{
		generator(ite.data(), i);
		++ite;
	}
}

} // namespace detail

template <typename Generator, typename T>
inline
void run_sequence_loop(
	const Generator &generator,
	T *pointer,
	std::size_t count,
	std::ptrdiff_t stride
)
{
	// The unit stride is resolved to a tag rather than left as a runtime
	// value because it is the case that vectorizes, and it is the case every
	// freshly allocated result takes. Only a caller writing into a view of
	// its own takes the other branch.
	if (stride == 1)
	{
		detail::run_sequence_loop_impl(
			generator,
			pointer,
			count,
			contiguous_stride_tag()
		);
	}
	else
	{
		detail::run_sequence_loop_impl(generator, pointer, count, stride);
	}
}

} // namespace cpu
} // namespace xmipp4
