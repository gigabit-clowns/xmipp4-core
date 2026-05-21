// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_command_cache.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <list>
#include <unordered_map>
#include <utility>

namespace xmipp4
{
namespace multidimensional
{

operation_command_cache_key::operation_command_cache_key() noexcept = default;
operation_command_cache_key::~operation_command_cache_key() = default;

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
		const auto ite = m_index.find(&key);
		if (ite == m_index.end())
		{
			return nullptr;
		}

		promote(ite->second);
		return ite->second->second;
	}

	void store(
		std::unique_ptr<operation_command_cache_key> key,
		std::shared_ptr<void> value
	)
	{
		XMIPP4_ASSERT(key);

		const auto ite = m_index.find(key.get());
		if (ite != m_index.end())
		{
			ite->second->second = std::move(value);
			promote(ite->second);
			return;
		}

		if (m_entries.size() >= m_capacity)
		{
			evict_oldest();
		}

		m_entries.push_front(entry_type(std::move(key), std::move(value)));
		m_index.emplace(
			m_entries.front().first.get(), m_entries.begin()
		);
	}

private:
	using entry_type = std::pair<
		std::unique_ptr<operation_command_cache_key>,
		std::shared_ptr<void>
	>;

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
	using index_map_type = std::unordered_map<
		const operation_command_cache_key*,
		entry_list_type::iterator,
		key_ptr_hash_type,
		key_ptr_equal_type
	>;

	std::size_t m_capacity;
	entry_list_type m_entries;
	index_map_type m_index;

	void evict_oldest()
	{
		XMIPP4_ASSERT(!m_entries.empty());
		const auto victim = std::prev(m_entries.end());
		m_index.erase(victim->first.get());
		m_entries.erase(victim);
	}

	void promote(entry_list_type::iterator ite)
	{
		m_entries.splice(m_entries.begin(), m_entries, ite);
	};
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
	return m_implementation->get_capacity();
}

std::shared_ptr<void>
operation_command_cache::touch_erased(
	const operation_command_cache_key &key
)
{
	return m_implementation->touch(key);
}

void operation_command_cache::store_erased(
	std::unique_ptr<operation_command_cache_key> key,
	std::shared_ptr<void> value
)
{
	m_implementation->store(std::move(key), std::move(value));
}

} // namespace multidimensional
} // namespace xmipp4
