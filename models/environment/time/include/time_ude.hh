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
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/include/time_ude.hh
 * Represents all instances of times with a user-defined epoch, accepting that
 * Mission Elapsed Time requires some further definition.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))

 

Library dependencies:
  ((../src/time_ude.cc))
******************************************************************************/

#ifndef JEOD_TIME_UDE_HH
#define JEOD_TIME_UDE_HH

// System includes
#include <string>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "time.hh"
#include "time_enum.hh"


//! Namespace jeod
namespace jeod {

class TimeManagerInit;
class TimeStandard;
class TimeDyn;

/**
 * Represents all instances of times with a user-defined epoch, accepting that
 * Mission Elapsed Time requires some further definition.
 */
class TimeUDE : public JeodBaseTime {

  JEOD_MAKE_SIM_INTERFACES(TimeUDE)

public:

   /**
    * Gregorian calendar year number at epoch.
    */
  int epoch_year;         //!< trick_units(--)
   /**
    * Gregorian calendar month number at epoch.
    */
  int epoch_month;        //!< trick_units(--)
   /**
    * Gregorian calendar day number at epoch.
    */
  int epoch_day;          //!< trick_units(day)
   /**
    * 24-hour clock hour number at epoch.
    */
  int epoch_hour;         //!< trick_units(hr)
   /**
    * Clock minute number at epoch.
    */
  int epoch_minute;       //!< trick_units(min)
   /**
    * Clock seconds value at epoch.
    */
  double epoch_second;    //!< trick_units(s)

   /**
    * Whole number of days since epoch, in clock format.
    */
  int clock_day;     //!< trick_units(--)

   /**
    * Whole number of hours since epoch, in clock format.
    */
  int clock_hour;    //!< trick_units(--)

   /**
    * Whole number of minutes since epoch, in clock format.
    */
  int clock_minute; //!< trick_units(--)

   /**
    * Number of seconds since epoch, in clock format.
    */
  double clock_second; //!< trick_units(s)

   /**
    * Simtime at the last time the clock was updated
    */
  double last_clock_update; //!< trick_units(s)

   /**
    * Format for expressing the epoch of this type (calendar, julian, etc)
    */
  TimeEnum::TimeFormat epoch_format; //!< trick_units(--)
   /**
    * Format for expressing the initial value of this type (calendar, julian,
    * etc )
    */
  TimeEnum::TimeFormat initial_value_format;  //!< trick_units(--)

   /**
    * Name of time type in which epoch defined
    */
  std::string epoch_defined_in_name;  //!< trick_units(--)

protected:
   /**
    * Value of epoch in appropriate format.
    */
  double epoch_initializing_value;//!< trick_units(--)

   /**
    * Whether initializing data is present.
    */
  bool initializing_data_present;  //!< trick_units(--)

   /**
    * Whether epoch data is present.
    */
  bool epoch_data_present;  //!< trick_units(--)

   /**
    * Whether there is some numerical input that could set epoch.
    */
  bool epoch_value_is_set_number; //!< trick_units(--)

   /**
    * Whether there is some calendar input that could set epoch.
    */
  bool epoch_value_is_set_calendar; //!< trick_units(--)

   /**
    * Whether there is some clock input that could set epoch.
    */
  bool epoch_value_is_set_clock;  //!< trick_units(--)

   /**
    * The index of the time-type from which this one is updated.
    */
  int update_index; //!< trick_units(--)

   /**
    * The index of the time-type in which this one's epoch is defined.
    */
  int epoch_index; //!< trick_units(--)


public:

  // constructor
   TimeUDE ();

  // destructor
   ~TimeUDE () override;

   void initialize_initializer_time (TimeManagerInit * tm_init) override;
   void add_type_initialize (const int seeking_status,
                             TimeManagerInit * tm_init) override;
   void initialize_from_parent (TimeManagerInit * tm_init) override;
   void set_time_by_clock (void);
   void set_time_by_seconds (const double new_seconds) override;
   void set_time_by_days (const double new_days) override;
   void set_epoch_initializing_value (const double simtime,
                                      const double epoch_initializing_value);

protected:
   // All TimeUDE objects default to NOT requiring singletons.
   bool must_be_singleton () override;

   void convert_epoch_to_update (JeodBaseTime * epoch_ptr,
                                 JeodBaseTime * update_ptr,
                                 TimeManagerInit * tm_init);

   void set_epoch_dyn (TimeDyn * epoch_ptr);

   void set_epoch_times (JeodBaseTime * epoch_ptr);

   void set_epoch_ude (TimeUDE * epoch_ptr);

   void set_epoch_std (TimeStandard * epoch_ptr);

   void set_initial_times (void);


   void clock_update ();

   void verify_epoch (void);

   void verify_init (void);

   void verify_update (void);

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:
   TimeUDE (const TimeUDE&);
   TimeUDE & operator = (const TimeUDE&);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
