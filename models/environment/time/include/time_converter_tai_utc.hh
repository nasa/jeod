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
 * @file models/environment/time/include/time_converter_tai_utc.hh
 * Converts between International Atomic Time and Coordinated Universal Time.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((Requires a data look-up table containing the occurrences of
  leap-seconds.)
  (Restricted applicability for simulations set at some future time.))


Library dependencies:
  ((time_converter_tai_utc.o))
******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_TAI_UTC_HH
#define JEOD_TIME_CONVERTER_TAI_UTC_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "time_converter.hh"


//! Namespace jeod
namespace jeod {

class TimeUTC;
class TimeTAI;
class JeodBaseTime;


/**
 * Converts between International Atomic Time and Coordinated Universal Time.
 */
class TimeConverter_TAI_UTC : public TimeConverter {

  JEOD_MAKE_SIM_INTERFACES(TimeConverter_TAI_UTC)

// Member Data
public:
   /**
    * "True" to enter user-specified tai-utc offset
    */
  bool override_data_table;       //!< trick_units(--)

private:
   /**
    * Converter parent time, always a TimeTAI for this converter.
    */
  TimeTAI * tai_ptr; //!< trick_units(--)

   /**
    * Converter parent time, always a TimeUTC for this converter.
    */
  TimeUTC * utc_ptr; //!< trick_units(--)

public:
   /**
    * User specified value (TAI - UTC)
    */
  double leap_sec_override_val; //!< trick_units(s)
   /**
    * Maximum index in the leap tables.
    */
  int last_index;   //!< trick_units(--)
   /**
    * Current index in the leap tables
    */
  int index;        //!< trick_units(--)
   /**
    * Tabulated values of leap_value
    */
  int * val_vec;    //!< trick_units(s)
   /**
    * Tabulated values of Julian time corresponding
    * to changes in leap_value
    */
  double * when_vec;    //!< trick_units(day)
private:
   /**
    * The next (future) UTC time of a leap second instance
    */
  double next_when;     //!< trick_units(--)
   /**
    * The most recent (past) UTC time of a leap
    * second instance
    */
  double prev_when;     //!< trick_units(--)
   /**
    * Flag to indicate that the current time is
    * not covered by the leap-second tables
    */
  bool off_table_end;    //!< trick_units(--)

// Member functions:
public:
  // Constructor
   TimeConverter_TAI_UTC ();
  // Destructor
   ~TimeConverter_TAI_UTC ();

  // Initialize the converter
   void initialize (JeodBaseTime * parent,
                    JeodBaseTime * child,
                    const int direction);

  // convert_a_to_b: Apply the converter in the forward direction
   void convert_a_to_b (void);

  // convert_b_to_a: Apply the converter in the reverse direction
   void convert_b_to_a (void);

 private:
  // initialize_leap_second: Initialize the leap second table
   void initialize_leap_second (void);

  // used at time reversals to verify the ends of the lookup table
   void verify_table_lookup_ends (void);

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
   TimeConverter_TAI_UTC (const TimeConverter_TAI_UTC&);
   TimeConverter_TAI_UTC & operator = (const TimeConverter_TAI_UTC&);

};
/*----------------------------------------------------------------------------*/


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
