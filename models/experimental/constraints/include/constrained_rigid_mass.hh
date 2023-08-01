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
 * Defines the class ConstrainedRigidMass.
 */

/*
Purpose: ()
Library dependencies: ((../src/constrained_rigid_mass.cc))
*/


#ifndef JEOD_CONSTRAINED_RIGID_MASS_HH
#define JEOD_CONSTRAINED_RIGID_MASS_HH


#include "constrained_point_mass.hh"

#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod 
namespace jeod {

class ConstraintFrame;
class VehicleNonGravState;
class VehicleProperties;


/**
 * A constrained rigid mass is a rigid body that is constrained to more or
 * less move with a DynBody object.
 */
class ConstrainedRigidMass : public ConstrainedPointMass
{

    JEOD_MAKE_SIM_INTERFACES(ConstrainedRigidMass)

public:

    // Instances of this class are assumed to be protected members of
    // a DynBodyConstraint-derived object, so all member data are public.

    // Member data.

    /**
     * The effective inertia tensor of this constrained object about its center
     * of mass, which is assumed to be at the inherited constraint position.
     */
    double inertia[3][3]; //!< trick_units(kg/m^2)

    /**
     * The torque on the associated DynBody, in constraint frame coordinates.
     */
    double torque_constraint_frame[3]; //!< trick_units(N*m)


    /**
     * Default constructor.
     */
    ConstrainedRigidMass (
        const ConstraintFrame* constraint_frame_in = nullptr);

    /**
     * Destructor.
     */
    ~ConstrainedRigidMass () override = default;

    /**
     * Prepare this object for solving a constraint problem.
     * In this case, that means computing constraint_position_com.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state Non-Gravitational State
     * @note This function is called prior to the calls to set_self_coeff(),
     *   set_cross_coeff(), and set_r_h_s().
     */
    void setup_constraint (
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state) override;


    // New functionality.

    /**
     * Set the constrained object's inertia tensor. The input matrix is
     * assumed to be symmetric; only the upper right triangle is used.
     * @param inertia_in  New inertia tensor, expressed in the constraint frame.
     */
    void set_inertia (const double inertia_in[3][3])
    {
        inertia[0][0]                 = inertia_in[0][0];
        inertia[0][1] = inertia[1][0] = inertia_in[0][1];
        inertia[0][2] = inertia[2][0] = inertia_in[0][2];
        inertia[1][1]                 = inertia_in[1][1];
        inertia[1][2] = inertia[2][1] = inertia_in[1][2];
        inertia[2][2]                 = inertia_in[2][2];
    }
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
