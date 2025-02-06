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
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines the class ConstrainedPointMass.
 */

/*
Purpose: ()
Library dependencies: ((constrained_point_mass.o))
*/


#ifndef JEOD_CONSTRAINED_POINT_MASS_HH
#define JEOD_CONSTRAINED_POINT_MASS_HH


#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod 
namespace jeod {

class ConstraintFrame;
class VehicleNonGravState;
class VehicleProperties;


/**
 * A constrained point mass is a point mass that is constrained to more or
 * less move with a DynBody object. The "more or less" means that this
 * object needs to be modeled as a constrained object rather than as a
 * rigidly attached object.
 */
class ConstrainedPointMass
{

    JEOD_MAKE_SIM_INTERFACES(ConstrainedPointMass)

public:

    // Instances of this class are assumed to be protected members of
    // a DynBodyConstraint-derived object, so all member data are public.

    // Member data.

    /**
     * The frame in which the constraint position is most naturally expressed.
     */
    const ConstraintFrame* constraint_frame;

    /**
     * The effective mass of this constrained object.
     */
    double mass; //!< trick_units(kg)

    /**
     * The position at which the constraint equation applies, expressed in the
     * constraint frame. Note well: This may or may not be the position of the
     * of the constrained mass. For example, the constraint position for a
     * pendulum whose mass is concentrated at the end of a massless rod is the
     * hinge point position rather than the bob position.
     */
    double constraint_position_constraint_frame[3]; //!< trick_units(m)

    /**
     * The constraint position, expressed in root structural frame coordinates.
     */
    double constraint_position_root[3]; //!< trick_units(m)

    /**
     * The constraint position relative to the root vehicle center of mass,
     * expressed in root structural frame coordinates.
     */
    double constraint_position_com[3]; //!< trick_units(m)

    /**
     * The non-gravitational acceleration at the constraint point in root
     * structural coordinates.
     */
    double non_grav_accel_root[3]; //!< trick_units(m/s^2)

    /**
     * The acceleration at the constraint point due to vehicular rotation and
     * external forces and torques, in constraint frame coordinates.
     */
    double non_grav_accel_constraint_frame[3]; //!< trick_units(m/s^2)

    /**
     * The force at the constraint point; mass * non_grav_accel_constraint_frame.
     */
    double force_constraint_frame[3]; //!< trick_units(N)


    /**
     * Default constructor.
     */
    ConstrainedPointMass (
        const ConstraintFrame* constraint_frame_in = nullptr);

    /**
     * Destructor.
     */
    virtual ~ConstrainedPointMass () = default;


    /**
     * Update information about the relation between this constrained object
     * and the root DynBody.
     */
    virtual void update_attachment (
        const VehicleProperties&)
    {
        compute_constraint_position_root ();
    }

    /**
     * Prepare this object for solving a constraint problem.
     * In this case, that means computing constraint_position_com.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  Non-gravitational State.
     * @note This function is called prior to the calls to set_self_coeff(),
     *   set_cross_coeff(), and set_r_h_s().
     */
    virtual void setup_constraint (
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state)
    {
        compute_constraint_position_root ();
        compute_constraint_position_com (vehicle_properties);
        compute_non_grav_accel (non_grav_state);
    }


    // New functionality.

    /**
     * Set the constrained object's effective mass.
     * @param mass_in  The effective mass.
     */
    void set_mass (double mass_in)
    {
        mass = mass_in;
    }

    /**
     * Set the constraint point position.
     * @param constraint_position  Constraint point, in constraint frame
     * coordinates.
     */
    void set_constraint_position (
        const double constraint_position[3]);

    /**
     * Compute the constraint point in root structural coordinates.
     */
    void compute_constraint_position_root ();

    /**
     * Compute the constraint point relative to the root body center of mass,
     * in root structural coordinates.
     */
    void compute_constraint_position_com (
        const VehicleProperties& vehicle_properties);

    /**
     * Compute the non-gravitational acceleration at the constraint point.
     */
    void compute_non_grav_accel (
        const VehicleNonGravState& non_grav_state);
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
