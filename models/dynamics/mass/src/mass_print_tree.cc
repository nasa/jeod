/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_print_tree.cc
 * Print out the MassTree members' data to a given file
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
  ()

Library Dependency:
  ((mass_print_tree.cc)
   (mass.cc)
   (mass_print_body.cc)
   (mass_messages.cc))



*******************************************************************************/

// System includes
#include <cstddef>
#include <cstdio>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/mass.hh"
#include "../include/mass_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Print out the MassTree members' data to a given file.
 * \param[in] file_name Desired output file name
 * \param[in,out] levels Max desired recursion level
 */
void MassBody::print_tree(const char * file_name, int levels) const
{
    FILE * fptr;

    // Open the file for output.
    fptr = std::fopen(file_name, "w"); // flawfinder: ignore

    // Failure to do so is a fatal error.
    if(fptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             MassBodyMessages::io_error,
                             "Could not open file '%s' for output",
                             file_name);

        // Not reached
        return;
    }

    // Get a pointer to the root of this's tree
    const MassBody * root_ptr = get_root_body();

    // Print out info for each MassBody in current tree to given depth
    root_ptr->print_body(fptr, levels);

    std::fclose(fptr);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
