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
 * Defines the class BaseTranslationalConstraint
 */

/*
Purpose: ()
*/


#include "experimental/math/include/solver_types.hh"
#include "utils/sim_interface/include/jeod_class.hh"


#ifndef JEOD_BASE_TRANSLATIONAL_CONSTRAINT_HH
#define JEOD_BASE_TRANSLATIONAL_CONSTRAINT_HH


//! Namespace jeod 
namespace jeod {

/**
 * Abstract base class that prescribes behaviors of a constraint related to
 * transfer of linear momentum between a vehicle and a constrained object.
 * A translational constraint has an effective mass and effectively acts at
 * some specific point on the vehicle.
 */
class BaseTranslationalConstraint
{

    JEOD_MAKE_SIM_INTERFACES(BaseTranslationalConstraint)

public:

    /**
     * Default constructor.
     */
    BaseTranslationalConstraint() = default;

    /**
     * Destructor.
     */
    virtual ~BaseTranslationalConstraint() = default;


   /**
     * Get the effective mass for this constraint.
     */
    virtual double get_mass () const = 0;

#ifndef SWIG
    /**
     * Get the position at which the constraint force is applied relative to
     * the constraint frame origin, in constraint frame coordinates.
     */
    virtual SolverTypes::ConstDecayedVector3T
    get_constraint_position_constraint_frame () const = 0;

    /**
     * Get the position at which the constraint force is applied relative to
     * the root structural origin, in root body structural coordinates.
     */
    virtual SolverTypes::ConstDecayedVector3T
    get_constraint_position_root () const = 0;

    /**
     * Get the position at which the constraint force is applied relative to
     * the root body center of mass, in root body structural coordinates.
     */
    virtual SolverTypes::ConstDecayedVector3T
    get_constraint_position_com () const = 0;
#endif
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
