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
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/include/mass_properties_init.hh
 * Define class MassPropertiesInit, which initializes a mass properties object.
 */

/********************************* TRICK HEADER *******************************

Purpose:
 ()

Library Dependencies:
 ((../src/mass_properties_init.cc))



*******************************************************************************/


#ifndef JEOD_MASS_PROPERTIES_INIT_HH
#define JEOD_MASS_PROPERTIES_INIT_HH


// Model includes
#include "class_declarations.hh"
#include "mass_point_init.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/orientation/include/orientation.hh"



//! Namespace jeod
namespace jeod {

/**
 * Contains data used to initialize a mass model object.
 */
class MassPropertiesInit : public MassPointInit {

   JEOD_MAKE_SIM_INTERFACES(MassPropertiesInit)

 // Enumerations

 public:

   /**
    * Specifies the origin and axes of the input inertia tensor.
    */
   enum InertiaSpec {
      NoSpec    = 0, ///< Inertia matrix is specified directly.
      Body      = 1, ///< Initial inertia frame is body frame.
      StructCG  = 2, ///< Initial inertia frame is struct at CG.
      Struct    = 3, ///< Initial inertia frame is structural frame.
      SpecCG    = 4, ///< Initial inertia frame is in frame at CG.
      Spec      = 5  ///< Initial inertia frame is specified frame.
   };


 // Member data
 public:

   /**
    * Mass of the core body.
    */
   double mass; //!< trick_units(kg)

   /**
    * Location of the core body center of mass in the structural frame.
    * This is just an alias for the generic position member.
    */
   double * cm; //!< trick_units(m)

   /**
    * Inertia tensor of the core body in an arbitrary reference frame.
    */
   double inertia[3][3]; //!< trick_units(kg*m2)

   /**
    * Indicates how the user has specified the core inertia matrix.
    */
   InertiaSpec inertia_spec; //!< trick_units(--)

   /**
    * Offset from inertia reference frame to the core center of mass,
    * expressed in inertia reference frame coordinates.
    */
   double inertia_offset[3]; //!< trick_units(m)

   /**
    * Orientation of body frame wrt inertia reference frame, i.e. the process by
    * which the frame in which the inertia tensor is specified may be
    * transformed to the body frame, e.g. T_{spec->body}.
    */
   Orientation inertia_orientation; //!< trick_units(--)


 // Member functions
 public:

   // Constructor.
   MassPropertiesInit (void);

   // Initialize a mass body.
   void initialize_mass_properties (MassProperties & mass_properties) const;

};

} // End JEOD namespace


#endif

/**
 * @}
 * @}
 * @}
 */
