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
 * @file models/environment/time/include/time_manager_init.hh
 * To initialize the Time Manager.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))


Library dependencies:
  ((../src/time_manager_init.cc))
******************************************************************************/

#ifndef JEOD_TIME_MANAGER_INIT_HH
#define JEOD_TIME_MANAGER_INIT_HH

// System includes
#include <string>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "time_enum.hh"

//! Namespace jeod
namespace jeod
{

class TimeManager;

/**
 * To initialize the Time Manager.
 */
class TimeManagerInit
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeManagerInit)

    // Member Data
public:
    /**
     * Count of the total number of time-types placed in the update tree
     * or in the initialization tree.
     */
    int num_added_total{}; //!< trick_units(--)

    /**
     * Calendar, truncated_julian, etc.
     */
    TimeEnum::TimeFormat sim_start_format{TimeEnum::undefined}; //!< trick_units(--)

    /**
     * Pointer to the Time Manager. Automatically linked during init routines.
     */
    TimeManager * time_manager{}; //!< trick_units(--)

    /**
     * Name of the time-type used for initialization
     */
    std::string initializer{""}; //!< trick_units(--)
protected:
    /**
     * Index-value of the initializer
     */
    int initializer_index{-1}; //!< trick_units(--)

    /**
     * Index-value of the type dyn-time
     */
    int dyn_time_index{-1}; //!< trick_units(--)

    /**
     * Count of number of time-types placed in the update tree
     * or in the initialization tree in any given pass.
     */
    int num_added_pass{-1}; //!< trick_units(--)

    // num_types * num_types elements in these vectors

    /**
     * List of the indices (in the TimeManager->time_converter_ptrs vector) of
     * all registered converters, sorted by the indices of the time-types the
     * converters act upon (most pairs of time-types have no converter
     * registered; the value of these indices is -1)
     */
    int * converter_ptrs_index{}; //!< trick_units(--)

    /**
     * List of directions available for initialization for each of the converters
     * listed in converter_class_ptrs
     */
    int * init_converter_dir_table{}; //!< trick_units(--)

    /**
     * List of directions available for run-time updates for each of the converters
     * listed in converter_class_ptrs
     */
    int * update_converter_dir_table{}; //!< trick_units(--)

    /**
     * A running ledger of properly linked times during update tree and
     * initialization tree construction. Entries correspond to times of shared
     * indexes in time_vector
     * ( e.g. status[2] : status of time_manager->time_vector[2] )
     * Update tree encoding:
     * -2: undefined. Requires auto-assignment or causes error.
     * -1: definitive error. Process will terminate.
     *  0: uninitialized
     *  1: THE 1st generation (root) time. dyn_time for update tree.
     *  2: a 2nd generation time, converted from root time.
     *  n: a nth gen time, converted from (n-1)th gen time.
     */
    int * status{}; //!< trick_units(--)

    // Member functions:
public:
    TimeManagerInit() = default;
    ~TimeManagerInit();
    TimeManagerInit(const TimeManagerInit &) = delete;
    TimeManagerInit & operator=(const TimeManagerInit &) = delete;

    int get_conv_ptr_index(const int conv_index);
    int get_conv_dir_init(const int conv_index);
    int get_conv_dir_upd(const int conv_index);
    int get_status(const int index);
    void set_status(const int index, const int status_value);
    void increment_status(const int slave_index, const int master_index);

    void initialize_manager(TimeManager * time_mgr);
    void organize_update_list();

private:
    void initialize();

    void verify_times_setup();

    void populate_converter_registry();

    void verify_converter_setup();

    void initialize_time_types();

    void create_init_tree();

    void create_update_tree();
};

/*----------------------------------------------------------------------------*/

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
