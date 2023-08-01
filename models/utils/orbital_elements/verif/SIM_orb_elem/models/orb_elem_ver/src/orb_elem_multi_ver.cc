/********************************* TRICK HEADER ***************************

PURPOSE:
  (Verification of orbital element routines)

REFERENCE:
  (((none)))

ASSUMPTIONS AND LIMITATIONS:
  (none)

CLASS:
  (scheduled)

LIBRARY DEPENDENCY:
  ((orb_elem_multi_ver.cc)
   (utils/orbital_elements/src/orbital_elements.cc))

 
************************************************************************/

// JEOD includes.
#include "../include/orb_elem_multi_ver.hh"

//! Namespace jeod
namespace jeod {

void OrbElemMultiVer::update(   // RETURN: -- None.
   OrbitalElements * orb_elem ) // INOUT:  -- Orbital elements object.
{
   int    i;           // Column index.
   double pos_in[3];   // Position vector to input for orbel conv.
   double vel_in[3];   // Velocity vector to input for orbel conv.
   double pos_out[3];  // Position vector for output from orbel conv.
   double vel_out[3];  // Velocity vector for output from orbel conv.

   // Check to see if multi-use option is on.
   if( multi_use ) {

      // Load in all the data.
      for(i=0;i<3;i++) {
         pos_in[i] = data_vals_in[data_row][i] ;
         vel_in[i] = data_vals_in[data_row][i+3];
         data_vals_inout[i] = data_vals_in[data_row][i] ;
         data_vals_inout[i+3] = data_vals_in[data_row][i+3] ;
      }

      // Compute orbital elements from cartesian state.
      orb_elem->from_cartesian( mu, pos_in, vel_in );

      // Compute cartesian state from orbital elements.
      orb_elem->to_cartesian( mu, pos_out, vel_out );

      // Store the output data.
      for(i=0;i<3;i++) {
         data_vals_out[i] = pos_out[i] ;
         data_vals_out[i+3] = vel_out[i] ;
      }

      // Move to the next data row.
      data_row++;

   }

   return;

}
} // End JEOD namespace
