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


#include <xmipp4/metadata/table.hpp>

namespace xmipp4
{
namespace metadata
{


void table::resize(std::size_t size)
{
    allocation_context context(size);
    resize(context);
}

void table::reserve(std::size_t capacity)
{   
    const auto size = m_allocated.get_size();
    allocation_context context(size, std::max(capacity, size));
    resize(context);
}


void table::resize(const allocation_context& context)
{
    if(m_allocated != context)
    {
        size_t i;
        try 
        {
            // Resize all columns
            for(i = 0; i < m_columns.size(); ++i) 
            {
                m_columns[i]->resize(m_allocated, context);
            }
        } 
        catch (...)
        {
            // An exception occurred when resizing. Undo commited changes
            for(size_t j = 0; j < i; ++j) 
            {
                m_columns[j]->resize(context, m_allocated);
            }
            throw;
        }

        // Commit changes
        m_allocated = context;
    }
}


void table::add_column(std::string label, 
                       std::unique_ptr<column_base> column, 
                       const allocation_context& allocated )
{
    if(column)
    {
        // Try to insert the label and the corresponding index
        decltype(m_label_to_index)::iterator ite;
        bool inserted;
        std::tie(ite, inserted) = m_label_to_index.emplace(
            std::move(label), m_columns.size()
        );

        if(inserted) 
        {
            // Insert the column itself
            try 
            {
                column->resize(allocated, m_allocated);
                m_columns.push_back(std::move(column));
            } 
            catch(...)
            {
                // Remove the label
                m_label_to_index.erase(ite);
                throw;
            }
        } 
        else
        {
            throw std::runtime_error(
                "A column with the provided label already exists in the table"
            );
        }
    }
}

} // namespace metadata
} // namespace xmipp4

