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
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/jeod_integrator_interface.hh
 * Define the interface for accessing / updating elements of a simulation
 * engine's integrator object.
 */

/*******************************************************************************

Purpose:
  ()

 
*******************************************************************************/


#ifndef JEOD_INTEGRATOR_INTERFACE_HH
#define JEOD_INTEGRATOR_INTERFACE_HH


// JEOD includes
#include "utils/sim_interface/include/config.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Integration includes
#include "er7_utils/integration/core/include/integration_technique.hh"
#include "er7_utils/integration/core/include/integrator_interface.hh"

// Forward declarations

// Forward declarations
#ifdef JEOD_SIM_INTEGRATOR_FORWARD
JEOD_SIM_INTEGRATOR_FORWARD
#endif


//! Namespace jeod
namespace jeod {

/**
 * A JeodIntegratorInterface extends the ER7 IntegratorInterface with
 * the concept of a pointer to the simulation engine's integration object.
 */
class JeodIntegratorInterface : public er7_utils::IntegratorInterface {
JEOD_MAKE_SIM_INTERFACES(JeodIntegratorInterface)

public:

   // Note: The default constructor, copy constructor and assignment operator
   // for this class are not declared. The C++ defaults for these are in
   // effect.

   /**
    * Destructor.
    */
   ~JeodIntegratorInterface () override {}


   // Methods

   /**
    * Interpret the integration technique
    */
   virtual er7_utils::Integration::Technique interpret_integration_type (int)
   const = 0;

   /**
    * Get the simulation engine's integrator.
    * @return Pointer to the simulation engine's integrator.
    */
   virtual JEOD_SIM_INTEGRATOR_POINTER_TYPE get_integrator () = 0;

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
