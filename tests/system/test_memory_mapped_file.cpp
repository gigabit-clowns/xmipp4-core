/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file test_memory_mapped_file.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for system/memory_mapped_file.hpp
 * @date 2024-03-01
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/system/memory_mapped_file.hpp>
#include <xmipp4/access_flags.hpp>

#include <fstream>
#include <string>
#include <cstring>

using namespace xmipp4;

static const std::string test_data = 
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";

static std::string write_test_data()
{
    const std::string path = "memory_map.txt";
    std::ofstream output_file(path.c_str());
    output_file.write(test_data.c_str(), test_data.size());
    return path;
}

static bool check_test_data(const char* data, std::size_t count)
{
    return std::strncmp(data, test_data.c_str(), count) == 0;
}

static void fill_test_pattern(char* data, std::size_t count)
{
    for(std::size_t i = 0; i < count; ++i)
    {
        data[i] = static_cast<char>('a' + i);
    }
}

static bool check_test_pattern(const char* data, std::size_t count)
{
    for(std::size_t i = 0; i < count; ++i)
    {
        if(data[i] != static_cast<char>('a' + i))
            return false;
    }
    return true;
}

TEST_CASE( "memory map a file for reading", "[memory_mapped_file]" ) 
{
    const auto path = write_test_data();

    SECTION ("Read first few bytes")
    {
        system::memory_mapped_file mm(
            path, 
            read_only, 
            8,
            false
        );

        // Expect the first 8 bytes to be equal to the test data
        REQUIRE( mm.is_open() );
        REQUIRE( mm.size() == 8 );
        REQUIRE( check_test_data(static_cast<const char*>(mm.data()), mm.size()) );
    }

    SECTION ("Read the entire file")
    {
        system::memory_mapped_file mm(
            path, 
            read_only, 
            system::memory_mapped_file::whole_file,
            false
        );
        
        // Expect the whole range to be equal to the test data
        REQUIRE( mm.is_open() );
        REQUIRE( mm.size() == test_data.size() );
        REQUIRE( check_test_data(static_cast<const char*>(mm.data()), mm.size()) );
    }

   SECTION ("Write the entire file as copy on write")
    {
        system::memory_mapped_file mm(
            path, 
            read_write, 
            system::memory_mapped_file::whole_file,
            true
        );

        // Check that the test data has been read
        REQUIRE( mm.is_open() );
        REQUIRE( mm.size() == test_data.size() );
        REQUIRE( check_test_data(static_cast<const char*>(mm.data()), mm.size()) );

        // Write some random data and close
        fill_test_pattern(static_cast<char*>(mm.data()), mm.size());
        mm.close();
        REQUIRE( !mm.is_open() );

        // Re-open and expect the original test data
        mm.open(
            path, 
            read_only, 
            system::memory_mapped_file::whole_file,
            false
        );

        REQUIRE( mm.is_open() );
        REQUIRE( mm.size() == test_data.size() );
        REQUIRE( check_test_data(static_cast<const char*>(mm.data()), mm.size()) );
    }
}

TEST_CASE( "memory map a file for writing", "[memory_mapped_file]" ) 
{
    const auto path = write_test_data();

    system::memory_mapped_file mm(
        path, 
        read_write, 
        system::memory_mapped_file::whole_file,
        false
    );

    // Check that the test data has been read
    REQUIRE( mm.is_open() );
    REQUIRE( mm.size() == test_data.size() );
    REQUIRE( check_test_data(static_cast<const char*>(mm.data()), mm.size()) );

    // Write some random data and close
    fill_test_pattern(static_cast<char*>(mm.data()), mm.size());
    mm.close();
    REQUIRE( !mm.is_open() );

    // Re-open and expect the modified test data
    mm.open(
        path, 
        read_only, 
        system::memory_mapped_file::whole_file,
        false
    );

    REQUIRE( mm.is_open() );
    REQUIRE( mm.size() == test_data.size() );
    REQUIRE( check_test_pattern(static_cast<const char*>(mm.data()), mm.size()) );
}
