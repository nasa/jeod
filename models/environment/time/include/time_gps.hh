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
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/include/time_gps.hh
 * To represent the time associated with the Global Positioning System.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((Astronomical Almanac)))

Assumptions and limitations:
  ((None))


Library dependencies:
  ((time_gps.o))
******************************************************************************/

#ifndef JEOD_TIME_GPS_HH
#define JEOD_TIME_GPS_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "time_standard.hh"


//! Namespace jeod
namespace jeod {

/**
 * To represent the time associated with the Global Positioning System.
 */
class TimeGPS : public TimeStandard { //Global Positioning Satellite Time

  JEOD_MAKE_SIM_INTERFACES(TimeGPS)

// Member Data
//private:
public:
   /**
    * Seconds elapsed in last (partial) day.
    */
  double seconds_of_day;  //!< trick_units(s)
   /**
    * Seconds elapsed in last (partial) week.
    */
  double seconds_of_week; //!< trick_units(s)
   /**
    * Number of whole days this week.
    */
  int day_of_week;        //!< trick_units(day)
   /**
    * Number of rollovers (1024 week blocks) since epoch
    */
  int rollover_count;     //!< trick_units(--)
   /**
    * Number of weeks in current 1024-week block.
    */
  int week;               //!< trick_units(--)
   /**
    * Number of rollovers (8192 week blocks) since epoch
    */
  int rollover_count_13_bit;     //!< trick_units(--)
   /**
    * Number of weeks in current 8192-week block.
    */
  int week_13_bit;               //!< trick_units(--)

// Member functions:
public:
  //Constructor
   TimeGPS ();
  // Destructor
   ~TimeGPS ();

   void set_time_by_seconds (const double new_seconds);
   void set_time_by_days (const double new_seconds);
   void set_time_by_trunc_julian (const double new_tjt);

private:

   void calculate_calendar_values (void);
   void convert_from_calendar (void);
   void set_epoch(void);

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:
   TimeGPS (const TimeGPS&);
   TimeGPS & operator = (const TimeGPS&);
};
/*----------------------------------------------------------------------------*/

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
