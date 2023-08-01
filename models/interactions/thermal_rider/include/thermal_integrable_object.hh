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
 * @file models/interactions/thermal_rider/include/thermal_integrable_object.hh
 * Define an IntegrableObject class adapted to thermal integration
 */


/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((../src/thermal_integrable_object.cc))


*******************************************************************************/

#ifndef JEOD_THERMAL_INTEGRABLE_OBJECT_HH
#define JEOD_THERMAL_INTEGRABLE_OBJECT_HH

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrable_object.hh"
#include "er7_utils/integration/core/include/integrator_result.hh"

// JEOD includes
#include "utils/integration/include/restartable_state_integrator.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes

//! Namespace jeod
namespace jeod {

class ThermalFacetRider;

/**
 * Encapsulates a thermal integrator for a facet.
 */
class ThermalIntegrableObject : public er7_utils::IntegrableObject {

   JEOD_MAKE_SIM_INTERFACES(ThermalIntegrableObject)

public:

   // Member methods

   ThermalIntegrableObject();
   ~ThermalIntegrableObject() override;

   void initialize (double temperature, ThermalFacetRider &associated_rider);

   // Required by IntegrableObject
   void create_integrators (
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls,
      const er7_utils::TimeInterface & time_if) override;

   // Destroy the integrators.
   void destroy_integrators () override; //cppcheck-suppress virtualCallInConstructor

   // Reset the integrators.
   void reset_integrators () override;

   // Propagate state.
   er7_utils::IntegratorResult integrate (
      double dyn_dt, unsigned int target_stage) override;

   // Computes time derivative of temperature and sets rider->power_emitted
   void compute_temp_dot();

   //Accessors

   /**
    * Get the temperature.
    * @return Facet temperature.
    */
   double get_temp ()
   {
      return temp;
   }

   /**
    * Get the temperature time derivative.
    * @return Facet temperature time derivative.
    */
   double get_temp_dot ()
   {
      return temp_dot;
   }


   // Member data

   /**
    * If true, this IntegrableObject will integrate temperature.
    */
   bool active; //!< trick_units(--)

private:

   // Member data
   /**
    * Integrates temperature on one facet
    */
   RestartableScalarFirstOrderODEIntegrator integrator; //!< trick_units(--)

   /**
    * Cached pointer to the associated ThermalFacetRider
    */
   ThermalFacetRider *rider; //!< trick_units(--)

   /**
    * Temperature of the facet
    */
   double temp; //!< trick_units(K)

   /**
    * Time derivative of temperature
    */
   double temp_dot; //!< trick_units(K/s)

   /**
    * Fourth power of current temperature
    */
   double t_pow4; //!< trick_units(K*K*K*K)


// Assignment and copy constructors are private and unimplemented
   ThermalIntegrableObject& operator = (const ThermalIntegrableObject& rhs);
   ThermalIntegrableObject(const ThermalIntegrableObject& rhs);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
