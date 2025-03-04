//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @file models/dynamics/body_action/include/dyn_body_init.hh
 * Define the class DynBodyInit, the base class used for initializing the
 * state of a DynBody object.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/dyn_body_init.cc))



*******************************************************************************/

#ifndef JEOD_DYN_BODY_INIT_HH
#define JEOD_DYN_BODY_INIT_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/orientation/include/orientation.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "body_action.hh"
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Base class for initialize the state of a DynBody.
 */
class DynBodyInit : public BodyAction
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DynBodyInit)

    // Member data
public:
    /**
     * The suffix of the frame name (i.e., the part of the name after the
     * vehicle identifier) to which this initializer pertains
     */
    std::string body_frame_id{""}; //!< trick_units(--)

    /**
     * The name of the reference frame against which state data specified
     * in a DynBodyInit subclass are referenced.
     */
    std::string reference_ref_frame_name{""}; //!< trick_units(--)

    /**
     * Contains state information set by the initializer, which is always
     * a subclass of DynBodyInit. The DynBodyInit apply method copies the
     * state elements indicated by the initializer's initializes_what method to
     * the frame indicated by the frame_id and then propagates the initialized
     * states up/down the vehicle attachment tree.
     */
    RefFrameState state; //!< trick_units(--)

    /**
     * Relative position between the subject and reference reference frame
     * origins. The normal sense (reverse_sense is not set) is the position of
     * the subject origin with respect to the reference origin, expressed in
     * reference coordinates. The reverse meaning (reverse_sense set) is the
     * position of the reference origin with respect to the subject origin,
     * expressed in subject coordinates.
     */
    double position[3]{}; //!< trick_units(m)

    /**
     * Relative velocity between the subject and reference reference frame
     * origins. The normal sense (reverse_sense is not set) is the velocity of
     * the subject origin with respect to the reference origin, expressed in and
     * observed in reference coordinates. The reverse meaning (reverse_sense set)
     * is the velocity of the reference origin with respect to the subject
     * origin, expressed in and observed in subject coordinates.
     */
    double velocity[3]{}; //!< trick_units(m/s)

    /**
     * Relative orientation between the subject and reference reference frame
     * axes. The normal sense (reverse_sense is not set) is the transformation
     * from reference to subject. The reverse meaning (reverse_sense set) is the
     * transformation from subject to reference.
     */
    Orientation orientation; //!< trick_units(--)

    /**
     * Relative angular velocity between the subject and reference axes. The
     * flags reverse_sense and rate_in_parent give four interpretations:
     * - Default (both reverse_sense and rate_in_parent are false):\n
     * Angular velocity of the subject frame with respect to the reference
     * frame, expressed in subject frame coordinates.
     * - reverse_sense is clear, rate_in_parent is set:\n
     * Angular velocity of the subject frame with respect to the reference
     * frame, expressed in reference frame coordinates.
     * - reverse_sense is set, rate_in_parent is clear:\n
     * Angular velocity of the reference frame with respect to the subject
     * frame, expressed in reference frame coordinates.
     * - Both reverse_sense and rate_in_parent are set:\n
     * Angular velocity of the reference frame with respect to the subject
     * frame, expressed in subject frame coordinates.
     */
    double ang_velocity[3]{}; //!< trick_units(rad/s)

    /**
     * Indicates how the user input state items are to be interpreted.
     * If clear (default setting), indicates that the user input position,
     * velocity, orientation, and angular velocity are to be interpreted in
     * the standard JEOD parent to child sense. The meaning is reversed when
     * this flag is set.
     * See the descriptions of the individual state items for details.
     */
    bool reverse_sense{}; //!< trick_units(--)

    /**
     * Indicates how the user input angular velocity is to be interpreted.
     * This item works in conjunction with reverse_sense.
     * See ang_velocity for a complete description.
     */
    bool rate_in_parent{}; //!< trick_units(--)

protected:
    /**
     * The reference frame whose name is vehicle_name.body_frame_id.
     * This is the frame to which the state is applied.
     */
    BodyRefFrame * body_ref_frame{}; //!< trick_io(**)

    /**
     * The reference frame whose name is reference_ref_frame_name.
     * This is the frame against which the user state is reference.
     */
    RefFrame * reference_ref_frame{}; //!< trick_io(**)

private:
    /**
     * The subscribed-to frame (the reference_ref_frame at initialization
     * time), cached so that this frame will be unsubscribed at application
     * time.
     */
    RefFrame * subscribed_frame{}; //!< trick_io(**)

    // Member functions
public:
    DynBodyInit() = default;
    ~DynBodyInit() override = default;
    DynBodyInit(const DynBodyInit &) = delete;
    DynBodyInit & operator=(const DynBodyInit &) = delete;

    // report_failure: Report that an initializer failed.
    virtual void report_failure();

    // initialize: Initialize the initializer.
    void initialize(DynManager & dyn_manager) override;

    // initializes_what: Indicate what aspect of the state is initialized.
    // Derived classes must override this method as the base DynBodyInit
    // initializes_what method indicates nothing is to be initialized.
    virtual RefFrameItems::Items initializes_what();

    // is_ready: Indicate whether the initializer is ready to be applied.
    // Derived classes that cannot run immediately due to dependencies must
    // override this method as the base DynBodyInit is always ready.
    bool is_ready() override;

    // apply: Initialize the elements of the subject dyn_body's body_ref_frame
    // based on the state member datum. Only those state elements selected by the
    // (overloaded) initializes_what member function are set.
    void apply(DynManager & dyn_manager) override;

protected:
    // apply_user_inputs: Compute relative state from user-input data.
    void apply_user_inputs();

    // compute_rotational_state: Obsolete. Do not call.
    void compute_rotational_state();

    // compute_translational_state: Obsolete. Do not call.
    void compute_translational_state();

    // find_planet: Find specified Planet, failing if not found.
    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name);

    // find_dyn_body: Find specified DynBody, failing if not found.
    DynBody * find_dyn_body(const DynManager & dyn_manager,
                            const std::string & dyn_body_name,
                            const std::string & variable_name);

    // find_ref_frame: Find specified BodyRefFrame, failing if not found.
    RefFrame * find_ref_frame(const DynManager & dyn_manager,
                              const std::string & ref_frame_name,
                              const std::string & variable_name);

    // find_body_frame: Find specified BodyRefFrame, failing if not found.
    BodyRefFrame * find_body_frame(DynBody & frame_container,
                                   const std::string & body_frame_identifier,
                                   const std::string & variable_name);
};

} // namespace jeod

#ifdef TRICK_VER
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
