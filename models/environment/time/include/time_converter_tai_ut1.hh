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
 * @file models/environment/time/include/time_converter_tai_ut1.hh
 * Define class TimeConverter_TAI_UT1, which converts between
 * International Atomic Time and Universal Time.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))



Library dependencies:
  ((../src/time_converter_tai_ut1.cc))
*******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_TAI_UT1_HH
#define JEOD_TIME_CONVERTER_TAI_UT1_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "time_converter.hh"

//! Namespace jeod
namespace jeod
{

class TimeTAI;
class TimeUT1;
class JeodBaseTime;

/**
 * Define class TimeConverter_TAI_UT1, which converts between
 * International Atomic Time and Universal Time.
 */
class TimeConverter_TAI_UT1 : public TimeConverter
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeConverter_TAI_UT1)

    // Member Data
public:
    /**
     * "True" to enter user-specified tai-ut1 offset
     */
    bool override_data_table{}; //!< trick_units(--)

private:
    /**
     * Converter parent time, always a TimeTAI for this converter.
     */
    TimeTAI * tai_ptr{}; //!< trick_units(--)

    /**
     * Converter parent time, always a TimeUT1 for this converter.
     */
    TimeUT1 * ut1_ptr{}; //!< trick_units(--)

public:
    /**
     * User specified value (UT1 - TAI)
     */
    double tai_to_ut1_override_val{}; //!< trick_units(s)

    /**
     * Index of last datum in table.
     */
    int last_index{}; //!< trick_units(--)

    /**
     * Current location in table.
     */
    int index{-1}; //!< trick_units(--)

    /**
     * Vector of values of difference between TAI-UT1.
     */
    double * val_vec{}; //!< trick_units(s)

    /**
     * Vector of corresponding times
     */
    double * when_vec{}; //!< trick_units(day)

private:
    /**
     * Time of previous calibrated datum.
     */
    double prev_when{}; //!< trick_units(day)

    /**
     * Offset value of previous datum.
     */
    double prev_value{}; //!< trick_units(s)

    /**
     * Time of next calibrated datum.
     */
    double next_when{}; //!< trick_units(day)

    /**
     * Offset value of next datum.
     */
    double next_value{}; //!< trick_units(s)

    /**
     * Rate at which "value" changes wrt "when".
     */
    double gradient{}; //!< trick_units(--)

    /**
     * Gone past the end of the table.
     */
    bool off_table_end{}; //!< trick_units(--)

    // Member Functions
public:
    TimeConverter_TAI_UT1();
    ~TimeConverter_TAI_UT1() override;
    TimeConverter_TAI_UT1(const TimeConverter_TAI_UT1 &) = delete;
    TimeConverter_TAI_UT1 & operator=(const TimeConverter_TAI_UT1 &) = delete;

    // Initialize the converter
    void initialize(JeodBaseTime * parent, JeodBaseTime * child, const int direction) override;

    // convert_a_to_b: Apply the converter in the forward direction
    void convert_a_to_b() override;

    // convert_b_to_a: Apply the converter in the reverse direction
    void convert_b_to_a() override;

private:
    // initialize_tai_to_ut1 tables:
    void initialize_tai_to_ut1();

    // used at time reversals to verify the ends of the lookup table
    void verify_table_lookup_ends() override;
};

/*----------------------------------------------------------------------------*/

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
