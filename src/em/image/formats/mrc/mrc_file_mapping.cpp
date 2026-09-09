// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_file_mapping.hpp"

#include <rexlib/em/image/exceptions/image_format_error.hpp>

#include <rexlib/core/platform/operating_system.h>

#include <boost/filesystem/operations.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#if defined(REXLIB_POSIX)
	#include <sys/mman.h>
	#include <unistd.h>
	#if defined(MADV_WILLNEED)
		#define REXLIB_HAS_MADVISE
	#endif
#endif

#include <algorithm>
#include <cstddef>
#include <fstream>

namespace rexlib
{
namespace em
{
namespace mrc
{

namespace
{

boost::interprocess::mode_t to_mode(access_flags access)
{
	if (access.contains(access_flag_bits::write))
	{
		return boost::interprocess::read_write;
	}

	return boost::interprocess::read_only;
}

} // anonymous namespace

mrc_file_mapping::mrc_file_mapping(
	const std::string &path,
	access_flags access
)
{
	const auto mode = to_mode(access);

	try
	{
		m_mapping = std::make_unique<boost::interprocess::file_mapping>(
			path.c_str(), mode
		);
		m_region = std::make_unique<boost::interprocess::mapped_region>(
			*m_mapping, mode
		);
	}
	catch (const boost::interprocess::interprocess_exception &error)
	{
		throw image_format_error(
			"mrc_file_mapping: The file could not be mapped: " +
			std::string(error.what())
		);
	}

	if (m_region->get_size() == 0)
	{
		throw image_format_error(
			"mrc_file_mapping: The file is empty."
		);
	}
}

mrc_file_mapping::mrc_file_mapping(
	mrc_file_mapping &&other
) noexcept = default;

mrc_file_mapping::~mrc_file_mapping() = default;

mrc_file_mapping&
mrc_file_mapping::operator=(mrc_file_mapping &&other) noexcept = default;

byte* mrc_file_mapping::get_data() const noexcept
{
	return static_cast<byte*>(m_region->get_address());
}

std::size_t mrc_file_mapping::get_size() const noexcept
{
	return m_region->get_size();
}

void mrc_file_mapping::prefetch(
	std::size_t offset,
	std::size_t size
) const noexcept
{
#if defined(REXLIB_HAS_MADVISE)
	const auto mapped = get_size();
	if (offset >= mapped || size == 0)
	{
		return;
	}

	static const auto page =
		static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
	const auto last = offset + std::min(size, mapped - offset);

	// madvise takes whole pages, so the stretch grows to the ones its ends
	// fall in rather than shrinking away from them.
	const auto first_page = offset - (offset % page);

	::madvise(
		static_cast<void*>(get_data() + first_page),
		last - first_page,
		MADV_WILLNEED
	);
#else
	// Windows advises through PrefetchVirtualMemory, which is not wired up
	// here: the mapping is read as it always was, one fault at a time.
	std::ignore = offset;
	std::ignore = size;
#endif
}

void mrc_file_mapping::flush()
{
	if (!m_region->flush())
	{
		throw image_format_error(
			"mrc_file_mapping: The mapping could not be flushed."
		);
	}
}

void create_file(const std::string &path, std::size_t size)
{
	if (size == 0)
	{
		throw image_format_error(
			"mrc::create_file: A file of no bytes can not be mapped."
		);
	}

	{
		std::filebuf file;
		const auto opened = file.open(
			path,
			std::ios_base::in | std::ios_base::out |
			std::ios_base::trunc | std::ios_base::binary
		);
		if (opened == nullptr)
		{
			throw image_format_error(
				"mrc::create_file: The file could not be created."
			);
		}
	}

	boost::system::error_code code;
	boost::filesystem::resize_file(path, size, code);
	if (code)
	{
		throw image_format_error(
			"mrc::create_file: The file could not be sized: " +
			code.message()
		);
	}
}

} // namespace mrc
} // namespace em
} // namespace rexlib
