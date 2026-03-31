// SPDX-License-Identifier: GPL-3.0-only

#include "convert_data_type.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template<typename T>
typename to_hwy_data_type<T>::type* to_hwy(T* ptr) noexcept
{
	return reinterpret_cast<typename to_hwy_data_type<T>::type*>(ptr);
}

template<typename T>
const typename to_hwy_data_type<T>::type* to_hwy(const T* ptr) noexcept
{
	return reinterpret_cast<const typename to_hwy_data_type<T>::type*>(ptr);
}

} // namespace multidimensional
} // namespace xmipp4