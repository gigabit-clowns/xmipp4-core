// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fourier_operation.hpp>

#include <xmipp4/core/span.hpp>

#include <cmath>
#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

fourier_operation::fourier_operation(
	std::vector<std::size_t> axes,
	fft_normalization normalization
)
	: m_axes(std::move(axes))
	, m_normalization(normalization)
{
	if (m_axes.empty())
	{
		throw std::invalid_argument(
			"fourier_operation requires at least one transform axis"
		);
	}
}

fourier_operation::~fourier_operation() = default;

span<const std::size_t> fourier_operation::get_axes() const noexcept
{
	return make_span(m_axes);
}

fft_normalization fourier_operation::get_normalization() const noexcept
{
	return m_normalization;
}

double get_forward_fft_normalization_factor(
	fft_normalization convention,
	std::size_t n
)
{
	const auto dn = static_cast<double>(n);
	switch (convention)
	{
	case fft_normalization::forward:
		return 1.0 / dn;
	case fft_normalization::ortho:
		return 1.0 / std::sqrt(dn);
	case fft_normalization::backward:
	case fft_normalization::none:
		return 1.0;
	default:
		throw std::invalid_argument("Unknown normalization convention");
	}
}

double get_backward_fft_normalization_factor(
	fft_normalization convention,
	std::size_t n
)
{
	const auto dn = static_cast<double>(n);
	switch (convention)
	{
	case fft_normalization::backward:
		return 1.0 / dn;
	case fft_normalization::ortho:
		return 1.0 / std::sqrt(dn);
	case fft_normalization::forward:
	case fft_normalization::none:
		return 1.0;
	default:
		throw std::invalid_argument("Unknown normalization convention");
	}
}

double get_fft_normalization_factor(
	fft_normalization convention,
	fft_direction direction,
	std::size_t n
)
{
	switch (direction)
	{
	case fft_direction::forward:
		return get_forward_fft_normalization_factor(convention, n);
	case fft_direction::backward:
		return get_backward_fft_normalization_factor(convention, n);
	default:
		throw std::invalid_argument("Unknown FFT direction");
	}
}

} // namespace multidimensional
} // namespace xmipp4
