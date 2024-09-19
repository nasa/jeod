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
 * @file models/environment/time/include/time_enum.hh
 * Contains an enumeration of the formats in which time can be represented.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))


******************************************************************************/

#ifndef JEOD_TIME_ENUM_HH
#define JEOD_TIME_ENUM_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Contains an enumeration of the formats in which time can be represented.
 */
class TimeEnum
{
    // Member Data
public:
    /**
     * The enumeration of the formats in which time can be represented.
     */
    enum TimeFormat
    {
        undefined = -1,     /**< Default setting. */
        Julian,             /**< Full Julian representation. */
        julian,             /**< Full Julian representation. */
        modified_julian,    /**< Modified-Julian representation. */
        truncated_julian,   /**< Truncated-Julian representation. */
        calendar,           /**< Calendar (Gregorian)  representation. */
        clock,              /**< "Calendar" representation for MET. */
                            /* (no years and months). */
        days_since_epoch,   /**< Days since the type's defined epoch. */
        seconds_since_epoch /**< Seconds since the type's defined epoch. */
    };
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
