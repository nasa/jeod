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
 * @addtogroup Spice
 * @{
 *
 * @file models/environment/spice/include/spice_ephem_orient.hh
 * Define class SpiceEphemOrient, which extends EphemerisOrientation for use
 * with the SPICE ephemeris model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/spice_ephem_orient.cc))



*******************************************************************************/

#ifndef JEOD_SPICE_EPHEM_ORIENT_HH
#define JEOD_SPICE_EPHEM_ORIENT_HH

// System includes
#include <string>

// JEOD includes
#include "environment/ephemerides/ephem_item/include/ephem_orient.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes

//! Namespace jeod
namespace jeod
{

/**
 * A SpiceEphemOrientation minimally extends EphemerisOrientation to include
 * the JEOD and SPICE names and an update method
 * for the target ephemeris reference frame.
 */
class SpiceEphemOrientation : public EphemerisOrientation
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SpiceEphemOrientation)

public:
    // Member functions
    SpiceEphemOrientation();
    ~SpiceEphemOrientation() override = default;
    SpiceEphemOrientation(const SpiceEphemOrientation &) = delete;
    SpiceEphemOrientation & operator=(const SpiceEphemOrientation &) = delete;

    // Update the rotational state of the target frame.
    void update(double time_tdb, double time_dyn);

    // Confirm that the target frame exists in the loaded kernels.
    void validate(double time_tdb);

    // Populate the SPICE 6 x 6 matrix via sxform_c().
    void get_spice_transformation(double time_tdb, double trans6x6[6][6]);

    /**
     * Setter for the name of the SPICE frame.
     * \param new_name  Name of the SPICE frame
     */
    void set_spice_frame_name(const std::string & new_name)
    {
        spice_frame_name = new_name;
    }

private:
    // Member data
    /**
     * SPICE name of the target reference frame
     */
    std::string spice_frame_name; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
