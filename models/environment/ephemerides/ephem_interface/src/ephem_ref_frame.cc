/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemInterface
 * @{
 *
 * @file models/environment/ephemerides/ephem_interface/src/ephem_ref_frame.cc
 * Define non-inlined member functions for the EphemRefFrame class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((ephem_ref_frame.cc)
   (environment/ephemerides/ephem_interface/src/ephem_messages.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_manager/include/base_ephem_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/ephem_interface.hh"
#include "../include/ephem_messages.hh"
#include "../include/ephem_ref_frame.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Set the EphemerisRefFrame's owner.
 * \param[in,out] manager Ephemeris manager
 */
void EphemerisRefFrame::set_ephem_manager(BaseEphemeridesManager * manager)
{
    ephem_manager = manager;
}

/**
 * Augment RefFrame::set_active_status by notifying the ephemerides manager
 * that the tree might need to be rebuilt.
 * \param[in] new_status Active status
 */
void EphemerisRefFrame::set_active_status(bool new_status)
{
    // Pass the message up the chain.
    RefFrame::set_active_status(new_status);

    // Nominally, tell the ephemerides manager that it might need to
    // rebuild the ephemerides models and the reference frame tree.
    if(ephem_manager != nullptr)
    {
        ephem_manager->ephem_note_tree_status_change();
    }

    // That can't be done if the object has no ephemerides manager.
    else
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::inconsistent_setup,
                              "Ephemeris reference frame \"%s\" has no ephemerides manager.\n"
                              "The simulation initializations may be out of order.\n",
                              name.c_str());
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
