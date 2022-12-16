/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_sim_interface.cc
 * Implement TrickSimInterface methods.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((trick_sim_interface.o)
   (checkpoint_input_manager.o)
   (checkpoint_output_manager.o)
   (sim_interface_messages.o)
   (simulation_interface.o)
   (trick_memory_interface.o)
   (trick_message_handler.o)
   (utils/memory/memory_manager.o))

 

*******************************************************************************/


#ifdef TRICK_VER

// System includes

// Trick includes
#include "sim_services/Executive/include/exec_proto.h"
#include "sim_services/Message/include/message_proto.h"
#include "sim_services/CommandLineArguments/include/command_line_protos.h"

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/sim_interface_messages.hh"
#include "../include/jeod_trick_integrator.hh"
#include "../include/trick_sim_interface.hh"

#include "../include/checkpoint_input_manager.hh"
#include "../include/checkpoint_output_manager.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a BasicJeodTrickSimInterface object.
 * \param[in,out] message_handler handler\n Units: Message
 */
BasicJeodTrickSimInterface::BasicJeodTrickSimInterface (
   MessageHandler & message_handler)
:
   generic_message_handler(message_handler),
   trick_memory_interface(),
   memory_manager(trick_memory_interface),
   checkpoint_file_name(),
   section_start(),
   section_end(),
   checkpoint_reader(NULL),
   checkpoint_writer(NULL)
{

   // Tell the message handler to register its checkpointable content, which it
   // could not do at construction time because the message handler was
   // constructed before the memory manager (or this).
   generic_message_handler.register_contents ();

   // Build the checkpoint file section marker strings.
   section_start = "// ++";
   section_end   = "// --";
   for (unsigned int ii = 0; ii < 4; ++ii) {
      section_start += "++++++++++";
      section_end   += "----------";
   }
   section_start += " Start of section ";
   section_end   += " End of section ";

   // Register classes.
   JEOD_REGISTER_CLASS (JeodTrickIntegrator);
}


/**
 * Destroy a BasicJeodTrickSimInterface object.
 */
BasicJeodTrickSimInterface::~BasicJeodTrickSimInterface (
   void)
{
   if (checkpoint_reader != NULL) {
      delete checkpoint_reader;
      checkpoint_reader = NULL;
   }

   if (checkpoint_writer != NULL) {
      delete checkpoint_writer;
      checkpoint_writer = NULL;
   }
}


/**
 * Set the mode.
 *
 * \par Assumptions and Limitations
 *  - See SimulationInterface::set_mode.
 * \param[in] new_mode New mode.
 */
void
BasicJeodTrickSimInterface::set_mode (
   JeodSimulationInterface::Mode new_mode)
{
   // Set the mode.
   JeodSimulationInterface::set_mode (new_mode);

   // Pass the mode off to interested members.
   // FIXME!! Making the message handler aware of the mode is a good idea.
   // message_handler.set_mode (get_mode());
   trick_memory_interface.set_mode (get_mode()),
   memory_manager.set_mode (get_mode());
}


/**
 * Create an integration interface object.
 * @return Integrator interface that encapsulates an sim engine integrator.
 */
JeodIntegratorInterface *
BasicJeodTrickSimInterface::create_integrator_internal (
   void)
{
   return JEOD_ALLOC_CLASS_OBJECT (JeodTrickIntegrator, ());
}


/**
 * Get the current job's cycle time.
 * @return Current job's cycle time\n Units: s
 */
double
BasicJeodTrickSimInterface::get_job_cycle_internal (
   void)
{
   return exec_get_job_cycle (NULL);
}


/**
 * Get the memory interface.
 * @return Memory interface
 */
JeodMemoryInterface &
BasicJeodTrickSimInterface::get_memory_interface_internal (
   void)
{
   return trick_memory_interface;
}


/**
 * Open the checkpoint output file.
 */
void
BasicJeodTrickSimInterface::open_checkpoint_file (
   void)
{
   std::string output_file_name;

   if (! trick_memory_interface.is_checkpoint_restart_supported()) {
      return;
   }

   if (checkpoint_file_name.empty()) {
      output_file_name =
         trick_memory_interface.get_trick_checkpoint_file (true);

      if (output_file_name.empty()) {
         output_file_name = "jeod_checkpoint";
      }
      else {
         std::size_t last_slash =
            output_file_name.find_last_of ('/');
         std::size_t insert_pos =
            (last_slash == output_file_name.npos) ? 0 : last_slash + 1;
         output_file_name.insert (insert_pos, "jeod_");
      }
   }

   else {
      output_file_name = checkpoint_file_name;
   }

   checkpoint_writer = new CheckPointOutputManager (
      output_file_name, section_start, section_end);
}


/**
 * Get a writer to a section of the currently open checkpoint file.
 * @return Checkpoint writer
 * \param[in] section_id Section name
 */
SectionedOutputStream
BasicJeodTrickSimInterface::get_checkpoint_writer_internal (
   const std::string & section_id)
{
   if (checkpoint_writer == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::phasing_error,
         "JEOD checkpoint file is not open for output.");
      return SectionedOutputStream ();
   }
   else {
      return checkpoint_writer->create_section_writer (section_id);
   }
}


/**
 * Close the checkpoint output file.
 */
void
BasicJeodTrickSimInterface::close_checkpoint_file (
   void)
{
   if (checkpoint_writer != NULL) {
      delete checkpoint_writer;
      checkpoint_writer = NULL;
   }
}


/**
 * Open the checkpoint input file.
 */
void
BasicJeodTrickSimInterface::open_restart_file (
   void)
{
   std::string input_file_name;

   if (! trick_memory_interface.is_checkpoint_restart_supported()) {
      return;
   }

   if (checkpoint_file_name.empty()) {
      input_file_name =
         trick_memory_interface.get_trick_checkpoint_file (false);

      if (input_file_name.empty()) {
         input_file_name = "jeod_checkpoint";
      }
      else {
         std::size_t last_slash =
            input_file_name.find_last_of ('/');
         std::size_t insert_pos =
            (last_slash == input_file_name.npos) ? 0 : last_slash + 1;
         input_file_name.insert (insert_pos, "jeod_");
      }
   }

   else {
      input_file_name = checkpoint_file_name;
   }

   checkpoint_reader = new CheckPointInputManager (
      input_file_name, section_start, section_end);
}


/**
 * Get a reader to a section of the currently open checkpoint file.
 * @return Checkpoint reader
 * \param[in] section_id Section name
 */
SectionedInputStream
BasicJeodTrickSimInterface::get_checkpoint_reader_internal (
   const std::string & section_id)
{
   if (checkpoint_reader == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::phasing_error,
         "JEOD checkpoint file is not open for output.");
      return SectionedInputStream ();
   }
   else {
      return checkpoint_reader->create_section_reader (section_id);
   }
}


/**
 * Close the checkpoint input file.
 */
void
BasicJeodTrickSimInterface::close_restart_file (
   void)
{
   if (checkpoint_reader != NULL) {
      delete checkpoint_reader;
      checkpoint_reader = NULL;
   }
}


/**
 * Dump the allocation information to the checkpoint file.
 */
void
BasicJeodTrickSimInterface::checkpoint_allocations (
   void)
{
   if (trick_memory_interface.is_checkpoint_restart_supported()) {
      trick_memory_interface.checkpoint_allocations ();
   }
}


/**
 * Restore the allocated data per the checkpoint file.
 */
void
BasicJeodTrickSimInterface::restore_allocations (
   void)
{
   if (trick_memory_interface.is_checkpoint_restart_supported()) {
      trick_memory_interface.restore_allocations (memory_manager);
   }
}


/**
 * Dump the container objects to the checkpoint file.
 */
void
BasicJeodTrickSimInterface::checkpoint_containers (
   void)
{
   if (trick_memory_interface.is_checkpoint_restart_supported()) {
      trick_memory_interface.checkpoint_containers ();
   }
}


/**
 * Restore the container objects from the checkpoint file.
 */
void
BasicJeodTrickSimInterface::restore_containers (
   void)
{
   if (trick_memory_interface.is_checkpoint_restart_supported()) {
      trick_memory_interface.restore_containers ();
   }
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
