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
 * Defines the class BasePendulumModel.
 */

/*
Purpose: ()
*/


#ifndef JEOD_BASE_PENDULUM_MODEL_HH
#define JEOD_BASE_PENDULUM_MODEL_HH


#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod 
namespace jeod {

/**
 * Abstract base class that defines interfaces needed to model a spherical
 * pendulum as a constrained object.
 */
class BasePendulumModel
{

    JEOD_MAKE_SIM_INTERFACES(BasePendulumModel)

public:


    /**
     * Default constructor.
     */
    BasePendulumModel() = default;

    /**
     * Destructor.
     */
    virtual ~BasePendulumModel() = default;


    // Interfaces needed by PendulumConstraintComponent.

    /**
     * Update the pendulum model.
     */
    virtual void update_pendulum_model() = 0;

    /**
     * Get the location of the hinge point in the constraint frame.
     * @param hinge_point  Vector to be populated with hinge point location,
     * in meters relative to the origin of the constraint frame.
     */
    virtual void get_hinge_point (double hinge_point[3]) const = 0;

    /**
     * Get the mass of the pendulum bob.
     * @return Pendulum bob mass, in kilograms.
     */
    virtual double get_pendulum_mass() const = 0;

    /**
     * Get the length of the pendulum rod.
     * @return Pendulum rod length, in meters.
     */
    virtual double get_pendulum_length() const = 0;

    /**
     * Compute the damping factor based on the input acceleration.
     * @param accel_mag Representative acceleration such as the magnitude of
     * the non-gravitational acceleration, or the tension force divided by mass.
     * @return Damping factor, with units of inverse time.
     */
    virtual double compute_damping_factor(double accel_mag) = 0;
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
