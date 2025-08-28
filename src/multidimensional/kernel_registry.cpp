// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/kernel_registry.hpp>

#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel_key.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/compute/device_backend.hpp>
#include <xmipp4/core/platform/assert.hpp>


#include <unordered_map>
#include <typeindex>
#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_registry::implementation
{
public:
    bool register_kernel_builder(kernel_key key,
                                 const compute::device_backend &backend,
                                 std::unique_ptr<kernel_builder> builder )
    {
        bool inserted = false;

        if (builder)
        {
            std::tie(std::ignore, inserted) = m_builders.emplace(
                key_type(key, &backend),
                std::move(builder)
            );
        }

        return inserted;
    }

    const kernel_builder * 
    get_kernel_builder(kernel_key key, const compute::device_backend &backend) const noexcept
    {
        const kernel_builder *result = nullptr;

        const auto ite = m_builders.find(key_type(key, &backend));
        if (ite != m_builders.cend())
        {
            result = ite->second.get();
            XMIPP4_ASSERT( result );
        }

        return result;
    }

private:
    using key_type = std::tuple<kernel_key, const compute::device_backend *>;
    using value_type = std::unique_ptr<kernel_builder>;

    struct key_hash
    {
        std::size_t operator()(const key_type &key) const noexcept
        {
            const auto h1 = 
                std::hash<kernel_key>()(std::get<0>(key));
            const auto h2 = 
                std::hash<const compute::device_backend *>()(std::get<1>(key));

            return h1 ^ (h2 << 1);
        }
    };

    std::unordered_map<key_type, value_type, key_hash> m_builders;

};





kernel_registry::kernel_registry() = default;

kernel_registry::kernel_registry(kernel_registry &&other) noexcept = default;

kernel_registry::~kernel_registry() = default;

kernel_registry& 
kernel_registry::operator=(kernel_registry &&other) noexcept = default;

bool kernel_registry::register_kernel_builder(kernel_key key,
                                              const compute::device_backend &backend,
                                              std::unique_ptr<kernel_builder> builder )
{
    return create_if_null().register_kernel_builder(
        key, 
        backend, 
        std::move(builder)
    );
}

const kernel_builder * 
kernel_registry::get_kernel_builder(kernel_key key,
                                    const compute::device_backend &backend ) const noexcept
{
    return m_implementation ? 
           m_implementation->get_kernel_builder(key, backend) :
           nullptr ;
}

kernel_registry::implementation& kernel_registry::create_if_null()
{
    if (!m_implementation)
    {
        m_implementation = std::make_unique<implementation>();
    }

    XMIPP4_ASSERT( m_implementation );
    return *m_implementation;
}

} // namespace multidimensional
} // namespace xmipp4
