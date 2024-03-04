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

TEST_CASE( "memory map a file", "[memory_mapped_file]" ) 
{
    const std::string path = "memory_map.txt";
    const std::string data = 
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    std::ofstream output_file(path.c_str());
    output_file.write(data.c_str(), data.size());
    output_file.close();

    SECTION ("Read first few bytes")
    {
        system::memory_mapped_file mm(
            path, 
            read_only, 
            8,
            false
        );

        REQUIRE( mm.size() == 8 );
        REQUIRE( std::strncmp(static_cast<const char*>(mm.data()), data.c_str(), 8) == 0 );
    }

    SECTION ("Read the entire file")
    {
        system::memory_mapped_file mm(
            path, 
            read_only, 
            system::memory_mapped_file::whole_file,
            false
        );

        REQUIRE( mm.size() == data.size() );
        REQUIRE( std::strncmp(static_cast<const char*>(mm.data()), data.c_str(), mm.size()) == 0 );
    }

    SECTION ("Write the entire file")
    {
        system::memory_mapped_file mm(
            path, 
            read_write, 
            system::memory_mapped_file::whole_file,
            false
        );

        REQUIRE( mm.size() == data.size() );
        REQUIRE( std::strncmp(static_cast<const char*>(mm.data()), data.c_str(), mm.size()) == 0 );

        char *memory_mapped_data = static_cast<char*>(mm.data());
        for(std::size_t i = 0; i < data.size(); ++i)
        {
            memory_mapped_data[i] = static_cast<char>('a' + i);
        }

        mm.close();
        mm.open(
            path, 
            read_only, 
            system::memory_mapped_file::whole_file,
            false
        );

        memory_mapped_data = static_cast<char*>(mm.data());
        for(std::size_t i = 0; i < data.size(); ++i)
        {
            REQUIRE( memory_mapped_data[i] == static_cast<char>('a' + i) );
        }
    }

    SECTION ("Write the entire file as copy on write")
    {
        system::memory_mapped_file mm(
            path, 
            read_write, 
            system::memory_mapped_file::whole_file,
            true
        );

        REQUIRE( mm.size() == data.size() );
        REQUIRE( std::strncmp(static_cast<const char*>(mm.data()), data.c_str(), mm.size()) == 0 );

        char *memory_mapped_data = static_cast<char*>(mm.data());
        for(std::size_t i = 0; i < data.size(); ++i)
        {
            memory_mapped_data[i] = static_cast<char>('a' + i);
        }

        mm.close();
        mm.open(
            path, 
            read_only, 
            system::memory_mapped_file::whole_file,
            false
        );

        REQUIRE( mm.size() == data.size() );
        REQUIRE( std::strncmp(static_cast<const char*>(mm.data()), data.c_str(), mm.size()) == 0 );
    }
}
