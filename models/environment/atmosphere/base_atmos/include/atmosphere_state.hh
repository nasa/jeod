//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/base_atmos/include/atmosphere.hh
 * General base class for atmosphere models.
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (Provides a generic atmosphere state as a base class for inheritance when
   creating classes to represent specific atmosphere models.
   AtmosphereState gives the base parameters expected back from an atmosphere
   model. If more parameters are needed they can either be included in an
   inheriting object from AtmosphereState, or added to a model inheriting
   from Atmosphere as member variables. AtmosphereState also gives the
   interface for calculating state, taking a polymorphic AtmosphereModel class
   as it's method for calculating the state.)
Library dependencies:
   (atmosphere_state.o)
*******************************************************************************/

#ifndef JEOD_ATMOSPHERE_STATE_HH
#define JEOD_ATMOSPHERE_STATE_HH

// System includes

// JEOD includes
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"
#include "environment/time/include/time_standard.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Local includes
#include "atmosphere.hh"
#include "wind_velocity.hh"

//! Namespace jeod
namespace jeod {

/**
 * A generic base class for atmosphere state, containing common
 * atmosphere state parameters,
 * i.e. pressure, density, temperature, wind velocity
 */
class AtmosphereState {

   JEOD_MAKE_SIM_INTERFACES(AtmosphereState)

  public: // public member variables

   bool   active;      /*!< trick_units(--)
      Activation flag for computing state.*/
   double temperature; /*!< trick_units(K)
      Temperature at altitude*/
   double density;     /*!< trick_units(kg/m3)
      total density at altitude*/
   double pressure;    /*!< trick_units(N/m2)
      Total pressure*/
   double wind[3];     /*!< trick_units(m/s)
      Wind velocity*/

  protected:
   Atmosphere                * atmos;
   const PlanetFixedPosition * pfix_pos;

  public: // public member functions

   // Constructors
   AtmosphereState ();
   AtmosphereState ( Atmosphere                & atmos,
                     const PlanetFixedPosition & pfix_pos);


   // destructor
   virtual ~AtmosphereState ();

   // Operator = for AtmosphereState
   AtmosphereState& operator = (const AtmosphereState& rhs);

   // Copy constructor for AtmosphereState
   AtmosphereState (const AtmosphereState& rhs);

   /* Updates this particular atmosphere state from a particular atmosphere.*/
   void update_state (Atmosphere * atmos_model_, PlanetFixedPosition * pfix_pos_);
   virtual void update_state ();

   /* Updates this particular atmosphere state from a particular wind model. */

   void update_wind (
      WindVelocity * wind_vel, double inrtl_pos[3], double altitude);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
