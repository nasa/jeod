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
 * @addtogroup NamedItem
 * @{
 *
 * @file models/utils/named_item/include/named_item.hh
 * Define the NamedItem utility class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/named_item.cc)
   (../src/named_item_demangle.cc))

 
*******************************************************************************/


#ifndef JEOD_NAMED_ITEM_HH
#define JEOD_NAMED_ITEM_HH


// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <cstdarg>              // Variable arguments
#include <string>               // std::string
#include <typeinfo>             // std::type_info
#include <utility>              // std::forward, std::move
#include <vector>               // std::vector

//! Namespace jeod
namespace jeod {

/**
 * Provides a set of static methods for constructing dot-conjoined names.
 * The methods defined in this class allocate memory and do not release it.
 * Releasing that memory is the responsibility of the calling function.
 * Use the macro JEOD_DELETE_ARRAY to release this memory.
 *
 * Prior to JEOD 4.0, the NamedItem class was not instantiable. It is in
 * JEOD 4.0. The NamedItem class forms the basis of a thing with a name,
 * with the name being a std::string. The construct_name functions and
 & related functions that allocate a C-style string are deprecated.
 */
class NamedItem {

   JEOD_MAKE_SIM_INTERFACES(NamedItem)

public:

   /**
    * The size type used in std::string.
    */
   using size_type = std::string::size_type;


   /**
    * Create a copy of the provided name.
    * @return The constructed name
    * \param[in] name_item1 First part of the name
    */
   static char * construct_name (const char * name_item1)
   {
      return vconstruct_name (name_item1,
                              (const char *)NULL);
   }

   /**
    * Construct a name as a dot-conjoined string.
    * @return The constructed name
    * \param[in] name_item1 First part of the name
    * \param[in] name_item2 Second part of the name
    */
   static char * construct_name (
      const char * name_item1,
      const char * name_item2)
   {
      return vconstruct_name (name_item1,
                              name_item2,
                              (const char *)NULL);
   }

   /**
    * Construct a name as a dot-conjoined string.
    * @return The constructed name
    * \param[in] name_item1 First part of the name
    * \param[in] name_item2 Second part of the name
    * \param[in] name_item3 Third part of the name
    */
   static char * construct_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3)
   {
      return vconstruct_name (name_item1,
                              name_item2,
                              name_item3,
                              (const char *)NULL);
   }

   /**
    * Construct a name as a dot-conjoined string.
    * @return The constructed name
    * \param[in] name_item1 First part of the name
    * \param[in] name_item2 Second part of the name
    * \param[in] name_item3 Third part of the name
    * \param[in] name_item4 Fourth part of the name
    */
   static char * construct_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4)
   {
      return vconstruct_name (name_item1,
                              name_item2,
                              name_item3,
                              name_item4,
                              (const char *)NULL);
   }

   /**
    * Construct a name as a dot-conjoined string.
    * @return The constructed name
    * \param[in] name_item1 First part of the name
    * \param[in] name_item2 Second part of the name
    * \param[in] name_item3 Third part of the name
    * \param[in] name_item4 Fourth part of the name
    * \param[in] name_item5 Fifth part of the name
    */
   static char * construct_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4,
      const char * name_item5)
   {
      return vconstruct_name (name_item1,
                              name_item2,
                              name_item3,
                              name_item4,
                              name_item5,
                              (const char *)NULL);
   }

   /**
    * Construct a name as a dot-conjoined string.
    * @return The constructed name
    * \param[in] name_item1 First part of the name
    * \param[in] name_item2 Second part of the name
    * \param[in] name_item3 Third part of the name
    * \param[in] name_item4 Fourth part of the name
    * \param[in] name_item5 Fifth part of the name
    * \param[in] name_item6 Sixth part of the name
    */
   static char * construct_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4,
      const char * name_item5,
      const char * name_item6)
   {
      return vconstruct_name (name_item1,
                              name_item2,
                              name_item3,
                              name_item4,
                              name_item5,
                              name_item6,
                              (const char *)NULL);
   }

   /**
    * Construct a name as a dot-conjoined string.
    * @return The constructed name
    * \param[in] name_item1 First part of the name
    * \param[in] name_item2 Second part of the name
    * \param[in] name_item3 Third part of the name
    * \param[in] name_item4 Fourth part of the name
    * \param[in] name_item5 Fifth part of the name
    * \param[in] name_item6 Sixth part of the name
    * \param[in] name_item7 Seventh part of the name
    */
   static char * construct_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4,
      const char * name_item5,
      const char * name_item6,
      const char * name_item7)
   {
      return vconstruct_name (name_item1,
                              name_item2,
                              name_item3,
                              name_item4,
                              name_item5,
                              name_item6,
                              name_item7,
                              (const char *)NULL);
   }


   /**
    * Construct a name as a dot-conjoined string.
    * Note that this is a varargs function. The last argument must be
    * (const char *)NULL to signal the end of the argument list.
    * @return The constructed name
    * \param[in] name_item First part of the name
    * \param[in] ... Rest of the name
    */
   static char * vconstruct_name (const char * name_item, ...);

#ifndef SWIG
   /**
    * Construct a name as a dot-conjoined string.
    * Notes --
    * - This function takes a va_list argument that contains any
    *   additional strings to be appended.
    * - The calling function must form the args argument by invoking
    *   va_start().
    * - The calling function should not invoke va_end(); this is done
    *   inside va_construct_name().
    * - The last argument embodied in the args argument must be a NULL
    *   to signal the end of the argument list.
    * @return The constructed name
    * \param[in] name_item First part of the name
    * \param[in] args Rest of the name
    */
   static char * va_construct_name (const char * name_item, va_list args);
#endif


   /**
    * Given a prefix and a dot-conjoined name, find the part of the name
    * that follows the prefix. For names of the form "prefix.suffix",
    * this function returns a pointer to "suffix". The function returns
    * the input name if the name does not start with "prefix.".
    * @return Suffix
    * \param[in] prefix Prefix
    * \param[in] name Name, possibly prefixed
    */
   static const char * suffix (const char * prefix, const char * name);


   /**
    * Demangle a C++ name.
    * @return Demangled name
    * \param[in] info Typeinfo to be demangled
    */
   static const std::string demangle (const std::type_info & info);


   /**
    * Checks whether a name is trivially invalid, failing if it is.
    * \param[in] file Usually __FILE__
    * \param[in] line Usually __LINE__
    * \param[in] variable_value Value to check
    * \param[in] variable_type Variable description
    * \param[in] variable_name Variable name
    */
   static void validate_name (
      const char * file,
      unsigned int line,
      const char * variable_value,
      const char * variable_type,
      const char * variable_name);


  /**
   * Construct a name from the given input, as a string.
   * The input must not be the empty string or the null pointer.
   * @tparam Arg  Type of the argument to construct_name_string.
   * @param arg  Argument to construct_name_string.
   * @return std::string that is conceptually equal to (==) arg.
   */
   template <typename Arg>
   static std::string construct_name_string (Arg&& arg)
   {
      std::string result { std::forward<Arg>(arg) };
      validate_name (__FILE__, __LINE__, result.c_str(), "Label", "arg");
      return result;
   }

  /**
   * Construct a name as a dot-conjoined string of the given inputs.
   * Each input must not be the empty string or the null pointer.
   * @tparam First  Type of the first argument to construct_name_string.
   * @tparam Rest  Types of the remaining arguments to construct_name_string.
   * @param first  First argument to construct_name_string.
   * @param rest  Remaining arguments to construct_name_string.
   * @return The given inputs as a dot-conjoined string.
   */
   template <typename First, typename... Rest>
   static std::string construct_name_string (First&& first, Rest&&... rest)
   {
      std::string result { std::forward<First>(first) };
      validate_name (__FILE__, __LINE__, result.c_str(), "Label", "arg");
      std::vector<std::string> rest_vec { { std::forward<Rest>(rest)... } };
      for (auto& elem : rest_vec)
      {
         result += '.';
         validate_name (__FILE__, __LINE__, elem.c_str(), "Elem", "elem");
         result += elem;
      }
      return result;
   }


   /**
    * Default constructor.
    * This is the default constructor by virtue of the defaults.
    * @param name_in  Initial value of the name, defaults to the empty string.
    * @param frozen_in  Initial value of is_frozen, defaults to false.
    */
   NamedItem (std::string name_in=std::string(), bool frozen_in=false)
   :
      name {std::move(name_in)},
      is_frozen {frozen_in}
   { }

   /**
    * Copy constructor. The default implementation works fine.
    */
   NamedItem (const NamedItem&) = default;

#ifndef SWIG
   /**
    * Move constructor. The default implementation works fine.
    */
   NamedItem (NamedItem&&) = default;
#endif

   /**
    * Destructor. The default implementation virtually works fine.
    */
   virtual ~NamedItem() = default;

#ifdef SWIG
   NamedItem& operator= (const NamedItem&) = delete;
   NamedItem& operator= (const NamedItem&&) = delete;
#else
   /**
    * Copy assignment.
    * Only the name is copied, and only if the name isn't frozen.
    */
   NamedItem& operator= (const NamedItem& src)
   {
      verify_unfrozen_name ();
      name = src.name;
      return *this;
   }

   /**
    * Move assignment. The default implementation works fine.
    */
   NamedItem& operator= (NamedItem&& src)
   {
      verify_unfrozen_name ();
      name = std::move(src.name);
      return *this;
   }
#endif

   /**
    * Assignment from a string.
    */
   NamedItem& operator= (const std::string& name_in)
   {
      verify_unfrozen_name ();
      name = name_in;
      return *this;
   }

   /**
    * Comparison of names
    */
   bool operator== (const NamedItem& rhs)
   {
      return (name == rhs.get_name());
   }

   /**
    * Getter for name.
    */
   const std::string& get_name () const
   {
      return name;
   }

   /**
    * Getter for name, as a C-style string.
    */
   const char* c_str () const
   {
      return name.c_str();
   }

   /**
    * Getter for the length of the name.
    */
   size_type size () const
   {
      return name.size();
   }

   /**
    * Getter for is_frozen.
    */
   bool get_is_frozen () const
   {
      return is_frozen;
   }

   /**
    * Compare the end of this string to a C-style string.
    * See std::string::compare.
    * @param pos1 The start index in the name.
    * @param other The C-style null-terminated string.
    * @return True if the end part of the name equals the given C-style string.
    */
   bool ends_with (size_type pos1, const char* other) const
   {
      return name.compare(pos1, name.size()-pos1, other) == 0;
   }

   /**
    * Given a dot-conjoined test name, find the part of the test name
    * that follows this name, as a prefix. For names of the form "prefix.suffix",
    * this function returns a pointer to "suffix". The function returns
    * the input name if the name does not start with "prefix.".
    * @return Suffix
    * \param[in] test_name Test name, possibly prefixed
    */
   const char * suffix (const char * test_name) const
   {
      return suffix (name.c_str(), test_name);
   }

  /**
   * Set the name from the given input, as a string.
   * The input must not be the empty string or the null pointer.
   * @tparam Arg  Type of the argument to construct_name_string.
   * @param arg  Argument to construct_name_string.
   */
   template <typename Arg>
   void set_name (Arg&& arg)
   {
      verify_unfrozen_name ();
      name = std::move(construct_name_string(std::forward<Arg>(arg)));
   }

  /**
   * Set the name as a dot-conjoined string of the given inputs.
   * Each input must not be the empty string or the null pointer.
   * @tparam First  Type of the first argument to construct_name_string.
   * @tparam Rest  Types of the remaining arguments to construct_name_string.
   * @param first  First argument to construct_name_string.
   * @param rest  Remaining arguments to construct_name_string.
   */
   template <typename First, typename... Rest>
   void set_name (First&& first, Rest&&... rest)
   {
      verify_unfrozen_name ();
      name = std::move(construct_name_string(
         std::forward<First>(first), std::forward<Rest>(rest)...));
   }

   /**
    * Verify that the name is not frozen.
    */
   void verify_unfrozen_name () const;

   /**
    * Checks whether a name is trivially invalid, failing if it is.
    * \param[in] file Usually __FILE__
    * \param[in] line Usually __LINE__
    * \param[in] variable_type Variable description
    * \param[in] variable_name Variable name
    */
   void validate_name (
      const char * file,
      unsigned int line,
      const char * variable_type,
      const char * variable_name)
   {
       validate_name (file, line, name.c_str(), variable_type, variable_name);
   }

   /**
    * Freeze the name -- i.e., denote that the name as no longer settable.
    */
   void freeze_name ()
   {
      is_frozen = true;
   }


protected:

   /**
    * Unfreeze the name -- i.e., denote that the name is now settable.
    * This exists solely to parallel freeze_name().
    */
   void unfreeze_name ()
   {
      is_frozen = false;
   }


private:

   /**
    * The item's name.
    */
   std::string name; //!< trick_units(--)

   /**
    * Indicates whether the name is frozen.
    */
   bool is_frozen; //!< trick_units(--)

};

} // End JEOD namespace

/**
 * Comparison to string
 */
bool operator== (const jeod::NamedItem& lhs, const std::string& rhs);
bool operator== (const std::string& lhs, const jeod::NamedItem& rhs);

#endif

/**
 * @}
 * @}
 * @}
 */
