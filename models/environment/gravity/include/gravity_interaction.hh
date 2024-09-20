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
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/include/gravity_interaction.hh
 * Define the GravityInteraction class, used to represent the gravitational
 * interaction betweens a DynBody and a set of planetary bodies. Note that while
 * each DynBody instance has a GravityInteraction data member, this class is
 * defined as a part of the gravity model rather than the dyn_body model.
 * This is because the coupling between this class and the other parts of the
 * gravity model is much stronger than the coupling between this class and
 * the dyn_body model.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((../src/gravity_interaction.cc))



*******************************************************************************/

#ifndef JEOD_GRAVITY_INTERACTION_HH
#define JEOD_GRAVITY_INTERACTION_HH

// System includes

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

class BaseDynManager;
class DynBody;
class EphemerisRefFrame;

/**
 * Specifies interactions between a vehicle and a set of gravitational bodies.
 */
class GravityInteraction
{
    JEOD_MAKE_SIM_INTERFACES(jeod, GravityInteraction)

    // Member data
public:
    /**
     * The integration frame index number of the DynBody's integration frame.
     * This data member must be kept in strict synchronization with the DynBody's
     * integration frame.
     */
    unsigned int integ_frame_index{9999}; //!< trick_units(--)

    /**
     * The total gravitational acceleration of the DynBody toward all planetary
     * with which the vehicle interacts gravitationally. The acceleration is
     * expressed in the DynBody's integration frame. The gravitational
     * acceleration of the integration frame itself toward the planetary bodies
     * is excluded from this total acceleration. For example, for a vehicle
     * integrated in Earth-centered inertial, the Sun component of the total
     * gravitational acceleration is the Newtonian gravitation acceleration of
     * the vehicle toward the Sun less the Newtonian gravitational acceleration
     * of the Earth toward the Sun.
     */
    double grav_accel[3]{}; //!< trick_units(m/s2)

    /**
     * The gradient of the gravitational acceleration vector evaluated at the
     * DynBody's position, expressed in the vehicle's integration frame.
     */
    double grav_grad[3][3]{}; //!< trick_units(1/s2)

    /**
     * The total gravitational potential at the location of the DynBody due
     * to the gravity fields of all "active" gravitational bodies
     * (i.e., planets).
     */
    double grav_pot{}; //!< trick_units(m2/s2)

    /**
     * The gravity controls list for a DynBody specifies the planetary bodies
     * with which the DynBody interacts and specifies the nature of those
     * interactions.
     */
    JeodPointerVector<GravityControls>::type grav_controls; //!< trick_io(**)

public:
    GravityInteraction();
    virtual ~GravityInteraction();
    GravityInteraction(const GravityInteraction & frame) = delete;
    GravityInteraction & operator=(const GravityInteraction & frame) = delete;

    // Set the integration frame
    virtual void set_integ_frame(               // Return: -- Void
        const EphemerisRefFrame & ref_frame_in, // In:     -- Integration frame
        const BaseDynManager & dyn_manager);    // In:     -- Dynamics manager

    // Add a new control to grav_controls list
    virtual void add_control(GravityControls * control);

    // Remove a control from the grav_controls list
    virtual void remove_control(GravityControls * control);

    // Initialize all controls in grav_controls list
    virtual void initialize_controls(BaseDynManager & dyn_manager, GravityManager & grav_manager);

    // Reset all controls in grav_controls list
    virtual void reset_controls(BaseDynManager & dyn_manager);

    // Sort the controls in increasing accel magnitude order
    virtual void sort_controls();
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
