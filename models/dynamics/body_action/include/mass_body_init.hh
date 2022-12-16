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
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/include/mass_body_init.hh
 * Define the class MassBodyInit, the base class used for initializing the
 * core mass properties of a MassBody object.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((mass_body_init.o))



*******************************************************************************/


#ifndef JEOD_MASS_BODY_INIT_HH
#define JEOD_MASS_BODY_INIT_HH

// System includes

// JEOD includes
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "dynamics/mass/include/class_declarations.hh"
#include "dynamics/mass/include/mass_properties_init.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "body_action.hh"




//! Namespace jeod
namespace jeod {

/**
 * Base class for initializing a MassBody.
 * Items initialized by this action are
 * - The body's core mass properties
 * - The body's mass points.
 */
class MassBodyInit : public BodyAction {

 JEOD_MAKE_SIM_INTERFACES(MassBodyInit)


 // Member data

 public:

   /**
    * Specifications for the subject mass body's core mass properties.
    */
   MassPropertiesInit properties; //!< trick_units(--)

   /**
    * Specifications for the subject mass body's mass points.
    */
   MassPointInit * points; //!< trick_units(--)

   /**
    * Size of the points array.
    */
   unsigned int num_points; //!< trick_units(--)


 // Methods

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   MassBodyInit (const MassBodyInit&);
   MassBodyInit & operator = (const MassBodyInit&);


 public:

   // Default constructor.
   MassBodyInit ();

   // Destructor.
   virtual ~MassBodyInit ();

   // apply: Initialize the subject body's mass properties and mass points.
   virtual void apply (DynManager & dyn_manager);

};


/**
 * Destructor.
 */
inline
MassBodyInit::~MassBodyInit (
   void)
{
   ; // Empty
}

} // End JEOD namespace

#ifdef TRICK_VER
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
