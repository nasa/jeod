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
 * @file models/environment/time/include/time_converter_std_ude.hh
 * Define class TimeConverter_STD_UDE, which converts from any specific
 * example of the generic Standard Time to any specific example of the
 * generic User-Defined-Epoch Time.  There can be multiple such instances
 * of this class.
 */

/******************************************************************************
Purpose:
      ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))


Library dependencies:
  ((../src/time_converter_std_ude.cc))
*******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_STD_UDE_HH
#define JEOD_TIME_CONVERTER_STD_UDE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "time_converter.hh"


//! Namespace jeod
namespace jeod {

class TimeStandard;
class TimeUDE;
class JeodBaseTime;


/**
 * Define class TimeConverter_STD_UDE, which converts from any specific
 * example of the generic Standard Time to any specific example of the
 * generic User-Defined-Epoch Time.
 */
class TimeConverter_STD_UDE : public TimeConverter {

  JEOD_MAKE_SIM_INTERFACES(TimeConverter_STD_UDE)

// Member Data
private:
   /**
    * Initializing converter could be done in one of two ways.  If it fails the
    * first time, this flag is set.  If it fails a second time, it terminates.
    */
  bool failed_null_test;  //!< trick_units(--)
   /**
    * Converter parent time, always a TimeSTD for this converter.
    */
  TimeStandard * std_ptr; //!< trick_units(--)

   /**
    * Converter parent time, always a TimeUDE for this converter.
    */
  TimeUDE * ude_ptr; //!< trick_units(--)

// Member functions:
public:
  // Constructor
   TimeConverter_STD_UDE ();
  // Destructor
   ~TimeConverter_STD_UDE () override;

   void reset_a_to_b_offset (void) override;

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
   TimeConverter_STD_UDE (const TimeConverter_STD_UDE&);
   TimeConverter_STD_UDE & operator = (const TimeConverter_STD_UDE&);
};
/*----------------------------------------------------------------------------*/


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
