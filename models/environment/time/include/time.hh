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
 * @file models/environment/time/include/time.hh
 * JeodBaseTime is an abstract class, containing the basic structure of all
 * clocks that run in JEOD.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))



Library dependencies:
  ((../src/time.cc)
   (../src/time__add_type_update.cc))
******************************************************************************/

#ifndef JEOD_TIME_HH
#define JEOD_TIME_HH

// System includes
#include <string>
#include <utility>

// JEOD includes
#include "utils/named_item/include/named_item.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "time_converter.hh"
#include "time_links.hh"

//! Namespace jeod
namespace jeod
{

class TimeManager;
class TimeManagerInit;

/**
 * JeodBaseTime is an abstract class, containing the basic structure of all
 * clocks that run in JEOD.
 */
class JeodBaseTime
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodBaseTime)

    friend class TimeConverter;
    friend class TimeManagerInit;

    // Member Data
public:
    /**
     * Value used to define sim start time.
     */
    double initializing_value{}; //!< trick_units(--)

    /**
     * Determines which converter function (a_to_b (+1) or b_to_a (-1)) to use.
     */
    int update_converter_direction{}; //!< trick_units(--)

    /**
     * Elapsed time from epoch
     */
    double seconds{}; //!< trick_units(s)

    /**
     * Name of time-type.
     */
    NamedItem name; //!< trick_units(--)

    /**
     * Name of time-type from which initial value is derived
     */
    std::string initialize_from_name{""}; //!< trick_units(--)

    /**
     * Name of time-type from which update values are derived
     */
    std::string update_from_name{""}; //!< trick_units(--)

    /**
     * Pointer to the TimeManager
     */
    TimeManager * time_manager{}; //!< trick_units(--)
    /**
     * Pointer to the converter class needed to update the time
     */
    TimeConverter * update_converter_ptr{}; //!< trick_units(--)

protected:
    /**
     * Index-value of time-type in the registry
     */
    int index{}; //!< trick_units(--)
    /**
     * Whether time has been initialized to a real time.
     */
    bool initialized{}; //!< trick_units(--)
    /**
     * Elapsed time from epoch
     */
    double days{}; //!< trick_units(day)

    /**
     * Value of "seconds" at the start of the sim.
     */
    double initial_value{}; //!< trick_units(s)

    /**
     * The resolution limit when generating clock and calendar-clock values.
     * Used for forcing a "tick-over" to the next minute if seconds comes
     * within this value of 60.
     */
    double clock_resolution{0.0001}; //!< trick_units(s)

    /**
     * Linkage to the hierarchy of time conversions. Provides accessors to
     * parent, siblings and children
     */
    TimeLinks links; //!< trick_units(--)

    // Member functions:
public:
    JeodBaseTime();
    virtual ~JeodBaseTime();
    JeodBaseTime(const JeodBaseTime &) = delete;
    JeodBaseTime & operator=(const JeodBaseTime &) = delete;

    virtual bool must_be_singleton();

    virtual void set_time_by_seconds(const double new_seconds);
    virtual void set_time_by_days(const double new_days);
    void add_type_update(const int seeking_status, TimeManagerInit * tm_init);

    /**
     * Setter for the name.
     */
    void set_name(std::string name_in)
    {
        name = std::move(name_in);
    }

    /**
     * Setter for the index (force user to be carefule)
     */
    void set_index(int idx)
    {
        index = idx;
    }

    /**
     * Getter for the index
     */
    int get_index()
    {
        return index;
    }

    /**
     * Force reset the initialization status
     */
    void override_initialized(bool init)
    {
        initialized = init;
    }

    /**
     * Read the initialization status
     */
    bool is_initialized()
    {
        return initialized;
    }

    /**
     * Initialize the time class that is used for initialization of the
     * simulation.
     * @param tm_init Time initializer.
     */
    virtual void initialize_initializer_time(TimeManagerInit * tm_init) = 0;

    virtual void add_type_initialize(const int seeking_status, TimeManagerInit * tm_init);
    virtual void initialize_from_parent(TimeManagerInit * tm_init);

    virtual void update();

protected:
    void add_parent(JeodBaseTime & parent);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
