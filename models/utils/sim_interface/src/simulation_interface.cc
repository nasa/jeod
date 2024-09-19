/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/simulation_interface.cc
 * Implement SimulationInterface methods.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((simulation_interface.cc)
   (sim_interface_messages.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/memory_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/sim_interface_messages.hh"
#include "../include/simulation_interface.hh"

//! Namespace jeod
namespace jeod
{

/*
Purpose: (Define storage for the simulation-wide SimulationInterface object.)
*/
JeodSimulationInterface * JeodSimulationInterface::sim_interface = nullptr;

/**
 * Construct a JeodSimulationInterfaceInit object.
 */
JeodSimulationInterfaceInit::JeodSimulationInterfaceInit()
    : message_suppression_level(MessageHandler::Warning)
{
#ifdef JEOD_MEMORY_DEBUG
#if JEOD_MEMORY_DEBUG > 2
    message_suppression_level = MessageHandler::Debug;
    memory_debug_level = 3;
#elif JEOD_MEMORY_DEBUG > 0
    memory_debug_level = JEOD_MEMORY_DEBUG;
#endif
#endif
}

/**
 * Construct a JeodSimulationInterface object.
 */
JeodSimulationInterface::JeodSimulationInterface()
{
    if(sim_interface == nullptr)
    {
        sim_interface = this;
    }
    else
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::singleton_error,
                              "Multiple instances of a SimulationInterface created");
    }
}

/**
 * Destruct a JeodSimulationInterface object.
 */
JeodSimulationInterface::~JeodSimulationInterface()
{
    if(sim_interface == this)
    {
        sim_interface = nullptr;
    }
}

/**
 * Configure a JeodSimulationInterface object.
 * \param[in] config Configuration spec
 */
void JeodSimulationInterface::configure(const JeodSimulationInterfaceInit & config)
{
    MessageHandler::set_suppression_level(config.message_suppression_level);

    MessageHandler::set_suppress_id(config.message_suppress_id);

    MessageHandler::set_suppress_location(config.message_suppress_location);

    JeodMemoryManager::set_debug_level(config.memory_debug_level);
}

/**
 * Create a simulation integrator interface object.
 * @return Constructed IntegratorInterface object.
 */
JeodIntegratorInterface * JeodSimulationInterface::create_integrator_interface()
{
    // Throw a fatal error if the singleton sim interface is not available.
    if(sim_interface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::singleton_error,
                             "The master simulation interface has not been established.");
        return nullptr;
    }

    // Nominal case: Pass the call on to the sim interface object.
    else
    {
        return sim_interface->create_integrator_internal();
    }
}

/**
 * Get the cycle time of the currently executing job.
 * @return Cycle time in simulation engine seconds of the currently executing job.\n Units: s
 */
double JeodSimulationInterface::get_job_cycle()
{
    // Throw a fatal error if the singleton sim interface is not available.
    if(sim_interface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::singleton_error,
                             "The master simulation interface has not been established.");
        return 0;
    }

    // Nominal case: Pass the call on to the sim interface object.
    else
    {
        return sim_interface->get_job_cycle_internal();
    }
}

/**
 * Get the interface with the simulation memory model.
 * @return Memory interface
 */
JeodMemoryInterface & JeodSimulationInterface::get_memory_interface()
{
    // Throw a fatal error if the singleton sim interface is not available.
    if(sim_interface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::singleton_error,
                             "The master simulation interface has not been established.");
    }

    // Nominal case: Pass the call on to the sim interface object.
    return sim_interface->get_memory_interface_internal();
}

/**
 * Translate the given address to a symbolic name.
 * @return Symbolic name
 * \param[in] addr Address
 * \param[in] tdesc Descriptor
 */
std::string JeodSimulationInterface::get_name_at_address(const void * addr, const JeodMemoryTypeDescriptor * tdesc)
{
    // Throw a fatal error if the singleton sim interface is not available.
    if(sim_interface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::singleton_error,
                             "The master simulation interface has not been established.");
        return nullptr;
    }

    // Nominal case: Pass the call on to the sim interface object.
    else
    {
        return sim_interface->get_memory_interface_internal().get_name_at_address(addr, tdesc);
    }
}

/**
 * Translate the given symbolic name to an address.
 * @return Address
 * \param[in] name Symbolic name
 */
void * JeodSimulationInterface::get_address_at_name(const std::string & name)
{
    // Throw a fatal error if the singleton sim interface is not available.
    if(sim_interface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::singleton_error,
                             "The master simulation interface has not been established.");
        return nullptr;
    }

    // Nominal case: Pass the call on to the sim interface object.
    else
    {
        return sim_interface->get_memory_interface_internal().get_address_at_name(name);
    }
}

/**
 * Get a reader of a section of the currently open checkpoint file.
 * @return Checkpoint reader
 * \param[in] section_id Section ID
 */
SectionedInputStream JeodSimulationInterface::get_checkpoint_reader(const std::string & section_id)
{
    // Throw a fatal error if the singleton sim interface is not available.
    if(sim_interface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::singleton_error,
                             "The master simulation interface has not been established.");
    }

    // Nominal case: Return a reference to the sim interface's checkpoint reader.
    return sim_interface->get_checkpoint_reader_internal(section_id);
}

/**
 * Get a writer to a section of the currently open checkpoint file.
 * @return Checkpoint writer
 * \param[in] section_id Section ID
 */
SectionedOutputStream JeodSimulationInterface::get_checkpoint_writer(const std::string & section_id)
{
    // Throw a fatal error if the singleton sim interface is not available.
    if(sim_interface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::singleton_error,
                             "The master simulation interface has not been established.");
    }

    // Nominal case: Return a reference to the sim interface's checkpoint writer.
    return sim_interface->get_checkpoint_writer_internal(section_id);
}

/**
 * Set the mode, but only if allowed per the mode state transition diagram.
 *
 * \par Assumptions and Limitations
 *  - The standard JEODSys Trick sim object follows the correct state transition
 *     diagram. A similar sequence must be implemented when JEOD is used outside
 *     of the Trick environment. In a Trick environment, *nobody* should call this
 *     function except the Trick scheduler, and these calls must conform with the
 *     sequence in the standard JEODSys Trick sim object.
 * \param[in] new_mode New mode
 */
void JeodSimulationInterface::set_mode(Mode new_mode)
{
    static const int allowed_transitions[NumModes][NumModes] = {
  /* The commentary to the side of the table below uses the abbreviations:
  Construction   =  0 => Cotr
  PreCheckpoint  =  1 => Prec
  Checkpoint     =  2 => Ckpt
  PostCheckpoint =  3 => Post
  Restart        =  4 => Redo
  Restore        =  5 => Rtor
  Initialization =  6 => Init
  Operational    =  7 => Oper
  Shutdown       =  8 => Shut
  Dead           =  9 => Dead. */
        {0, 0, 0, 0, 1, 0, 1, 0, 1, 0}, // Cotr -> Init, Redo, Shut
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, // Prec -> Ckpt, Shut
        {0, 0, 0, 1, 0, 1, 0, 0, 1, 0}, // Ckpt -> Post, Rtor, Shut
        {0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, // Post -> Rtor, Shut
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, // Redo -> Shut (error), rest is magic!
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Rtor -> nothing (not a mode)
        {0, 1, 0, 0, 1, 0, 0, 1, 1, 0}, // Init -> Prec, Redo, Oper, Shut
        {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, // Oper -> Prec, Redo, Shut
        {0, 1, 0, 0, 1, 0, 0, 0, 1, 1}, // Shut -> Prec, Redo, Shut, Dead
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Dead -> nothing (Dead is dead)
    };

    // Check that the new_mode is in range.
    if((new_mode < Construction) || (new_mode > Dead))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::implementation_error,
                             "Illegal value %d to JeodSimulationInterface::set_mode()",
                             static_cast<int>(new_mode));
        return;
    }

    // Warn and return (should be die) if the transition is forbidden.
    if(allowed_transitions[mode][new_mode] == 0)
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               SimInterfaceMessages::phasing_error,
                               "Transition %d -> %d is illegal. Ignoring transition.",
                               mode,
                               new_mode);
        return;
    }

    // Set the mode per the commanded mode.
    switch(new_mode)
    {
        // PreCheckpoint:
        // Transition to the new_mode, but save the current mode so the transition
        // out of the checkpoint sequence will be back to the current mode.
        case PreCheckpoint:
            saved_mode = mode;
            mode = new_mode;
            break;

        // Restart:
        // Transition to the new mode, but set the saved mode to something bad.
        // The reload should set the saved mode to the value at checkpoint time.
        case Restart:
            saved_mode = Shutdown;
            mode = new_mode;
            break;

        // Restore:
        // Transition back to the saved mode.
        case Restore:
            mode = saved_mode;
            break;

        // Checkpoint and PostCheckpoint:
        // Just set the mode. Do not touch the saved_mode.
        case Checkpoint:
        case PostCheckpoint:
            mode = new_mode;
            break;

        // Everything else is non-funky.
        case Construction:
        case Initialization:
        case Operational:
        case Shutdown:
        case Dead:
        case NumModes:
        default:
            mode = saved_mode = new_mode;
            break;
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
