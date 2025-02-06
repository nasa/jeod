/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_memory_interface.cc
 * Define JeodTrickMemoryInterface methods.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependency:
  ((trick_memory_interface.cc)
   (trick_memory_interface_alloc.cc)
   (trick_memory_interface_attrib.cc)
   (utils/container/src/primitive_serializer.cc))



*******************************************************************************/

#ifdef TRICK_VER

// System includes
#include <cstddef>
#include <dlfcn.h>
#include <iomanip>
#include <sstream>

// Trick includes

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/sim_interface_messages.hh"
#include "../include/simulation_interface.hh"
#include "../include/trick_memory_interface.hh"

//! Namespace jeod
namespace jeod
{

/**
 * JeodTrickMemoryInterface default constructor.
 */
JeodTrickMemoryInterface::JeodTrickMemoryInterface()
{
    dlhandle = dlopen(nullptr, RTLD_LAZY);

    if(dlhandle == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::implementation_error,
                             "dlopen() failed: %s",
                             dlerror());
    }
}

/**
 * JeodTrickMemoryInterface destructor.
 */
JeodTrickMemoryInterface::~JeodTrickMemoryInterface()
{
    if(dlhandle != nullptr)
    {
        dlclose(dlhandle);
        dlhandle = nullptr;
    }
}

/**
 * Set the mode and perform mode transitions.
 * @param new_mode  New mode
 */
void JeodTrickMemoryInterface::set_mode(JeodSimulationInterface::Mode new_mode)
{
    mode = new_mode;
}

/**
 * Construct an identifier for a chunk of JEOD-allocated memory.
 * @return Identifier string
 * @param unique_id_number  Identifier number
 */
std::string JeodTrickMemoryInterface::construct_identifier(uint32_t unique_id_number)
{
    std::stringstream sstream;

    // Construct the unique ID.
    sstream << id_prefix << std::setfill('0') << std::setw(id_length) << unique_id_number;
    return sstream.str();
}

/**
 * Register the checkpointable object with Trick.
 * This function is typically called at construction or initialization time
 * via JEOD_REGISTER_CHECKPOINTABLE.
 * This default implementation does nothing.
 * @param[in]     owner       Owner of the container
 * @param[in]     owner_type  Owner type descriptor
 * @param[in]     elem_name   Container element
 * @param[in,out] container   The container
 */
void JeodTrickMemoryInterface::register_container(const void * owner JEOD_UNUSED,
                                                  const JeodMemoryTypeDescriptor & owner_type JEOD_UNUSED,
                                                  const std::string & elem_name JEOD_UNUSED,
                                                  JeodCheckpointable & container JEOD_UNUSED)
{
    ; // Empty
}

/**
 * Revoke the registrations performed by register_container.
 * This function is typically called at destruction time
 * via JEOD_DEREGISTER_CHECKPOINTABLE.
 * This default implementation does nothing.
 * @param[in]     owner       Owner of the container
 * @param[in]     owner_type  Owner type descriptor
 * @param[in]     elem_name   Container element
 * @param[in,out] container   The container
 */
void JeodTrickMemoryInterface::deregister_container(const void * owner JEOD_UNUSED,
                                                    const JeodMemoryTypeDescriptor & owner_type JEOD_UNUSED,
                                                    const std::string & elem_name JEOD_UNUSED,
                                                    JeodCheckpointable & container JEOD_UNUSED)
{
    ; // Empty
}

/**
 * Stubbed-out implementation of get_name_at_address for Trick implementations
 * that do not fully support JEOD checkpoint/restart requirements.
 * @return Name of the address, if any.
 * @param addr   Address of memory whose name is to be found
 * @param tdesc  How to interpret address
 */
const std::string JeodTrickMemoryInterface::get_name_at_address(
    const void * addr JEOD_UNUSED, const JeodMemoryTypeDescriptor * tdesc JEOD_UNUSED) const
{
    std::string result;
    return result;
}

/**
 * Stubbed-out implementation of get_address_at_name for Trick implementations
 * that do not fully support JEOD checkpoint/restart requirements.
 * @return Address of named item in memory
 * @param name  Name of item to be found
 */
void * JeodTrickMemoryInterface::get_address_at_name(const std::string & name JEOD_UNUSED) const
{
    return nullptr;
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
