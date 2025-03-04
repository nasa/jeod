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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/include/restartable_state_integrator_templates.hh
 * Define template classes that encapsulate the construction, destruction,
 * checkpointing, and restarting of state integrators.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_RESTARTABLE_INTEGRATOR_TEMPLATES_HH
#define JEOD_RESTARTABLE_INTEGRATOR_TEMPLATES_HH

// System includes
#include <cstddef>

// ER7 utilities includes
#include "er7_utils/integration/core/include/first_order_ode_integrator.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"

// JEOD includes
#include "utils/container/include/simple_checkpointable.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Local includes
#include "integration_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * A RestartableStateIntegrator establishes the basic capabilities needed to
 * make a state integrator a managed resource. This includes
 *  - Creating the integrator object,
 *  - Restoring the integrator's derivative function on restart, and
 *  - Destroying the integrator when the RestartableStateIntegrator instance
 *    goes out of scope.
 *
 * @tparam IntegratorType  The type of integrator to be managed, either
 *   er7_utils::FirstOrderODEIntegrator or er7_utils::SecondOrderODEIntegrator.
 *
 * ### Usage:
 *
 * This base class template is not directly usable. One must instead use one of
 * the three class templates that derive from this class template:
 *  - RestartableFirstOrderODEIntegrator to manage an
 *    er7_utils::FirstOrderODEIntegrator pointer;
 *  - RestartableGeneralizedDerivSecondOrderODEIntegrator to manage an
 *    er7_utils::SecondOrderODEIntegrator pointer for a generalized second
 *    order ODE, one in which the time derivative of generalized position
 *    is a function of generalized position and generalized velocity; and
 *  - RestartableGeneralizedStepSecondOrderODEIntegrator to manage an
 *    er7_utils::SecondOrderODEIntegrator pointer for a generalized second
 *    order ODE, one in which the time derivative of generalized position
 *    is a function of generalized position and generalized velocity; and
 *  - RestartableSimpleSecondOrderODEIntegrator to manage an
 *    er7_utils::SecondOrderODEIntegrator pointer for a simple second order
 *    ODE, one in which the time derivative of generalized position is
 *    the generalized velocity.
 *
 * Each state integrator to be used in some class needs a pair of data members
 * declared in the definition of that class.
 * The first of these pairs is a pointer to the appropriate state integrator
 * type (either er7_utils::FirstOrderODEIntegrator or
 * er7_utils::SecondOrderODEIntegrator).  The second of the pairs
 * of data members is an instance of the appropriate derived class of
 * RestartableStateIntegrator that will manage the pointer. An example:
 * @code
 * class MyClass {
 *    ...
 *    er7_utils::SecondOrderODEIntegrator * integrator;
 *    RestartableSimpleSecondOrderODEIntegrator\<3\> integ_manager;
 *    ...
 * };
 * @endcode
 * The pointer itself must be exposed to Trick for checkpoint and restart. The
 * RestartableStateIntegrator-derived object should be hidden from Trick.
 *
 * The connection between the pointer and the manager for that pointer is made
 * in the initializer lists of the constructors for the class. Use the
 * RestartableStateIntegrator non-default constructor to tie the
 * RestartableStateIntegrator object with the pointer it is to manage:
 * @code
 * MyClass::MyClass ()
 * :
 *    ...
 *    integrator(nullptr),
 *    integ_manager(integrator),
 *    ...
 * {
 *    // Body of MyClass constructor
 * }
 * @endcode
 * Note that there the RestartableStateIntegrator default constructors exist
 * only for the sake of the simulation engine. The default constructors do
 * not create viable instances. One must use the non-default constructor to
 * initialize RestartableStateIntegrator instances.
 *
 * The integrator itself is not created at construction time. Creating the
 * integrator is the job of the create_integrator method. This method should
 * be called at initialization time after having created the integration
 * constructor and the integration controls objects.
 *
 * The counterpart to the create_integrator method is destroy_integrator.
 * This method must be called prior to calling create_integrator if the
 * integrator has already been created via a previous call to create_integrator.
 * (For example, switching to a different integration technique).
 * The destroy_integrator can be called at shutdown or destruction time,
 * but this call is not essential. The RestartableStateIntegrator object
 * will call this method internally when it goes out of scope.
 *
 * ### Class Design
 *
 * This class template uses the non-virtual interface (NVI) design pattern, aka
 * the template method design pattern (no relation to C++ class templates).
 * The public create_integrator and simple_restore member functions use the
 * private virtual create_integrator_internal and simple_restore_internal
 * functions to create and restore the integrators.
 */
template<typename IntegratorType> class RestartableStateIntegrator : public SimpleCheckpointable
{
public:
    // Note: The constructors are protected. This is not an instantiable class.

    /**
     * Destructor.
     */
    ~RestartableStateIntegrator() override
    {
        destroy_integrator();
    }

    /**
     * Create the integrator to be managed.
     * @param[in] generator  Integrator constructor used to create the integrator.
     * @param[in,out] controls   Integration controls to be passed to the generator.
     */
    void create_integrator(const er7_utils::IntegratorConstructor & generator,
                           er7_utils::IntegrationControls & controls)
    {
        // Using an instance created with the default constructor is an error.
        if(integrator_handle == nullptr)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 IntegrationMessages::invalid_request,
                                 "The pointer to the integrator object has not been set.");
            return;
        }

        // Destroy the integrator, if it exists.
        destroy_integrator();

        // Create the integrator via the technique-specific creator.
        *integrator_handle = create_integrator_internal(generator, controls);
    }

    /**
     * Destroy the integrator.
     */
    void destroy_integrator()
    {
        if(integrator_handle != nullptr)
        {
            er7_utils::Er7UtilsDeletable::delete_instance(*integrator_handle);
        }
    }

    /**
     * Clear the pointer to the external integrator object.
     * This currently (pre-Trick 13.0) needs to be called on restart because
     * both pointers point to invalid objects.
     */
    void clear_integrator_reference()
    {
        integrator_handle = nullptr;
    }

    /**
     * Set the pointer to the external integrator object.
     * This currently (pre-Trick 13.0) needs to be called on restart because
     * the integrator_handle is not properly restored by checkpoint.
     * @param[in,out] integ_ptr Reference to the external integrator object.
     */
    void set_integrator_reference(IntegratorType *& integ_ptr)
    {
        integrator_handle = &integ_ptr;
    }

    /**
     * Restore the integrator on restart.
     * This currently (pre-Trick 13.0) needs to be called after calling
     * set_integrator_reference.
     */
    void simple_restore() override
    {
        // Restore the integrator from the checkpoint-restored value in the
        // external pointer and perform technique-specific actions on it.
        if(integrator_handle != nullptr)
        {
            simple_restore_internal(*integrator_handle);
        }
    }

    // Unimplemented member functions
    RestartableStateIntegrator(const RestartableStateIntegrator &) = delete;
    RestartableStateIntegrator & operator=(const RestartableStateIntegrator &) = delete;

protected:
    RestartableStateIntegrator() = default;

    /**
     * Non-default constructor.
     * @param[in,out] integ_ref  Reference to the pointer to the integrator that
     *                           is to be managed.
     */
    explicit RestartableStateIntegrator(IntegratorType *& integ_ref)
        : integrator_handle(&integ_ref)
    {
    }

private:
    // NVI member functions

    /**
     * Create the integrator to be managed.
     * @param[in] generator  Integrator constructor used to create the integrator.
     * @param[in,out] controls   Integration controls to be passed to the generator.
     * @return The constructed integrator.
     */
    virtual IntegratorType * create_integrator_internal(const er7_utils::IntegratorConstructor & generator,
                                                        er7_utils::IntegrationControls & controls) = 0;

    /**
     * Perform technique-specific restart actions.
     * The default is to do nothing.
     * @param[in,out] integrator_ptr  The integrator object to be restored
     */
    virtual void simple_restore_internal(IntegratorType * integrator_ptr JEOD_UNUSED) {}

    // Member data

    /**
     * Pointer to the containing object's integrator pointer.
     */
    IntegratorType ** integrator_handle{}; //!< trick_io(**)
};

/**
 * A RestartableFirstOrderODEIntegrator is-a RestartableStateIntegrator
 * that manages an er7_utils::FirstOrderODEIntegrator.
 * @tparam size        Dimensionality of the state vector.
 */
template<unsigned int size>
class RestartableFirstOrderODEIntegrator : public RestartableStateIntegrator<er7_utils::FirstOrderODEIntegrator>
{
public:
    RestartableFirstOrderODEIntegrator() = default;
    ~RestartableFirstOrderODEIntegrator() override = default;
    RestartableFirstOrderODEIntegrator(const RestartableFirstOrderODEIntegrator &) = delete;
    RestartableFirstOrderODEIntegrator & operator=(const RestartableFirstOrderODEIntegrator &) = delete;

    /**
     * Non-default constructor.
     * @param[in,out] integ_ref  Reference to the pointer to the integrator that
     *                           is to be managed.
     */
    explicit RestartableFirstOrderODEIntegrator(er7_utils::FirstOrderODEIntegrator *& integ_ref)
        : RestartableStateIntegrator<er7_utils::FirstOrderODEIntegrator>(integ_ref)
    {
    }

private:
    // NVI methods

    /**
     * Create the integrator to be managed.
     * @param[in] generator  Integrator constructor used to create the integrator.
     * @param[in,out] controls   Integration controls to be passed to the generator.
     */
    er7_utils::FirstOrderODEIntegrator * create_integrator_internal(const er7_utils::IntegratorConstructor & generator,
                                                                    er7_utils::IntegrationControls & controls) override
    {
        return generator.create_first_order_ode_integrator(size, controls);
    }
};

/**
 * A RestartableSecondOrderODEIntegrator is-a
 * RestartableStateIntegrator that manages the integrator for a second
 * order ODE problem.
 */
class RestartableSecondOrderODEIntegrator : public RestartableStateIntegrator<er7_utils::SecondOrderODEIntegrator>
{
public:
    // Note: The constructors are protected. This is not an instantiable class.
    ~RestartableSecondOrderODEIntegrator() override = default;
    RestartableSecondOrderODEIntegrator(const RestartableSecondOrderODEIntegrator &) = delete;
    RestartableSecondOrderODEIntegrator & operator=(const RestartableSecondOrderODEIntegrator &) = delete;

protected:
    RestartableSecondOrderODEIntegrator() = default;

    /**
     * Non-default constructor.
     * @param[in,out] integ_ref  Reference to the pointer to the integrator that
     *                           is to be managed.
     */
    explicit RestartableSecondOrderODEIntegrator(er7_utils::SecondOrderODEIntegrator *& integ_ref)
        : RestartableStateIntegrator<er7_utils::SecondOrderODEIntegrator>(integ_ref)
    {
    }
};

/**
 * A RestartableSimpleSecondOrderODEIntegrator is-a
 * RestartableSecondOrderODEIntegrator that manages the integrator for a simple
 * second order ODE problem, one in which the time derivative of the generalized
 * position is the generalized velocity.
 * @tparam size  Size of the position vector (and also of velocity).
 */
template<unsigned int size> class RestartableSimpleSecondOrderODEIntegrator : public RestartableSecondOrderODEIntegrator
{
public:
    RestartableSimpleSecondOrderODEIntegrator() = default;
    ~RestartableSimpleSecondOrderODEIntegrator() override = default;
    RestartableSimpleSecondOrderODEIntegrator(const RestartableSimpleSecondOrderODEIntegrator &) = delete;
    RestartableSimpleSecondOrderODEIntegrator & operator=(const RestartableSimpleSecondOrderODEIntegrator &) = delete;

    /**
     * Non-default constructor.
     * @param[in,out] integ_ref  Reference to the pointer to the integrator that
     *                           is to be managed.
     */
    explicit RestartableSimpleSecondOrderODEIntegrator(er7_utils::SecondOrderODEIntegrator *& integ_ref)
        : RestartableSecondOrderODEIntegrator(integ_ref)
    {
    }

private:
    // NVI methods

    /**
     * Create the integrator to be managed.
     * @param[in] generator  Integrator constructor used to create the integrator.
     * @param[in,out] controls   Integration controls to be passed to the generator.
     */
    er7_utils::SecondOrderODEIntegrator * create_integrator_internal(const er7_utils::IntegratorConstructor & generator,
                                                                     er7_utils::IntegrationControls & controls) override
    {
        return generator.create_second_order_ode_integrator(size, controls);
    }
};

/**
 * A RestartableGeneralizedDerivSecondOrderODEIntegrator is-a
 * RestartableStateIntegrator that manages the integrator for a generalized
 * second order ODE problem, one in which the time derivative of the
 * generalized position is some function of the generalized position and
 * the generalized velocity.
 * @tparam position_size   The dimensionality of the generalized position.
 * @tparam velocity_size   The dimensionality of the generalized velocity.
 * @tparam DerivFunctions  Class that derives from the class
 *                         er7_utils::GeneralizedPositionDerivativeFunctions.
 */
template<unsigned int position_size, unsigned int velocity_size, typename DerivFunctions>
class RestartableGeneralizedDerivSecondOrderODEIntegrator : public RestartableSecondOrderODEIntegrator
{
public:
    RestartableGeneralizedDerivSecondOrderODEIntegrator() = default;
    ~RestartableGeneralizedDerivSecondOrderODEIntegrator() override = default;
    RestartableGeneralizedDerivSecondOrderODEIntegrator(const RestartableGeneralizedDerivSecondOrderODEIntegrator &) =
        delete;
    RestartableGeneralizedDerivSecondOrderODEIntegrator & operator=(
        const RestartableGeneralizedDerivSecondOrderODEIntegrator &) = delete;

    /**
     * Non-default constructor.
     * @param[in,out] integ_ref  Reference to the pointer to the integrator that
     *                           is to be managed.
     */
    explicit RestartableGeneralizedDerivSecondOrderODEIntegrator(er7_utils::SecondOrderODEIntegrator *& integ_ref)
        : RestartableSecondOrderODEIntegrator(integ_ref)
    {
    }

private:
    // NVI methods

    /**
     * Create the integrator to be managed.
     * @param[in] generator  Integrator constructor used to create the integrator.
     * @param[in,out] controls   Integration controls to be passed to the generator.
     */
    er7_utils::SecondOrderODEIntegrator * create_integrator_internal(const er7_utils::IntegratorConstructor & generator,
                                                                     er7_utils::IntegrationControls & controls) override
    {
        return generator.create_generalized_deriv_second_order_ode_integrator(position_size,
                                                                              velocity_size,
                                                                              DerivFunctions(),
                                                                              controls);
    }

    /**
     * Perform technique-specific restart actions.@n
     * The generalized second order ODE integrators need the pointer
     * to the derivative function to be restored.
     * @param[in,out] integrator_ptr  The base class's integrator data member
     */
    void simple_restore_internal(er7_utils::SecondOrderODEIntegrator * integrator_ptr) override
    {
        if(integrator_ptr != nullptr)
        {
            integrator_ptr->set_position_derivative_functions(DerivFunctions());
        }
    }
};

/**
 * A RestartableGeneralizedStepSecondOrderODEIntegrator is-a
 * RestartableStateIntegrator that manages the integrator for a generalized
 * second order ODE problem, one in which the time derivative of the
 * generalized position is some function of the generalized position and
 * the generalized velocity.
 * @tparam position_size  The dimensionality of the generalized position.
 * @tparam velocity_size  The dimensionality of the generalized velocity.
 * @tparam StepFunctions  Class that derives from
 *                        er7_utils::GeneralizedPositionStepFunctions.
 */
template<unsigned int position_size, unsigned int velocity_size, typename StepFunctions>
class RestartableGeneralizedStepSecondOrderODEIntegrator : public RestartableSecondOrderODEIntegrator
{
public:
    RestartableGeneralizedStepSecondOrderODEIntegrator() = default;
    ~RestartableGeneralizedStepSecondOrderODEIntegrator() override = default;
    RestartableGeneralizedStepSecondOrderODEIntegrator(const RestartableGeneralizedStepSecondOrderODEIntegrator &) =
        delete;
    RestartableGeneralizedStepSecondOrderODEIntegrator & operator=(
        const RestartableGeneralizedStepSecondOrderODEIntegrator &) = delete;

    /**
     * Non-default constructor.
     * @param[in,out] integ_ref  Reference to the pointer to the integrator that
     *                           is to be managed.
     */
    explicit RestartableGeneralizedStepSecondOrderODEIntegrator(er7_utils::SecondOrderODEIntegrator *& integ_ref)
        : RestartableSecondOrderODEIntegrator(integ_ref)
    {
    }

private:
    // NVI methods

    /**
     * Create the integrator to be managed.
     * @param[in] generator  Integrator constructor used to create the integrator.
     * @param[in,out] controls   Integration controls to be passed to the generator.
     */
    er7_utils::SecondOrderODEIntegrator * create_integrator_internal(const er7_utils::IntegratorConstructor & generator,
                                                                     er7_utils::IntegrationControls & controls) override
    {
        return generator.create_generalized_step_second_order_ode_integrator(position_size,
                                                                             velocity_size,
                                                                             StepFunctions(),
                                                                             controls);
    }

    /**
     * Perform technique-specific restart actions.@n
     * The generalized second order ODE integrators need the pointer
     * to the derivative function to be restored.
     * @param[in,out] integrator_ptr  The base class's integrator data member
     */
    void simple_restore_internal(er7_utils::SecondOrderODEIntegrator * integrator_ptr) override
    {
        if(integrator_ptr != nullptr)
        {
            integrator_ptr->set_position_step_functions(StepFunctions());
        }
    }
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
