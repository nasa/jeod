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
 * @addtogroup Interactions
 * @{
 * @addtogroup GravityTorque
 * @{
 *
 * @file
 * Defines the class GravityTorque.
 */

/************************** TRICK HEADER***************************************
PURPOSE:
   ()

REFERENCE:
   (((None)))

ASSUMPTIONS AND LIMITATIONS:
   ((None))

Library dependencies:
   ((../src/gravity_torque.cc))



*******************************************************************************/

#ifndef JEOD_GRAVITY_TORQUE_HH
#define JEOD_GRAVITY_TORQUE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes

//! Namespace jeod
namespace jeod
{

/**
 * Computes the torque on an object due to gravitation.
 */
class GravityTorque
{
    JEOD_MAKE_SIM_INTERFACES(jeod, GravityTorque)

public:
    GravityTorque() = default;
    ~GravityTorque() = default;
    GravityTorque & operator=(const GravityTorque &) = delete;
    GravityTorque(const GravityTorque &) = delete;

    void initialize(DynBody & subject);

    void update();

    /**
     * The output torque, in the structural frame
     */
    double torque[3]{}; //!< trick_units(N*m)

    /**
     * Is the model active?
     */
    bool active{true}; //!< trick_units(--)

protected:
    /**
     * The subject body for the gradient torque
     */
    DynBody * subject_body{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
