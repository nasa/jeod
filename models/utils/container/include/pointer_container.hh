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
 * @addtogroup Container
 * @{
 *
 * @file models/utils/container/include/pointer_container.hh
 * Define class template JeodPointerContainer.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/


#ifndef JEOD_MEMORY_POINTER_CONTAINER_H
#define JEOD_MEMORY_POINTER_CONTAINER_H

// Model includes
#include "container.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/sim_interface/include/simulation_interface.hh"

// System includes
#include <string>


//! Namespace jeod
namespace jeod {

/**
 * A JeodPointerContainer is a JeodContainer that contains pointers to objects
 * of type ElemType.
 */
template <typename ContainerType, typename ElemType>
class JeodPointerContainer : public JeodContainer<ContainerType, ElemType*> {

public:
   /**
    * Construct a JeodPointerContainer.
    */
   JeodPointerContainer (void)
   :
      JeodContainer<ContainerType, ElemType*>(),
      base_type_descriptor(NULL)
   {}

   /**
    * Copy-construct a JeodPointerContainer.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Pointer container to be copied.
    */
   JeodPointerContainer (const JeodPointerContainer & source)
   :
      JeodContainer<ContainerType, ElemType*>(source),
      base_type_descriptor(NULL)
   {}

   /**
    * Copy-construct a JeodPointerContainer.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Pointer container to be copied.
    */
   JeodPointerContainer (
      const typename ContainerType::stl_container_type & source)
   :
      JeodContainer<ContainerType, ElemType*>(source),
      base_type_descriptor(NULL)
   {}

   /**
    * Copy from a JeodPointerContainer.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Pointer container to be copied.
    */
   JeodPointerContainer &
   operator= (const JeodPointerContainer & source)
   {
      JeodContainer<ContainerType, ElemType*>::operator= (source);
      return *this;
   }

   /**
    * Copy from an STL container.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Pointer container to be copied.
    */
   JeodPointerContainer &
   operator= (const typename ContainerType::stl_container_type & source)
   {
      JeodContainer<ContainerType, ElemType*>::operator= (source);
      return *this;
   }

   /**
    * Destruct a JeodPointerContainer.
    */
   virtual ~JeodPointerContainer (void) {}

   /**
    * Initialize a checkpointable object, called by the checkpoint manager.
    *
    * In the case of a JeodPointerContainer, this method gets the descriptor for
    * the type of data pointed to members of the container.
    */
   virtual void
   initialize_checkpointable (
     const void * container,                // In: -- Not used.
     const std::type_info & container_type, // In: -- Not used.
     const std::string elem_name)           // In: -- Not used.
   {
      JeodContainer<ContainerType, ElemType*>::initialize_checkpointable (
         container, container_type, elem_name);

      if (base_type_descriptor == NULL) {
         base_type_descriptor =
            JeodMemoryManager::get_type_descriptor (typeid(ElemType));
      }
   }

   /**
    * Return the value of the item to be written to the checkpoint file.
    * For a JeodPointerContainer, the value names the pointed-to object.
    */
   virtual const std::string
   get_item_value (void)
   {
      return JeodSimulationInterface::get_name_at_address (
                reinterpret_cast<void *> (*this->checkpoint_iter),
                base_type_descriptor);
   }

   /**
    * Interpret the provided value and add it to the list.
    * For a JeodPointerContainer, the value should specify (in string form)
    * the address of some object in active memory.
    */
   virtual void
   perform_insert_action (
      const std::string & value)
   {
      this->insert (
         this->end(),
         reinterpret_cast<ElemType *> (
            JeodSimulationInterface::get_address_at_name (value)));
   }


protected:

   /**
    * Memory model descriptor of the type of data stored in the container.
    */
   const JeodMemoryTypeDescriptor * base_type_descriptor; //!< trick_io(**)
};


// DO NOT USE.
// This is deprecated and will disappear with the next release.
// Use one of the JeodPointerXxx classes instead.
#define JEOD_POINTER_CONTAINER(container_type,elem_type) \
   JeodPointerContainer<Jeod##container_type<elem_type*>,elem_type>


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
