/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_print_body.cc
 * Define MassBody::print_body.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Library Dependency:
  ((mass_print_body.cc)
   (mass.cc))



******************************************************************************/


// System includes
#include <cstdio>

// JEOD includes
#include "utils/ref_frames/include/tree_links_iterator.hh"

// Model includes
#include "../include/mass.hh"



//! Namespace jeod
namespace jeod {

/**
 * Recursively print out the data in this and its children to given file
 * \param[in,out] file_ptr Output file stream
 * \param[in,out] levels Max desired recursion level
 */
void
MassBody::print_body (
   FILE *file_ptr,
   int levels)
const
{

   // Sanity check; cannot recurse negative number of levels
   if (levels < 0) {
      return;
   }

   // Print this's properties
   fprintf(file_ptr, "\n\n"
            "=============================================================");
   fprintf(file_ptr, "\n\n%s", this->name.c_str());
   fprintf(file_ptr, "\n"
            "-------------------------------------------------------------");
   fprintf(file_ptr, "\n\nBody Area");
   fprintf(file_ptr, "\nOffset Vector [m]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->structure_point.position[0],
            this->structure_point.position[1],
            this->structure_point.position[2]);
   fprintf(file_ptr, "\nT_struct_struct [-]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->structure_point.T_parent_this[0][0],
            this->structure_point.T_parent_this[0][1],
            this->structure_point.T_parent_this[0][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->structure_point.T_parent_this[1][0],
            this->structure_point.T_parent_this[1][1],
            this->structure_point.T_parent_this[1][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->structure_point.T_parent_this[2][0],
            this->structure_point.T_parent_this[2][1],
            this->structure_point.T_parent_this[2][2]);
   fprintf(file_ptr, "\n"
            "-------------------------------------------------------------");
   fprintf(file_ptr, "\nMass Properties");
   fprintf(file_ptr, "\nM.P. CM vector [m]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->core_properties.position[0],
            this->core_properties.position[1],
            this->core_properties.position[2]);
   fprintf(file_ptr, "\nM.P. Mass [kg]:\n % 20lf", this->core_properties.mass);
   fprintf(file_ptr, "\nM.P. Ib tensor [kgM2]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->core_properties.inertia[0][0],
            this->core_properties.inertia[0][1],
            this->core_properties.inertia[0][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->core_properties.inertia[1][0],
            this->core_properties.inertia[1][1],
            this->core_properties.inertia[1][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->core_properties.inertia[2][0],
            this->core_properties.inertia[2][1],
            this->core_properties.inertia[2][2]);
   fprintf(file_ptr, "\nM.P. T_struct_body [q]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->core_properties.T_parent_this[0][0],
            this->core_properties.T_parent_this[0][1],
            this->core_properties.T_parent_this[0][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->core_properties.T_parent_this[1][0],
            this->core_properties.T_parent_this[1][1],
            this->core_properties.T_parent_this[1][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->core_properties.T_parent_this[2][0],
            this->core_properties.T_parent_this[2][1],
            this->core_properties.T_parent_this[2][2]);
   fprintf(file_ptr, "\n"
            "-------------------------------------------------------------");
   fprintf(file_ptr, "\nComposite Mass Properties");
   fprintf(file_ptr, "\nC.M.P. CM vector [m]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.position[0],
            this->composite_properties.position[1],
            this->composite_properties.position[2]);
   fprintf(file_ptr, "\nC.M.P. Mass [kg]:\n % 20lf",
            this->composite_properties.mass);
   fprintf(file_ptr, "\nC.M.P. Ib tensor [kgM2]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.inertia[0][0],
            this->composite_properties.inertia[0][1],
            this->composite_properties.inertia[0][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.inertia[1][0],
            this->composite_properties.inertia[1][1],
            this->composite_properties.inertia[1][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.inertia[2][0],
            this->composite_properties.inertia[2][1],
            this->composite_properties.inertia[2][2]);
   fprintf(file_ptr, "\nC.M.P. T_struct_body [q]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.T_parent_this[0][0],
            this->composite_properties.T_parent_this[0][1],
            this->composite_properties.T_parent_this[0][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.T_parent_this[1][0],
            this->composite_properties.T_parent_this[1][1],
            this->composite_properties.T_parent_this[1][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.T_parent_this[2][0],
            this->composite_properties.T_parent_this[2][1],
            this->composite_properties.T_parent_this[2][2]);
   fprintf(file_ptr, "\n"
            "-------------------------------------------------------------");
   fprintf(file_ptr, "\nDerived Items");
   fprintf(file_ptr, "\nC.M.P. Inverse mass [1/kg]:\n % 20lf",
            this->composite_properties.inverse_mass);
   fprintf(file_ptr, "\nC.M.P. Inverse inertia tensor [1/(kgM2)]:");
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.inverse_inertia[0][0],
            this->composite_properties.inverse_inertia[0][1],
            this->composite_properties.inverse_inertia[0][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.inverse_inertia[1][0],
            this->composite_properties.inverse_inertia[1][1],
            this->composite_properties.inverse_inertia[1][2]);
   fprintf(file_ptr, "\n % 20lf% 20lf% 20lf",
            this->composite_properties.inverse_inertia[2][0],
            this->composite_properties.inverse_inertia[2][1],
            this->composite_properties.inverse_inertia[2][2]);
   fprintf(file_ptr, "\n"
            "-------------------------------------------------------------");


   // Print out children of this if requested
   if (levels > 0) {

      // Reduce the recursion count
      levels -= 1;

      // Traverse this's children and print them out
      for (auto* link : TreeLinksChildrenRange<const MassBodyLinks>(
              links)) {
         const MassBody* child = &(link->container());
         child->print_body(file_ptr, levels);
      }
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
