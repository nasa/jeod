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
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/include/radiation_default_surface.hh
 * Individual facets for use with radiation environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((../src/radiation_default_surface.cc))


*******************************************************************************/

#ifndef JEOD_RADIATION_DEFAULT_SURFACE_HH
#define JEOD_RADIATION_DEFAULT_SURFACE_HH

// System includes
#include <string>
#include <utility>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "interactions/thermal_rider/include/thermal_facet_rider.hh"

// Model includes
#include "radiation_base_facet.hh"


//! Namespace jeod
namespace jeod {

class RadiationThirdBody;
class DynBody;

/**
 * The default spherical surface for radiation pressure
 */
class RadiationDefaultSurface : public RadiationBaseFacet {

   JEOD_MAKE_SIM_INTERFACES(RadiationDefaultSurface)

public:

   //  Must be defined in input or Modified_data file

   /**
    * The radiation-equivalent of a drag coefficient.  It is the value by
    * which the area must be multiplied in order to generate the same force
    * if the reflecting surface were considered a perfectly absorbing
    * surface.  For a sphere, this has a value between 1.0 (perfectly
    * absorbing, or perfect specular reflection) to 1.4444 (all diffuse
    * reflection). Specify either:  rad_coeff OR (albedo AND diffuse).
    * NOTE 1 - this value is used at initialization only; changes to its
    * value mid-simulation cannot be effected.
    * NOTE 2 - the values albedo and diffuse are inherited from
    * RadiationBaseFacet.
    */
   double rad_coeff;  //!< trick_units(--)

   /**
    * The value of the surface kinetic temperature.
    */
   double temperature; //!< trick_units(K)

   /**
    * The name of the surface. This is optional.
    */
   std::string name;  //!< trick_units(--)


   //  Either surface_area, or cx_area, but not both, must be defined.
   //  Spherical geometry is assumed, so surface_area = 4 * cx_area.
   //  cx_area is inherited from RadiationBaseFacet.

   /**
    * surface area of the default sphere.
    */
   double surface_area; //!< trick_units(m2)

   // run-time values, not defined prior to simulation.

protected:


   // Member methods

public:

   // constructor
   RadiationDefaultSurface ();

   // destructor
   ~RadiationDefaultSurface () override;

   void initialize (void) override;

   void incident_radiation ( const double flux_mag,
                                     const double flux_struc_hat[3],
                                     const bool calculate_forces) override;

   void thermal_update( void );

   void add_thermal_integrator_to (DynBody * dyn_body);


   /**
    * Setter for the name.
    */
   void set_name (std::string name_in)
   {
      name = std::move(name_in);
   }


private:
   RadiationDefaultSurface& operator = (const RadiationDefaultSurface& rhs);
   RadiationDefaultSurface (const RadiationDefaultSurface& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
