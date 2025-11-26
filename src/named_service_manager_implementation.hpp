// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

namespace xmipp4
{

template <typename T>
class named_service_manager_implementation
{
public:
	using backend_type = T;
	using backend_map = std::unordered_map<
		std::string,
		std::unique_ptr<backend_type>
	>;

	bool register_backend(std::unique_ptr<backend_type> backend);
	void enumerate_backends(std::vector<std::string> &names) const;
	backend_type* get_backend(const std::string &name) const;
	const backend_map& get_backend_map() const noexcept;

private:
	backend_map m_backends;
};

} // namespace xmipp4

#include "named_service_manager_implementation.inl"
