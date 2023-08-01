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
 * @file models/environment/time/include/time_converter_tai_gps.hh
 * Define class TimeConverter_TAI_GPS, which converts between International Atomic
 * Time and the clock associated with the Global Positioning System.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))


Library dependencies:
  ((../src/time_converter_tai_gps.cc))
*******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_TAI_GPS_HH
#define JEOD_TIME_CONVERTER_TAI_GPS_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "time_converter.hh"


//! Namespace jeod
namespace jeod {

class TimeGPS;
class TimeTAI;
class JeodBaseTime;


/**
 * Define class TimeConverter_TAI_GPS, which converts between International Atomic
 * Time and the clock associated with the Global Positioning System.
 */
class TimeConverter_TAI_GPS : public TimeConverter {

  JEOD_MAKE_SIM_INTERFACES(TimeConverter_TAI_GPS)

// Member Data
private:
   /**
    * Converter parent time, always a TimeTAI for this converter.
    */
  TimeTAI * tai_ptr; //!< trick_units(--)

   /**
    * Converter parent time, always a TimeGPS for this converter.
    */
  TimeGPS * gps_ptr; //!< trick_units(--)

// Member functions:
public:
  // Constructor
   TimeConverter_TAI_GPS ();
  // Destructor
   ~TimeConverter_TAI_GPS () override;

  // Initialize the converter
   void initialize (JeodBaseTime * parent,
                    JeodBaseTime * child,
                    const int direction) override;

  // convert_a_to_b: Apply the converter in the forward direction
   void convert_a_to_b (void) override;

  // convert_b_to_a: Apply the converter in the reverse direction
   void convert_b_to_a (void) override;

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:
   TimeConverter_TAI_GPS (const TimeConverter_TAI_GPS&);
   TimeConverter_TAI_GPS & operator = (const TimeConverter_TAI_GPS&);
};
/*----------------------------------------------------------------------------*/


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
