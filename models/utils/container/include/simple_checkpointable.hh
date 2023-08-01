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
 * @file models/utils/container/include/simple_checkpointable.hh
 * Define the class SimpleCheckpointable.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/


#ifndef JEOD_SIMPLE_CHECKPOINTABLE_HH
#define JEOD_SIMPLE_CHECKPOINTABLE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "checkpointable.hh"


//! Namespace jeod
namespace jeod {

/**
 * The SimpleCheckpointable class provides a simple checkpoint/restart interface
 * by which an object can complete the restart process. Typical use of the class
 * is to restore inherently uncheckpointable data such as file streams and
 * function pointers.
 *
 * The SimpleCheckpointable is an incomplete class. Derived classes must define
 * a simple_restore() method to make the derived class complete. This method
 * will be called as a part of the container restart process. Those derived
 * classes should not override the overrides provided by this class. Derived
 * classes can override the pre_ and post_ checkpoint and restart methods.
 */
class SimpleCheckpointable : public JeodCheckpointable {

   JEOD_MAKE_SIM_INTERFACES(SimpleCheckpointable)

public:
   /**
    * Construct a SimpleCheckpointable object.
    */
   SimpleCheckpointable () {
   }

   /**
    * Destruct a SimpleCheckpointable object.
    */
   ~SimpleCheckpointable (void) override {}

   /**
    * Return the name of the initial restart action, in this case "restore".
    * A derived class can of course override this.
    */
   const std::string get_init_name (void) override {
      return "restore";
   }

   /**
    * Return the name of the current restart action, in this case "".
    * This method is not called because the class immediately designates the
    * checkpoint to be finished.
    */
   const std::string get_item_name (void) override { return ""; }

   /**
    * Return the value of the current restart action, in this case "".
    * This method is not called because the class immediately designates the
    * checkpoint to be finished.
    */
   const std::string get_item_value (void) override { return ""; }

   /**
    * In general, start the checkpoint process.
    * For this class, do nothing.
    */
   void start_checkpoint (void) override { }

   /**
    * In general, advance to the next checkpoint item; in this case, do nothing.
    * This method is not called because the class immediately designates the
    * checkpoint to be finished.
    */
   void advance_checkpoint (void) override { }

   /**
    * In general, indicate when checkpointing is complete.
    * For this class, always return true.
    */
   bool is_checkpoint_finished (void) override {
      return true;
   }

   /**
    * In general, respond to the actions recorded in the checkpoint file.
    * For this class, the only recorded action is "restore", and the response
    * is to invoke the (undefined) simple_restore method.
    * @param action_name  The name of the action; here just "restore".
    * @param action_value The value of the action; here ignored.
    * @return             Success (zero) / failure (non-zero).
    */
   int perform_restore_action (
      const std::string & action_name,
      const std::string & action_value JEOD_UNUSED) override
   {
      int return_code;

      // "restore" action is to call the (undefined) simple_restore method.
      if (action_name.compare("restore") == 0) {
         simple_restore ();
         return_code = 0;
      }

      // All other actions are illegal and should never happen.
      else {
         return_code = -1;
      }

      return return_code;
   }

protected:

   /**
    * Perform the sole restore action.
    */
   virtual void simple_restore (void) = 0;


private:

   /**
    * Not implemented.
    */
   SimpleCheckpointable (const SimpleCheckpointable &);

   /**
    * Not implemented.
    */
   SimpleCheckpointable & operator = (const SimpleCheckpointable &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
