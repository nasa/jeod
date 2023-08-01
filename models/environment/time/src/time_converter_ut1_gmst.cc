/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_ut1_gmst.cc
 * Define member functions for class TimeConverter_UT1_GMST.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter_ut1_gmst.cc)
   (time.cc)
   (time_converter.cc)
   (time_gmst.cc)
   (time_ut1.cc)
   (time_messages.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))

 
******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_converter_ut1_gmst.hh"
#include "../include/time_ut1.hh"
#include "../include/time_gmst.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_UT1_GMST
 */
TimeConverter_UT1_GMST::TimeConverter_UT1_GMST (
   void)
{
   ut1_ptr  = nullptr;
   gmst_ptr = nullptr;
   a_name   = "UT1";
   b_name   = "GMST";
   valid_directions = A_TO_B;
}


/**
 * Initialize the converter.
 *
 * \par Assumptions and Limitations
 *  - None
 * \param[in] parent_ptr Time used to initialize the converter
 * \param[in] child_ptr Other Time used to initialize the converter
 * \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 */
void
TimeConverter_UT1_GMST::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{
   verify_setup (parent_ptr, child_ptr, int_dir);

   if (int_dir == 1) {
      ut1_ptr  = dynamic_cast<TimeUT1 *> (parent_ptr);
      gmst_ptr = dynamic_cast<TimeGMST *> (child_ptr);
   }
   else if (int_dir == -1) {
      ut1_ptr  = dynamic_cast<TimeUT1 *> (child_ptr);
      gmst_ptr = dynamic_cast<TimeGMST *> (parent_ptr);
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Illegal value of int_dir in UT1->GMST initializer");
   }
   initialized = true;

   return;
}

/**
 * Convert from TimeUT1 to TimeGMST.
 *
 * \par Assumptions and Limitations
 *  - None
 */
void
TimeConverter_UT1_GMST::convert_a_to_b (
   void)
{

   double dd = ut1_ptr->get_days() - 0.000738762;
   // this line changes days since J2000 into days since noon January 1, 2000
   // UT1, as specified in Astronomical Almanac.
   // Note, this only makes sense if the UT1 epoch is J2000.
   // If the developer uses a different epoch for UT1, the offset will be
   // different.  The following line also works, with a loss of significance.
   // double dd = ut1_ptr->trunc_julian_time - 11544.5;
   double dd2 = dd * dd;
   double dd3 = dd * dd2;

   gmst_ptr->set_time_by_days (0.7790572733 + 1.002737909350795 * dd +
                               8.0775E-16 * dd2 - 1.5E-24 * dd3);
   // gmst_ptr->days =  0.7790572733 + 1.002737909350795 * dd +
   //                  8.0775E-16 * dd2 - 1.5E-24 * dd3;
   // gmst_ptr->seconds = gmst_ptr->days * 86400;


   return;
}



// NOTE: no reverse converter available

/**
 * Destroy a TimeConverter_UT1_GMST
 */

/******************************************************************************
 Function: TimeConverter_UT1_GMST::~TimeConverter_UT1_GMST
 Purpose: (Destroy a TimeConverter_UT1_GMST)
 Class:   (N/A)
******************************************************************************/
TimeConverter_UT1_GMST::~TimeConverter_UT1_GMST (
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
