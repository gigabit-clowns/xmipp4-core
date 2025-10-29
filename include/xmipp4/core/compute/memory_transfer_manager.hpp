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

/**
 * @brief This class centralizes the management of memory_transfer_backend-s
 * to create memory_transfer objects between different memory_resource-s.
 * 
 */
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

    /**
     * @brief Register a new memory transfer backend.
     * 
     * @param backend The backend to be registered.
     * @return true If the backend was registered successfully.
     * @return false If the backend could not be registered.
     */
    bool register_backend(std::unique_ptr<memory_transfer_backend> backend);

    /**
     * @brief Create a new memory_transfer object to move data between 
     * two memory_resource-s.
     * 
     * @param src The source memory resource.
     * @param dst The destination memory resource.
     * @return std::shared_ptr<memory_transfer> The memory_transfer object.
     * nullptr if no backend is able to create a transfer between the two
     * memory_resource-s.
     */
    std::shared_ptr<memory_transfer> create_transfer(
        const memory_resource& src,
        const memory_resource& dst
    );

private:
    class implementation;
    std::unique_ptr<implementation> m_implementation;

    void create_implementation_if_null();

};


} // namespace compute
} // namespace xmipp4

