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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/include/jeod_integration_time.hh
 * Define the class JeodIntegrationTime.
 */

/******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/jeod_integration_time.cc))



******************************************************************************/

#ifndef JEOD_INTEGRATION_TIME_HH
#define JEOD_INTEGRATION_TIME_HH

// System includes

// ER7 utilities includes
#include "er7_utils/integration/core/include/time_interface.hh"

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class TimeChangeSubscriber;

/**
 * The class JeodIntegrationTime adds the concepts of timestamping and
 * notification of changes in the nature of time to the ER7 numerical
 * utilities TimeInterface class.
 */
class JeodIntegrationTime : public er7_utils::TimeInterface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodIntegrationTime)

public:
    JeodIntegrationTime();
    ~JeodIntegrationTime() override;
    JeodIntegrationTime(const JeodIntegrationTime &) = delete;
    JeodIntegrationTime & operator=(const JeodIntegrationTime &) = delete;

    /**
     * Get the time used to timestamp some object.
     */
    virtual double get_timestamp_time() const = 0;

    // Add a time change subscriber.
    void add_time_change_subscriber(TimeChangeSubscriber & subscriber);

    // Remove a time change subscriber.
    void remove_time_change_subscriber(TimeChangeSubscriber & subscriber);

protected:
    // Member functions

    // Notify subscribers that the nature of time has changed.
    void notify_time_change_subscribers();

private:
    // Member data

    /**
     * List of pointers to objects that wish to be notified of a change
     * in the nature of time.
     */
    JeodPointerVector<TimeChangeSubscriber>::type time_change_subscribers; //!< trick_io(**)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
