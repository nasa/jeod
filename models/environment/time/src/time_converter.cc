/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter.cc
 * An abstract class that defines the basic structure of all the methods used by
 * the converter objects.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter.cc)
   (time_messages.cc)
   (utils/message/src/message_handler.cc))


******************************************************************************/

// System includes
#include <cstddef>
#include <cstdlib>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/time.hh"
#include "../include/time_converter.hh"
#include "../include/time_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Return internal initialized status bool
 */
bool TimeConverter::is_initialized()
{
    return initialized;
}

/**
 * Verify the setup.
 *
 * \par Assumptions and Limitations
 *  - None
 * \param[in] master_ptr Time used to initialize the converter
 * \param[in] sub_ptr Other time-type associated with the converter
 * \param[in] int_dir +1 a=parent; -1 b=parent; 0 error
 */
void TimeConverter::verify_setup(const JeodBaseTime * master_ptr, const JeodBaseTime * sub_ptr, const int int_dir)
{
    if(master_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__, __LINE__, TimeMessages::invalid_setup_error, "\nThe master pointer is NULL\n");
        return;
    }
    if(sub_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__, __LINE__, TimeMessages::invalid_setup_error, "\nThe sub pointer is NULL\n");
        return;
    }
    if(!master_ptr->initialized)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::initialization_error,
                             "Trying to initialize converter between %s and "
                             "%s, but the parent type (%s) has not been "
                             "initialized. \n",
                             master_ptr->name.c_str(),
                             sub_ptr->name.c_str(),
                             master_ptr->name.c_str());
        return;
    }
    if(std::abs(int_dir) != 1)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n Failed to initialize converter between %s and "
                             "%s because one or more of the inputs was invalid. \n",
                             master_ptr->name.c_str(),
                             sub_ptr->name.c_str());
        return;
    }
}

/**
 * Check whether this converter is able to handle the requested conversion(s).
 * If query is compound (e.g. CONV_ALL, CONV_A_TO_B_UPDATE|CONV_B_TO_A_UPDATE)
 * then return true only if capable of all conversions
 * @return whether this converter can do all the conversions
 * \param[in] query converter directions to check
 */
bool TimeConverter::can_convert(Direction query)
{
    if(query == NO_DIRECTION)
    {
        if(valid_directions == NO_DIRECTION)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return (query & valid_directions) == query;
    }
}

/**
 * Default converter from time 'a' to time 'b'.
 * This default converter simply terminates the program.
 * A subclass must override this default.
 */
void TimeConverter::convert_a_to_b()
{
    MessageHandler::fail(__FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n Converter not available\n");
}

/**
 * Default converter from time 'b' to time 'a'.
 * This default converter simply terminates the program.
 * A subclass must override this default.
 */
void TimeConverter::convert_b_to_a()
{
    MessageHandler::fail(__FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n Converter not available.\n");
}

/**
 * Resets the offset between type a and type b mid-sim
 */
void TimeConverter::reset_a_to_b_offset() {}

/**
 * This function does absolutely nothing.
 * It is called when the simulation reverses direction (in time).
 * If the converter uses a table lookup, this function should be
 * replaced in that class.  If the converter uses an analytic
 * conversion, no action is needed and this (non)-function should
 * be inherited.
 *
 * \par Assumptions and Limitations
 *  - None
 */
void TimeConverter::verify_table_lookup_ends() {}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
