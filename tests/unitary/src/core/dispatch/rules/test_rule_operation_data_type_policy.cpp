// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <xmipp4/core/dispatch/rules/rule_operation_data_type_policy.hpp>
#include <xmipp4/core/dispatch/rules/operand_type_rule_engine.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

#include <xmipp4/ops/policies/homogeneous_operation_data_type_policy.hpp>

#include <ops/policies/abs_operation_data_type_policy.hpp>
#include <ops/policies/copy_operation_data_type_policy.hpp>

#include <stdexcept>
#include <string>
#include <vector>

using namespace xmipp4;

namespace
{

using type_vector = std::vector<numerical_type>;

type_vector all_concrete_types()
{
    type_vector result;
    const auto count = static_cast<int>(numerical_type::count);
    for (int i = 0; i < count; ++i)
    {
        result.push_back(static_cast<numerical_type>(i));
    }
    return result;
}

struct deduce_outcome
{
    bool rejected = false;
    type_vector outputs;
};

bool operator==(const deduce_outcome &lhs, const deduce_outcome &rhs)
{
    if (lhs.rejected != rhs.rejected)
    {
        return false;
    }
    return lhs.rejected || lhs.outputs == rhs.outputs;
}

deduce_outcome run_deduce(
    const operation_data_type_policy &policy,
    const type_vector &inputs,
    std::size_t output_count
)
{
    deduce_outcome outcome;
    outcome.outputs.assign(output_count, numerical_type::unknown);

    try
    {
        policy.deduce(make_span(outcome.outputs), make_span(inputs));
    }
    catch (const std::invalid_argument&)
    {
        outcome.rejected = true;
    }

    return outcome;
}

bool run_accept(
    const operation_data_type_policy &policy,
    const type_vector &user_outputs,
    const type_vector &inputs
)
{
    // The canonical types are whatever deduce produced, which is how the
    // dispatcher calls accept.
    const auto canonical = run_deduce(policy, inputs, user_outputs.size());
    if (canonical.rejected)
    {
        return true;
    }

    try
    {
        policy.accept(
            make_span(user_outputs),
            make_span(canonical.outputs),
            make_span(inputs)
        );
    }
    catch (const std::invalid_argument&)
    {
        return true;
    }

    return false;
}

} // anonymous namespace

TEST_CASE(
    "binary_homogeneous_rule should reproduce the homogeneous policy",
    "[rule_operation_data_type_policy]"
)
{
    const auto &legacy = ops::homogeneous_operation_data_type_policy::get();
    const auto &rule =
        rule_data_type_policy<ops::binary_homogeneous_rule<>>::get();

    // Sweeping every operand pair is what makes this a parity statement
    // rather than a spot check: the rule must accept and reject exactly
    // what the policy it replaces does.
    for (const auto left : all_concrete_types())
    {
        for (const auto right : all_concrete_types())
        {
            const type_vector inputs = { left, right };

            INFO( "deduce(" << left << ", " << right << ")" );
            CHECK( run_deduce(legacy, inputs, 1) ==
                   run_deduce(rule, inputs, 1) );

            const type_vector outputs = { left };
            INFO( "accept(" << left << ")" );
            CHECK( run_accept(legacy, outputs, inputs) ==
                   run_accept(rule, outputs, inputs) );
        }
    }
}

TEST_CASE(
    "unary_real_of_rule should reproduce the abs policy",
    "[rule_operation_data_type_policy]"
)
{
    const auto &legacy = ops::abs_operation_data_type_policy::get();
    const auto &rule = rule_data_type_policy<
        ops::unary_real_of_rule<arithmetic_type_domain>
    >::get();

    for (const auto input : all_concrete_types())
    {
        const type_vector inputs = { input };

        INFO( "deduce(" << input << ")" );
        CHECK( run_deduce(legacy, inputs, 1) == run_deduce(rule, inputs, 1) );

        for (const auto output : all_concrete_types())
        {
            const type_vector outputs = { output };
            INFO( "accept(" << output << ", " << input << ")" );
            CHECK( run_accept(legacy, outputs, inputs) ==
                   run_accept(rule, outputs, inputs) );
        }
    }
}

TEST_CASE(
    "converting_rule should reproduce the copy policy",
    "[rule_operation_data_type_policy]"
)
{
    const auto &legacy = ops::copy_operation_data_type_policy::get();
    const auto &rule = rule_data_type_policy<ops::converting_rule<>>::get();

    for (const auto input : all_concrete_types())
    {
        const type_vector inputs = { input };

        INFO( "deduce(" << input << ")" );
        CHECK( run_deduce(legacy, inputs, 1) == run_deduce(rule, inputs, 1) );

        for (const auto output : all_concrete_types())
        {
            const type_vector outputs = { output };
            INFO( "accept(" << output << ", " << input << ")" );
            CHECK( run_accept(legacy, outputs, inputs) ==
                   run_accept(rule, outputs, inputs) );
        }
    }
}

TEST_CASE(
    "nullary_free_rule should reproduce the homogeneous policy for fill",
    "[rule_operation_data_type_policy]"
)
{
    const auto &legacy = ops::homogeneous_operation_data_type_policy::get();
    const auto &rule =
        rule_data_type_policy<ops::nullary_free_rule<>>::get();

    const type_vector no_inputs;

    // With no input to fix it, the element type stays undetermined.
    CHECK( run_deduce(legacy, no_inputs, 1) == run_deduce(rule, no_inputs, 1) );
    CHECK( run_deduce(rule, no_inputs, 1).outputs ==
           type_vector{ numerical_type::unknown } );

    for (const auto output : all_concrete_types())
    {
        const type_vector outputs = { output };
        INFO( "accept(" << output << ")" );
        CHECK( run_accept(legacy, outputs, no_inputs) ==
               run_accept(rule, outputs, no_inputs) );
    }
}

TEST_CASE(
    "a rule should reject inputs outside its declared domain",
    "[rule_operation_data_type_policy]"
)
{
    const auto &rule = rule_data_type_policy<
        ops::binary_homogeneous_rule<real_arithmetic_type_domain>
    >::get();

    type_vector outputs(1, numerical_type::unknown);

    const type_vector accepted = {
        numerical_type::int32,
        numerical_type::int32
    };
    CHECK_NOTHROW(
        rule.deduce(make_span(outputs), make_span(accepted))
    );

    // This is the drift the declarative rules exist to remove: a domain the
    // operation declares once, rather than a backend predicate silently
    // narrowing what the policy already accepted.
    const type_vector rejected = {
        numerical_type::complex_float32,
        numerical_type::complex_float32
    };
    CHECK_THROWS_AS(
        rule.deduce(make_span(outputs), make_span(rejected)),
        std::invalid_argument
    );

    const type_vector booleans = {
        numerical_type::boolean,
        numerical_type::boolean
    };
    CHECK_THROWS_AS(
        rule.deduce(make_span(outputs), make_span(booleans)),
        std::invalid_argument
    );
}

TEST_CASE(
    "a rule should produce a boolean output for a predicate",
    "[rule_operation_data_type_policy]"
)
{
    const auto &rule =
        rule_data_type_policy<ops::binary_predicate_rule<>>::get();

    type_vector outputs(1, numerical_type::unknown);
    const type_vector inputs = {
        numerical_type::float64,
        numerical_type::float64
    };

    rule.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == numerical_type::boolean );
}

TEST_CASE(
    "a rule should name the offending operand and the expected type",
    "[rule_operation_data_type_policy]"
)
{
    const auto &rule =
        rule_data_type_policy<ops::binary_homogeneous_rule<>>::get();

    type_vector outputs(1, numerical_type::unknown);
    const type_vector mismatched = {
        numerical_type::float32,
        numerical_type::float64
    };

    try
    {
        rule.deduce(make_span(outputs), make_span(mismatched));
        FAIL( "expected the mismatching operands to be rejected" );
    }
    catch (const std::invalid_argument &error)
    {
        const std::string message = error.what();
        using Catch::Matchers::ContainsSubstring;
        CHECK_THAT( message, ContainsSubstring("input operand 1") );
        CHECK_THAT( message, ContainsSubstring("float32") );
        CHECK_THAT( message, ContainsSubstring("float64") );
    }
}

TEST_CASE(
    "a rule should report the admissible types when a domain is violated",
    "[rule_operation_data_type_policy]"
)
{
    const auto &rule = rule_data_type_policy<
        ops::unary_homogeneous_rule<floating_point_type_domain>
    >::get();

    type_vector outputs(1, numerical_type::unknown);
    const type_vector inputs = { numerical_type::int32 };

    try
    {
        rule.deduce(make_span(outputs), make_span(inputs));
        FAIL( "expected the input to be rejected" );
    }
    catch (const std::invalid_argument &error)
    {
        const std::string message = error.what();
        using Catch::Matchers::ContainsSubstring;
        CHECK_THAT( message, ContainsSubstring("int32") );
        CHECK_THAT( message, ContainsSubstring("float16, float32, float64") );
    }
}

TEST_CASE(
    "an operation supplied pivot should fix the output type",
    "[rule_operation_data_type_policy]"
)
{
    // The astype shape: the output type is a parameter of the operation
    // instance rather than a property of the operands, so the policy is
    // owned by the operation instead of shared.
    using astype_rule = operand_type_rule<
        type_list<pivot_from_operation<0>, pivot_from_input<0>>,
        type_list<slot_same_as<0>>,
        type_list<slot_same_as<1>>
    >;

    const rule_operation_data_type_policy policy(
        pivot_descriptor_table<astype_rule::pivot_list>::get(),
        slot_descriptor_table<astype_rule::output_slot_list>::get(),
        slot_descriptor_table<astype_rule::input_slot_list>::get(),
        { numerical_type::float64 }
    );

    type_vector outputs(1, numerical_type::unknown);
    const type_vector inputs = { numerical_type::int32 };

    policy.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == numerical_type::float64 );
}
