// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/program.hpp>

using namespace xmipp4::hardware;

namespace
{

class test_program final : public program
{
};

} // namespace

TEST_CASE(
	"program::get_scratch_requirements returns empty vector by default",
	"[hardware]"
)
{
	test_program prog;

	std::vector<program_scratch_requirement> reqs;
	reqs.emplace_back(1, 1, memory_resource_affinity::device);
	prog.get_scratch_requirements(reqs);
	CHECK(reqs.empty());
}
