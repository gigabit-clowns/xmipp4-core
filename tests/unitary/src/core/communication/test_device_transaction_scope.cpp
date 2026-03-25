// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <trompeloeil/sequence.hpp>

#include <xmipp4/core/communication/device_transaction_scope.hpp>

#include "mock/mock_device_transaction.hpp"


#include <array>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "device_transaction_scope can be default constructed", "[device_transaction_scope]" )
{
	device_transaction_scope scope;
}

TEST_CASE( "device_transaction_scope can be constructed with nullptr", "[device_transaction_scope]" )
{
	device_transaction_scope scope(nullptr);
}

TEST_CASE( "device_transaction_scope calls begin() on construction and commit() on destruction", "[device_transaction_scope]" )
{
	mock_device_transaction mock;
	trompeloeil::sequence seq;

	REQUIRE_CALL(mock, begin()).IN_SEQUENCE(seq);
	auto scope = std::make_unique<device_transaction_scope>(&mock);
	REQUIRE( seq.is_completed() );

	REQUIRE_CALL(mock, commit()).IN_SEQUENCE(seq);
	scope.reset();
	REQUIRE( seq.is_completed() );
}

TEST_CASE( "device_transaction_scope can be prematurely commited", "[device_transaction_scope]" )
{
	mock_device_transaction mock;
	REQUIRE_CALL(mock, begin());
	device_transaction_scope scope(&mock);

	trompeloeil::sequence seq;
	REQUIRE_CALL(mock, commit()).IN_SEQUENCE(seq);
	scope.commit();
	REQUIRE( seq.is_completed() );
}
