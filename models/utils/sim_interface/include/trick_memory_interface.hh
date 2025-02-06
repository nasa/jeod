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
 * @file models/utils/sim_interface/include/trick_memory_interface.hh
 * Define the interface for registering / deregistering memory with Trick.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/trick_memory_interface.cc))

 

*******************************************************************************/


#ifndef JEOD_TRICK_MEMORY_INTERFACE_HH
#define JEOD_TRICK_MEMORY_INTERFACE_HH

#ifdef TRICK_VER

// System includes
#include <cstddef>
#include <cstring>
#include <list>
#include <map>
#include <stdint.h>
#include <string>

// JEOD includes

// Model includes
#include "jeod_class.hh"
#include "memory_attributes.hh"
#include "memory_interface.hh"
#include "simulation_interface.hh"


//! Namespace jeod
namespace jeod {

// Forward declarations
class JeodCheckpointable;
class JeodMemoryItem;
class JeodMemoryManager;
class JeodMemoryTypeDescriptor;


/**
 * A TrickMemoryInterface implements the two required methods needed to register
 * and deregister memory with the simulation engine, Trick in this case.
 */
class JeodTrickMemoryInterface : public JeodMemoryInterface {

JEOD_MAKE_SIM_INTERFACES(JeodTrickMemoryInterface)

public:

   // Member functions

    // NOTE: This class has no copy constructor or assignment operator.

   // Constructor and destructor
   JeodTrickMemoryInterface ();
   ~JeodTrickMemoryInterface () override;


   // Set the mode
   void set_mode (JeodSimulationInterface::Mode new_mode);

   // Construct an identifier for a chunk of JEOD-allocated memory
   std::string construct_identifier (uint32_t unique_id_number);


   // Find attributes in the symbol table.
   const JEOD_ATTRIBUTES_POINTER_TYPE find_attributes (
      const std::string & type_name) const override;

   const JEOD_ATTRIBUTES_POINTER_TYPE find_attributes (
      const std::type_info & data_type) const override;

   // Create an attributes structure that represents a primitive type.
   JEOD_ATTRIBUTES_TYPE primitive_attributes (
      const std::type_info & data_type) const override;

   // Create an attributes structure that represents a pointer type.
   JEOD_ATTRIBUTES_TYPE pointer_attributes (
      const JEOD_ATTRIBUTES_TYPE & target_attr) const override;

   // Create a simulation engine description of void*.
   JEOD_ATTRIBUTES_TYPE void_pointer_attributes () const override;

   // Create an attributes structure that represents a structured type.
   JEOD_ATTRIBUTES_TYPE structure_attributes (
      const JEOD_ATTRIBUTES_POINTER_TYPE target_attr,
      std::size_t target_size) const override;


   // Register allocated memory with the simulation engine.
   bool register_allocation (
      const void * addr,
      const JeodMemoryItem & item,
      const JeodMemoryTypeDescriptor & tdesc,
      const char * file,
      unsigned int line) override;

   // Revoke registation of memory that is about to be deleted.
   void deregister_allocation (
      const void * addr,
      const JeodMemoryItem & item,
      const JeodMemoryTypeDescriptor & tdesc,
      const char * file,
      unsigned int line) override;


   // Register a JeodCheckpointable object with the simulation engine.
   void register_container (
      const void * owner,
      const JeodMemoryTypeDescriptor & owner_type,
      const char * elem_name,
      JeodCheckpointable & container) override;

   // Revoke registration of a JeodCheckpointable that is about to be deleted.
   void deregister_container (
      const void * owner,
      const JeodMemoryTypeDescriptor & owner_type,
      const char * elem_name,
      JeodCheckpointable & container) override;


   // Get the simulation name (if any) of the address.
   const std::string get_name_at_address (
      const void * addr,
       const JeodMemoryTypeDescriptor * tdesc) const override;

   // Get the address (if any) corresponding to the given name.
   void * get_address_at_name (
      const std::string & name) const override;


   /**
    * The generic Trick memory interface does not support checkpoint/restart.
    */
   bool is_checkpoint_restart_supported () const override { return false; }

   /**
    * Get the name of the current Trick checkpoint file.
    * \param[in] checkpoint  True for checkpoint, false for restart
    * \return Current checkpoint file, or the empty string.
    * \note
    * The default implementation always returns the empty string;
    * checkpoint/restart is not supported by default.
    */
   virtual const std::string get_trick_checkpoint_file (
      bool checkpoint JEOD_UNUSED)
   {
      return std::string();
   }

   /**
    * Dump the container checkpointable objects to the checkpoint file.
    * \note
    * The default implementation does nothing;
    * checkpoint/restart is not supported by default.
    */
   virtual void checkpoint_containers () {}

   /**
    * Restore the container checkpointables objects from the checkpoint file.
    * \note
    * The default implementation does nothing;
    * checkpoint/restart is not supported by default.
    */
   virtual void restore_containers () {}

   /**
    * Dump the allocation information to the checkpoint file.
    * \note
    * The default implementation does nothing;
    * checkpoint/restart is not supported by default.
    */
   virtual void checkpoint_allocations () {}

   /**
    * Restore the allocated data per the checkpoint file.
    * @param memory_manager  JEOD memory manager
    * \note
    * The default implementation does nothing;
    * checkpoint/restart is not supported by default.
    */
   virtual void restore_allocations (
      JeodMemoryManager & memory_manager JEOD_UNUSED)
   {}


protected:

   // Data types

   /**
    * Describes a Checkpointable object.
    */
   struct ContainerListEntry {

      /**
       * The object that contains the container.
       */
      const void * owner; //!< trick_units(--)

      /**
       * Type description of the object that contains the container.
       */
      const JeodMemoryTypeDescriptor & owner_type; //!< trick_units(--)

      /**
       * The name of the element of the container in the owning object.
       */
      std::string elem_name; //!< trick_units(--)

      /**
       * The container itself.
       */
      JeodCheckpointable & container; //!< trick_units(--)


      /**
       * Construct an ContainerListEntry object.
       * @param parent  Parent object
       * @param tdesc   Type descriptor
       * @param sub_id  Parent element
       * @param obj     Checkpointable itself
       */
      ContainerListEntry (
         const void * parent,
         const JeodMemoryTypeDescriptor & tdesc,
         const std::string & sub_id,
         JeodCheckpointable & obj)
      :
         owner(parent),
         owner_type(tdesc),
         elem_name(sub_id),
         container(obj)
      {}
   };


   /**
    * Describes a chunk of JEOD-allocated memory.
    */
   struct AllocationMapEntry {

      /**
       * Descriptor of the data type.
       */
      const std::type_info & typeid_info; //!< trick_units(--)

      /**
       * The number of elements in the allocated chunk of memory.
       */
      uint32_t nelements; //!< trick_units(--)

      /**
       * Is the item an array or a single object?
       */
      bool is_array; //!< trick_units(--)

      /**
       * Construct an AllocationMapEntry object.
       * @param type_info  Type info
       * @param nelem      Array size
       * @param arrayp     Is item an array?
       */
      AllocationMapEntry (
         const std::type_info & type_info,
         uint32_t nelem,
         bool arrayp)
      :
         typeid_info(type_info),
         nelements(nelem),
         is_array(arrayp)
      {}
   };


   /**
    * Maps JEOD-allocated data names to (type, size) pairs.
    */
   typedef std::map <uint32_t, AllocationMapEntry> AllocationMap;

   /**
    * Container of a list of ContainerListEntry objects.
    */
   typedef std::list <ContainerListEntry> ContainerList;


   // Member data

   /**
    * dlhandle, from dlopen.
    */
   void * dlhandle; //!< trick_io(**)

   /**
    * Map of allocated names to type info.
    */
   AllocationMap allocation_map; //!< trick_io(**)

   /**
    * List of container checkpointables.
    */
   ContainerList container_list; //!< trick_io(**)

   /**
    * Prefix used for constructing a unique name for JEOD-allocated memory.
    */
   const std::string id_prefix; //!< trick_io(*o) trick_units(--)

   /**
    * Number of digits in the numeric part of the unique identifier.
    */
   const uint32_t id_length; //!< trick_io(*o) trick_units(--)

   /**
    * Simulation interface mode.
    */
   JeodSimulationInterface::Mode mode; //!< trick_units(--)


private:

   // The parent JeodMemoryInterface hides the copy constructor and
   // assignment operator. These are hidden here as well.

   /**
    * Not implemented.
    */
   JeodTrickMemoryInterface (const JeodTrickMemoryInterface &);

   /**
    * Not implemented.
    */
   JeodTrickMemoryInterface & operator= (const JeodTrickMemoryInterface &);
};


} // End JEOD namespace

#endif

#endif

/**
 * @}
 * @}
 * @}
 */
