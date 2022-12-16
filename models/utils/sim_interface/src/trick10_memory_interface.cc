/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick10_memory_interface.cc
 * Define JeodTrickMemoryInterface methods.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependency:
  ((trick10_memory_interface.o)
   (trick_memory_interface.o)
   (trick_memory_interface_alloc.o)
   (trick_memory_interface_attrib.o)
   (trick_memory_interface_chkpnt.o)
   (trick_memory_interface_xlate.o)
   (memory_interface.o))

 

*******************************************************************************/


#ifdef TRICK_VER

// System includes
#include <cstddef>
#include <dlfcn.h>
#include <iomanip>
#include <sstream>

// Trick includes
#include <iosfwd>  // Grrr!
using std::ostream; // Double grrr!
#include "sim_services/CheckPointAgent/include/ClassicCheckPointAgent.hh"
#include "sim_services/MemoryManager/include/MemoryManager.hh"
extern Trick::MemoryManager * trick_MM;

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/simulation_interface.hh"
#include "../include/sim_interface_messages.hh"
#include "../include/trick10_memory_interface.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a JeodTrick10MemoryInterface object.
 */
JeodTrick10MemoryInterface::JeodTrick10MemoryInterface (
   void)
{

   // UAS.
   Trick::CheckPointAgent * current_agent = trick_MM->get_CheckPointAgent();
   trick_MM->reset_CheckPointAgent();
   trick_checkpoint_agent = dynamic_cast<Trick::ClassicCheckPointAgent*> (
      trick_MM->get_CheckPointAgent());
   trick_MM->set_CheckPointAgent(current_agent);

   if (trick_checkpoint_agent == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Trick checkpoint agent is not accessible.");
   }
}


/**
 * Destruct a JeodTrick10MemoryInterface object.
 */
JeodTrick10MemoryInterface::~JeodTrick10MemoryInterface (
   void)
{
   ; // Empty
}

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
