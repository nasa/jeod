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
 * @addtogroup ThermalRider
 * @{
 *
 * @file models/interactions/thermal_rider/include/thermal_params.hh
 * A virtual base class for thermal facet parameters, used to add to the
 * parameter lists for specific-model facets in their respective
 * InteractionSurfaceFactorys
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/thermal_params.cc))


*******************************************************************************/

#ifndef JEOD_THERMAL_PARAMS_HH
#define JEOD_THERMAL_PARAMS_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes



//! Namespace jeod
namespace jeod {

/**
 * A virtual base class for thermal facet parameters, used to add to the
 * parameter lists for specific-model facets in their respective
 * InteractionSurfaceFactorys
 */
class ThermalParams {
   JEOD_MAKE_SIM_INTERFACES(ThermalParams)


public:
   /**
    * Fraction of sigma-T^4 (potential for emissive thermal) that is
    * actually emitted.
    */
  double emissivity; //!< trick_units(--)

   /**
    * Heat Capacity per unit area of surface.
    */
  double heat_capacity_per_area; //!< trick_units(--)

   /**
    * Rate at which thermal energy is dumped (positive) / extracted
    * (negative) into a facet from within the vehicle. Used for radiators and
    * the like.
    */
  double thermal_power_dump; //!< trick_units(--)

public:

   // constructor
   ThermalParams ();

   // destructor
   virtual ~ThermalParams ();

protected:

private:

   ThermalParams& operator = (const ThermalParams& rhs);
   ThermalParams (const ThermalParams& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
