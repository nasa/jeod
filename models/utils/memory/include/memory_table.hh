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
 * @file models/utils/memory/include/memory_table.hh
 * Define classes for representing data types.
 */

/*******************************************************************************

Purpose:
   ()

 

*******************************************************************************/

#ifndef JEOD_MEMORY_TABLE_HH
#define JEOD_MEMORY_TABLE_HH

/**
 * \addtogroup classes
 * @{
 */

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>


//! Namespace jeod
namespace jeod {

/**
 * A JeodMemoryTable maps strings to values with a coordinated map/vector pair.
 * \tparam ValueType  The underlying type of the values maintained in the table.
 *                    The stored values are pointers to this underlying type.
 *
 * \details
 * A JeodMemoryTable contains two data members: a std::map and a std::vector.
 * The map data member maps keys to integers. The integer mapped by a key is the
 * index into the vector where the value associated with the key is stored.
 *
 * So why not just use a map? The reason is that storing an integer requires
 * less memory than storing a string or a pointer to a string, particularly on
 * 64 bit machines. In the application at hand, keeping track of memory
 * allocations, the number of data types is relatively small compared to the
 * to the number of allocated chunks of data. The extra overhead of maintaining
 * a map and a vector is small compared to the savings that results from storing
 * thousands of integers rather than pointers or strings.
 *
 * \par Principal Operations
 *  - add()\n
 *    Returns the integer value associated with a key in the table's map.
 *    In the case of a new key/value pair, a new key/vector size entry is added
 *    to the map and the value is added to the end of the vector.
 *    Note well: The value is ignored when the key is already in the map.
 *  - del()\n
 *    Deletes the key from the table's map and deletes the cloned value at the
 *    corresponding index. The vector itself is modified (truncated) only in
 *    the special case of deleting the last-added entry. This ensures that
 *    stored indices will remain valid.
 *  - get()\n
 *    Returns the value in the table's vector at the specified index.
 *
 * \par Assumptions and Limitations
 *  - The value is ignored for duplicate key entries. The underlying assumption
 *    is that all of the values for those duplicate entries are somehow
 *    equal to one another.
 *  - As-is, the table is not thread-safe. Calls to add() and get() made in a
 *    multi-threaded environment should be protected by a mutex. This protection
 *    is the responsibility of the (programmatic) users.
 *  - JEOD reserves index 0 for internal use. Valid indices are positive.
 *  - The del() method should be used only if the (programmatic) user @e knows
 *    that no other references to the to-be-deleted entry exist.
 */
template <typename ValueType>
class JeodMemoryTable {

 // Types
 public:

   /**
    * Maps strings to an index number.
    */
   typedef std::map <const std::string, unsigned int> NameIndex;

   /**
    * Maps index numbers to key values.
    */
   typedef std::vector <const ValueType *> ValueList;

   /**
    * Const iterator over values.
    */
   typedef typename ValueList::const_iterator const_value_iterator;



 // Constructors, destructors, etc.
 public:

   /**
    * Default constructor.
    * Note that JEOD reserves table index 0 as meaning nothing.
    */
   JeodMemoryTable ()
   :
      string_to_index(),
      value_list()
   {
      // Reserve the zero index by pushing a null pointer on the vector.
      value_list.push_back (NULL);
   }

   /**
    * Destructor.
    * The contents of the vector are clones created by add() and hence must
    * be deleted to avoid a leak.
    */
   virtual ~JeodMemoryTable ()
   {
      // Delete the contents of the identifier list.
      // Note that element zero is null, so loop starts at 1.
      for (unsigned int ii = 1; ii < value_list.size(); ++ii) {
         if (value_list[ii] != NULL) {
            delete value_list[ii];
            value_list[ii] = NULL;
         }
      }

      string_to_index.clear();
      value_list.clear();
   }

 // The copy constructor and assignment operator are private and unimplemented.
 private:

   /**
    * Not implemented.
    */
   explicit JeodMemoryTable (const JeodMemoryTable &);

   /**
    * Not implemented.
    */
   JeodMemoryTable & operator = (const JeodMemoryTable &);


 // Implemented interfaces
 public:

   /**
    * Find the index number at which key/value pair is stored in the table.
    * \return         Index number mapped by the key
    * \param[in] key  Key
    */
   unsigned int find (const std::string & key) const
   {
      unsigned int found_idx;

      // Find the key in the map.
      NameIndex::const_iterator iter(string_to_index.find (key));

      // Not found: Set the found index to zero to indicate so.
      // (The zeroth element is reserved just for this purpose.)
      if (iter == string_to_index.end()) {
         found_idx = 0;
      }

      // Found: The mapped index is the return value.
      else {
         found_idx = iter->second;
      }

      return found_idx;
   }


   /**
    * Returns a const iterator that points to the first element of the list.
    */
   const_value_iterator begin (void) const
   {
      return value_list.begin()+1;
   }


   /**
    * Returns a const iterator that points past the last element of the list.
    */
   const_value_iterator end (void) const
   {
      return value_list.end();
   }


   /**
    * Add a key/value pair to the table.
    * \return         Index number mapped by the key
    * \param[in] key  Key
    * \param[in] val  Value
    */
   unsigned int add (const std::string & key, const ValueType & val)
   {
      unsigned int found_idx;

      // Find the key in the map.
      NameIndex::iterator iter(string_to_index.find (key));

      // Not found: Create new entries in both the map and the vector.
      if (iter == string_to_index.end()) {
         found_idx = value_list.size();
         string_to_index.insert (NameIndex::value_type(key, found_idx));
         value_list.push_back (clone (val));
      }

      // Found: The mapped index is the return value.
      else {
         found_idx = iter->second;
      }

      return found_idx;
   }

   /**
    * Delete the key and associated data from the table.
    * Use with care.
    * \param[in] key  Key
    * \throw std::invalid_argument on attempting to delete an element that
    *    is not in the table.
    */
   void del (const std::string & key)
   {
      // Find the key in the map.
      NameIndex::iterator iter(string_to_index.find (key));

      // Found: Delete. Note: This assumes something is in value_list[idx].
      if (iter != string_to_index.end()) {
         unsigned int idx = iter->second;
         delete value_list[idx];
         value_list[idx] = NULL;

         // Deleting last element added: Shrink the vector.
         if (idx == value_list.size() - 1) {
            value_list.pop_back ();
         }

         // Finally, delete the key/index pair from the map.
         string_to_index.erase (iter);
      }

      // Not found: (programmatic) user error.
      else {
         throw std::invalid_argument ("key not in table");
      }
   }

   /**
    * Retrieve the value for the specified index from the list.
    * \return Value for specified index.
    * \param[in] idx Table index whose value is to be retrieved.
    * \throw std::out_of_range for an index of zero or for an index beyond
    *   the range of the vector.
    * \throw std::invalid_argument when the index is in range but the
    *   value is null. This only happens when the item in question has
    *   previously been deleted.
    */
   const ValueType * get (unsigned int idx) const
   {
      const ValueType * result;

      if ((idx == 0) || (idx >= value_list.size())) {
         throw std::out_of_range ("invalid index");
      }

      result = value_list[idx];

      if (result == NULL) {
         throw std::invalid_argument ("deleted entry");
      }

      return result;
   }


 // Virtual interfaces
 protected:

   /**
    * (Somehow) clone the input value.
    * \return           Clone of input value.
    * \param[in] value  Value to be cloned.
    */
   virtual const ValueType * clone (const ValueType & value) const = 0;


 // Member data
 private:

   /**
    * Maps keys to indices in the value_list.
    */
   NameIndex string_to_index; //!< trick_io(**)

   /**
    * Vector of values.
    */
   ValueList value_list;      //!< trick_io(**)
};



/**
 * A JeodMemoryTableClonable is a JeodMemoryTable that implements the
 * required clone() functionality by invoking the @a ValueType's clone()
 * method to create a clone of the input value.
 */
template <typename ValueType>
class JeodMemoryTableClonable : public JeodMemoryTable<ValueType> {

 // Constructors, destructors, etc.
 public:

   /**
    * Default constructor.
    */
   JeodMemoryTableClonable () : JeodMemoryTable<ValueType>() { }


 // The copy constructor and assignment operator are private and unimplemented.
 private:

   /**
    * Not implemented.
    */
   explicit JeodMemoryTableClonable (const JeodMemoryTableClonable &);

   /**
    * Not implemented.
    */
   JeodMemoryTableClonable & operator = (const JeodMemoryTableClonable &);


 // Required extension to JeodMemoryTable
 protected:

   /**
    * Creates a copy of the input value by invoking its clone method.
    * \return Duplicate of input value.
    * \param[in] value  Value to be cloned.
    */
   virtual const ValueType * clone (const ValueType & value) const
   {
      return value.clone();
   }

};


/**
 * A JeodMemoryTableCopyable is a JeodMemoryTable that implements the
 * required clone() functionality by invoking the @a ValueType's copy
 * constructor to create a clone of the input value.
 */
template <typename ValueType>
class JeodMemoryTableCopyable : public JeodMemoryTable<ValueType> {

 // Constructors, destructors, etc.
 public:

   /**
    * Default constructor.
    */
   JeodMemoryTableCopyable () : JeodMemoryTable<ValueType>() { }

 // The copy constructor and assignment operator are private and unimplemented.
 private:

   /**
    * Not implemented.
    */
   explicit JeodMemoryTableCopyable (const JeodMemoryTableCopyable &);

   /**
    * Not implemented.
    */
   JeodMemoryTableCopyable & operator = (const JeodMemoryTableCopyable &);


  // Required extension to JeodMemoryTable
  protected:

   /**
    * Creates a copy of the input value by invoking its copy constructor.
    * \return Duplicate of input value.
    * \param[in] value  Value to be cloned.
    */
   virtual const ValueType * clone (const ValueType & value) const
   {
      return new ValueType (value);
   }

};


/**
 * A JeodMemoryReflectiveTable maps strings to themselves.
 */
class JeodMemoryReflectiveTable : public JeodMemoryTableCopyable<std::string> {

 // Constructors, destructors, etc.
 public:

   /**
    * Default constructor.
    */
   JeodMemoryReflectiveTable () : JeodMemoryTableCopyable<std::string>()
   {
      ;
   }

 // The copy constructor and assignment operator are private and unimplemented.
 private:

   /**
    * Not implemented.
    */
   explicit JeodMemoryReflectiveTable (const JeodMemoryReflectiveTable &);

   /**
    * Not implemented.
    */
   JeodMemoryReflectiveTable & operator = (const JeodMemoryReflectiveTable &);


 // Hide the two argument add() method.
 private:

   /**
    * Not implemented.
    */
   unsigned int add (const std::string & key, const std::string & val);


 public:
   /**
    * Add a key to the table. A reflective table has values equal to keys.
    * \return           Index number mapped by the key.
    * \param[in] keyval Key (and value) to be added to the table.
    */
   unsigned int add (
      const std::string & keyval)
   {
      return JeodMemoryTableCopyable<std::string>::add (keyval, keyval);
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
