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
 * @file models/utils/sim_interface/include/memory_interface.hh
 * Define the MemoryInterface class, which abstractly defines the interface
 * between the memory manager and the simulation engine.
 */

/*******************************************************************************

Purpose:
   ()

Library dependencies:
   ((../src/memory_interface.cc))

 

*******************************************************************************/


#ifndef JEOD_MEMORY_INTERFACE_HH
#define JEOD_MEMORY_INTERFACE_HH


// System includes
#include <cstddef>
#include <string>
#include <typeinfo>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "memory_attributes.hh"

//! Namespace jeod
namespace jeod {

// Forward declarations
class JeodCheckpointable;
class JeodMemoryItem;
class JeodMemoryTypeDescriptor;


/**
 * Abstract interface between the JEOD memory manager and the simulation engine.
 */
class JeodMemoryInterface {

JEOD_MAKE_SIM_INTERFACES(JeodMemoryInterface)

public:

   // Member functions

   // Constructors, destructor, assignment operator
   JeodMemoryInterface ();
   virtual ~JeodMemoryInterface ();
   explicit JeodMemoryInterface(const JeodMemoryInterface &);
   JeodMemoryInterface & operator = (const JeodMemoryInterface &);


   // Virtual methods, all pure virtual.

   /**
    * Find the attributes for a given class name.
    * \param[in] type_name  Name of the class.
    * \return Attributes pointer. Note: This is not an allocated pointer.
    */
   virtual const JEOD_ATTRIBUTES_POINTER_TYPE find_attributes (
      const std::string & type_name) const = 0;

   /**
    * Find the attributes for a given class.
    * \param[in] data_type  RTTI descriptor of the type.
    * \return Attributes pointer. Note: This is not an allocated pointer.
    */
   virtual const JEOD_ATTRIBUTES_POINTER_TYPE find_attributes (
      const std::type_info & data_type) const = 0;

   /**
    * Create an attributes structure that represents a primitive type.
    * \param[in] data_type  RTTI descriptor of the type.
    * \return Attributes structure.
    */
   virtual JEOD_ATTRIBUTES_TYPE primitive_attributes (
      const std::type_info & data_type) const = 0;

   /**
    * Create an attributes structure that represents a pointer type.
    * \param[in] pointed_to_attr  Attributes of the pointed-to type.
    * \return Attribute structure.
    */
   virtual JEOD_ATTRIBUTES_TYPE pointer_attributes (
      const JEOD_ATTRIBUTES_TYPE & pointed_to_attr) const = 0;

   /**
    * Create a simulation engine description of void*.
    * \return Attribute structure.
    */
   virtual JEOD_ATTRIBUTES_TYPE void_pointer_attributes (void) const = 0;

   /**
    * Create an attributes structure that represents a structured type.
    * \param[in] target_attr    Attributes from find_attributes
    * \param[in] target_size    Size of the underlying type
    * \return Attribute structure.
    */
   virtual JEOD_ATTRIBUTES_TYPE structure_attributes (
      const JEOD_ATTRIBUTES_POINTER_TYPE target_attr,
      std::size_t target_size) const = 0;

   /**
    * Register allocated memory with the simulation engine.
    * \param[in] addr  Address of allocated memory to be registered.
    * \param[in] item  JEOD descriptor of the allocated memory
    * \param[in] tdesc JEOD descriptor of the type of the allocated memory
    * \param[in] file  File in which allocation was performed
    * \param[in] line  Line number in that file
    */
   virtual bool register_allocation (
      const void * addr,
      const JeodMemoryItem & item,
      const JeodMemoryTypeDescriptor & tdesc,
      const char * file,
      unsigned int line) = 0;

   /**
    * Revoke registation of memory that is about to be deleted.
    * \param[in] addr  Address of allocated memory to be de-registered.
    * \param[in] item  JEOD descriptor of the memory
    * \param[in] tdesc JEOD descriptor of the type of the allocated memory
    * \param[in] file  File in which allocation was performed
    * \param[in] line  Line number in that file
    */
   virtual void deregister_allocation (
      const void * addr,
      const JeodMemoryItem & item,
      const JeodMemoryTypeDescriptor & tdesc,
      const char * file,
      unsigned int line) = 0;

   /**
    * Register a JeodCheckpointable object with the simulation engine.
    * \param[in]     container      Object that contains the checkpointable
    * \param[in]     container_type Checkpointable container type info
    * \param[in]     elem_name      Element name of checkpointable object
    * \param[in,out] checkpointable The checkpointable object itself
    */
   virtual void register_container (
      const void * container,
      const JeodMemoryTypeDescriptor & container_type,
      const char * elem_name,
      JeodCheckpointable & checkpointable) = 0;

   /**
    * Deregister a JeodCheckpointable object with the simulation engine.
    * \param[in]     container      Object that contains the checkpointable
    * \param[in]     container_type Checkpointable container type info
    * \param[in]     elem_name      Element name of checkpointable object
    * \param[in,out] checkpointable The checkpointable object itself
    */
   virtual void deregister_container (
      const void * container,
      const JeodMemoryTypeDescriptor & container_type,
      const char * elem_name,
      JeodCheckpointable & checkpointable) = 0;

   /**
    * Indicates whether the checkpoint/restart methods are viable.
    * Checkpoint/restart can be used only in an environment that
    * provides viable checkpoint/restart methods.
    * \return  True if the checkpoint / restart is supported, false otherwise.
    */
   virtual bool is_checkpoint_restart_supported (void) const = 0;

   /**
    * Get the simulation engine's name (if any) of the address.
    * A derived class associated with a simulation engine that does not support
    * this translation should return an empty string for all calls.
    * When the underlying simulation engine does support this translation,
    * the implementation should return values as follows:
    *  - The string "NULL" if the input address is the null pointer.
    *  - The empty string to indicate an invalid input address or an input
    *    address that is unknown to the simulation engine.
    *  - A non-empty, non-"NULL" string to indicate a valid address.
    *    Applying the get_address_at_name method to this result must yield
    *    the input address.
    * \note
    * A stubbed implementation should have its is_checkpoint_restart_supported
    * method return false.
    * \return          Name of the address, if any
    * \param[in] addr  Address of memory to identified by name
    * \param[in] tdesc Type context in which to interpret the address
    */
   virtual const std::string get_name_at_address (
      const void * addr,
      const JeodMemoryTypeDescriptor * tdesc) const = 0;

   /**
    * Get the address (if any) identified by the given name.
    * \note
    * An implementation that does not support name translation will
    * return the null pointer.
    * \note
    * A stubbed implementation should have its is_checkpoint_restart_supported
    * method return false.
    * \return          Address corresponding to the given name, if any
    * \param[in] name  Value previously constructed by get_name_at_address()
    */
   virtual void * get_address_at_name (
      const std::string & name) const = 0;
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
