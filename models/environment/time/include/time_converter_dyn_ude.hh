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
 * @file models/environment/time/include/time_converter_dyn_ude.hh
 * Define class TimeConverter_Dyn_UDE, which converts from simulation dynamic
 * time to any specific instance of the generic User-Defined-Epoch Time.
 * There can be multiple instances of this class.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((This class converts from simulation dynamic time to UDE only. There is
    no reverse conversion.))



Library dependencies:
  ((../src/time_converter_dyn_ude.cc))
******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_DYN_UDE_HH
#define JEOD_TIME_CONVERTER_DYN_UDE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "time_converter.hh"


//! Namespace jeod
namespace jeod {

class TimeDyn;
class TimeUDE;
class JeodBaseTime;


/**
 * Define class TimeConverter_Dyn_UDE, which converts from simulation dynamic
 * time to any specific instance of the generic User-Defined-Epoch Time.
 */
class TimeConverter_Dyn_UDE : public TimeConverter {

  JEOD_MAKE_SIM_INTERFACES(TimeConverter_Dyn_UDE)

// Member Data
private:
   /**
    * Converter parent time, always a TimeDyn for this converter.
    */
  TimeDyn * dyn_ptr; //!< trick_units(--)

   /**
    * Converter child time, always a TimeUDE for this converter.
    */
  TimeUDE * ude_ptr; //!< trick_units(--)

// Member functions:
public:
  // Constructor
   TimeConverter_Dyn_UDE ();
  // Destructor
   ~TimeConverter_Dyn_UDE () override;

   void reset_a_to_b_offset (void) override;

  // Initialize the converter
   void initialize (JeodBaseTime * parent,
                    JeodBaseTime * child,
                    const int direction) override;

  // convert_a_to_b: Apply the converter in the forward direction
   void convert_a_to_b (void) override;

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:
   TimeConverter_Dyn_UDE (const TimeConverter_Dyn_UDE&);
   TimeConverter_Dyn_UDE & operator = (const TimeConverter_Dyn_UDE&);
};
/*----------------------------------------------------------------------------*/


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
