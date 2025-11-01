// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/device_manager.hpp>

#include <xmipp4/core/hardware/cpu/cpu_device_backend.hpp>

#include <unordered_map>

namespace xmipp4
{
namespace hardware
{

device_manager::device_manager() = default;
device_manager::device_manager(device_manager&&) noexcept = default;
device_manager::~device_manager() = default;
device_manager& device_manager::operator=(device_manager&&) noexcept = default;

void device_manager::register_builtin_backends()
{
    cpu_device_backend::register_at(*this);
}

void device_manager::enumerate_devices(std::vector<device_index> &indices) const
{
    std::vector<std::string> backend_names;
    enumerate_backends(backend_names);

    indices.clear();
    std::vector<std::size_t> ids;
    for(const auto &backend_name : backend_names)
    {
        get_backend(backend_name)->enumerate_devices(ids);
        for(const auto &id : ids)
        {
            indices.emplace_back(backend_name, id);
        }
    }
}

bool device_manager::get_device_properties(const device_index &index, 
                                           device_properties &desc ) const
{
    const auto *backend = get_backend(index.get_backend_name());

    bool result = backend != nullptr;
    if (result)
    {
        result = backend->get_device_properties(index.get_device_id(), desc);
    }

    return result;
}

std::shared_ptr<device> 
device_manager::create_device(const device_index& index) const
{
    std::shared_ptr<device> result;

    auto *backend = get_backend(index.get_backend_name());
    if (backend)
    {
        result = backend->create_device(index.get_device_id());
    }

    return result;
}

} // namespace hardware
} // namespace xmipp4
