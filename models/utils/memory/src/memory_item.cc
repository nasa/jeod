/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/src/memory_item.cc
 * Implement the JeodMemoryItem class.
 */

/*******************************************************************************

Purpose:
  ()

 
*******************************************************************************/


/**
 * \addtogroup classes
 * @{
 */

// System includes

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/memory_item.hh"
#include "../include/memory_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct the flags for a new JeodMemoryItem.
 * @return Constructed flags
 * \param[in] placement_new Constructed with placement new?
 * \param[in] is_array Constructed with new []?
 * \param[in] is_guarded Is the item an array?
 * \param[in] is_structured Is the item a structured data type?
 */
uint8_t
JeodMemoryItem::construct_flags (
   bool placement_new,
   bool is_array,
   bool is_guarded,
   bool is_structured)
{
   uint8_t flags = 0;

   if (placement_new) {
      flags |= JeodMemoryItem::PlacementNew;
   }

   if (is_array) {
      flags |= JeodMemoryItem::IsArray;
   }

   if (is_guarded) {
      flags |= JeodMemoryItem::IsGuarded;
   }

   if (is_structured) {
      flags |= JeodMemoryItem::IsStructured;
   }

   return flags;
}


/**
 * Construct a JeodMemoryItem.
 * This default constructor generates meaningless values.
 * The intent is that this is to be used in conjunction with a copy.
 */
JeodMemoryItem::JeodMemoryItem (
   void)
:
   nelems (0),
   alloc_info_index (0),
   unique_id (0),
   descriptor_index_hi (0),
   descriptor_index_lo (0),
   flags (0)
{
   ; // Empty
}


/**
 * Construct a JeodMemoryItem.
 * The data are essentially constant, so the only viable way to set elements
 * to meaningful values is via this non-default constructor.
 * \param[in] placement_new Constructed with placement new?
 * \param[in] is_array Constructed with new []?
 * \param[in] is_guarded Is the item an array?
 * \param[in] is_structured Is the item a structured data type?
 * \param[in] nelems_in Number of elements
 * \param[in] type_idx Type descriptor (index)
 * \param[in] alloc_idx Macro invocation info (index)
 */
JeodMemoryItem::JeodMemoryItem (
   bool placement_new,
   bool is_array,
   bool is_guarded,
   bool is_structured,
   unsigned int nelems_in,
   unsigned int type_idx,
   unsigned int alloc_idx)
:
   nelems (nelems_in),
   alloc_info_index (alloc_idx),
   unique_id (0),
   descriptor_index_hi (static_cast<uint16_t> (type_idx >> 8)),
   descriptor_index_lo (static_cast<uint8_t> (type_idx & 0xff)),
   flags (construct_flags (placement_new, is_array, is_guarded, is_structured))
{
   ; // Empty
}


/**
 * Destruct a JeodMemoryItem.
 */
JeodMemoryItem::~JeodMemoryItem (void)
{
   ; // Empty
}


/**
 * Set the unique identifier.
 * \param[in] id Unique identifier
 */
void
JeodMemoryItem::set_unique_id (
   uint32_t id)
{
   if (unique_id != 0) {
      MessageHandler::error (
         __FILE__, __LINE__, MemoryMessages::internal_error,
         "Illegal attempt to reset a memory item's unique identifier.");
      return;
   }

   if (id == 0) {
      MessageHandler::error (
         __FILE__, __LINE__, MemoryMessages::internal_error,
         "The unique identifier must be positive.");
      return;
   }

   unique_id = id;
}


/**
 * Set the is_registered flag.
 * \param[in] value New value of the is_registered flag
 */
void
JeodMemoryItem::set_is_registered (
   bool value)
{
   if (value) {
      flags |= IsRegistered;
   }
   else {
      flags &= ~IsRegistered & 0xff;
   }
}


} // End JEOD namespace

/**
 * @}
 */

/**
 * @}
 * @}
 * @}
 */
