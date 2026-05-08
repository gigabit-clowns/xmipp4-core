// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include "mock/mock_kernel.hpp"
#include "mock/mock_kernel_builder.hpp"
#include "mock/mock_operation.hpp"
#include "../hardware/mock/mock_buffer_sentinel.hpp"
#include "../hardware/mock/mock_device.hpp"
#include "../hardware/mock/mock_device_executor.hpp"
#include "../hardware/mock/mock_device_backend.hpp"
#include "../hardware/mock/mock_memory_resource.hpp"
#include "../hardware/mock/mock_memory_allocator.hpp"
#include "../hardware/mock/mock_memory_allocator_backend.hpp"


#include <stdexcept>

using namespace xmipp4;
using namespace xmipp4::hardware;
using namespace xmipp4::multidimensional;

namespace {

class operation_execute_fixture
{
public:
    service_catalog catalog;
    std::shared_ptr<mock_device> device;
    mock_memory_resource device_resource;
    mock_memory_resource host_resource;
    std::shared_ptr<mock_memory_allocator> device_allocator;
    std::shared_ptr<mock_memory_allocator> host_allocator;
    mock_operation operation;
    std::shared_ptr<mock_kernel> kernel;
    std::unique_ptr<execution_context> context;
    std::vector<std::unique_ptr<trompeloeil::expectation>> expectations;

	operation_execute_fixture()
		: catalog(false)
		, device(std::make_shared<mock_device>())
		, device_allocator(std::make_shared<mock_memory_allocator>())
		, host_allocator(std::make_shared<mock_memory_allocator>())
		, kernel(std::make_shared<mock_kernel>())
	{
	}

    execution_context& 
	setup_context(const std::shared_ptr<mock_device_executor>& queue) 
	{
        const hardware::device_index index("mock", 1234);

        auto device_backend = std::make_unique<mock_device_backend>();
        auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();

        REQUIRE_CALL(*device_backend, get_name())
            .RETURN(index.get_backend_name());
        REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), trompeloeil::_))
            .LR_SIDE_EFFECT(_2.set_optimal_data_alignment(256))
            .RETURN(true);
        REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
            .LR_RETURN(device);
        
        REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::device))
            .LR_RETURN(device_resource);
        REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::host))
            .LR_RETURN(host_resource);

        REQUIRE_CALL(*allocator_backend, get_suitability(trompeloeil::_))
            .LR_WITH(&_1 == &device_resource)
			.RETURN(backend_priority::normal);
        REQUIRE_CALL(*allocator_backend, create_memory_allocator(trompeloeil::_))
            .LR_WITH(&_1 == &device_resource)
			.LR_RETURN(device_allocator);
            
        REQUIRE_CALL(*allocator_backend, get_suitability(trompeloeil::_))
            .LR_WITH(&_1 == &host_resource)
			.RETURN(backend_priority::normal);
        REQUIRE_CALL(*allocator_backend, create_memory_allocator(trompeloeil::_))
            .LR_WITH(&_1 == &host_resource)
			.LR_RETURN(host_allocator);

        catalog.get_service_manager<device_manager>().register_backend(std::move(device_backend));
        catalog.get_service_manager<memory_allocator_manager>().register_backend(std::move(allocator_backend));

        context = std::make_unique<execution_context>(catalog, index);
        context->set_active_queue(queue);

        return *context;
    }

    array_descriptor create_descriptor() const 
	{
        const std::array<std::size_t, 4> extents = {8, 64, 3, 4};
        return array_descriptor(
            strided_layout::make_contiguous_layout(make_span(extents)),
            numerical_type::float32
        );
    }

    std::vector<std::shared_ptr<buffer>> 
	create_device_buffers(std::size_t count, const array_descriptor& descriptor) 
	{
        const auto size = compute_storage_requirement(descriptor);
        std::vector<std::shared_ptr<buffer>> result;
        result.reserve(count);
        for (std::size_t i = 0; i < count; ++i) 
		{
            result.push_back(
				std::make_shared<buffer>(
					nullptr, 
					size, 
					device_resource, 
					std::make_unique<mock_buffer_sentinel>()
            	)
			);
        }
        return result;
    }

    template<typename ArrayT>
    std::vector<ArrayT> create_device_arrays(
		const std::vector<std::shared_ptr<buffer>>& buffers, 
		const array_descriptor& descriptor
	) 
	{
        auto& queue = context->get_active_queue();
        std::vector<ArrayT> result;
        result.reserve(buffers.size());
        for (const auto& buf : buffers) 
		{
            REQUIRE_CALL(*device_allocator, get_max_alignment())
				.RETURN(256);
            REQUIRE_CALL(*device_allocator, allocate(buf->get_size(), 256, queue.get()))
				.RETURN(buf);
            result.push_back(empty(descriptor, memory_resource_affinity::device, *context));
        }
        return result;
    }

    void allow_default_allocator_queries() 
	{
		expectations.push_back(
			NAMED_ALLOW_CALL(*device_allocator, get_max_alignment())
				.RETURN(256)
		);
    }

    void register_kernel_builder(std::unique_ptr<mock_kernel_builder> builder) 
	{
        REQUIRE_CALL(*builder, get_operation_id()).LR_RETURN(operation.get_id());
        catalog.get_service_manager<kernel_manager>().register_kernel(std::move(builder));
    }

    void expect_queue_records(const std::vector<std::shared_ptr<buffer>>& buffers, device_executor& queue) {
        for (const auto& buffer : buffers) 
		{
            auto* sentinel = static_cast<mock_buffer_sentinel*>(buffer->get_sentinel());
            expectations.push_back(
                NAMED_REQUIRE_CALL(*sentinel, record_queue(trompeloeil::_, false))
					.LR_WITH(&_1 == &queue)
            );
        }
    }
};

} // anonymous namespace



TEST_CASE_METHOD(operation_execute_fixture, "execute should execute a properly configured kernel", "[operation_execute]") {
    auto queue = GENERATE(
		std::shared_ptr<mock_device_executor>(), 
		std::make_shared<mock_device_executor>()
	);

    auto& context = setup_context(queue);
    allow_default_allocator_queries();
    
    const auto descriptor = create_descriptor();
    const auto input_buffers = create_device_buffers(3, descriptor);
    const auto input_arrays = create_device_arrays<array_view>(input_buffers, descriptor);
    
    const auto output_buffers = create_device_buffers(2, descriptor);
    std::vector<array> output_arrays;

    SECTION("provided a pre-allocated output should re-use") 
	{
        output_arrays = create_device_arrays<array>(output_buffers, descriptor);
    }
    SECTION("if empty output is provided it should allocate") 
	{
        output_arrays.resize(2); // Empties
        for (const auto& buffer : output_buffers) {
            expectations.push_back(
                NAMED_REQUIRE_CALL(*device_allocator, allocate(buffer->get_size(), 256, queue.get()))
                    .RETURN(buffer)
            );
        }
    }


    auto kernel_builder = std::make_unique<mock_kernel_builder>();
    const array_signature signature(descriptor, &device_resource);

    REQUIRE_CALL(*kernel_builder, get_suitability(trompeloeil::_, trompeloeil::_, trompeloeil::_))
        .LR_WITH(&_1 == &operation)
        .WITH(_2.size() == 2 && _2[0] == signature && _2[1] == signature)
        .WITH(_3.size() == 3 && _3[0] == signature && _3[1] == signature && _3[2] == signature)
        .RETURN(backend_priority::normal);

    REQUIRE_CALL(*kernel_builder, build(trompeloeil::_, trompeloeil::_, trompeloeil::_))
        .LR_WITH(&_1 == &operation)
        .WITH(_2.size() == 2 && _2[0] == signature && _2[1] == signature)
        .WITH(_3.size() == 3 && _3[0] == signature && _3[1] == signature && _3[2] == signature)
        .LR_RETURN(kernel);

    register_kernel_builder(std::move(kernel_builder));

    REQUIRE_CALL(operation, sanitize_operands(trompeloeil::_, trompeloeil::_))
        .WITH(_1.size() == 2)
        .LR_WITH(_1[0] == output_arrays[0].get_descriptor() && _1[1] == output_arrays[1].get_descriptor())
        .WITH(_2.size() == 3 && _2[0] == descriptor && _2[1] == descriptor && _2[2] == descriptor)
        .SIDE_EFFECT(_1[0] = descriptor)
        .SIDE_EFFECT(_1[1] = descriptor);

    REQUIRE_CALL(*kernel, execute(trompeloeil::_, trompeloeil::_, queue.get()))
        .WITH(_1.size() == 2)
        .WITH(_2.size() == 3 && _2[0] == input_buffers[0] && _2[1] == input_buffers[1] && _2[2] == input_buffers[2]);

    if(queue) 
	{
        expect_queue_records(input_buffers, *queue);
        expect_queue_records(output_buffers, *queue);
    }

    execute(operation, make_span(output_arrays), make_span(input_arrays), context);
}


TEST_CASE_METHOD(operation_execute_fixture, "execute should throw if an storage-less input array is provided", "[operation_execute]") {
    auto queue = GENERATE(
		std::shared_ptr<mock_device_executor>(), 
		std::make_shared<mock_device_executor>()
	);
    auto& context = setup_context(queue);
    allow_default_allocator_queries();
    
    const auto descriptor = create_descriptor();
    const auto input_buffers = create_device_buffers(2, descriptor);
    auto input_arrays = create_device_arrays<array_view>(input_buffers, descriptor);
    
    input_arrays.emplace_back(array(nullptr, descriptor));

    const auto output_buffers = create_device_buffers(2, descriptor);
    auto output_arrays = create_device_arrays<array>(output_buffers, descriptor);

    register_kernel_builder(std::make_unique<mock_kernel_builder>());

    REQUIRE_CALL(operation, sanitize_operands(trompeloeil::_, trompeloeil::_))
        .WITH(_1.size() == 2 && _1[0] == descriptor && _1[1] == descriptor)
        .WITH(_2.size() == 3 && _2[0] == descriptor && _2[1] == descriptor && _2[2] == descriptor);

    REQUIRE_THROWS_MATCHES(
        execute(operation, make_span(output_arrays), make_span(input_arrays), context),
        std::invalid_argument,
        Catch::Matchers::Message( 
            "One of the input operands does not have associated storage. "
            "Input arrays must be populated before calling execute."
        )
    );
}
