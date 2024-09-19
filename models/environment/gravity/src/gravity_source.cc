/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/gravity_source.cc
 * Define member functions for the GravitySource class.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((None.))

Library dependencies:
  ((gravity_source.cc)
   (environment/ephemerides/ephem_interface/src/ephem_ref_frame.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/gravity_integ_frame.hh"
#include "../include/gravity_source.hh"

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(GravityIntegFrame)

/**
 * GravitySource constructor.
 */
GravitySource::GravitySource()
{
    JEOD_REGISTER_CLASS(GravityIntegFrame);
}

/**
 * GravitySource destructor.
 */
GravitySource::~GravitySource()
{
    JEOD_DELETE_ARRAY(frames);
}

/**
 * Initialize frame states for the gravity body.
 * \param[in] integ_frames All possible integration frames
 * \param[in] gravity_manager Gravity Manager
 */
void GravitySource::initialize_state(const std::vector<EphemerisRefFrame *> & integ_frames,
                                     const GravityManager & gravity_manager JEOD_UNUSED)
{
    unsigned int n_integ_frames = integ_frames.size();

    // Allocate an array of frame information, one element per integration frame.
    frames = JEOD_ALLOC_CLASS_ARRAY(n_integ_frames, GravityIntegFrame);

    // Initialize each element in the frames array.
    // NOTE: The nth element in the frames array corresponds to the nth
    // integration frame.
    for(unsigned int ii = 0; ii < n_integ_frames; ++ii)
    {
        const EphemerisRefFrame & integ_frame = *integ_frames[ii];

        frames[ii].ref_frame = &integ_frame;

        // Determine if the gravitational body acts as a primary body (Newton's
        // law of gravitation) or as a third body (Newton's law less frame accel
        // toward the gravitational body).
        // ASSUMPTION: Gravity is direct if the planet inertial frame is a child
        //             of the integration frame, 3rd body otherwise.
        // CONSTRAINT: The reference frame tree must be constructed to ensure
        //             the above constraint is satisfied.
        //             This constraint applies to the tree construction.
        if(inertial->is_progeny_of(integ_frame))
        {
            frames[ii].is_third_body = false;
        }
        else
        {
            frames[ii].is_third_body = true;
        }

        // Initialize the frame data (position, acceleration, and bogus time).
        Vector3::initialize(frames[ii].pos);
        Vector3::initialize(frames[ii].accel);
        frames[ii].time = 9e99;
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
