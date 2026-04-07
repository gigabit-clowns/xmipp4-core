// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>

#include "kernel_cache_key.hpp"

#include <memory>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_cache
{
public:
	std::shared_ptr<const kernel> get(const kernel_cache_key &key) const;
	void put(kernel_cache_key &&key, std::shared_ptr<const kernel> kernel);

private:
	struct by_key {};
    struct by_order {};
	using item_type = 
		std::pair<const kernel_cache_key, std::shared_ptr<const kernel>>;
	using container_type = boost::multi_index::multi_index_container<
        item_type,
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<boost::multi_index::tag<by_order>>,
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<by_key>,
                boost::multi_index::member<
					item_type, 
					const kernel_cache_key, 
					&item_type::first
				>
            >
        >
    >;

	container_type m_items;
	std::size_t m_capacity;

};

} // namespace multidimensional
} // namespace xmipp4
