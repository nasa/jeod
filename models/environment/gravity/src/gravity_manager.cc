/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/gravity_manager.cc
 * Define member functions for the GravityManager class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((gravity_manager.cc)
   (gravity_source.cc)
   (gravity_controls.cc)
   (gravity_messages.cc)
   (utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <cstddef>
#include <cstring>
#include <string>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/gravity_controls.hh"
#include "../include/gravity_interaction.hh"
#include "../include/gravity_manager.hh"
#include "../include/gravity_messages.hh"
#include "../include/gravity_source.hh"

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(GravitySource)

/**
 * GravityManager constructor.
 */
GravityManager::GravityManager()
{
    JEOD_REGISTER_CLASS(GravityManager);
    JEOD_REGISTER_INCOMPLETE_CLASS(GravitySource);
    JEOD_REGISTER_CHECKPOINTABLE(this, sources);
}

/**
 * GravityManager destructor.
 */
GravityManager::~GravityManager()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, sources);
    sources.clear();
}

/**
 * Find the gravitational body with the given name.
 * @return Pointer to found body
 * \param[in] source_name Name of gravity source to be found
 */
GravitySource * GravityManager::find_grav_source(const std::string & source_name) const
{
    GravitySource * found_source = nullptr;
    unsigned int nbodies = sources.size();

    if(source_name.empty())
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              GravityMessages::invalid_name,
                              "Null / empty string supplied to GravityManager::find_grav_source");
        return nullptr;
    }

    for(unsigned int ii = 0; ii < nbodies; ++ii)
    {
        GravitySource * grav_source = sources[ii];
        if(source_name == grav_source->name)
        {
            found_source = grav_source;
            break;
        }
    }
    return found_source;
}

/**
 * Create a gravitational body, initialize it with the supplied
 * gravity coefficients, and add it to the vector of bodies.
 * \param[in] source Gravity source to be added
 */
void GravityManager::add_grav_source(GravitySource & source)
{
    // The body's planet name must be non-NULL and non-empty.
    if(source.name.empty())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             GravityMessages::invalid_name,
                             "GravitySource has a null / empty planet name");
        return;
    }

    // Body names must be unique. Check for conflicts.
    if(find_grav_source(source.name) != nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             GravityMessages::duplicate_entry,
                             "Duplicate planet name '%s'",
                             source.name.c_str());
        return;
    }

    // Save the body in the bodies container.
    sources.push_back(&source);
}

/**
 * Perform base initialization.
 * @note This method differs from the other initialize_models methods
 * in that this method takes no coefficients as arguments.
 * The S_define must call add_grav_source explicitly when this
 * signature is used.
 * \param[in,out] manager Dynamics manager
 */
void GravityManager::initialize_model(BaseDynManager & manager)
{
    manager.set_gravity_manager(*this);
}

/**
 * Pass the initialize_state method to each GravitySource object registered
 * with the gravity manager.
 *
 * \par Assumptions and Limitations
 *  - Initialization phasing: The following must have been called prior to
 *       calling this method:
 *        - GravityManager::initialize_model to register the GravityManager object
 *          with the dynamics manager
 *        - GravityManager::add_grav_source to register all GravitySource objects in
 *          the simulation with the gravity manager.
 *        - Planet::register_model to associate the planet with a GravitySource.
 *        - DynamicsManager::activate_ephemerides to identify which reference
 *          frames can serve as integration frames.
 * \param[in,out] manager Dynamics manager
 */
void GravityManager::initialize_state(const BaseDynManager & manager)
{
    // Pass the initialize_state method to each gravitational body in the model.
    for(unsigned int ii = 0; ii < sources.size(); ++ii)
    {
        sources[ii]->initialize_state(manager.get_integ_frames(), *this);
    }
}

// Compute gravitational attraction of gravitational bodies on the
// provided dynamic body.
// Note: This overload of GravityManager::gravitation is deprecated.
void GravityManager::gravitation(const double integ_pos[3], GravityInteraction & grav)
{
    double total_grav_accel[3] = {0.0, 0.0, 0.0}; // M/s2 Total accel

    double total_grav_grad[3][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    }; // 1/S2 Total gravity gradient

    double total_grav_pot[1] = {}; // -- Total gravitational potential

    unsigned int n_controls = grav.grav_controls.size(); // --   Number of controls for the dyn body

    unsigned int integ_idx = grav.integ_frame_index; // --   Vehicle integration frame index

    /* Compute the gravitational acceleration from each gravitational body
       for which the vehicle has a control on the body and accumulate the
       total gravitational acceleration. */
    for(unsigned int ii = 0; ii < n_controls; ++ii)
    {
        GravityControls & control_ii = *(grav.grav_controls[ii]);
        if(control_ii.active)
        {
            control_ii.gravitation(integ_pos,
                                   integ_idx,
                                   control_ii.grav_accel,
                                   control_ii.grav_grad,
                                   &control_ii.grav_pot);
            Vector3::incr(control_ii.grav_accel, total_grav_accel);
            Matrix3x3::incr(control_ii.grav_grad, total_grav_grad);
            total_grav_pot[0] += control_ii.grav_pot;
        }
    }

    // Save the total acceleration, gradient in the gravity interaction instance.
    Vector3::copy(total_grav_accel, grav.grav_accel);
    Matrix3x3::copy(total_grav_grad, grav.grav_grad);
    grav.grav_pot = total_grav_pot[0];
}

// Compute gravitational attraction of gravitational bodies on the
// provided dynamic body.
// Note: This overload of GravityManager::gravitation provides the ability
// to compute gravitation with a relativistic correction.
void GravityManager::gravitation(const RefFrame & point, GravityInteraction & grav)
{
    double total_grav_accel[3] = {0.0, 0.0, 0.0}; // M/s2 Total accel
    double total_grav_grad[3][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    }; // 1/S2 Total gravity gradient

    double total_grav_pot = 0.0;                         // -- Total gravitational potential
    unsigned int n_controls = grav.grav_controls.size(); // --   Number of controls for the dyn body
    unsigned int integ_idx = grav.integ_frame_index;     // --   Vehicle integration frame index

    /* Compute the gravitational acceleration from each gravitational body
       for which the vehicle has a control on the body and accumulate the
       total gravitational acceleration, gradient, and potential. */
    for(unsigned int ii = 0; ii < n_controls; ++ii)
    {
        GravityControls & control_ii = *(grav.grav_controls[ii]);
        if(control_ii.active)
        {
            control_ii.gravitation(point, integ_idx, control_ii.grav_accel, control_ii.grav_grad, control_ii.grav_pot);
            Vector3::incr(control_ii.grav_accel, total_grav_accel);
            Matrix3x3::incr(control_ii.grav_grad, total_grav_grad);
            total_grav_pot += control_ii.grav_pot;
        }
    }

    // Save the total acceleration, gradient in the gravity interaction instance.
    Vector3::copy(total_grav_accel, grav.grav_accel);
    Matrix3x3::copy(total_grav_grad, grav.grav_grad);
    grav.grav_pot = total_grav_pot;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
