//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @file models/utils/sim_interface/include/trick_sim_interface.hh
 * Define the class JeodTrickSimInterface.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((trick_sim_interface.o))

 

*******************************************************************************/

#ifndef JEOD_TRICK_SIM_INTERFACE_HH
#define JEOD_TRICK_SIM_INTERFACE_HH

// This only makes sense in a Trick setting.
#ifdef TRICK_VER

// Trick includes

// JEOD includes
#include "utils/memory/include/memory_manager.hh"

// Model includes
#include "simulation_interface.hh"
#include "trick_memory_interface.hh"
#include "trick10_memory_interface.hh"
#include "trick_message_handler.hh"
#include "jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * The BasicJeodTrickSimInterface implements the required capabilities of the
 * generic JeodSimulationInterface in a Trick simulation environment.
 * By virtue of member data ownership, the class creates the requisite
 * MessageHandler and MemoryManager and does so in the correct order.
 */
class BasicJeodTrickSimInterface : public JeodSimulationInterface {
JEOD_MAKE_SIM_INTERFACES(BasicJeodTrickSimInterface)

public:

   // Methods
   // NOTE: The copy constructor or assignment operator are deleted.

   // Constructor and destructor
   explicit BasicJeodTrickSimInterface (MessageHandler & message_handler);
   virtual ~BasicJeodTrickSimInterface ();

   /**
    * Set the checkpoint file name.
    */
   void set_checkpoint_file_name (std::string name)
   { checkpoint_file_name = name; }

   /**
    * Get the checkpoint file name.
    */
   std::string get_checkpoint_file_name () const
   { return checkpoint_file_name; }


   // The next set of functions are public because they are called by the
   // JEODSysSimObject sim object (see).
   // DO NOT call from Python.
   // DO NOT call from model code.
   // DO NOT call from anything but the S_define,
   // and then only do so with the proper sequencing.
   // Suggested usage: Use the default JEODSysSimObject.


   // Set the mode.
   virtual void set_mode (JeodSimulationInterface::Mode new_mode);


   // Record declarations of JEOD_ALLOCed data to the checkpoint file.
   void checkpoint_allocations (void);

   // Re-allocate JEOD_ALLOCed data per checkpoint file declarations.
   void restore_allocations (void);

   // Record contents of checkpointable objects to the checkpoint file.
   void checkpoint_containers (void);

   // Restore contents of checkpointable objects per checkpoint file contents.
   void restore_containers (void);


   // Open the checkpoint file.
   void open_checkpoint_file (void);

   // Close the checkpoint file.
   void close_checkpoint_file (void);

   // Open the restart file.
   void open_restart_file (void);

   // Close the restart file.
   void close_restart_file (void);


protected:

   // Member functions

   // Create an integration interface object.
   virtual JeodIntegratorInterface * create_integrator_internal (
      void);

   // Get the currently executing function's cycle time.
   virtual double get_job_cycle_internal (void);

   // Get the interface with the simulation memory manager.
   virtual JeodMemoryInterface & get_memory_interface_internal (void);

   // Get a checkpoint section reader.
   virtual SectionedInputStream get_checkpoint_reader_internal (
      const std::string & section_id);

   // Get a checkpoint section writer.
   virtual SectionedOutputStream get_checkpoint_writer_internal (
      const std::string & section_id);


   // Member data

   /**
    * The global MessageHandler.
    */
   MessageHandler & generic_message_handler; //!< trick_units(--)

   /**
    * The interface between JEOD and Trick's memory management schemes.
    */
   JeodTrick10MemoryInterface trick_memory_interface; //!< trick_units(--)

   /**
    * The global JEOD memory manager.
    */
   JeodMemoryManager memory_manager; //!< trick_units(--)

   /**
    * The name of the segmented checkpoint file used for the next
    * checkpoint / restart action.
    * If the name is the empty string (default), the checkpoint / restart
    * mechanisms will attempt to construct a name from the corresponding
    * Trick checkpoint file name.
    */
   std::string checkpoint_file_name; //!< trick_units(--)

   /**
    * String indicating the start of a checkpoint file section.
    */
   std::string section_start;  //!< trick_io(*o) trick_units(--)

   /**
    * String indicating the end of a checkpoint file section.
    */
   std::string section_end; //!< trick_io(*o) trick_units(--)

   /**
    * The object that manages reading from a checkpoint file.
    */
   CheckPointInputManager * checkpoint_reader;  //!< trick_io(**)

   /**
    * The object that manages writing to a checkpoint file.
    */
   CheckPointOutputManager * checkpoint_writer; //!< trick_io(**)


private:

   // The parent SimulationInterface hides the copy constructor and
   // assignment operator. These are hidden here as well.

   /**
    * Not implemented.
    */
   BasicJeodTrickSimInterface (const BasicJeodTrickSimInterface &);

   /**
    * Not implemented.
    */
   BasicJeodTrickSimInterface & operator= (const BasicJeodTrickSimInterface &);

};


/**
 * The TrickMessageHandlerMixin implements the required capabilities of the
 * generic JeodSimulationInterface in a Trick simulation environment.
 * By virtue of member data ownership, the class creates the requisite
 * MessageHandler and MemoryManager and does so in the correct order.
 */
class TrickMessageHandlerMixin {
JEOD_MAKE_SIM_INTERFACES(TrickMessageHandlerMixin)

public:

      /**
    * Default constructor.
    */
   TrickMessageHandlerMixin () {}

      /**
    * Destructor.
    */
   virtual ~TrickMessageHandlerMixin () {}

protected:

   // Member data

   /**
    * The global MessageHandler.
    */
   TrickMessageHandler message_handler; //!< trick_units(--)

private:

   // The message handler is a singleton.
   // There is no reason to have a copy constructor or assignment operator.

   /**
    * Not implemented.
    */
   TrickMessageHandlerMixin (const TrickMessageHandlerMixin &);

   /**
    * Not implemented.
    */
   TrickMessageHandlerMixin & operator= (const TrickMessageHandlerMixin &);

};



/**
 * A JEODTrickSimInterface implements the required capabilities of the generic
 * JeodSimulationInterface in a Trick simulation environment.
 * By virtue of member data ownership, the class creates the requisite
 * MessageHandler and MemoryManager and does so in the correct order.
 */
class JeodTrickSimInterface :
  public TrickMessageHandlerMixin, public BasicJeodTrickSimInterface {
JEOD_MAKE_SIM_INTERFACES(JeodTrickSimInterface)

public:

      /**
    * Non-default constructor.
    */
   explicit JeodTrickSimInterface ()
   : BasicJeodTrickSimInterface (message_handler) {}

      /**
    * Destructor.
    */
   virtual ~JeodTrickSimInterface () {}


private:

   // The parent SimulationInterface hides the copy constructor and
   // assignment operator. These are hidden here as well.

   /**
    * Not implemented.
    */
   JeodTrickSimInterface (const JeodTrickSimInterface &);

   /**
    * Not implemented.
    */
   JeodTrickSimInterface & operator= (const JeodTrickSimInterface &);

};


} // End JEOD namespace

#ifdef TRICK_VER
#include "utils/sim_interface/include/jeod_trick_integrator.hh"
#endif

#endif

#endif

/**
 * @}
 * @}
 * @}
 */
