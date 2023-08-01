//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/include/aero_facet.hh
 * Individual facets for use with aero environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/aero_facet.cc))


*******************************************************************************/

#ifndef AERO_FACET_HH
#define AERO_FACET_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/interaction_facet.hh"

//! Namespace jeod
namespace jeod {

class AeroDragParameters;

/**
 * An aerodynamic interaction specific facet for use in the surface model.
 */
class AeroFacet : public InteractionFacet {

   JEOD_MAKE_SIM_INTERFACES(AeroFacet)

public:

   // constructor
   AeroFacet ();

   // destructor
   ~AeroFacet () override;


   /**
    * A pure virtual function defining the interface for all
    * aerodynamic interaction facets. All aerodynamic interaction
    * facets inherited from AeroFacet must implement this function
    * \param[in] velocity_mag The magnitude of the relative inertial velocity, including wind, of the vehicle\n Units: m/s
    * \param[in] rel_vel_hat The Unit vector of the relative inertial velocity
    * \param[in] aero_drag_param_ptr The parameters used to calculate aerodynamic drag
    * \param[in] center_grav The position of the center of graviy of the vehicle, in the structural frame\n Units: m
    */

   virtual void aerodrag_force(
      const double velocity_mag,
      const double rel_vel_hat[3],
      AeroDragParameters* aero_drag_param_ptr,
      double center_grav[3])
 = 0;


protected:

private:

   /* Operator = and copy constructor hidden from use by being private */

   AeroFacet& operator = (const AeroFacet& rhs);
   AeroFacet (const AeroFacet& rhs);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
