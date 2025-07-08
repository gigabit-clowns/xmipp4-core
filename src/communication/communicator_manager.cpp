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
 * @file communicator_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of communicator_manager.hpp
 * @date 2024-10-25
 * 
 */

#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>
#include <xmipp4/core/communication/dummy/dummy_communicator_backend.hpp>

#include <vector>
#include <algorithm>

namespace xmipp4
{
namespace communication
{

static 
std::shared_ptr<communicator> 
create_communicator(const communicator_backend* backend)
{
    std::shared_ptr<communicator> result;
    
    if (backend)
    {
        result = backend->create_world_communicator();
    }

    return result;
}



communicator_manager::~communicator_manager() = default;

void communicator_manager::register_builtin_backends()
{
    dummy_communicator_backend::register_at(*this);
}

communicator_backend* 
communicator_manager::get_preferred_backend() const
{
    communicator_backend* result;

    std::vector<communicator_backend*> available_backends;
    get_available_backends(available_backends);
    if( available_backends.empty())
    {
        result = nullptr;
    }
    else if (available_backends.size() == 1)
    {
        result = available_backends[0];
    }
    else // available_backends.size() > 1
    {
        // Get the best two
        std::partial_sort(
            available_backends.begin(),
            std::next(available_backends.begin(), 2),
            available_backends.end(),
            [] (const communicator_backend *lhs, 
                const communicator_backend *rhs )
            {
                return lhs->get_priority() > rhs->get_priority();
            }
        );

        // Ensure the second is worse
        if (available_backends[0]->get_priority() == 
            available_backends[1]->get_priority() )
        {
            throw ambiguous_backend_error(
                "Could not disambiguate among multiple "
                "communicator_backend-s. Ensure that only one has "
                "been installed."
            );
        }

        result = available_backends[0];
    }

    return result;
}

std::shared_ptr<communicator>
communicator_manager::create_world_communicator(const std::string &name) const
{
    return create_communicator(get_backend(name));
}

std::shared_ptr<communicator>
communicator_manager::create_preferred_world_communicator() const
{
    return create_communicator(get_preferred_backend());
}

} // namespace communication
} // namespace xmipp4
