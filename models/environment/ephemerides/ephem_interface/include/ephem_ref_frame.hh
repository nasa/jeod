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
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemInterface
 * @{
 *
 * @file models/environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh
 * Define the class EphemerisRefFrame.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((ephem_ref_frame.o))


*******************************************************************************/


#ifndef JEOD_EPHEM_REF_FRAME_HH
#define JEOD_EPHEM_REF_FRAME_HH

// System includes
#include <cstddef>

// JEOD includes
#include "utils/ref_frames/include/ref_frame.hh"
#include "utils/ref_frames/include/ref_frame_interface.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

class BaseEphemeridesManager;

/**
 * An EphemerisRefFrame is a RefFrame whose state is set by an ephemeris model.
 * Ephemeris reference frames come in three basic flavors:
 * - Planet-centered inertial frames. These are non-rotating frames whose
 *   origin is the center of mass of some planet (the Sun is a planet)
 *   and whose translational motion is (for now) assumed to be due to
 *   gravitation only. The orientation with respect to inertial is the
 *   identity transformation.
 * - Barycenter inertial frames. These are non-rotating frames whose origin
 *   is the center of mass of two or more planets.
 * - Planet-fixed frames. These are rotating frames whose origin is the
 *   center of mass of some planet (see ISSUE) and that rotate with the
 *   planet in question. The parent is always a planet-centered inertial
 *   frame with a zero translation offset between the planet-centered
 *   inertial and planet-fixed frames.
 *
 * Only planet-centered inertial and barycenter inertial frames can serve
 * as integration frames or as the root of the reference frame tree.
 */
class EphemerisRefFrame : public RefFrame {
JEOD_MAKE_SIM_INTERFACES(EphemerisRefFrame)

public:

   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Constructor and destructor
   EphemerisRefFrame ();
   virtual ~EphemerisRefFrame ();

   // Set the ephemerides manager, needed for callbacks.
   virtual void set_ephem_manager (BaseEphemeridesManager * manager);

protected:

   // Member functions

   // Set the activity of the frame and relay the change to the
   // ephemerides manager.
   virtual void set_active_status (bool new_status);

   // Member data

   /**
    * The ephemerides manager to which notifications of changes in
    * ephemeris reference frame activity status are sent.
    */
   BaseEphemeridesManager * ephem_manager; //!< trick_units(--)


private:
   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies.

   ///
   /// Not implemented.
   EphemerisRefFrame (const EphemerisRefFrame &);
   ///
   /// Not implemented.
   EphemerisRefFrame & operator= (const EphemerisRefFrame &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
