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
 * @addtogroup Utils
 * @{
 * @addtogroup PlanetFixed
 * @{
 * @addtogroup NorthEastDown
 * @{
 *
 * @file models/utils/planet_fixed/north_east_down/include/north_east_down.hh
 * Implementation of the North-East-Down reference frame.
 */

/********************************* TRICK HEADER *******************************
Purpose:
   ()

Library Dependency:
   ((../src/north_east_down.cc))


*******************************************************************************/

#ifndef JEOD_NORTH_EAST_DOWN_HH
#define JEOD_NORTH_EAST_DOWN_HH

// System includes

// JEOD includes
#include "utils/ref_frames/include/ref_frame.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "utils/planet_fixed/planet_fixed_posn/include/class_declarations.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Defines a local North-East-Down reference frame.
 */
class NorthEastDown : public PlanetFixedPosition
{
    JEOD_MAKE_SIM_INTERFACES(jeod, NorthEastDown)

    // Member Data
public:
    /**
     * The local North-East-Down reference frame, centered at the reference
     * point stored in the ellip_coords, sphere_coords, and cart_coords data
     * fields inherited from PlanetFixedPosition
     */
    RefFrame ned_frame; //!< trick_units(--)

    /**
     * Specifies whether the latitude is spherical or elliptical.
     */
    enum AltLatLongType
    {
        undefined = -1, /*
        Default value, which will lead to an error if left unchanged */

        spherical, /*
   Spherical coordinates form basis for the orientation of the
   North-East-Down frame to which this enum refers */

        elliptical /*
   Elliptical coordinates form basis for the orientation of the
   North-East-Down frame to which this enum refers */
    };

    /**
     * Is reference point specified in spherical or elliptical coords?
     */
    AltLatLongType altlatlong_type{undefined}; //!< trick_units(--)

    // Member functions
public:
    NorthEastDown() = default;
    ~NorthEastDown() override = default;
    NorthEastDown(const NorthEastDown &) = delete;
    NorthEastDown & operator=(const NorthEastDown &) = delete;

    // Update from Cartesian position input
    void update_from_cart(const double cart[3]) override;

    // Update from spherical position input
    void update_from_spher(const AltLatLongState & spher) override;

    // Update from elliptical position input
    void update_from_ellip(const AltLatLongState & ellip) override;

    // Build NED frame orientation based on current reference point information
    virtual void build_ned_orientation();

    // Set NED frame translational states using given values
    virtual void set_ned_trans_states(const double pos[3], const double vel[3]);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
