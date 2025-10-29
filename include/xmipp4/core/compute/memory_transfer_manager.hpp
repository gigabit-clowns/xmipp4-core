// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../backend_manager.hpp"

#include <memory>

namespace xmipp4 
{
namespace compute
{

class memory_resource;
class memory_transfer;
class memory_transfer_backend;

class memory_transfer_manager
    : public backend_manager
{
public:
    memory_transfer_manager() noexcept;
    memory_transfer_manager(const memory_transfer_manager &other) = delete;
    memory_transfer_manager(memory_transfer_manager &&other) noexcept;
    virtual ~memory_transfer_manager();

    memory_transfer_manager&
    operator=(const memory_transfer_manager &other) = delete;
    memory_transfer_manager&
    operator=(memory_transfer_manager &&other) noexcept;
    
    void register_builtin_backends() override;

    bool register_backend(std::unique_ptr<memory_transfer_backend> backend);

    std::shared_ptr<memory_transfer> create_transfer(
        const memory_resource& src,
        const memory_resource& dst
    ) const;

private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;

    void create_implementation_if_null();

};


} // namespace compute
} // namespace xmipp4

