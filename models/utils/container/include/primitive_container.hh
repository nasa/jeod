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
 * @file models/utils/container/include/primitive_container.hh
 * Define class template JeodPrimitiveContainer.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/


#ifndef JEOD_MEMORY_PRIMITIVE_CONTAINER_H
#define JEOD_MEMORY_PRIMITIVE_CONTAINER_H

// Model includes
#include "container.hh"
#include "primitive_serializer.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <string>



//! Namespace jeod
namespace jeod {

/**
 * A JeodPrimitiveContainer is a JeodContainer that contains primitive
 * data of type ElemType.
 */
template <typename ContainerType, typename ElemType>
class JeodPrimitiveContainer : public JeodContainer<ContainerType, ElemType> {

public:

   /**
    * Construct a JeodPrimitiveContainer.
    */
   JeodPrimitiveContainer (void) {}

   /**
    * Copy-construct a JeodPrimitiveContainer.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Primitive container to be copied.
    */
   JeodPrimitiveContainer (const JeodPrimitiveContainer & source)
   :
      JeodContainer<ContainerType, ElemType>(source)
   {}

   /**
    * Copy-construct a JeodPrimitiveContainer.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Primitive container to be copied.
    */
   JeodPrimitiveContainer (
      const typename ContainerType::stl_container_type & source)
   :
      JeodContainer<ContainerType, ElemType>(source)
   {}

   /**
    * Copy from a JeodPrimitiveContainer.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Primitive container to be copied.
    */
   JeodPrimitiveContainer &
   operator= (const JeodPrimitiveContainer & source)
   {
      JeodContainer<ContainerType, ElemType>::operator= (source);
      return *this;
   }

   /**
    * Copy from an STL container.
    * @note
    * This copies the Container contents, but not the Checkpointable contents.
    * @param source Primitive container to be copied.
    */
   JeodPrimitiveContainer &
   operator= (const typename ContainerType::stl_container_type & source)
   {
      JeodContainer<ContainerType, ElemType>::operator= (source);
      return *this;
   }

   /**
    * Destruct a JeodPrimitiveContainer.
    */
   virtual ~JeodPrimitiveContainer (void) {}

   /**
    * Return the value of the item to be written to the checkpoint file.
    * JeodPrimitiveContainer use the serializer to translate values to strings.
    */
   virtual const std::string get_item_value (void)
   {
      return serializer.to_string (*this->checkpoint_iter);
   }

   /**
    * Interpret the provided value and insert it at the end of the object.
    * JeodPrimitiveContainer use the serializer to interpret the input value.
    */
   virtual void perform_insert_action (
      const std::string & value)
   {
      this->insert (this->end(), serializer.from_string (value));
   }

protected:

   // Member data

   /**
    * Serializer / deserializer
    */
   JeodPrimitiveSerializer<ElemType> serializer; //!< trick_io(**)
};


// DO NOT USE.
// This is deprecated and will disappear with the next release.
// Use one of the JeodPrimitiveXxx classes instead.
#define JEOD_PRIMITIVE_CONTAINER(container_type,elem_type) \
   JeodPrimitiveContainer<Jeod##container_type<elem_type>,elem_type>


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
