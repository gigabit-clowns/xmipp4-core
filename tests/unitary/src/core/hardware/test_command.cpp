// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/command.hpp>

using namespace xmipp4::hardware;

namespace
{

class test_command final : public command
{
};

} // namespace

TEST_CASE(
	"command::get_scratch_requirements returns empty span by default",
	"[hardware]"
)
{
	test_command cmd;

	auto reqs = cmd.get_scratch_requirements();
	CHECK(reqs.empty());
}
