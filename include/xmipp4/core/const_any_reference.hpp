// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeindex>

namespace xmipp4 
{

class const_any_reference
{
public:
    const_any_reference() noexcept;

    template <typename T>
    const_any_reference(const T &value) noexcept;

    template <typename T>
    const_any_reference(T &&value) = delete;

    const_any_reference(const const_any_reference &other) = default;
    const_any_reference(const_any_reference &&other) = default;
    ~const_any_reference() = default;

    const_any_reference& operator=(const const_any_reference &other) = default;
    const_any_reference& operator=(const_any_reference &&other) = default;

    std::type_index get_type() const noexcept;

    template <typename T>
    const T& get_value() const;

private:
    std::type_index m_type;
    const void *m_reference;

};

} // namespace xmipp4

#include "const_any_reference.inl"
