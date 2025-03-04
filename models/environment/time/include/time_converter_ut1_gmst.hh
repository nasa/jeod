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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/include/time_converter_ut1_gmst.hh
 * Converts between Universal Time and Greenwich Mean Sidereal Time.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((Astronomical Almanac)))

Assumptions and limitations:
  ((This class converts from UT1 to GMST only. There is
    no reverse conversion.))


Library dependencies:
  ((../src/time_converter_ut1_gmst.cc))
******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_UT1_GMST_HH
#define JEOD_TIME_CONVERTER_UT1_GMST_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "time_converter.hh"

//! Namespace jeod
namespace jeod
{

class TimeGMST;
class TimeUT1;
class JeodBaseTime;

/**
 * Converts between Universal Time and Greenwich Mean Sidereal Time.
 */
class TimeConverter_UT1_GMST : public TimeConverter
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeConverter_UT1_GMST)

    // Member Data
private:
    /**
     * Converter parent time, always a TimeUT1 for this converter.
     */
    TimeUT1 * ut1_ptr{}; //!< trick_units(--)

    /**
     * Converter parent time, always a TimeGMST for this converter.
     */
    TimeGMST * gmst_ptr{}; //!< trick_units(--)

    // Member functions:
public:
    TimeConverter_UT1_GMST();
    ~TimeConverter_UT1_GMST() override = default;
    TimeConverter_UT1_GMST(const TimeConverter_UT1_GMST &) = delete;
    TimeConverter_UT1_GMST & operator=(const TimeConverter_UT1_GMST &) = delete;

    // Initialize the converter
    void initialize(JeodBaseTime * parent, JeodBaseTime * child, const int direction) override;

    // convert_a_to_b: Apply the converter in the forward direction
    void convert_a_to_b() override;
};

/*----------------------------------------------------------------------------*/

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
