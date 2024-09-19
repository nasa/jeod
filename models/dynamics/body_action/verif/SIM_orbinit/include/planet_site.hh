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
/*******************************************************************************

Purpose:
  (Define the class PlanetSite, which defines a point on the Earth.)

Library dependencies:
  ((../src/planet_site.cc))

*******************************************************************************/

#ifndef JEOD_PLANET_SITE_HH
#define JEOD_PLANET_SITE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "utils/planet_fixed/north_east_down/include/north_east_down.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/alt_lat_long_state.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/*******************************************************************************
Purpose: (Define a point on the Earth.)
*******************************************************************************/
class PlanetSite
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PlanetSite)

    // Member data

public:
    NorthEastDown ned; /* trick_units(--) @n
       The NorthEastDown frame plus spherical/elliptical selector. */

    std::string site_id; /* trick_units(--) @n
       Site identifier. */

    AltLatLongState loc; /* trick_units(--) @n
       User-input site location. */

    NorthEastDown::AltLatLongType altlatlong_type{NorthEastDown::elliptical}; /* trick_units(--) @n
       Use spherical or elliptical coordinates? */

private:
    DynManager * local_dm{};

    // Methods

public:
    // Default constructor and destructor
    PlanetSite() = default;
    ~PlanetSite();

    // initialize(): Initialize the site.
    void initialize(Planet & planet, DynManager & dyn_manager);

    PlanetSite(const PlanetSite &) = delete;
    PlanetSite & operator=(const PlanetSite &) = delete;
};

} // namespace jeod

#endif
