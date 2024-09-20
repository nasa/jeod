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
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/include/body_action.hh
 * Define the class BodyAction, the base class used for performing actions
 * on a MassBody or DynBody object.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/body_action.cc))



*******************************************************************************/

#ifndef JEOD_BODY_ACTION_HH
#define JEOD_BODY_ACTION_HH

// System includes
#include <string>

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "dynamics/mass/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * BodyAction is the base class for the BodyAction model.
 * A BodyAction instance that performs some operation on a MassBody object.
 * The simulation Dynamics Manager object manages a collection of BodyAction
 * objects for the purpose of initializing MassBody objects and later, for
 * performing asynchronous actions on them.
 *
 * The BodyAction model hinges on three methods:
 * - initialize()
 *   The initialize() method initializes the BodyAction.
 *   This method does not and must not operate on the subject of the action.
 *   All derived classes must forward the initialize() call to the immediate
 *   parent class and then perform class-dependent object initializations.
 * - is_ready()
 *   The is_ready method indicates whether the action is ready to be applied.
 *   For example, an action that initializes the translation state of a
 *   vehicle relative to some other vehicle cannot do its job until that
 *   other vehicle's translational state is set. The is_ready() method for such
 *   an action should return false until the other vehicle's translational
 *   state has been set.
 * - apply()
 *   The apply() method applies the action -- it does something to the subject
 *   of the action.
 *   All derived classes must perform class-dependent actions and then
 *   must forward the apply() call to the immediate parent class.
 */
class BodyAction
{
    JEOD_MAKE_SIM_INTERFACES(jeod, BodyAction)

    // Member data
public:
    /**
     * Set the subject mass body of this action. Resets dyn_subject to null
     */
    void set_subject_body(MassBody & mass_body_in);

    /**
     * Set the subject dyn body of this action. Resets mass_subject to null
     */
    void set_subject_body(DynBody & dyn_body_in);

    /**
     * Test the input mass body against the subject body and return true if they are the same body.
     */
    bool is_same_subject_body(MassBody & mass_body_in);

    /**
     * Check if the subject is a DynBody.
     */
    bool is_subject_dyn_body();

    DynBody * get_subject_dyn_body();

    /**
     * Controls when the action is performed.
     * The action will be performed when the action is activated via this flag
     * and when all other prerequisites for the action have been satisified.
     * The default value for this flag is class-dependent, set in various
     * constructors. The default is true for actions that can reasonably be
     * performed during initialization time and false for actions that are most
     * likely performed while the simulation is running.
     */
    bool active{true}; //!< trick_units(--)

    /**
     * Indicates whether errors encountered while performing the action are
     * to terminate the simulation.
     * Several of the low-level methods used to perform the action do not
     * terminate the simulation on encountering an error condition. They instead
     * leave states unchanged and return an error indicator.
     * This flag, if set, causes the simulation to be terminated when such an
     * error condition occurs.
     * The default value for this flag is true, set in the constructor.
     */
    bool terminate_on_error{true}; //!< trick_units(--)

    /**
     * An identifier for this action. This can be left as empty (default value).
     * The action_name is used only when an error is detected. The generated
     * error message identifies the action name if supplied. The intent is to
     * generate an error message that helps the user pinpoint the source of the
     * error.
     */
    std::string action_name{""}; //!< trick_units(--)

protected:
    /**
     * The MassBody of the body that is the subject of this action.
     * This or the dyn_subject pointer must be supplied.
     * Actions on the body are performed by the apply methods of specific
     * class derived from the BodyAction class.
     */
    MassBody * mass_subject{}; //!< trick_units(--)

    /**
     * The DynBody of the body that is the subject of this action.
     * This or the subject pointer must be supplied.
     * Actions on the body are performed by the apply methods of specific
     * class derived from the BodyAction class.
     */
    DynBody * dyn_subject{}; //!< trick_units(--)

    /**
     * An identifier for this action, constructed from the class name and the
     * action name at initialization time. This is used for generating error
     * and debug messages.
     */
    std::string action_identifier{""}; //!< trick_units(--)

    // Check the dyn and mass body pointers to make sure one is set.
    virtual bool validate_body_inputs(DynBody *& dyn_body_in,
                                      MassBody *& mass_body_in,
                                      const std::string & body_base_name,
                                      bool allow_failure = false);

public:
    BodyAction() = default;
    virtual ~BodyAction() = default;
    BodyAction(const BodyAction &) = delete;
    BodyAction & operator=(const BodyAction &) = delete;

    // Release allocated resources
    virtual void shutdown(); // cppcheck-suppress virtualCallInConstructor

    // get_identifier(): Accessor for the action_identifier.
    const std::string & get_identifier() const;

    // initialize(): Initialize the initializer (*not* the subject)
    // Rules for derived classes:
    // All derived classes must forward the initialize() call to the immediate
    // parent class and then perform class-dependent object initializations.
    virtual void initialize(DynManager & dyn_manager);

    // is_ready()
    // This method indicates whether the initializer is ready to be applied.
    // For example, an initializer that initializes the translation state of a
    // vehicle relative to some other vehicle cannot do its job until that
    // other vehicle's translational state is set. The is_ready() method for such
    // an initializer will return false until the other vehicle's translational
    // state has been set.
    virtual bool is_ready();

    // apply(): Apply the initializer (i.e., initialize the subject)
    // Rules for derived classes:
    // All derived classes must perform class-dependent actions and then
    // must forward the apply() call to the immediate parent class.
    virtual void apply(DynManager & dyn_manager);

    // Member functions

protected:
    // validate_name: Ensure that a string is not trivially invalid.
    void validate_name(const std::string & variable_value,
                       const std::string & variable_name,
                       const std::string & variable_type);
};

/**
 * Accessor for action_identifier.
 * @return Action identifier
 */
inline const std::string & BodyAction::get_identifier() const
{
    return action_identifier;
}

} // namespace jeod

#ifdef TRICK_VER
#include "dynamics/mass/include/mass.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
