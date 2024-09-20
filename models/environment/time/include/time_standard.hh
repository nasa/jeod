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
 * @file models/environment/time/include/time_standard.hh
 * A class that serves as the base for all time
 * representations that are well defined outside the simulation.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((The default names on time standards should not be changed))



Library dependencies:
  ((../src/time_standard.cc))
******************************************************************************/

#ifndef JEOD_TIME_STANDARD_HH
#define JEOD_TIME_STANDARD_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// model includes
#include "time.hh"

//! Namespace jeod
namespace jeod
{

class TimeManagerInit;

/*----------------------------------------------------------------------------*/
/**
 * A class that serves as the base for all time
 * representations that are well defined outside the simulation.
 */
class TimeStandard : public JeodBaseTime
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeStandard)

    friend class TimeUDE;

    // Member Data
public:
    /**
     * The simtime when the calendar update was last run.
     */
    double last_calendar_update{-100000.0}; //!< trick_units(--)

    /**
     * Used for determining whether to update the
     * date in the calendar function
     */
    int prev_julian_day{-1000000000}; //!< trick_units(day)

    /**
     * The value of "seconds" at the start of the year in which the last
     * seconds_of_year calculation was made.  Used for
     * seconds_of_year calculations only.
     */
    double seconds_at_year_start{}; //!< trick_units(s)

    /**
     * The year in which the last seconds_of_year calculation was made.  At the
     * start of this year, seconds had value seconds_at_year_start.  Used for
     * seconds_of_year calculations only.
     */
    int year_of_last_soy{-1000000}; //!< trick_units(--)

    /**
     * This flag can be turned off by developers wanting to avoid warnings
     * about a simulation being initialized pre-1968.
     * The flag defaults to true - warning will be sent.
     */
    bool send_warning_pre_1968{true}; //!< trick_units(--)

    /**
     * Difference between Truncated Julian and Modified Julian.
     */
    const double tjt_mjt_offset{40000.0}; //!< trick_units(day)

    /**
     * Difference between Julian and Truncated Julian.
     */
    const double tjt_jd_offset{2440000.5}; //!< trick_units(day)

    /**
     * Truncated Julian time for this time-type.
     */
    double trunc_julian_time{}; //!< trick_units(day)

    /**
     * Conventional Julian Date.  NOTE - because this value is typically so
     * large, it has very little room for fine-detail precision.  It should only
     * ever be used as an output for the likes of terminal displays and for input
     * to legacy code.  Never use for newly developed code.
     */
    double julian_date{}; //!< trick_units(day)

    /**
     * Truncated Julian Date at epoch.
     */
    double tjt_at_epoch{}; //!< trick_units(day)

    /**
     * Gregorian calendar date day number.
     */
    int calendar_day{}; //!< trick_units(day)

    /**
     * 24-hour clock hour number.
     */
    int calendar_hour{}; //!< trick_units(hr)

    /**
     * Clock minute number.
     */
    int calendar_minute{}; //!< trick_units(min)

    /**
     * Clock second number.
     */
    double calendar_second{}; //!< trick_units(s)

    /**
     * Gregorian calendar year.
     */
    int calendar_year{}; //!< trick_units(--)

    /**
     * Gregorian calendar month.
     */
    int calendar_month{}; //!< trick_units(--)

    // Member functions:
public:
    TimeStandard() = default;
    ~TimeStandard() override = default;
    TimeStandard(const TimeStandard &) = delete;
    TimeStandard & operator=(const TimeStandard &) = delete;

    void calendar_update(double simtime);
    void initialize_initializer_time(TimeManagerInit * tm_init) override;
    void add_type_initialize(const int seeking_status, TimeManagerInit * tm_init) override;
    void initialize_from_parent(TimeManagerInit * tm_init) override;

    void set_time_by_seconds(const double new_seconds) override;
    void set_time_by_days(const double new_days) override;
    void set_time_by_trunc_julian(const double new_tjt);

    double julian_date_at_epoch();

    double seconds_of_year();

protected:
    virtual void convert_from_calendar();

    virtual void calculate_calendar_values();

    /**
     * Set the epoch time.
     */
    virtual void set_epoch() = 0;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
