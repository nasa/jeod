/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_tai_tdb.cc
 * Converts from International Atomic Time to Barycentric Dynamic Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((Explanatory Supplement to the Astronomical Almanac, 2006)))

ASSUMPTIONS AND LIMITATIONS:
  ((The equation for calculating TDB, per page 42 in the citation above,
    requires Julian Date to 2 decimal places. Since the various timescales
    differ only in the 3rd decimal place, this equation is valid whether
    TAI, UTC, UT1, or TT is used.
    In this representation, the TAI JD is used.
    This ambiguity produces a potential difference of 0.01 degrees in the
    position of Earth on its orbit, which produces a potential difference
    on the order of 0.1 microseconds in the calculation of TDB.
    The method used is therefore accurate only to 0.1 microseconds.))

LIBRARY DEPENDENCY:
  ((time_converter_tai_tdb.cc)
   (time_converter.cc)
   (time.cc)
   (time_tai.cc)
   (time_tdb.cc)
   (time_messages.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))

 
******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_converter_tai_tdb.hh"
#include "../include/time_tai.hh"
#include "../include/time_tdb.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/******************************************************************************
 Function: TimeConverter_TAI_TDB
 Purpose: Construct a TimeConverter_TAI_TDB object
 ******************************************************************************/
TimeConverter_TAI_TDB::TimeConverter_TAI_TDB (
   void)
{
   TAI_to_TT_offset = 32.184;    // seconds difference.  This is needed
                                 // because first TAI must convert to TT before
                                 // applying the periodic corrections.

   a_to_b_offset_epoch   = 0.0;
   a_to_b_offset         = 0.0;
   prev_tai_seconds      = 0.0;
   prev_tdb_seconds      = 0.0;
   tai_ptr               = nullptr;
   tdb_ptr               = nullptr;
   a_name                = "TAI";
   b_name                = "TDB";
   valid_directions = ANY_DIRECTION;
   nSteps                = 0;
   nIter                 = 0;
}


/******************************************************************************
 Function: initialize
 Purpose:  Initialize the converter.
           \param[in] parent_ptr Time used to initialize the converter
           \param[in] child_ptr Other Time used to initialize the converter
           \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 ******************************************************************************/
void
TimeConverter_TAI_TDB::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{
   verify_setup (parent_ptr, child_ptr, int_dir);

   if (int_dir == 1) {
      tai_ptr = dynamic_cast<TimeTAI *> (parent_ptr);
      tdb_ptr = dynamic_cast<TimeTDB *> (child_ptr);
   }
   else if (int_dir == -1) {
      tai_ptr = dynamic_cast<TimeTAI *> (child_ptr);
      tdb_ptr = dynamic_cast<TimeTDB *> (parent_ptr);
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Illegal value of int_dir in TAI->TDB initializer");
   }
   initialized = true;

   a_to_b_offset_epoch =
      (tdb_ptr->tjt_at_epoch - tai_ptr->tjt_at_epoch) * 86400 -
      TAI_to_TT_offset;

   set_a_to_b_offset();
   return;
}


/******************************************************************************
 Function: set_a_to_b_offset
 Purpose: Calculate and set the a_to_b_offset
 ******************************************************************************/
void
TimeConverter_TAI_TDB::set_a_to_b_offset (
void)
{
    double g = (M_PI / 180) * (357.53 + 0.9856003 *
                              (tai_ptr->trunc_julian_time - tai_ptr->tjt_at_epoch));
    /* -- g parameter used in calculating TDB */
    //  NB - Astronomical Almanac provides conversion from TT to TDB;
    // it uses (JD - 2451545.0), or (TT_JD - TT_JD_at_J2000).
    // TT and TAI tick at the same rate, so the same number is obtained by
    // taking (TAI_tjt - TAI_tjt_at_J2000).
    a_to_b_offset = 0.001658 * sin (g) + 0.000014 * sin (2.0 * g);
}


/******************************************************************************
 Function: convert_a_to_b
 Purpose: Convert from TimeTAI to TimeTDB
 ******************************************************************************/
void
TimeConverter_TAI_TDB::convert_a_to_b (
   void)
{
   set_a_to_b_offset();
   tdb_ptr->set_time_by_seconds (tai_ptr->seconds  + a_to_b_offset -
                                 a_to_b_offset_epoch);

   return;
}


/******************************************************************************
 Function: convert_b_to_a
 Purpose: Convert from TimeTDB to TimeTAI
 ******************************************************************************/
void
TimeConverter_TAI_TDB::convert_b_to_a (
void)
{
    // Initial guess - iterative solution
    tai_ptr->set_time_by_seconds(prev_tai_seconds + (tdb_ptr->seconds - prev_tdb_seconds));

    while (1) {
        nSteps++;
        nIter++;
        set_a_to_b_offset();
        double dtai = (tdb_ptr->seconds - tai_ptr->seconds)
                    - (a_to_b_offset - a_to_b_offset_epoch);
        tai_ptr->set_time_by_seconds(tai_ptr->seconds + dtai);
        if (nSteps > 5 || std::abs(dtai/tai_ptr->seconds) < 1.0e-15) {
            prev_tdb_seconds = tdb_ptr->seconds;
            prev_tai_seconds = tai_ptr->seconds;
            break;
        }
    }
}


/******************************************************************************
 Function: ~TimeConverter_TAI_TDB
 Purpose: Destroy a TimeConverter_TAI_TDB object
******************************************************************************/
TimeConverter_TAI_TDB::~TimeConverter_TAI_TDB (
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
