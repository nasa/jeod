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
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemItem
 * @{
 *
 * @file models/environment/ephemerides/ephem_item/include/ephem_orient.hh
 * Define class EphemerisOrientation.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/ephem_orient.cc)
   (../src/ephem_item.cc))



*******************************************************************************/

#ifndef JEOD_EPHEM_ORIENT_HH
#define JEOD_EPHEM_ORIENT_HH

// System includes

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "ephem_item.hh"

//! Namespace jeod
namespace jeod
{

/**
 * An EphemerisOrientation object updates the rotational state of an ephemeris
 * reference frame.
 */
class EphemerisOrientation : public EphemerisItem
{
    JEOD_MAKE_SIM_INTERFACES(jeod, EphemerisOrientation)

public:
    // Member functions
    EphemerisOrientation() = default;
    ~EphemerisOrientation() override = default;
    EphemerisOrientation(const EphemerisOrientation &) = delete;
    EphemerisOrientation & operator=(const EphemerisOrientation &) = delete;

    // EphemerisOrientation objects modify the rotational state.
    TargetAspect updates_what() const override;

    // Enable the item.
    void enable() override;

    // Note that the planet-fixed frame's active status has changed
    void note_frame_status_change(RefFrame * frame) override;

    // Default suffix, "pfix" in the case of an orientation.
    std::string default_suffix() const override;

    // Disconnect (no-op for an orientation)
    void disconnect_from_tree() override;

protected:
    // Member data

    /**
     * A subscription to the planet's inertial frame is issued whenever
     * the planet's planet-fixed frame is active to ensure that the
     * the planet-fixed frame is a part of the ref frame tree.
     * This flag is set when such a subscription is made.
     */
    bool subscribed_to_inertial{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
