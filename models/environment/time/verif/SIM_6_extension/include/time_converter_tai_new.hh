//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
/******************************************************************************
Purpose:
  (Define class TimeConverter_TAI_New, which converts from atomic time to New
   time.)

Reference:
  ((TBS))

Assumptions and limitations:
  ((TBS))



Library dependencies:
  ((../src/time_converter_tai_new.cc)
   (../src/time_new.cc))
******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_TAI_New_HH
#define JEOD_TIME_CONVERTER_TAI_New_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "environment/time/include/time_converter.hh"

//! Namespace jeod
namespace jeod
{

class TimeTAI;
class TimeNew;

/*----------------------------------------------------------------------------*/
class TimeConverter_TAI_New : public TimeConverter
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeConverter_TAI_New)

    // Member Data
private:
    TimeTAI * tai_ptr{}; /* trick_io(**) @n
      Converter parent time, always a TimeTAI for this converter. */

    TimeNew * new_ptr{}; /* trick_io(**) @n
      Converter parent time, always a TimeTT for this converter. */

    // Member functions:
public:
    TimeConverter_TAI_New();
    ~TimeConverter_TAI_New() override = default;
    TimeConverter_TAI_New(const TimeConverter_TAI_New &) = delete;
    TimeConverter_TAI_New & operator=(const TimeConverter_TAI_New &) = delete;

private:
    // Initialize the converter
    void initialize(JeodBaseTime * parent, JeodBaseTime * child, const int direction) override;

    // convert_a_to_b: Apply the converter in the forward direction
    void convert_a_to_b() override;
};

} // namespace jeod

#endif
