/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_properties.cc
 * Define basic methods for the MassProperties class.
 */

/*******************************************************************************
  Purpose:
    ()

  Reference:
    (((TBS)))

  Assumptions and limitations:
    ((N/A))

  Class:
    (N/A)

  LIBRARY DEPENDENCY:
    ((mass_properties.o)
    (mass_point.o)
    (mass_point_state.o))


*******************************************************************************/


// System includes

// JEOD includes
#include "utils/math/include/matrix3x3.hh"

// Model includes
#include "../include/mass_properties.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor; constructs a MassProperties object.
 */
MassProperties::MassProperties (
   void)
{
   // Initialize top-level member data to a mass of zero.
   mass = 0.0;
   Matrix3x3::initialize (inertia);
   inverse_mass = 0.0;
   Matrix3x3::initialize(inverse_inertia);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
