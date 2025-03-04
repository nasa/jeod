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
 * @file models/environment/time/include/time_links.hh
 * Define the class TimeLinks, which defines the hierarchy of JEOD
 * time conversions.
 */

/*******************************************************************************

Purpose:
 ()



*******************************************************************************/

#ifndef MODELS_ENVIRONMENT_TIME_INCLUDE_TIME_LINKS_HH_
#define MODELS_ENVIRONMENT_TIME_INCLUDE_TIME_LINKS_HH_

// System includes

// Model includes
#include "class_declarations.hh"

// JEOD includes
#include "utils/ref_frames/include/tree_links.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class TimeLinks;

class TimeLinks : public TreeLinks<TimeLinks, JeodBaseTime, TimeMessages>
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeLinks)

    // Member functions
public:
    explicit TimeLinks(JeodBaseTime & time_in)
        : TreeLinks<TimeLinks, JeodBaseTime, TimeMessages>(time_in, default_path_size)
    {
    }

    // The default constructor, copy constructor, and copy assignment
    // operator don't make sense and are deleted.
    TimeLinks() = delete;
    ~TimeLinks() override = default;
    TimeLinks(const TimeLinks &) = delete;
    void operator=(const TimeLinks &) = delete;

    // Member attributes
private:
    /**
     * Default allocated number of entries in linkage container
     */
    const static unsigned int default_path_size = 8; //!< trick_units(--)
};

} // namespace jeod

#endif /* MODELS_ENVIRONMENT_TIME_INCLUDE_TIME_LINKS_HH_ */

/**
 * @}
 * @}
 * @}
 */
