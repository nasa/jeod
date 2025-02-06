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
 * @addtogroup PlanetFixed
 * @{
 * @addtogroup PlanetFixedPosn
 * @{
 *
 * @file models/utils/planet_fixed/planet_fixed_posn/include/alt_lat_long_state.hh
 * Basic orthogonal Altitude-Latitude-Longitude state definition.
 */

/********************************* TRICK HEADER *******************************
Purpose:
   ()

Library Dependency:
   ((../src/alt_lat_long_state.cc))



*******************************************************************************/

#ifndef JEOD_ALT_LAT_LONG_STATE_HH
#define JEOD_ALT_LAT_LONG_STATE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Specifies positional state in planetary altitude, latitude, and longitude.
 */
class AltLatLongState
{
    JEOD_MAKE_SIM_INTERFACES(jeod, AltLatLongState)

    // Member Data
public:
    /**
     * An object's height above the reference surface of the local GravBody
     */
    double altitude; //!< trick_units(m)

    /**
     * An object's north-south angular offset from the local GravBody's
     * reference equator
     */
    double latitude; //!< trick_units(rad)

    /**
     * An object's east-west angular offset from the local GravBody's
     * reference prime meridian
     */
    double longitude; //!< trick_units(rad)

    // Member functions
public:
    // Constructor
    AltLatLongState();

    /**
     * Destructor
     */
    virtual ~AltLatLongState() = default;

    // Data accessor methods
    void set_data(double alt, double lat, double lon);
    void get_data(double & alt, double & lat, double & lon);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
