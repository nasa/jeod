/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_dyn_tdb.cc
 * Converts between Dynamic Time and Barycentric Dynamic Time.
 */

/******************************************************************************
 PURPOSE:
 ()

 REFERENCE:
 (((None)))

 ASSUMPTIONS AND LIMITATIONS:
 ((This class converts from TimeDyn to TimeTDB *only*))

 LIBRARY DEPENDENCY:
 ((time_converter_dyn_tdb.o)
 (time_converter.o)
 (time.o)
 (time_dyn.o)
 (time_tdb.o)
 (time_messages.o)
 (utils/sim_interface/memory_interface.o)
 (utils/message/message_handler.o)
 (utils/named_item/named_item.o))

  
 ******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_converter_dyn_tdb.hh"
#include "../include/time_dyn.hh"
#include "../include/time_tdb.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_Dyn_TDB
 */
TimeConverter_Dyn_TDB::TimeConverter_Dyn_TDB (
                                              void)
{
    dyn_ptr               = NULL;
    tdb_ptr               = NULL;
    a_name                = "Dyn";
    b_name                = "TDB";
    valid_directions = A_TO_B;
}


/**
 * Initialize the converter.
 * \param[in] parent_ptr Time used to initialize the converter
 * \param[in] child_ptr Other Time used to initialize the converter
 * \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 */
void
TimeConverter_Dyn_TDB::initialize (
                                   JeodBaseTime * parent_ptr,
                                   JeodBaseTime * child_ptr,
                                   const int int_dir)
{
    // NOT const Time *; will be casting to different type.

    // Direction = 1 => Translate from parent (TimeDyn) to child (TimeTDB) ONLY
    // Note: This converter will only support conversions from Dyn to TDB, but
    // must be initialized from TDB.  If TDB has not been initialized at this
    // point, the verify_setup routines will fail the simulation.

    if (int_dir == 1) {
        verify_setup (child_ptr, parent_ptr, int_dir);

        // Convert the parent to a TimeDyn, ensuring that this conversion works.
        dyn_ptr = dynamic_cast<TimeDyn *> (parent_ptr);
        if (dyn_ptr == NULL) {
            MessageHandler::fail (
                                  __FILE__, __LINE__, TimeMessages::invalid_setup_error,
                                  "\n Object '%s' is not an object of type %s, as was expected when"
                                  "initializing the converter with int_dir = 1. \n"
                                  "TimeDyn", parent_ptr);
        }

        // Convert the child to a TimeTDB, ensuring that this conversion works.
        tdb_ptr = dynamic_cast<TimeTDB *> (child_ptr);
        if (tdb_ptr == NULL) {
            MessageHandler::fail (
                                  __FILE__, __LINE__, TimeMessages::invalid_setup_error,
                                  "\n Object '%s' is not an object of type %s, as was expected when"
                                  "initializing the converter with int_dir = 1.\n"
                                  "TimeTDB", child_ptr);
        }
    }

    // All other int_dir values are *ALWAYS* invalid.
    else {
        MessageHandler::fail (
                              __FILE__, __LINE__, TimeMessages::invalid_setup_error,
                              "\nIllegal value of int_dir in Dyn->TDB initializer.\n");
    }

    // Compute the initial offset.
    if (dyn_ptr->seconds != 0) {
        MessageHandler::fail (
                              __FILE__, __LINE__, TimeMessages::initialization_error, "\n"
                              "DynTime must be 0 for the Dyn-TDB converter to be "
                              "initialized.\n");
    }
    a_to_b_offset = tdb_ptr->seconds;

    initialized = true;

    return;
}


/**
 * Convert from TimeDyn to TimeTDB.
 */
void
TimeConverter_Dyn_TDB::convert_a_to_b (
                                       void)
{
    tdb_ptr->set_time_by_seconds (dyn_ptr->seconds + a_to_b_offset);

    return;
}


/**
 * Destroy a TimeConverter_Dyn_TDB
 */
TimeConverter_Dyn_TDB::~TimeConverter_Dyn_TDB (
                                               void)
{
   // Default
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
