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
 * @addtogroup Environment
 * @{
 * @addtogroup Spice
 * @{
 *
 * @file models/environment/spice/include/spice_ephem_point.hh
 * Define class SpiceEphemPoint, which extends EphemerisPoint for use with
 * the SPICE ephemeris model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/spice_ephem_point.cc))



*******************************************************************************/


#ifndef JEOD_SPICE_EPHEM_POINT_HH
#define JEOD_SPICE_EPHEM_POINT_HH


// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "environment/ephemerides/ephem_item/include/ephem_point.hh"

// Model includes


//! Namespace jeod
namespace jeod {

/**
 * A SpiceEphemPoint minimally extends EphemerisPoint, primarily in order to
 * contain the ID of the SPICE object that will be used to update the state
 * of the target ephemeris reference frame.
 */
class SpiceEphemPoint : public EphemerisPoint {
JEOD_MAKE_SIM_INTERFACES(SpiceEphemPoint)

public:

   // Data types

   /**
    * Enumerates the status values of a SpiceEphemPoint.
    */
   enum Status {
      IsRoot     = 0,  /**< The target reference frame is extant, active, and
                          is the root of the reference frame tree. Hence its
                          state is the trivial state. */
      InTree     = 1,  /**< The target reference frame is extant and active.
                          However, present responsibility for updating the
                          frame lies with some other ephemeris model. */
      Active     = 2,  /**< The target reference frame is extant, active, and
                          is to be updated by this ephemeris model. */
   };


   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Constructor and destructor.
   SpiceEphemPoint ();
   ~SpiceEphemPoint () override;


   // Status and SPICE ID accessors.
   virtual void set_status (SpiceEphemPoint::Status new_status);
   virtual SpiceEphemPoint::Status get_status (void) const;

   virtual void set_spice_id (int new_id);
   virtual int get_spice_id (void) const;

   virtual void set_parent_id (int new_id);
   virtual int get_parent_id (void) const;


protected:

   // Member data

   /**
    * The status for the ephemeris reference frame associated with this item.
    */
   Status status; //!< trick_units(--)

   /**
    * The SPICE kernel object to be used to maintain the target frame's state.
    */
   int spice_id; //!< trick_units(--)

   /**
    * The SPICE ID of the parent to this object.
    */
   int parent_id; //!< trick_units(--)

private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   SpiceEphemPoint (const SpiceEphemPoint &);

   /**
    * Not implemented.
    */
   SpiceEphemPoint & operator= (const SpiceEphemPoint &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
