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
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines the class TorqueConstraintComponent.
 */

/*
Purpose: ()
Library dependencies: ((../src/torque_constraint_component.cc))
*/

#ifndef JEOD_TORQUE_CONSTRAINT_COMPONENT_HH
#define JEOD_TORQUE_CONSTRAINT_COMPONENT_HH

#include "constraint_component.hh"

#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class ConstrainedRigidMass;

/**
 * Models one component of a constrained object whose constraints dictate
 * the application of a torque.
 * The constraint torque is the product of a one newton-meter torque in the
 * constraint direction with a unitless constraint value, the latter of which
 * is determined by a constraints solver.
 */
class TorqueConstraintComponent : public ConstraintComponent
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TorqueConstraintComponent)

public:
    // Member functions

    /**
     * Default constructor.
     */
    TorqueConstraintComponent(ConstraintFrame * constraint_frame_in = nullptr,
                              ConstrainedRigidMass * constrained_mass_in = nullptr);

    /**
     * Destructor.
     */
    ~TorqueConstraintComponent() override = default;

    // The copy constructor and copy assignment operator are deleted
    // to avoid erroneous copies.
    TorqueConstraintComponent(const TorqueConstraintComponent &) = default;
    TorqueConstraintComponent(TorqueConstraintComponent &&) = default;
    TorqueConstraintComponent & operator=(const TorqueConstraintComponent &) = default;
    TorqueConstraintComponent & operator=(TorqueConstraintComponent &&) = default;

    /**
     * Perform actions related to the just-performed addition of this constraint
     * to a constraints solver. In this case, do nothing.
     */
    void attach_to_solver(DynBodyConstraintsSolver *, DynBody *) override {}

    /**
     * Perform actions related to the just-performed removal of this constraint
     * from a constraints solver. In this case, do nothing.
     */
    void detach_from_solver(DynBodyConstraintsSolver *, DynBody *) override {}

    /**
     * Prepare this object for solving a constraint problem.
     *
     * The direction in which the constraint torque acts is known at this point,
     * but the magnitude is not. This function sets the torque to a unit torque
     * in the known constraint direction. This enables the computation of
     * interactions with other constraints (the A matrix in A*x=b) and the
     * computation of the constraint response (the b vector in A*x=b).
     * The solver subsequently solves A*x=b, which yields the torque magnitude.
     *
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to non-constraint forces and torques, including
     *   the pre-constraint wrenches.
     */
    void setup_constraint(const VehicleProperties & vehicle_properties,
                          const VehicleNonGravState & non_grav_state) override;

    /**
     * Computes the coefficient of the solver A matrix that represents the
     * interaction of some other constrained object and this constrained object.
     * @param vehicle_properties  Various vehicle properties.
     * @param other   The other constrained object.
     * @return  Interaction coefficient.
     */
    double get_cross_coeff(const VehicleProperties & vehicle_properties,
                           const ConstraintComponent & other) const override;

#ifndef SWIG
    /**
     * Get the translational response by the vehicle to this constraint at a
     * specified point in root body structural coordinates.
     * @param vehicle_properties  Various vehicle properties.
     * @param point  The point at with the translational response is to be
     *    computed, in root body structural components.
     * @param response  The response vector, which is filled with the
     *    translational acceleration at the specified point on the vehicle
     *    due to a unitary value of the specified component of this constraint.
     * @return The response vector.
     */
    SolverTypes::DecayedVector3T get_translational_response(const VehicleProperties & vehicle_properties,
                                                            SolverTypes::ConstVector3T point,
                                                            SolverTypes::Vector3T response) const override;
#endif

    /**
     * Set this constrained object's scalar constraint value.
     * @param constraint_value  The solution to the matrix constraint equation
     *   for this constrained object.
     */
    void set_constraint_value(double constraint_value) override;

    /**
     * Compute the response (if any) of the constrained object to the
     * overall behavior of the vehicle. In this case, do nothing.
     */
    void compute_constraint_response(const VehicleProperties &, const VehicleNonGravState &) override {}

protected:
    // Member data.

    /**
     * The constrained mass, as a rigid body, for this component.
     */
    ConstrainedRigidMass * constrained_mass{}; //!< trick_units(--)

    /**
     * The unitless constraint value.
     */
    double constraint_torque{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
