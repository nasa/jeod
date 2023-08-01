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
 * @file models/utils/sim_interface/include/checkpoint_input_manager.hh
 * Define class CheckPointInputManager and related classes.
 */

/*
 PURPOSE: ()
*/


#ifndef JEOD_CHECKPOINTINPUTMANAGER_HH
#define JEOD_CHECKPOINTINPUTMANAGER_HH


// JEOD includes
#include "utils/sim_interface/include/config.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <cstddef>
#include <istream>
#include <fstream>
#include <string>
#include <map>


//! Namespace jeod 
namespace jeod {

class SectionedInputStream;
class CheckPointInputManager;


/**
 A SectionedInputBuffer is a std::streambuf that reads from a section in a
 checkpoint file. This class will indicate EOF when the input pointer in the
 checkpoint file file buffer goes beyond the end of the section.

 This is a barebones implementation. It does not provide buffering, it does
 not support seek and tell, and it does not support putback or unget.

 Note that with the exception of the destructor and the inherited members
 from std::streambuf, *everything* in this class is private.
 This class is not extensible.
 */
class SectionedInputBuffer : public std::streambuf {
friend class SectionedInputStream;

public:

   /**
    Destructor.
    For now, this does nothing.
    */
   ~SectionedInputBuffer() override {}

#ifndef SWIG
   /**
    Conversion to boolean.
    @return False if object is OK.
    */
   bool operator ! () const
   { return file_buf == nullptr; }
#endif

private:

   // Member functions

   // Default constructor
   SectionedInputBuffer (void);

   // Activate the object
   void activate (std::ifstream & stream, std::size_t spos, std::size_t epos);

   /**
    Deactivate the object.
    Used to force a badly behaving stream to disconnect.
    */
   void deactivate (void) {
      file_buf = nullptr;
      at_eof = true;
   }

   // Get a character (future: set of characters) from the file buffer.
   std::streambuf::int_type underflow () override;


   // Member data

   /**
    * The file buffer that reads from the checkpoint file.
    */
   std::filebuf * file_buf; //!< trick_io(**)

   /**
    * The position of the start of the contents of the
    * checkpoint file section being read by this object.
    */
   JEOD_SIZE_T start_pos; //!< trick_io(**)

   /**
    * The position just after the end of the contents of the
    * checkpoint file section being read by this object.
    */
   JEOD_SIZE_T end_pos; //!< trick_io(**)

   /**
    * The current position of the file_buf reader.
    */
   JEOD_SIZE_T curr_pos; //!< trick_io(**)

   /**
    * At EOF in the file or in the section?
    */
   bool at_eof; //!< trick_io(**)

   /**
    * Input buffer.
    */
   char buf; //!< trick_io(**)



   // Nix to the C++ freebie copy constructor and assignment operator.

   /**
    Not implemented.
    */
   SectionedInputBuffer (const SectionedInputBuffer &);

   /**
    Not implemented.
    */
   SectionedInputBuffer & operator= (const SectionedInputBuffer &);
};


/**
 A SectionedInputStream is a std::istream that reads from a section in a
 checkpoint file. This class will indicate EOF when the input pointer in the
 checkpoint file file buffer goes beyond the end of the section.

 This is a barebones implementation. It does not provide buffering, it does
 not support seek and tell, and it does not support putback or unget.

 @par Usage
 A SectionedInputStream object is used in a preload_checkpoint or restart
 job to read and then act on contents stored in a checkpoint file.
 @code
 return_type function_name (
    SomeStructureType & stuff_to_restore)
 {
    std::string section_name;
    double number;
    char c_style_line[256];
    std::string cpp_line;
    char character;
    int char_as_int;

    std::string section_name;
    // Set to name of the checkpoint section

    // Construct a checkpoint input stream.
    // Notes:
    // - This object must go out of scope by the end of the job.
    // - DO NOT make a copy of this object.
    // - DO NOT save a pointer to this object in a permanent structure.
    // - The code below assumes that function_name is called as a
    //   preload_checkpoint or a restart job.
   SectionedInputStream reader (
      JeodSimulationInterface::get_checkpoint_reader(section_name));

   // Activate the reader.
   // Fail to do so and you'll get EOF on the first read.
   reader.activate();

   // You can use the C++ operator >> to read various kinds of data ...
   reader >> number;

   // ... even data structures if the structure has a deserializer.
   reader >> stuff_to_restore;

   // Lines can be read with the getline member or std::getline global.
   reader.getline (c_style_line, 255);
   std::getline (reader, cpp_line);

   // Individual characters can be read in a variety of ways.
   reader >> std::noskipws >> character;
   reader.get (character);
   char_as_int = reader.rdbuf()->sbumpc();

   // A bunch of numbers can be read using operator >>:
   while (!! (reader >> number)) {
      stuff_to_restore.add_number (number);
   }

   // An alternative is to implicitly use operator void*:
   while (reader >> number) {
      stuff_to_restore.add_number (number);
   }

   // The file can be scanned via getline, here using the bang-bang trick:
   while (!! std::getline (reader, cpp_string)) {
      process_line (cpp_string);
   }

   // Same as the above, but implicitly using operator void*:
   while (std::getline (reader, cpp_string)) {
      process_line (cpp_string);
   }

   // The file can be processed a character at a time.
   // Once again, either the bang-bang trick or operator void* can be
   // used to check for EOF.
   while (!! std::get (reader, character)) {
      stuff_to_restore.add_char (character);
   }

   // Yet another alternative is to test for EOF using sbumpc:
   while ((char_as_int = rdbuf->sbumpc()) != EOF) {
      stuff_to_restore.add_char ((char)char_as_int);
   }

   // Or use sgetc/sbumpc if the above grates too much:
   while (reader.rdbuf->sgetc()) != EOF) {
      stuff_to_restore.add_char ((char)reader.rdbuf()->sbumpc());
   }
 }
 @endcode

 @par Diagnosing problems
  - Nothing is being read.
    This can be caused by several problems, described below.
  - Is the JEOD checkpoint file open for input?\n
    Checkpoint file sections can only be read from a JEOD checkpoint file
    that is open for input. In a Trick context, the checkpoint file is only
    open for preload_checkpoint and restart jobs. Reading from a checkpoint
    file in other contexts won't work.
  - Are multiple threads trying to read from the same checkpoint file?\n
    Don't do that. This package is not thread-safe.
  - Have you cached some another active checkpoint reader somewhere?\n
    Don't do that, either. Only one reader can be active at a time.
  - Is the checkpoint file section in the checkpoint file?\n
    You will get a diagnostic message if the section doesn't exist.
  - Is the checkpoint reader viable?\n
    The above problems will result in a non-viable checkpoint reader.
    The method is_activatable() can be called prior to calling activate()
    to check whether the stream is viable.
  - Did you call reader.activate()?\n
    Whether compilers make two different objects in the construction of the
    SectionedInputStream or just one object depends on the compiler and on the
    optimization level. Making the package robustly handle the complexities of
    RVO (return value optimization) was too much for the author of the package.
    The call to reader.activate() is essential.
  - Did the call to reader.activate() work?\n
    The method activate() returns true or false to indicate success or failure.
    While the above code did not check status, doing so is a good idea.
  - Did you call reader.deactivate()?\n
    Don't do that until you are done reading. The call to deactivate() is
    irreversible.
  - Did you mix scanned input with line reading?\n
    As with any other stream, operator >> will mark the stream as failed
    if the operator fails to parse.
 */
class SectionedInputStream : public std::istream {
friend class CheckPointInputManager;
public:

   // Default constructor.
   SectionedInputStream ();

   // Copy constructor.
   SectionedInputStream (const SectionedInputStream &);

   // Destructor.
   ~SectionedInputStream () override;

   // Is the object able to be activated?
   bool is_activatable () const;

   // Activate the object.
   bool activate ();

   // Deactivate the object.
   void deactivate (void);

#ifndef SWIG
   /**
    Conversion to boolean.
    Use the bang-bang trick to determine if the object is OK.
    @return False if object is OK, true if something is wrong.
    */
   bool operator ! () const
   { return (!is_active || !sectbuf || stream == nullptr); }

   /**
    Conversion to void*.
    This method provides an alternative to the bang-bang trick to determine
    if the object is OK.
    @return this pointer (cast to void*) if object is OK, NULL otherwise.
    */
   operator void* () const
   { return (!*this) ? nullptr : const_cast<SectionedInputStream*>(this); }
#endif

private:

   // Construct a SectionedInputStream (non-default constructor).
   SectionedInputStream (
      CheckPointInputManager * mngr,
      std::ifstream & fstream,
      std::size_t spos,
      std::size_t epos);


   // Member data

   /**
    * The std::streambuf that does the reading from the file.
    */
   SectionedInputBuffer sectbuf; //!< trick_io(**)

   /**
    * The input manager that created this object.
    */
   CheckPointInputManager * manager; //!< trick_io(**)

   /**
    * The C++ file stream that reads from the checkpoint file.
    */
   std::ifstream * stream; //!< trick_io(**)

   /**
    * The position of the start of the contents of the
    * checkpoint file section being read by this object.
    */
   JEOD_SIZE_T start_pos; //!< trick_io(**)

   /**
    * The position just after the end of the contents of the
    * checkpoint file section being read by this object.
    */
   JEOD_SIZE_T end_pos; //!< trick_io(**)

   /**
    * Is this a copy of some other SectionedInputStream?
    * Copies of copies are verboten.
    */
   bool is_copy; //!< trick_io(**)

   /**
    * Is this an active object?
    * In the end, there can be only one.
    */
   bool is_active; //!< trick_io(**)


   // Nix to the C++ freebie assignment operator.

   /**
    Not implemented.
    */
   SectionedInputStream & operator= (const SectionedInputStream &);
};


/**
 A CheckPointInputManager provides tools for reading a checkpoint file.
 A Trick 10 checkpoint file comprises multiple sections delineated by
 section markers. This class recognizes those markers and generates C++
 input streams that other objects can use to read the contents of one of
 those checkpoint file sections. The interpretation of the contents of
 a checkpoint file section is the responsibility of those other objects.
 */
class CheckPointInputManager {

public:

   // Non-default constructor.
   CheckPointInputManager (const std::string & fname,
                          const std::string & start_marker,
                          const std::string & end_marker);

   /**
    Create a C++ input stream that reads from a checkpoint file section.
    @par Error handling
    A null SectionedInputStream is created when the CheckPointInputManager
    itself is invalid or when the designated section is not present in the
    checkpoint file.
    @param tag Tag that identifies the section to be read.
    @return    A SectionedInputStream object, which must be used to
              initialize a local SectionedInputStream variable.
    */
   SectionedInputStream create_section_reader (const std::string & tag)
   { return create_section_reader (false, tag); }

#ifndef SWIG
   /**
    Conversion to boolean.
    @return False if object is OK.
    */
   bool operator ! () const
   { return (!is_open || !stream); }
#endif

   /**
    Is there an active checkpoint section reader?
    @return True if there is an active reader, false otherwise.
    */
   bool have_active_reader () const
   { return (current_reader != nullptr); }

   // Denote a reader as *the* currently active reader.
   bool register_reader (SectionedInputStream * reader);

   // Denote a reader as no longer being *the* currently active reader.
   bool deregister_reader (const SectionedInputStream * reader);


private:

   // Types

   /**
    A SectionInfo contains the start and end positions of a checkpoint
    file section.
    */
   struct SectionInfo {
      /**
       * Position of the first readable character of a section.
       */
      JEOD_SIZE_T start_pos; //!< trick_io(**)

      /**
       * Position of the first unreadable character after a section.
       */
      JEOD_SIZE_T end_pos; //!< trick_io(**)

            /**
       * Non-default constructor.
       * \param[in] start Start position
       * \param[in] end End position
       */
      SectionInfo (
         std::size_t start,
         std::size_t end)
      : start_pos(start), end_pos(end) {}
   };


   // Member functions

   // Record locations of section markers.
   void initialize (void);

   // Create a C++ input stream that reads from a checkpoint file section.
   SectionedInputStream create_section_reader (
      bool trick,
      const std::string & tag);

   // Create a C++ input stream that reads the Trick checkpoint file section.
   SectionedInputStream create_trick_section_reader (); //cppcheck-suppress unusedPrivateFunction


   // Member data

   /**
    * Maps section names to section start/end positions.
    */
   std::map<std::string, SectionInfo> sections; //!< trick_io(**)

   /**
    * The C++ file stream that reads the checkpoint file.
    */
   std::ifstream stream; //!< trick_io(**)

   /**
    * The reader that currently is active.
    */
   SectionedInputStream * current_reader; //!< trick_io(**)

   /**
    * The name of the checkpoint file.
    */
   const std::string filename;

   /**
    * The string that indicates the start of a checkpoint file section.
    */
   const std::string & section_start;

   /**
    * The string that indicates the start of a checkpoint file section.
    */
   const std::string & section_end;

   /**
    * Is the checkpoint file open?
    */
   bool is_open; //!< trick_io(**)



   // Nix to the C++ freebie copy constructor and assignment operator.

   /**
    Not implemented.
    */
   CheckPointInputManager (const CheckPointInputManager &);

   /**
    Not implemented.
    */
   CheckPointInputManager & operator= (const CheckPointInputManager &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
