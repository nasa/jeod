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
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/checkpoint_output_manager.hh
 * Define class CheckPointOutputManager and related classes.
 */

/*
 PURPOSE: ()
*/


#ifndef JEOD_CHECKPOINTOUTPUTMANAGER_HH
#define JEOD_CHECKPOINTOUTPUTMANAGER_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <ostream>
#include <fstream>
#include <string>
#include <map>


//! Namespace jeod 
namespace jeod {

class SectionedOutputStream;
class CheckPointOutputManager;
class MemoryManagerWrapper;


/**
 A SectionedOutputBuffer is a std::streambuf that writes a section of a
 checkpoint file.

 This is a barebones implementation. It does not provide buffering, and it does
 not support seek and tell.

 Note that with the exception of the destructor and the inherited members
 from std::streambuf, *everything* in this class is private.
 This class is not extensible.
 */
class SectionedOutputBuffer : public std::streambuf {
friend class SectionedOutputStream;

public:

   /**
    Destructor.
    For now, this does nothing.
    */
   ~SectionedOutputBuffer() {}

#ifndef SWIG
   /**
    Conversion to boolean.
    @return False if object is OK.
    */
   bool operator ! () const
   { return ((file_buf == NULL) || !file_buf); }
#endif

private:

   // Default constructor.
   SectionedOutputBuffer (void);

   // Non-default constructor.
   explicit SectionedOutputBuffer (std::ofstream * stream);

   // Activate the object.
   void activate (std::ofstream & stream);

   /**
    Deactivate the object.
    Used to disconnect the buffer when the stream is done, sometimes by force.
    */
   void deactivate (void) {
      if (!!*this) {
         this->sync();
      }
      file_buf = NULL;
   }

   // Write a character to the file when the output buffer overflows.
   virtual std::streambuf::int_type overflow (std::streambuf::int_type c);


   // Member data

   /**
    * The file buffer that writes to the checkpoint file.
    */
   std::filebuf * file_buf; //!< trick_io(**)


   // Deleted contents

   /**
    Not implemented.
    */
   SectionedOutputBuffer (const SectionedOutputBuffer &);

   /**
    Not implemented.
    */
   SectionedOutputBuffer & operator= (const SectionedOutputBuffer &);

};


/**
 A SectionedOutputStream is a std::ostream that writes a section of a
 checkpoint file. This class automatically writes the start and end markers.
 Standard C++ output mechanisms can be used to write the contents of the
 section.

 This is a barebones implementation. It does not provide buffering, it does
 not support seek and tell, and it does not support putback or unget.

 Note that most of the content of this class is private.
 This class is not extensible and is intended to be used within the
 context of a CheckPointOutputManager.
 */
class SectionedOutputStream : public std::ostream {
friend class CheckPointOutputManager;
public:

   // Default constructor.
   SectionedOutputStream ();

   // Copy constructor.
   SectionedOutputStream (const SectionedOutputStream &);

   // Destructor.
   ~SectionedOutputStream ();

   // Is the object able to be activated?
   bool is_activatable () const;

   // Activate the object.
   bool activate ();

   // Deactivate the object.
   void deactivate ();

#ifndef SWIG
   /**
    Conversion to boolean.
    @return False if object is OK.
    */
   bool operator ! () const
   { return (!is_active || (stream == NULL) || !stream || !sectbuf); }

   /**
    Conversion to void*.
    This method provides an alternative to the bang-bang trick to determine
    if the object is OK.
    @return this pointer (cast to void*) if object is OK, NULL otherwise.
    */
   operator void* () const
   { return (!*this) ? NULL : const_cast<SectionedOutputStream*>(this); }
#endif


private:

   // Construct a SectionedOutputStream (non-default constructor).
   SectionedOutputStream (CheckPointOutputManager * mngr,
                         std::ofstream & ofstream,
                         const std::string & start_marker,
                         const std::string & end_marker,
                         const std::string & section_name);


   // Member data

   /**
    * The std::streambuf that does the writing to the file.
    */
   SectionedOutputBuffer sectbuf; //!< trick_io(**)

   /**
    * The input manager that created this object.
    */
   CheckPointOutputManager * manager; //!< trick_io(**)

   /**
    * The C++ file stream that writes to the checkpoint file.
    */
   std::ofstream * stream; //!< trick_io(**)

   /**
    * The string that indicates the start of a checkpoint file section.
    */
   const std::string * section_start;

   /**
    * The string that indicates the start of a checkpoint file section.
    */
   const std::string * section_end;

   /**
    * The name of the checkpoint file section.
    */
   const std::string tag;

   /**
    * Is this a copy of some other SectionedOutputStream?
    * Copies of copies are verboten.
    */
   bool is_copy; //!< trick_io(**)

   /**
    * Is this an active object?
    * In the end, there can be only one.
    */
   bool is_active; //!< trick_io(**)


   // Deleted contents

   /**
    Not implemented.
    */
   SectionedOutputStream & operator= (const SectionedOutputStream &);
};


/**
 A CheckPointOutputManager provides the basic tools for writing a checkpoint
 file. Section markers split a Trick 10 checkpoint file into multiple parts.
 This class generates C++ output streams that write the section markers and
 that other objects can use to write checkpoint file section data.
 */
class CheckPointOutputManager {
friend class MemoryManagerWrapper;

public:

   // Non-default constructor.
   CheckPointOutputManager (const std::string & fname,
                           const std::string & start_marker,
                           const std::string & end_marker);

   /**
    Create a C++ output stream that writes a checkpoint file section.
    @return Constructed SectionedOutputStream.
   */
   SectionedOutputStream create_section_writer (const std::string & tag)
   { return create_section_writer (false, tag); }

#ifndef SWIG
   /**
    Conversion to boolean.
    @return False if object is OK.
    */
   bool operator ! () const
   { return (!is_open || !stream); }
#endif

   /**
    Is there an active checkpoint section writer?
    @return True if there is an active writer, false otherwise.
   */
   bool have_active_writer () const
   { return (current_writer != NULL); }

   // Denote a writer as *the* currently active writer.
   bool register_writer (SectionedOutputStream * writer);

   // Denote a writer as no longer being *the* currently active writer.
   bool deregister_writer (SectionedOutputStream * writer);

private:

   // Member functions

   // Create a C++ output stream that writes a checkpoint file section.
   SectionedOutputStream create_section_writer (
      bool trick,
      const std::string & tag);

   /**
    Create a C++ output stream that writes a Trick checkpoint file section.
    @return  A SectionedOutputStream object, which must be used to
             initialize a local SectionedOutputStream variable.
    */
   SectionedOutputStream create_trick_section_writer ();


   // Member data

   /**
    * The C++ file stream that writes to the checkpoint file.
    */
   std::ofstream stream; //!< trick_io(**)

   /**
    * The writer that currently is active.
    */
   SectionedOutputStream * current_writer; //!< trick_io(**)

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


   // Deleted contents.

   /**
    Not implemented.
    */
   CheckPointOutputManager (const CheckPointOutputManager &);

   /**
    Not implemented.
    */
   CheckPointOutputManager & operator= (const CheckPointOutputManager &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
