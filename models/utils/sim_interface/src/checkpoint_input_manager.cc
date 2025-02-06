/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/checkpoint_input_manager.cc
 * Define CheckPointInputManager member functions and of related classes.
 */

/*
 Purpose:
   ()
*/

// System includes
#include <cstddef>
#include <cstring>
#include <iostream>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/checkpoint_input_manager.hh"
#include "../include/sim_interface_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default constructor.
 *
 * This constructor creates an empty SectionedInputBuffer -- one that
 * will return EOF on the first read attempt.
 * An empty SectionedInputBuffer has two purposes:
 * - As the basis for a copy constructor of a containing stream, and
 * - As a graceful means of handling of erroneous conditions.
 */
SectionedInputBuffer::SectionedInputBuffer()
    : std::streambuf()
{
}

/**
 * Activate the object.
 * @note
 * Using the object for reading prior to activation will result in EOF.
 * \param[in] stream Checkpoint file input file stream
 * \param[in] spos Section data start position
 * \param[in] epos Section data end position
 */
void SectionedInputBuffer::activate(std::ifstream & stream, std::size_t spos, std::size_t epos)
{
    file_buf = stream.rdbuf();
    curr_pos = start_pos = spos;
    end_pos = epos;
    at_eof = start_pos >= end_pos;
}

/**
 * Get a character in the case of depletion of the read buffer.
 * For now, the buffer is always depleted.
 * @return Character read from the underlying file.
 */
std::streambuf::int_type SectionedInputBuffer::underflow()
{
    std::streambuf::int_type result;

    // Protect against reads from an inoperable object.
    if(!*this)
    {
        result = std::streambuf::traits_type::eof();
    }

    // End of section *is* end of file.
    else if(curr_pos >= end_pos)
    {
        at_eof = true;
        result = std::streambuf::traits_type::eof();
    }

    // Should be OK. Try to read from the file.
    else
    {
        try
        {
            result = file_buf->sbumpc();
            if(result == std::streambuf::traits_type::eof())
            {
                at_eof = true;
            }
            else
            {
                buf = static_cast<char>(result);
                this->setg(&buf, &buf, &buf + 1);
                ++curr_pos;
            }
        }
        catch(...)
        {
            at_eof = true;
            result = std::streambuf::traits_type::eof();
        }
    }
    return result;
}

/**
 * Construct a SectionedInputStream object.
 * @note
 * This default constructor creates a disconnected and hence unusable
 * stream. Usable streams are created by the non-default constructor.
 */
SectionedInputStream::SectionedInputStream()
    : std::istream(&sectbuf)
{
}

/**
 * Construct a SectionedInputStream object that is connected to a file stream
 * and to a CheckPointInputManager.
 * \param[in] mngr The stream manager
 * \param[in] ifstream The input file stream
 * \param[in] spos Start position of section data
 * \param[in] epos End position of section data
 */
SectionedInputStream::SectionedInputStream(CheckPointInputManager * mngr,
                                           std::ifstream & ifstream,
                                           std::size_t spos,
                                           std::size_t epos)
    : std::istream(&sectbuf),
      manager(mngr),
      stream(&ifstream),
      start_pos(spos),
      end_pos(epos)
{
}

/**
 * Construct a SectionedInputStream object by copying from another.
 * \param[in] source Source object
 */
SectionedInputStream::SectionedInputStream(const SectionedInputStream & source)
    : std::basic_ios<char>(&sectbuf),
      std::istream(&sectbuf),
      manager(source.manager),
      stream(source.stream),
      start_pos(source.start_pos),
      end_pos(source.end_pos),
      is_copy(true)
{
    // No making copies of a copy, an active object, or an invalid object.
    // Oops. Too late now; we just did just that. Undo the copy.
    if((source.is_copy) || (source.is_active) || (source.stream == nullptr))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Illegal attempt to copy a SectionedInputStream.");
        manager = nullptr;
        stream = nullptr;
    }
}

/**
 * Destruct a SectionedInputStream object.
 */
SectionedInputStream::~SectionedInputStream()
{
    if(is_active)
    {
        manager->deregister_reader(this);
    }
}

/**
 * Determine if the stream is able to be activated.
 * @return True if object can be activated.
 */
bool SectionedInputStream::is_activatable() const
{
    // The object cannot be activated if
    // - It is an invalid object,
    // - Already active, or
    // - The manager has an active writer on hand.
    if((manager == nullptr) || (stream == nullptr) || is_active || manager->have_active_reader())
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * Activate the object.
 * @note
 * Using the object for reading prior to activation will result in EOF.
 * @return True if activated.
 */
bool SectionedInputStream::activate()
{
    // Activating an invalid object is verboten.
    if((manager == nullptr) || (stream == nullptr))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Illegal attempt to activate an invalid SectionedInputStream.");
        return false;
    }

    // Activating an already-activated object is verboten.
    if(is_active)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Illegal attempt to activate an already-active SectionedInputStream");
        return false;
    }

    // There can only be one active object present at any one time.
    if(!manager->register_reader(this))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Illegal attempt to activate a reader while one is active.");
        return false;
    }

    // All is cool.
    // Point the file to the start of the checkpoint section and
    // activate the buffer.
    stream->clear();
    stream->seekg(start_pos);
    sectbuf.activate(*stream, start_pos, end_pos);
    is_active = true;

    return true;
}

/**
 * Deactivate the object.
 * @note
 * Deactivation is undoable.
 */
void SectionedInputStream::deactivate()
{
    // Deregister this object as the active reader.
    if(is_active)
    {
        manager->deregister_reader(this);
    }

    // Deactivate the sectioned input stream buffer.
    sectbuf.deactivate();

    // Mark the object as (permanently) inactive.
    manager = nullptr;
    stream = nullptr;
    is_active = false;
}

/**
 * Construct a CheckPointInputManager object.
 * \param[in] fname Name of file to be opened
 * \param[in] start_marker Start of section marker
 * \param[in] end_marker End of section marker
 */
CheckPointInputManager::CheckPointInputManager(const std::string & fname,
                                               const std::string & start_marker,
                                               const std::string & end_marker)
    : stream(fname.c_str(), std::ios::in),
      filename(fname),
      section_start(start_marker),
      section_end(end_marker)
{
    if(stream.is_open())
    {
        initialize();
    }
    else
    {
        is_open = false;
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Unable to open '%s'.",
                              filename.c_str());
    }
}

/**
 * Determine the locations of the various sections that comprise the file.
 */
void CheckPointInputManager::initialize()
{
    std::string line;
    bool in_section = false;
    bool have_sections = false;
    bool old_style = false;
    bool corrupted = false;
    std::size_t prev_pos = 0;
    std::size_t start_pos = 0;
    std::string section_id;

    while(std::getline(stream, line))
    {
        auto next_pos = static_cast<std::size_t>(stream.tellg());
        if(!in_section)
        {
            if(line.compare(0, section_start.length(), section_start) == 0)
            {
                section_id = line.substr(section_start.length());
                start_pos = next_pos;
                have_sections = in_section = true;
                corrupted = corrupted || old_style;
            }

            else if(line.compare(section_end) == 0)
            {
                corrupted = true;
            }

            else if(line.compare("") != 0)
            {
                corrupted = corrupted || have_sections;
                old_style = true;
            }
        }

        else
        {
            if(line.compare(0, section_end.length(), section_end) == 0)
            {
                if(line.compare(section_end.length(), std::string::npos, section_id) == 0)
                {
                    sections.insert(std::pair<std::string, SectionInfo>(section_id, SectionInfo(start_pos, prev_pos)));
                }
                else
                {
                    corrupted = true;
                }
                in_section = false;
            }
        }
        prev_pos = next_pos;
    }

    corrupted = corrupted || in_section;

    if(corrupted)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Checkpoint file '%s' is corrupted.",
                              filename.c_str());
    }

    else if(old_style)
    {
        sections.insert(std::pair<std::string, SectionInfo>("Trick", SectionInfo(0, prev_pos)));
    }

    stream.clear();
    stream.seekg(0, std::ios::beg);
}

/**
 * Create a C++ input stream that reads from a checkpoint file section.
 * @par Usage
 * Use this function as the initializer of a section reader variable.
 * @par Error handling
 * A null SectionedInputStream is created when the CheckPointInputManager
 * itself is invalid or when the designated section is not present in the
 * checkpoint file.
 * @return A SectionedInputStream object.
 * \param[in] trick OK to create the Trick section reader?
 * \param[in] tag Tag identifying the section to be read.
 */
SectionedInputStream CheckPointInputManager::create_section_reader(bool trick, const std::string & tag)
{
    auto iter = sections.find(tag);

    if(!*this)
    {
        const char fileOpenStrings[2][9] = {"not open", "invalid"};
        int index = 1;
        if(!is_open)
        {
            index = 0;
        }
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Checkpoint file '%s' is %s.",
                              filename.c_str(),
                              fileOpenStrings[index]);
        return SectionedInputStream();
    }

    else if(iter == sections.end())
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Checkpoint file '%s' has no section named '%s'.",
                              filename.c_str(),
                              tag.c_str());
        return SectionedInputStream();
    }

    else if((tag.compare("Trick") == 0) && !trick)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              SimInterfaceMessages::implementation_error,
                              "Access to the Trick checkpoint section is restricted.");
        return SectionedInputStream();
    }

    else
    {
        SectionInfo info(iter->second);
        return SectionedInputStream(this, stream, info.start_pos, info.end_pos);
    }
}

/**
 * Create a C++ input stream that reads from the Trick checkpoint file section.
 * @return Trick SectionedInputStream object.
 */
SectionedInputStream CheckPointInputManager::create_trick_section_reader()
{
    if(current_reader != nullptr)
    {
        current_reader->deactivate();
        current_reader = nullptr;
    }

    return create_section_reader(true, "Trick");
}

/**
 * Register the supplied section reader as the currently-active reader.
 * @return True => success.
 * \param[in] reader Reader to be registered
 */
bool CheckPointInputManager::register_reader(SectionedInputStream * reader)
{
    if(current_reader != nullptr)
    {
        return false;
    }
    else
    {
        current_reader = reader;
        return true;
    }
}

/**
 * Deregister the supplied section reader as the currently-active reader.
 * @return True => success.
 * \param[in] reader Reader to be deregistered
 */
bool CheckPointInputManager::deregister_reader(const SectionedInputStream * reader)
{
    if(current_reader != reader)
    {
        return false;
    }
    else
    {
        current_reader = nullptr;
        return true;
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
