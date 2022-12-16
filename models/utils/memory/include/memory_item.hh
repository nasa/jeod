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
 * @file models/utils/memory/include/memory_item.hh
 * Define the class JeodMemoryItem.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((memory_item.o))

 

*******************************************************************************/

#ifndef JEOD_MEMORY_ITEM_HH
#define JEOD_MEMORY_ITEM_HH

/**
 * \addtogroup classes
 * @{
 */

// System includes
#include <stdint.h>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * A JeodMemoryItem contains metadata about some chunk of allocated memory.
 * This is a simple datatype that contains POD elements only.
 * All data members are private and are accessible only through getters;
 * the members are essentially constant. The only way to change the values
 * is via a wholesale copy.
 */
class JeodMemoryItem {

 // Types
 public:

   /**
    * Identifies by name the bit flags the comprise a JeodMemoryItem::flag.
    */
   enum Flags {
      PlacementNew =  1, /**< Was the item constructed with placement new?
                            There is no functional placement delete in C++. */
      IsArray      =  2, /**< Was the item an array constructed via new []?
                            This addresses the delete[] versus delete issue. */
      IsGuarded    =  4, /**< Is the allocated buffer surrounded by guard words?
                            This flag is always false in regular new mode. */
      IsStructured =  8, /**< Is the item a class (versus a primitive type)?
                            Classes add several other twists. */
      IsRegistered = 16, /**< Has the item been registered with the simulation
                            engine? */
      CheckPointed = 32  /**< Reserved for future work,
                            as are flag bits 6 ando 7 (64 and 128). */
   };

 // Static methods
 private:
   // Construct the flags for a new instance of a memory item
   static uint8_t construct_flags (
      bool placement_new, bool is_array, bool is_guarded, bool is_structured);

 // Constructors, destructors, etc.
 public:

   // Default constructor.
   JeodMemoryItem();

   // Non-default constructor.
   JeodMemoryItem(
      bool placement_new,
      bool is_array,
      bool is_guarded,
      bool is_structured,
      unsigned int nelems_in,
      unsigned int type_idx,
      unsigned int alloc_idx);

   // Destructor.
   ~JeodMemoryItem();

 // NOTE: The copy constructor and assignment operator are not masked out.
 // The default implementations work just fine here; this is a POD structure.


 // Public accessor functions.
 // Note: Only the unique_id and is_registered flag has a setter; these are
 // not known at creation time.
 // All others members do not have setters; the elements are immutable.
 // Second note: The unique_id can only be set once.
 public:

   // Set the unique id.
   void set_unique_id (uint32_t id);

   // Set the is_registered flag.
   void set_is_registered (bool value);


   // Access the array size.
   uint32_t get_nelems () const;

   // Access the allocation index.
   uint32_t get_alloc_index () const;

   // Access the unique id.
   uint32_t get_unique_id () const;

   // Access the descriptor index.
   uint32_t get_descriptor_index () const;

   // Access the is_array flag.
   bool get_is_array () const;

   // Access the is_guarded flag.
   bool get_is_guarded () const;

   // Access the placement_new flag.
   bool get_placement_new () const;

   // Access the is_structured flag.
   bool is_structured_data () const;

   // Access the is_registered flag.
   bool get_is_registered () const;

   // Access the checkpointed flag.
   bool get_checkpointed () const;



 // Member data.
 // Only 16 bytes!
 private:

   /**
    * Number of elements in the allocated array.
    */
   uint32_t nelems;  //!< trick_units(--)

   /**
    * Allocation information index, max of 2^32-2 tracked locations.
    * The allocation information is a string of the form "file.cc:line#" that
    * indicates where in the code the data was allocated. The underlying string
    * is maintained in the global memory manager's string table.
    */
   uint32_t alloc_info_index; //!< trick_units(--)

   /**
    * Unique identifier, max of 2^32-2 allocations (zero is not used).
    * The unique identifier forms the basis of the item name sent to the
    * simulation engine for this memory item.
    */
   uint32_t unique_id; //!< trick_units(--)

   /**
    * High order bits of the descriptor index.
    * The descriptor_index specifies the type decriptor that describes the data.
    * The underlying descriptor is maintained in the global memory manager's
    * type descriptor table.
    */
   uint16_t descriptor_index_hi; //!< trick_units(--)

   /**
    * Low order bits of the descriptor index.
    */
   uint8_t descriptor_index_lo; //!< trick_units(--)

   /**
    * Flags indicating whether
    * - The data was constructed with default new or placement new
    * - The data was allocated as an array or as a single object
    * - The allocated are guarded
    * - The data is a structured or non-structured data type
    * - The data has been checkpointed (future)
    * - Plus three more future-use spares.
    */
   uint8_t flags; //!< trick_units(--)
};


/**
 * Access the array size.
 * @return Array size
 */
inline uint32_t
JeodMemoryItem::get_nelems (
   void)
const
{
   return nelems;
}


/**
 * Access the unique identifier.
 * @return Unique identifier
 */
inline uint32_t
JeodMemoryItem::get_unique_id (
   void)
const
{
   return unique_id;
}


/**
 * Access the allocation information index.
 * @return Allocation information index
 */
inline uint32_t
JeodMemoryItem::get_alloc_index (
   void)
const
{
   return alloc_info_index;
}


/**
 * Access the type descriptor index.
 * @return Type descriptor index
 */
inline uint32_t
JeodMemoryItem::get_descriptor_index (
   void)
const
{
   return ((static_cast<uint32_t> (descriptor_index_hi)) << 8) +
           (static_cast<uint32_t> (descriptor_index_lo));
}


/**
 * Access the placement_new flag.
 * @return Allocated for placement new?
 */
inline bool
JeodMemoryItem::get_placement_new (
   void)
const
{
   return (flags & PlacementNew) != 0;
}


/**
 * Access the is_array flag.
 * @return Allocated as an array?
 */
inline bool
JeodMemoryItem::get_is_array (
   void)
const
{
   return (flags & IsArray) != 0;
}


/**
 * Access the is_guarded flag.
 * @return Is allocated memory guarded?
 */
inline bool
JeodMemoryItem::get_is_guarded (
   void)
const
{
   return (flags & IsGuarded) != 0;
}


/**
 * Is the associated data a structure/class?
 * @return True for structured data
 */
inline bool
JeodMemoryItem::is_structured_data (
   void)
const
{
   return (flags & IsStructured) != 0;
}


/**
 * Access the checkpointed flag.
 * @return Registered with sim engine?
 */
inline bool
JeodMemoryItem::get_is_registered (
   void)
const
{
   return (flags & IsRegistered) != 0;
}


/**
 * Access the checkpointed flag.
 * @return Checkpointed?
 */
inline bool
JeodMemoryItem::get_checkpointed (
   void)
const
{
   return (flags & CheckPointed) != 0;
}



/**
 * @}
 */


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
