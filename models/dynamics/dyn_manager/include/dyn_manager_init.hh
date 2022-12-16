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
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/include/dyn_manager_init.hh
 * Define the DynManagerInit class, which contains the data used to initialize
 * a DynManager object.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_manager_init.o))



*******************************************************************************/


#ifndef JEOD_DYN_MANAGER_INIT_HH
#define JEOD_DYN_MANAGER_INIT_HH


// System includes

// ER7 utilities includes
#include "er7_utils/integration/core/include/integration_technique.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes


/**
 * Namespace er7_utils contains the state integration models used by JEOD.
 */
namespace er7_utils {
   class IntegratorConstructor;
}


//! Namespace jeod
namespace jeod {

// Forward declarations
class DynamicsIntegrationGroup;

/**
 * This class contains data used to initialize a DynManager object.
 */
class DynManagerInit {

 // Data types
 public:


   /**
    * Identify modes in which the DynManager can operate.
    */
   enum EphemerisMode {
      EphemerisMode_EmptySpace   = 0,  /*
         A universe void of matter. There is only one integration frame,
         defined by the central point.  Ephemeris and gravitational models are
         disabled when the manager operates in empty space mode. */

      EphemerisMode_SinglePlanet = 1,  /*
         A universe with one massive body. There is only one integration frame,
         defined by the central point.  Ephemeris (but not gravitational) models
         are disabled when the manager operates in single planet mode. */

      EphemerisMode_Ephemerides  = 2   /*
         A universe with (potentially) many massive bodies. The ephemeris models
         establish the reference frames that may be used as integration frames
         and that may be associated with massive bodies. */
   };


 // Member data
 public:

   /**
    * Dynamics manager mode.
    */
   EphemerisMode mode; //!< trick_units(--)

   /**
    * Name of central point, used when the manager operates in empty space
    * or single planet mode.
    */
   char * central_point_name; //!< trick_units(--)

   /**
    * An integration group object used by the simulation's dynamics manager
    * to create the default integration group. The integ_group_constructor
    * does not have to be a functional integration group object; it can
    * be created using the group's default constructor.
    * If this object is not NULL, the dynamics manager will call this object's
    * create_group method to create a functional integration group object
    * to serve as the simulation's default integration group.
    * If this object is NULL, the dynamics manager will use create the
    * default integration group from the DynamicsIntegrationGroup class.
    */
   DynamicsIntegrationGroup * integ_group_constructor; //!< trick_units(--)

   /**
    * The simulation's dynamics manager uses an integrator constructor to
    * generate the dynamic manager's time integrator and to generate a state
    * integrator for each dynamic body managed by the dynamics manager.
    * The dynamics manager uses the following priority scheme to identify
    * its integrator constructor:
    * - The dynamics manager uses the DynManagerInit integ_constructor data
    * member if that member is not NULL.  Note well: This is the only way by
    * which a user-developed integration technique can be used within JEOD.
    * - The dynamics manager uses the IntegratorConstructorFactory::create
    * method to create an integrator constructor. The value supplied to
    * this method is the first of the following that specifies a
    * valid JEOD integration technique:
    * - The DynManagerInit object's jeod_integ_opt data member.
    * - The JEOD equivalent of the Trick 7 integration structure's option
    * member (Trick 7 only).
    * - The JEOD equivalent of the DynManagerInit object's sim_integ_opt
    * data member.
    */
   er7_utils::IntegratorConstructor * integ_constructor; //!< trick_units(--)

   /**
    * Integrator type. This data member provides an alternative means for
    * specifying the integration technique to be used.
    * See the integ_constructor documentation for usage.
    */
   er7_utils::Integration::Technique jeod_integ_opt; //!< trick_units(--)

   /**
    * Integrator type. This data member provides yet another alternative
    * means for specifying the integration technique to be used.
    * See the integ_constructor documentation for usage.
    */
   int sim_integ_opt; //!< trick_units(--)


 // Member functions

 public:
   // Constructor and destructor
   DynManagerInit (void);
   ~DynManagerInit (void);

 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies
 private:
   DynManagerInit (const DynManagerInit &);
   DynManagerInit & operator= (const DynManagerInit &);

};

} // End JEOD namespace


#if 0
#ifdef TRICK_VER
#include "dynamics_integration_group.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#endif
#endif


#endif

/**
 * @}
 * @}
 * @}
 */
