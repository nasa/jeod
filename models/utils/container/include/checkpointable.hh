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
 * @addtogroup Container
 * @{
 *
 * @file models/utils/container/include/checkpointable.hh
 * Define the class JeodCheckpointable, the base class for checkpointing
 * and restoring data that are opaque to the simulation engine.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/


#ifndef JEOD_MEMORY_CHECKPOINTABLE_H
#define JEOD_MEMORY_CHECKPOINTABLE_H


// System includes
#include <string>
#include <typeinfo>


// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * A JeodCheckpointable is an object whose contents are opaque to Trick,
 * and presumably other simulation engines, whose contents can nonetheless
 * be checkpointed and restarted by using the methods defined herein.
 */
class JeodCheckpointable {
JEOD_MAKE_SIM_INTERFACES(JeodCheckpointable)

public:

   // Default constructor
   JeodCheckpointable ();

   // Destructor
   virtual ~JeodCheckpointable ();


   // Virtual functions with null implementations

   // Perform object-specific pre-checkpoint actions.
   virtual void pre_checkpoint (void);

   // Perform object-specific post-checkpoint actions.
   virtual void post_checkpoint (void);

   // Perform object-specific pre-restart actions.
   virtual void pre_restart (void);

   // Perform object-specific post-restart actions.
   virtual void post_restart (void);

   // Initialize the object.
   virtual void initialize_checkpointable (
      const void * container,
      const std::type_info & container_type,
      const std::string & elem_name);

   // Prepare for object destruction
   virtual void undo_initialize_checkpointable (
      const void * container,
      const std::type_info & container_type,
      const std::string & elem_name);

   // Return the value of the init action.
   virtual const std::string get_init_value (void);

   // Return the name of the final action.
   virtual const std::string get_final_name (void);

   // Return the value of the final action.
   virtual const std::string get_final_value (void);


   // Pure virtual functions

   /**
    * Prepare to checkpoint the object in question.
    */
   virtual void start_checkpoint (void) = 0;

   /**
    * Advance to the next item to be checkpointed.
    */
   virtual void advance_checkpoint (void) = 0;

   /**
    * Return true if all contents have been checkpointed, false otherwise.
    */
   virtual bool is_checkpoint_finished (void) = 0;

   /**
    * Return the name of the action, if any, that will be performed prior to
    * performing the individual actions.
    * Note: The init name must be alphanumeric or empty.
    */
   virtual const std::string get_init_name (void) = 0;

   /**
    * Return the name of the action that will restore the value at the current
    * checkpoint position. This action name and the corresponding value will be
    * written to the checkpoint file in the form "owner.action(value);".
    * Note: The item name must be alphanumeric.
    */
   virtual const std::string get_item_name (void) = 0;

   /**
    * Return the value of the item to be written to the checkpoint file.
    * Translation of the true value to a string is up to the implementation.
    * The string value must be something that the restore_perform_action method
    * can translate back to the true value and should also be human-readable;
    * people as well as the Memory Manager read checkpoint files.
    */
   virtual const std::string get_item_value (void) = 0;

   /**
    * Perform a checkpoint-restart action that will, in part, restore the object
    * to its state at the time of the checkpoint. The method is called for
    * each entry in the checkpoint file that pertains to this object.
    * @param action_name  The name of the action.
    * @param action_value The value of the action.
    * @return             Success (zero) / failure (non-zero).
    */
   virtual int perform_restore_action (
      const std::string & action_name,
      const std::string & action_value) = 0;

private:

   /**
    * Not implemented.
    */
   JeodCheckpointable (const JeodCheckpointable &);

   /**
    * Not implemented.
    */
   JeodCheckpointable & operator = (const JeodCheckpointable &);
};


/**
 * Default constructor; does nothing.
 */
inline
JeodCheckpointable::JeodCheckpointable (
   void)
{
   ; // Empty
}


/**
 * Destructor; does nothing.
 */
inline
JeodCheckpointable::~JeodCheckpointable (
   void)
{
   ; // Empty
}


/**
 * In general, return the value of the initialization action.
 * The returned value is written to the checkpoint file as the argument
 * of the init action, but only if the initialization action is not empty.
 *
 * The default implementation is the empty string.
 */
inline const std::string
JeodCheckpointable::get_init_value (
   void)
{
   return "";
}


/**
 * In general, return the name of the finalization action.
 * The returned value is written to the checkpoint file as the name
 * of the final action, but only if this name is not empty.
 *
 * The default implementation is the empty string.
 */
inline const std::string
JeodCheckpointable::get_final_name (
   void)
{
   return "";
}


/**
 * In general, return the value of the finalization action.
 * The returned value is written to the checkpoint file as the argument
 * of the final action, but only if the finalization action is not empty.
 *
 * The default implementation is the empty string.
 */
inline const std::string
JeodCheckpointable::get_final_value (
   void)
{
   return "";
}


/**
 * In general, perform object-specific operations that need to be performed in
 * anticipation of a checkpoint, typically allocating and populating memory.
 * The simulation engine calls this method prior to checkpointing allocations.
 *
 * The default implementation is to do nothing.
 */
inline void
JeodCheckpointable::pre_checkpoint (
   void)
{
   ; // Empty
}


/**
 * In general, perform object-specific operations that need to be performed
 * after checkpoint completion, typically freeing memory used for checkpointing.
 * The simulation engine calls this method after checkpoint-proper completion.
 *
 * The default implementation is to do nothing.
 */
inline void
JeodCheckpointable::post_checkpoint (
   void)
{
   ; // Empty
}


/**
 * In general, perform object-specific operations that need to be performed in
 * anticipation of a restart, typically releasing resources.
 * The simulation engine calls this method prior to restoring allocated data.
 *
 * The default implementation is to do nothing.
 */
inline void
JeodCheckpointable::pre_restart (
   void)
{
   ; // Empty
}


/**
 * In general, perform object-specific operations that need to be performed
 * after restart completion.
 *
 * The default implementation is to do nothing.
 */
inline void
JeodCheckpointable::post_restart (
   void)
{
   ; // Empty
}


/**
 * In general, perform initialization actions such as obtaining requisite type
 * information, registering Checkpointable objects contained within the object,
 * etc.
 *
 * The default implementation is to do nothing.
 *
 * @param container      The object that contains this object.
 * @param container_type The type of the containing object.
 * @param elem_name      The name of the this object in the containing object.
 */
inline void
JeodCheckpointable::initialize_checkpointable (
   const void * container JEOD_UNUSED,
   const std::type_info & container_type JEOD_UNUSED,
   const std::string & elem_name JEOD_UNUSED)
{
   ; // Empty
}


/**
 * In general, undo external actions performed by initialize_checkpointable.
 *
 * The default implementation is to do nothing.
 *
 * @param container      The object that contains this object.
 * @param container_type The type of the containing object.
 * @param elem_name      The name of the this object in the containing object.
 */
inline void
JeodCheckpointable::undo_initialize_checkpointable (
   const void * container JEOD_UNUSED,
   const std::type_info & container_type JEOD_UNUSED,
   const std::string & elem_name JEOD_UNUSED)
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
