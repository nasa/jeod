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
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/simulation_interface.hh
 * Define the abstract class JeodSimulationInterface.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/simulation_interface.cc))

 
*******************************************************************************/

#ifndef JEOD_SIMULATION_INTERFACE_HH
#define JEOD_SIMULATION_INTERFACE_HH


// System includes
#include <string>

// Model includes
#include "class_declarations.hh"
#include "jeod_class.hh"
#include "checkpoint_input_manager.hh"
#include "checkpoint_output_manager.hh"
#include "jeod_integrator_interface.hh"


//! Namespace jeod
namespace jeod {

class JeodMemoryTypeDescriptor;
class CheckPointInputManager;
class CheckPointOutputManager;


/**
 * Define configuration data needed to configure the dynamically-created
 * message handler and memory manager.
 */
class JeodSimulationInterfaceInit {

// This is (almost) a POD class. Everything is public, nothing is virtual,
// but there is a default constructor that sets default values.
public:

   // Member functions.

   // Constructor.
   JeodSimulationInterfaceInit();


   // Member data.

   /**
    * Specifies the message handler's message suppression level;
    * see MessageHandler::suppression_level for details.
    * Default value: MessageHandler::Warning (warnings and non-fatal errors).
    */
   unsigned int message_suppression_level; //!< trick_units(--)

   /**
    * Specifies the message handler's suppress_id flag;
    * see MessageHandler::suppression_id for details.
    * Default value: false.
    */
   bool message_suppress_id; //!< trick_units(--)

   /**
    * Specifies the message handler's suppress_location flag;
    * see MessageHandler::suppression_location for details.
    * Default value: false.
    */
   bool message_suppress_location; //!< trick_units(--)

   /**
    * Specifies the memory manager's debug level;
    * see JeodMemoryManager::debug_level for details.
    * Default value: 0.
    */
   unsigned int memory_debug_level; //!< trick_units(--)
};


/**
 * This abstract class defines the basis for the interface between JEOD and a
 * simulation engine. A compliant derived class must contain one instance each
 * of a class that derives from MessageHandler and a class that derives from
 * JeodMemoryManager. The MessageHandler object must be constructed before
 * the JeodMemoryManager object; destruction must be performed in reverse order.
 */
class JeodSimulationInterface {
 JEOD_MAKE_SIM_INTERFACES (JeodSimulationInterface)

public:

   // Types

   /**
    * Defines the states of the JeodSimulationInterface state machine.
    */
   enum Mode {
      Construction   =  0,  /*
         Construction mode.
         This is the initial mode of the model. Some interactions with the
         simulation engine must be enqueued while in this mode because the
         simulation is not quite operational yet. */

      PreCheckpoint  =  1, /*
         Pre-checkpoint mode.
         The simulation switches to this mode at the very start of the
         checkpoint process. */

      Checkpoint     =  2, /*
         Checkpoint mode.
         The simulation switches to this mode at the very end of the
         pre-checkpoint phase. */

      PostCheckpoint =  3, /*
         Post-checkpoint mode.
         The simulation switches to this mode at the very start of the
         post-checkpoint process. */

      Restart        =  4, /*
         Restart mode.
         The simulation switches to this mode at the very start of the
         restart process.

         Note: At the end of the restart process the mode will be restored
         to the value it had prior to the saved checkpoint. */

      Restore        =  5,  /*
         Restore mode.
         The model is never in this mode. Restore mode signals the model
         to restore the mode to what it was prior to dropping a checkpoint. */

      Initialization =  6,  /*
         Initialization mode.
         Construction mode normally transitions to this mode.
         Classes registered at or beyond this phase may mean the simulation
         is not restartable. */

      Operational    =  7,  /*
         Operational mode.
         Initialization mode normally transitions to this mode.
         Memory allocations in this mode may indicate thrashing of memory. */

      Shutdown       =  8, /*
         Shutdown mode.
         The simulation switches to this mode at the onset of shutdown. */

      Dead           =  9, /*
         Dead mode.
         The only thing that should happen in this mode is data destruction. */

      NumModes       = 10  /*
         This is not a mode; it is instead a counter of the number of modes. */
   };


   // Static functions

   // Create an integration interface object.
   static JeodIntegratorInterface * create_integrator_interface (
      void);

   // Get the simulation cycle time of the currently executing function.
   static double get_job_cycle (void);

   // Translate address to name
   static std::string get_name_at_address (
      const void * addr,
      const JeodMemoryTypeDescriptor * tdesc);

   // Translate name to address
   static void * get_address_at_name (
      const std::string & name);

   // Get the interface with the simulation memory manager.
   static JeodMemoryInterface & get_memory_interface (void);

   // Get a checkpoint section reader.
   static SectionedInputStream get_checkpoint_reader (
      const std::string & section_id);

   // Get a checkpoint sectionwriter.
   static SectionedOutputStream get_checkpoint_writer (
      const std::string & section_id);


   // Member functions
   // NOTE: The copy constructor and assigment operator are deleted.

   // Constructor and destructor
   JeodSimulationInterface ();
   virtual ~JeodSimulationInterface ();

   // Configure the message handler and memory manager.
   virtual void configure (const JeodSimulationInterfaceInit & config);

   /**
    * Get the current mode.
    */
   Mode get_mode (void) const { return mode; }

   // Set the mode, under the purview of a phase transition diagram.
   virtual void set_mode (Mode new_mode);


protected:

   // Static data

   /**
    * The singleton instance of a SimulationInterface object that must be
    * created by a conforming JEOD simulation before any call can be made to
    * one of the three static methods declared above.@n
    * The first created instance of a class that derives from this base class
    * becomes @b the SimulationInterface object used during the course of the
    * simulation. Creation of more than one SimulationInteface objects is a
    * non-fatal error. Attempts to allocate memory or generate a message prior
    * creating a SimulationInteface object is a fatal error.
    */
   static JeodSimulationInterface * sim_interface;  //!< trick_io(*o) trick_units(--)


   // Pure virtual member functions

   /**
    * Create an integration interface object.
    * The calling object is responsible for destroying the created object.
    * @return Created integration interface object.
    */
   virtual JeodIntegratorInterface * create_integrator_internal (
      void) = 0;

   /**
    * Get the simulation cycle time of the currently executing function.
    * @return Cycle time in simulation engine seconds
    */
   virtual double get_job_cycle_internal (void) = 0;

   /**
    * Get the interface with the simulation memory manager.
    * @return JEOD/simulation engine memory interface.
    */
   virtual JeodMemoryInterface & get_memory_interface_internal (void) = 0;

   /**
    * Get a checkpoint section reader.
    */
   virtual SectionedInputStream get_checkpoint_reader_internal (
      const std::string & section_id) = 0;

   /**
    * Get a checkpoint section writer.
    */
   virtual SectionedOutputStream get_checkpoint_writer_internal (
      const std::string & section_id) = 0;


   // Member data

   /**
    * The mode in which the simulation interface is operating.
    */
   Mode mode; //!< trick_units(--)

   /**
    * The mode prior to a checkpoint or restart process.
    * set_mode(Restore) restores the mode to this saved value.
    */
   Mode saved_mode; //!< trick_units(--)


private:

   // Since having multiple instances of a JeodSimulationInterface is verboten,
   // having a copy constructor or assignment operator makes no sense.
   // Declare these to be private (and unimplemented).

   /**
    * Not implemented.
    */
   JeodSimulationInterface (const JeodSimulationInterface &);

   /**
    * Not implemented.
    */
   JeodSimulationInterface & operator= (const JeodSimulationInterface &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
