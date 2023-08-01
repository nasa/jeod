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
 * @file models/environment/time/include/time_converter_tai_tdb.hh
 * Define class TimeConverter_TAI_TDB, which converts from International Atomic
 * Time to Barycentric Dynamic Time.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((This class converts from TAI to TDB only. There is
    no reverse conversion.))



Library dependencies:
  ((../src/time_converter_tai_tdb.cc))
******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_TAI_TDB_HH
#define JEOD_TIME_CONVERTER_TAI_TDB_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "time_converter.hh"


//! Namespace jeod
namespace jeod {

class TimeTDB;
class TimeTAI;
class JeodBaseTime;


/**
 * Define class TimeConverter_TAI_TDB, which converts from International Atomic
 * Time to Barycentric Dynamic Time.
 */
class TimeConverter_TAI_TDB : public TimeConverter {

  JEOD_MAKE_SIM_INTERFACES(TimeConverter_TAI_TDB)

// Member Data
private:
   /**
    * The offset from TAI to TT
    */
  double TAI_to_TT_offset;    //!< trick_units(s)
   /**
    * The epoch value of a_to_b_offset.
    */
  double a_to_b_offset_epoch; //!< trick_units(s)
    /**
     * TAI seconds from previous loop iteration
     */
  double prev_tai_seconds;    //!< trick_units(s)
    /**
     * TDB seconds from previous loop iteration
     */
  double prev_tdb_seconds;    //!< trick_units(s)
    /**
     * Counter for number of steps in iteration
     */
  int nSteps;                 //!< trick_units(--)
    /**
     * Counter for number of iterations
     */
  int nIter;                  //!< trick_units(--)
   /**
    * Converter parent time, always a TimeTAI for this converter.
    */
  TimeTAI * tai_ptr;          //!< trick_units(--)
   /**
    * Converter parent time, always a TimeTDB for this converter.
    */
  TimeTDB * tdb_ptr;          //!< trick_units(--)

// Member functions:
public:
  // Constructor
   TimeConverter_TAI_TDB ();
  // Destructor
   ~TimeConverter_TAI_TDB () override;

  // Initialize the converter
   void initialize (JeodBaseTime * parent,
                    JeodBaseTime * child,
                    const int direction) override;

  // set_a_to_b_offset: Calculate and set the offset to be used in convert_a_to_b and _b_to_a
  void set_a_to_b_offset (void);

  // convert_a_to_b: Apply the converter in the forward direction
   void convert_a_to_b (void) override;

  // convert_b_to_a: Apply the converter in the reverse direction
   void convert_b_to_a (void) override;

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
private:
   TimeConverter_TAI_TDB (const TimeConverter_TAI_TDB&);
   TimeConverter_TAI_TDB & operator = (const TimeConverter_TAI_TDB&);
};
/*----------------------------------------------------------------------------*/


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
