// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_command_cache.hpp>

#include <xmipp4/core/platform/assert.hpp>
#include <core/logger.hpp>

#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace xmipp4
{
namespace multidimensional
{

class operation_command_cache::implementation
{
public:
	explicit implementation(std::size_t capacity) noexcept
		: m_capacity(capacity)
	{
	}

	std::size_t get_capacity() const noexcept
	{
		return m_capacity;
	}

	std::shared_ptr<void> touch(const operation_command_cache_key &key)
	{
		const std::type_index type = typeid(key);
		const auto outer_ite = m_outer_index.find(type);
		if (outer_ite == m_outer_index.end())
		{
			return nullptr;
		}

		const auto &inner = outer_ite->second;
		const auto inner_ite = inner.find(&key);
		if (inner_ite == inner.end())
		{
			return nullptr;
		}

		promote(inner_ite->second);
		return inner_ite->second->value;
	}

	void store(
		std::unique_ptr<const operation_command_cache_key> key,
		std::shared_ptr<void> value
	)
	{
		XMIPP4_ASSERT(key);
		XMIPP4_ASSERT(value);

		const std::type_index type = typeid(*key);
		auto &inner = m_outer_index[type];
		const auto inner_ite = inner.find(key.get());
		if (inner_ite != inner.end())
		{
			inner_ite->second->value = std::move(value);
			promote(inner_ite->second);
			return;
		}

		if (m_entries.size() >= m_capacity)
		{
			XMIPP4_LOG_DEBUG(
				"operation_command_cache::store: at capacity, evicting "
				"least recently used entry"
			);
			evict_oldest();
		}

		m_entries.push_front(
			entry_type{type, std::move(key), std::move(value)}
		);
		inner.emplace(m_entries.front().key.get(), m_entries.begin());
	}

private:
	struct entry_type
	{
		std::type_index type;
		std::unique_ptr<const operation_command_cache_key> key;
		std::shared_ptr<void> value;
	};

	struct key_ptr_hash_type
	{
		std::size_t operator()(
			const operation_command_cache_key *key
		) const noexcept
		{
			XMIPP4_ASSERT(key);
			return key->hash();
		}
	};

	struct key_ptr_equal_type
	{
		bool operator()(
			const operation_command_cache_key *lhs,
			const operation_command_cache_key *rhs
		) const noexcept
		{
			XMIPP4_ASSERT(lhs);
			XMIPP4_ASSERT(rhs);
			return lhs->equals(*rhs);
		}
	};

	using entry_list_type = std::list<entry_type>;
	using index_type = std::unordered_map<
		std::type_index,
		std::unordered_map<
			const operation_command_cache_key*,
			entry_list_type::iterator,
			key_ptr_hash_type,
			key_ptr_equal_type
		>
	>;

	std::size_t m_capacity;
	entry_list_type m_entries;
	index_type m_outer_index;

	void evict_oldest()
	{
		XMIPP4_ASSERT(!m_entries.empty());
		const auto victim = std::prev(m_entries.end());
		const auto outer_ite = m_outer_index.find(victim->type);
		XMIPP4_ASSERT(outer_ite != m_outer_index.end());
		outer_ite->second.erase(victim->key.get());
		m_entries.erase(victim);

		if (outer_ite->second.empty())
		{
			m_outer_index.erase(outer_ite);
		}
	}

	void promote(entry_list_type::iterator ite)
	{
		m_entries.splice(m_entries.begin(), m_entries, ite);
	}
};

operation_command_cache::operation_command_cache(
	std::size_t capacity
) noexcept
	: m_implementation(std::make_unique<implementation>(capacity))
{
	XMIPP4_ASSERT(capacity > 0);
}

operation_command_cache::~operation_command_cache() = default;

std::size_t operation_command_cache::get_capacity() const noexcept
{
	return m_implementation ? m_implementation->get_capacity() : 0UL;
}

std::shared_ptr<void>
operation_command_cache::touch(const operation_command_cache_key &key)
{
	return m_implementation ? m_implementation->touch(key) : nullptr;
}

void operation_command_cache::store(
	std::unique_ptr<const operation_command_cache_key> key,
	std::shared_ptr<void> value
)
{
	if (!key)
	{
		throw std::invalid_argument(
			"operation_command_cache::store: key must not be null"
		);
	}

	if (!value)
	{
		throw std::invalid_argument(
			"operation_command_cache::store: value must not be null"
		);
	}

	if (m_implementation)
	{
		m_implementation->store(std::move(key), std::move(value));
	}
	else
	{
		XMIPP4_LOG_WARN(
			"operation_command_cache::store: dropping entry because the "
			"cache has no backing implementation"
		);
	}
}

} // namespace multidimensional
} // namespace xmipp4
