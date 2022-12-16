/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/checkpoint_output_manager.cc
 * Define CheckPointOutputManager member functions and of related classes.
 */

/*
 PURPOSE:
   ()
*/


// System includes
#include <cstddef>
#include <cstring>
#include <iostream>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/checkpoint_output_manager.hh"
#include "../include/sim_interface_messages.hh"



//! Namespace jeod 
namespace jeod {

/**
 * Default constructor.
 *
 * This constructor creates an empty SectionedOutputBuffer -- one that
 * will return EOF on the first write attempt.
 * An empty SectionedOutputBuffer has two purposes:
 * - As the basis for a copy constructor of a containing stream, and
 * - As a graceful means of handling of erroneous conditions.
 */
SectionedOutputBuffer::SectionedOutputBuffer ()
:
   std::streambuf(),
   file_buf(NULL)
{
   setp (0,0);
}


/**
 * Activate the object.
 * @note
 * Using the object for writing prior to activation will result in EOF.
 * \param[in] stream Output file stream
 */
void
SectionedOutputBuffer::activate (
   std::ofstream & stream)
{
   file_buf = stream.rdbuf();
   setp (0,0);
}


/**
 * Write a character in the case of overflow of the write buffer.
 * For now, the buffer always overflows.
 * @return Status: EOF => failed
 * \param[in] ch Character to be writter
 */
std::streambuf::int_type
SectionedOutputBuffer::overflow (
   std::streambuf::int_type ch)
{
   std::streambuf::int_type result;

   // Protect against writes to an inoperable object.
   if (!*this) {
      result = std::streambuf::traits_type::eof();
   }

   // The response to EOF is to flush the (real) buffer.
   else if (std::streambuf::traits_type::eq_int_type (
               ch, std::streambuf::traits_type::eof())) {
      try {
         result = file_buf->pubsync();
      }
      catch (...) {
         result = std::streambuf::traits_type::eof();
      }
   }

   // The response to non-EOF is write the character to the real buffer.
   else {
      try {
         result = file_buf->sputc(
                     std::streambuf::traits_type::to_char_type(ch));
      }
      catch (...) {
         result = std::streambuf::traits_type::eof();
      }
   }

   return result;
}


/**
 * Construct a SectionedOutputStream object.
 * @note
 * This default constructor creates a disconnected and hence unusable
 * stream. Usable streams are created by the non-default constructor.
 */
SectionedOutputStream::SectionedOutputStream ()
:
   std::ostream  (&sectbuf),
   sectbuf       (),
   manager       (NULL),
   stream        (NULL),
   section_start (NULL),
   section_end   (NULL),
   tag           (""),
   is_copy       (false),
   is_active     (false)
{
   ; // Empty
}


/**
 * Construct a SectionedOutputStream object that is connected to a file stream
 * and to a CheckPointOutputManager.
 * \param[in] mngr The stream manager
 * \param[in] ofstream The output file stream
 * \param[in] start_marker Start of section marker
 * \param[in] end_marker End of section marker
 * \param[in] section_name Name of the section
 */
SectionedOutputStream::SectionedOutputStream (
   CheckPointOutputManager * mngr,
   std::ofstream & ofstream,
    const std::string & start_marker,
    const std::string & end_marker,
    const std::string & section_name)
:
    std::ostream  (&sectbuf),
    sectbuf       (),
    manager       (mngr),
    stream        (&ofstream),
    section_start (&start_marker),
    section_end   (&end_marker),
    tag           (section_name),
    is_copy       (false),
    is_active     (false)
{
    ; // Empty
}


/**
 * Construct a SectionedOutputStream object by copying from another.
 * \param[in] source Source object
 */
SectionedOutputStream::SectionedOutputStream (
   const SectionedOutputStream & source)
:
   std::basic_ios<char> (&sectbuf),
   std::ostream  (&sectbuf),
   sectbuf       (),
   manager       (source.manager),
   stream        (source.stream),
   section_start (source.section_start),
   section_end   (source.section_end),
   tag           (source.tag),
   is_copy       (true),
   is_active     (false)
{
   // No making copies of a copy, an active object, or an invalid object.
   // Oops. Too late now; we just did just that. Undo the copy.
   if ((source.is_copy) || (source.is_active) || (source.stream == NULL)) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::implementation_error,
         "Illegal attempt to copy a SectionedOutputStream.");
      manager = NULL;
      stream  = NULL;
   }
}


/**
 * Destruct a SectionedOutputStream object.
 */
SectionedOutputStream::~SectionedOutputStream (
   void)
{
   // Deactivate the stream.
   deactivate ();
}


/**
 * Determine if the stream is able to be activated.
 * @return True if object can be activated.
 */
bool
SectionedOutputStream::is_activatable (
   void)
const
{
   // The object cannot be activated if
   // - It is an invalid object,
   // - Already active, or
   // - The manager has an active writer on hand.
   if ((manager == NULL) || (!manager) ||
       (stream == NULL)  || (!stream)  ||
       is_active ||
       manager->have_active_writer()) {
      return false;
   }
   else {
      return true;
   }
}


/**
 * Activate the object.
 * @note
 * Using the object for writing prior to activation will write nothing.
 * @return True if activated.
 */
bool
SectionedOutputStream::activate (
   void)
{
   // Activating an invalid object is verboten.
   if ((manager == NULL) || (!manager) ||
       (stream == NULL)  || (!stream)) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::implementation_error,
         "Illegal attempt to activate an invalid SectionedOutputStream.");
      return false;
   }

    // Activating an already-activated object is verboten.
    if (is_active) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::implementation_error,
         "Illegal attempt to activate an already-active SectionedOutputStream");
      return false;
   }

   // There can only be one active object present at any one time.
   if (! manager->register_writer(this)) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::implementation_error,
         "Illegal attempt to activate a writer while one is active.");
      return false;
   }

   // All is cool. Write the section header to the file and activate.
   // Note: The destructor eventually writes the corresponding section trailer.

   // Preface the header with blank lines unless we're at start of file.
   // Note: The static cast is needed to support old versions of GNU c++.
   if (static_cast<std::size_t>(stream->tellp()) != 0) {
      *stream << "\n\n";
   }

   // Write the section header proper to the file.
   *stream << *section_start << tag << "\n" << std::endl;

   // Activate the stream buffer and the stream.
   sectbuf.activate (*stream);
   is_active = true;

   return true;
}


/**
 * Deactivate the object.
 * @note
 * Deactivation is undoable.
 */
void
SectionedOutputStream::deactivate (
   void)
{

   // Write the section trailer to the file if OK.
   if (!!*this) {
      *stream << "\n" << *section_end << tag << std::endl;
   }

   // Deregister this object as the active writer.
   if (is_active) {
      manager->deregister_writer(this);
   }
   // Deactivate the sectioned output stream buffer.
   // Note: This will flush buffered output if this is a well-behaved stream.
   sectbuf.deactivate();

   // Mark the object as (permanently) inactive.
   manager = NULL;
   stream = NULL;
   is_active = false;
}


/**
 * Construct a CheckPointOutputManager object.
 * \param[in] fname Name of file to be opened
 * \param[in] start_marker Start of section marker
 * \param[in] end_marker End of section marker
 */
CheckPointOutputManager::CheckPointOutputManager (
   const std::string & fname,
   const std::string & start_marker,
   const std::string & end_marker)
:
   stream         (fname.c_str(), std::ios::out),
   current_writer (NULL),
   filename       (fname),
   section_start  (start_marker),
   section_end    (end_marker),
   is_open        (true)
{
   if (! stream.is_open ()) {
      is_open = false;
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::implementation_error,
         "Unable to open '%s'.",
         filename.c_str());
   }
}


/**
 * Create a C++ output stream that writes to a checkpoint file section.
 * @par Usage
 * Use this function as the initializer of a section writer variable.
 * @par Error handling
 * A null SectionedOutputStream is created when the CheckPointOutputManager
 * itself is invalid or the designated section is invalid.
 * @return A SectionedOutputStream object.
 * \param[in] trick OK to create the Trick section writer?
 * \param[in] tag Tag identifying the section to be written.
 */
SectionedOutputStream
CheckPointOutputManager::create_section_writer (
   bool trick,
   const std::string & tag)
{

   if (!*this) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::implementation_error,
         "Checkpoint file '%s' is %s.",
         filename.c_str(),
         !is_open ? "not open" : "invalid");
      return SectionedOutputStream ();
   }

   else if ((tag.compare ("Trick") == 0) && !trick) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::implementation_error,
         "Access to the Trick checkpoint section is restricted.");
      return SectionedOutputStream ();
   }

   else {
      return SectionedOutputStream (
                this, stream, section_start, section_end, tag);
    }
}


/**
 * Create a C++ output stream that writes to the Trick checkpoint file section.
 * @return Trick SectionedOutputStream object.
 */
SectionedOutputStream
CheckPointOutputManager::create_trick_section_writer (
   void)
{
   if (current_writer != NULL) {
      current_writer->deactivate();
      current_writer = NULL;
   }

   return create_section_writer (true, "Trick");
}


/**
 * Register the supplied section writer as the currently-active writer.
 * @return True => success.
 * \param[in] writer Writer to be Registered
 */
bool
CheckPointOutputManager::register_writer (
   SectionedOutputStream * writer)
{
   if (current_writer != NULL) {
      return false;
   }
   else {
      current_writer = writer;
      return true;
   }
}


/**
 * Deregister the supplied section writer as the currently-active writer.
 * @return True => success.
 * \param[in] writer Writer to be deregistered
 */
bool
CheckPointOutputManager::deregister_writer (
   SectionedOutputStream * writer)
{
   if (current_writer != writer) {
      return false;
   }
   else {
      current_writer = NULL;
      return true;
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
