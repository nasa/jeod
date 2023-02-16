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
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/include/memory_attributes_templates.hh
 * Define the class template JeodSimEngineAttributes.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/

#ifndef JEOD_SIM_ENGINE_ATTRIBUTES_HH
#define JEOD_SIM_ENGINE_ATTRIBUTES_HH

/**
 * @addtogroup classes
 * @{
 */


// Model includes

// JEOD includes
#include "utils/sim_interface/include/memory_attributes.hh"
#include "utils/sim_interface/include/memory_interface.hh"
#include "utils/sim_interface/include/simulation_interface.hh"

// System includes
#include <typeinfo>
#include <type_traits>

//! Namespace jeod
namespace jeod {

/**
 * Class template to construct a simulation engine attributes object
 * that represents some type.
 * All partial template instantiations of this template define a
 * class with a single static function named attributes.
 * This default implementation is for a primitive type.
 * Subsequent partial instantiations will address other types.
 * @tparam Type      The type for which an attributes is to be constructed.
 * @tparam is_class  True if the type is a class, false otherwise.
 */
template<typename Type, bool is_class>
class JeodSimEngineAttributes {
public:

   /**
    * Construct a JEOD_ATTRIBUTES_TYPE that represents a primitive type.
    * @return Constructed attributes object.
    */
   static JEOD_ATTRIBUTES_TYPE attributes (bool)
   {
      const JeodMemoryInterface & memory_interface =
         JeodSimulationInterface::get_memory_interface();
      return memory_interface.primitive_attributes (typeid(Type));
   }
};


/**
 * Partial template instantiation of JeodSimEngineAttributes for
 * a pointer type.
 * @tparam Type      The pointed-to type.
 */
template<typename Type>
class JeodSimEngineAttributes<Type*, false> {
public:

   /**
    * Construct a JEOD_ATTRIBUTES_TYPE that represents a pointer type.
    * @param is_exportable  True => type is exportable.
    * @return Constructed attributes object.
    */
   static JEOD_ATTRIBUTES_TYPE attributes (bool is_exportable = true)
   {
      const JeodMemoryInterface & memory_interface =
         JeodSimulationInterface::get_memory_interface();
      const bool type_is_class = std::is_class<Type>::value;
      return memory_interface.pointer_attributes (
         JeodSimEngineAttributes<Type, type_is_class>::attributes(
            is_exportable));
   }
};


/**
 * Template specialization of JeodSimEngineAttributes for void*.
 */
template<>
class JeodSimEngineAttributes<void*, false> {
public:

   /**
    * Construct a JEOD_ATTRIBUTES_TYPE that represents a void pointer.
    * @return Constructed attributes object.
    */
   static JEOD_ATTRIBUTES_TYPE attributes (bool)
   {
      const JeodMemoryInterface & memory_interface =
         JeodSimulationInterface::get_memory_interface();
      return memory_interface.void_pointer_attributes ();
   }
};


/**
 * Partial template instantiation of JeodSimEngineAttributes for a class.
 * @tparam Type      The class.
 */
template<typename Type>
class JeodSimEngineAttributes<Type, true> {
public:

   /**
    * Construct a JEOD_ATTRIBUTES_TYPE that represents a structured type.
    * @param is_exportable  True => type is exportable.
    * @return Constructed attributes object.
    */
   static JEOD_ATTRIBUTES_TYPE attributes (bool is_exportable = true)
   {
      const JeodMemoryInterface & memory_interface =
         JeodSimulationInterface::get_memory_interface();
      const JEOD_ATTRIBUTES_POINTER_TYPE attr = is_exportable ?
         memory_interface.find_attributes (typeid(Type)) : NULL;
      return memory_interface.structure_attributes (attr, sizeof (Type));
   }
};


} // End JEOD namespace

/**
 * @}
 */

#endif

/**
 * @}
 * @}
 * @}
 */
