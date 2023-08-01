/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_dyn.cc
 * Define member functions for Dynamic Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_dyn.cc)
   (time.cc)
   (time_manager.cc)
   (time_messages.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))

 
******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/time_dyn.hh"
#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"
#include "../include/time_standard.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a Time_Dyn
 */
TimeDyn::TimeDyn (
   void)
{
   name = "Dyn";
   links.make_root();

   scale_factor = 1.0;
   ref_scale    = 1.0;
   offset       = 0.0;
}

/**
 * Each time type is initialized from its parent in the initialization
 * tree, except one.  In order to have an absolute reference time, one of
 *   the time types must be defined ahead of time.  This is called the
 *   initializer time. This function initializes the initializer time.
 *
 * \par Assumptions and Limitations
 *  - TimeDyn cannot be used as the initializer time
 *  - Each time representation can have its own initializer function, or can
 *     inherit the one in TimeDerived
 * \param[in] time_manager_init TM initializer
 */
void
TimeDyn::initialize_initializer_time (
   TimeManagerInit * time_manager_init JEOD_UNUSED)
{
   // cannot use TimeDyn as the initializer when there are time-types with
   // absolute values in the simulation.
   if (time_manager->time_standards_exist()) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Cannot use TimeDyn as the initializer when there are time-types \n"
         "with absolute values in the simulation.");
   }
   seconds     = 0.0;
   initialized = true;

   return;
}



/**
 * TimeDyn updates directly from simtime, and everything else from
 * TimeDyn.  This function does that first update from simtime
 *
 * \par Assumptions and Limitations
 *  - ref_scale is positive for forward-pregoressing sims, and negative
 *                for reverse-progressing sims.
 */
void
TimeDyn::update (
   void)
{
   seconds = time_manager->simtime * ref_scale + offset;
   return;
}



/**
 * Changeing time direction and/or scale factor
 * @return Void
 */
bool
TimeDyn::update_offset (
   void)
{
   /* The scale-factor can be changed mid-sim.
      TimeDyn uses this value to determine the rate and direction in which time
      propagates with respect to simtime.
      ref_scale is the recorded scale, testing scale_factor against ref-scale
      identifies whether scale_factor has been externally modified. */
   if (!Numerical::compare_exact(ref_scale,scale_factor)) {
      offset    = seconds - (scale_factor * time_manager->simtime);
      if (ref_scale * scale_factor < 0) { // if there is a direction change
         time_manager->verify_table_lookup_ends(); // adjusts end-of-table settings
      }
      ref_scale = scale_factor;
      return true;
   }
   else {
      return false;
   }
}


/**
 * Destroy a Time_Dyn
 */
TimeDyn::~TimeDyn (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
