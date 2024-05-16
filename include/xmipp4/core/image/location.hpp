#pragma once

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
 * @file location.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::location class
 * @date 2024-05-15
 * 
 */

#include "../platform/constexpr.hpp"

#include <string>
#include <ostream>

namespace xmipp4 
{
namespace image
{

/**
 * @brief Defines the location of an image on a filesystem.
 * 
 * Many image formats may contain multiple images on a single file.
 * In such cases, the file is considered to be a stack. This class
 * allows to reference to a particular image inside a stack. To do so,
 * an index (position) is stored alongside the filename of the stack.
 * It can also be used to reference a single image file by setting the
 * position to no_position.
 * 
 */
class location
{
public:
    /**
     * @brief Signals that the file is not a stack.
     * 
     */
    static XMIPP4_CONST_CONSTEXPR std::size_t no_position = 0;

    /**
     * @brief Construct a location object.
     * 
     * @tparam Str String type.
     * @param filename Filename of the stack or image file.
     * @param position Position of the image inside the stack. Base 1. 
     * May be no_position.
     */
    template <typename Str>
    location(Str &&filename, std::size_t position);
    location() = default; 
    location(const location &other) = default; 
    location(location &&other) = default; 
    ~location() = default;

    location& operator=(const location &other) = default; 
    location& operator=(location &&other) = default; 

    /**
     * @brief Set the filename
     * 
     * @tparam Str String
     * @param filename Filename of the stack or image file.
     */
    template <typename Str>
    void set_filename(Str &&filename);

    /**
     * @brief Get the filename
     * 
     * @return const std::string& Filename of the stack or image file.
     */
    const std::string& get_filename() const noexcept;

    /**
     * @brief Set the position of the image inside the stack.
     * 
     * @param position Position of the image inside the stack. Base 1. 
     * May be no_position.
     */
    void set_position(std::size_t position) noexcept;

    /**
     * @brief Get the position
     * 
     * @return std::size_t Base 1 position of the image inside the stack
     * or no_position for single image files.
     */
    std::size_t get_position() const noexcept;

private:
    std::string m_filename; ///< Filename of the image object.
    std::size_t m_position; ///< 1-based index into the image object.

};


/**
 * @brief Check if a location has position in stack.
 * 
 * A location has position if its position value is different to
 * location::no_position.
 * 
 * @param loc Location object.
 * @return bool True If a it has position.
 */
XMIPP4_CONSTEXPR bool has_position(const location &loc) noexcept;

/**
 * @brief Check if a pair of location objects are contiguous in 
 * a stack.
 * 
 * Two locations are contiguous in a stack if they have the same filename
 * (this is, they refer to the same stack) and the position of the next
 * object is the position of the previous object plus one. If the locations
 * do not have a position, they cannot be contiguous.
 * 
 * @param prev Previous location.
 * @param next Next location.
 * @return bool True if they are contiguous.
 */
XMIPP4_CONSTEXPR 
bool is_contiguous(const location &prev, const location &next) noexcept;

/**
 * @brief Find a contiguous run in a range.
 * 
 * Finds the longest span of locations where adjacent objects are
 * contiguous starting from first.
 * 
 * @tparam ForwardIt Forward iterator to locations.
 * @param first First element in the range.
 * @param last Past-the-end element in the range.
 * @return ForwardIt Element where the range stops being continuous.
 * last if no such element is found.
 */
template <typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
ForwardIt find_contiguous_location_run(ForwardIt first,
                                       ForwardIt last );

/**
 * @brief Parse a location object from a path.
 * 
 * Traditionally image locations on a stack have been stored as the position
 * and filename separated by a '@'. This function parses such a pattern to
 * a location object. In case the path is not stack-like, it sets the filename
 * of the location and the position to no_position.
 * 
 * @param path Path of the stack. May be a single image filename or a '@'
 * separated position-filename pair.
 * @param result Parsed location. Not modified on failure.
 * @return true On success.
 * @return false On failure.
 */
bool parse_location(const std::string &path, location &result);

template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, const location &loc);

} // namespace image
} // namespace xmipp4

#include "location.inl"
