//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @file models/interactions/radiation_pressure/include/radiation_base_facet.hh
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
    (radiation_base_facet.o)


*******************************************************************************/

#ifndef JEOD_RADIATION_BASE_FACET_HH
#define JEOD_RADIATION_BASE_FACET_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/interaction_facet.hh"
#include "interactions/thermal_rider/include/thermal_facet_rider.hh"

// Model includes


//! Namespace jeod
namespace jeod {

class RadiationParams;
class Facet;
class RadiationThirdBody;

/**
 * Generic type of facet for radiation pressure
 */
class RadiationBaseFacet {

   JEOD_MAKE_SIM_INTERFACES(RadiationBaseFacet)

public:

// defined by the parameter list, common to a type of facet material.

   /**
    * Usable value of albedo, set to either albedo_IR or albedo_vis,
    * depending on situation.
    */
  double albedo;     //!< trick_units(--)

   /**
    * Fraction of incident visible radiation that is immediately reflected.
    */
  double albedo_vis;     //!< trick_units(--)

   /**
    * Fraction of incident IR radiation that is immediately reflected.
    */
  double albedo_IR;  //!< trick_units(--)

   /**
    * Fraction of reflected radiation that is reflected diffusely (balance
    * reflected specularly)
    */
  double diffuse;    //!< trick_units(--)

   /**
    * thermal characteristics of the facet.
    */
  ThermalFacetRider thermal; //!< trick_units(--)



   /**
    * cross-sectional area projected perpendicular to the radiation vector.
    * For facets that have an orientation, this is intended to be a
    * variable value, dependent upon the orientation.  For spherical
    * surfaces, this can be set at initialization.
    */
  double cx_area;  //!< trick_units(m2)


// run-time values, typically not defined prior to simulation.

   /**
    * product of momentum flux and cross-sectional area (cx_area).
    * Highly variable.
    */
  double areaxflux; //!< trick_units(--)

   /**
    * product of energy flux and cross-sectional area
    */
  double areaxflux_e; //!< trick_units(--)

   /**
    * Force due to photon absorption from ONLY ONE source.
    */
  double F_absorption[3];  //!< trick_units(--)

   /**
    * Force due to photon specular reflection from ONLY ONE source.
    */
  double F_specular[3]; //!< trick_units(--)

   /**
    * Force due to photon diffuse reflection from ONLY ONE source.
    */
  double F_diffuse[3]; //!< trick_units(--)

   /**
    * Force due to photon (thermal) emission.
    */
  double F_emission[3]; //!< trick_units(--)

protected:

// shared between plates
   /**
    * Speed of light in vacuum
    */
  const static double speed_of_light; //!< trick_io(*o) trick_units(m/s)




// Member methods

public:

   // constructor
   RadiationBaseFacet ();

   // destructor
   virtual ~RadiationBaseFacet ();

   /*
     Purpose:
       (Initializes the general radiation facet)
    */
   virtual void initialize( void );


   /*
      Purpose:( Runs the methods to obtain fluxes from third body objects.)
   */
   virtual void interact_with_third_body(
      RadiationThirdBody * third_body,
      const bool calc_forces);

   /*
      Purpose:( Zeroes elements to allow for incremental calculations.)
   */
  virtual void initialize_runtime_values( void);

      /**
    * Calculates the effect on the facet of the incident radiation
    * \param[in] flux_mag Magnitude of the incident flux
    * \param[in] flux_hat unit vector associated with the incident flux vector
    * \param[in] calc_forc true/false, do forces get calculated
    */
   virtual void incident_radiation(
      const double flux_mag,

      const double flux_hat[3],

      const bool calc_forc)
 = 0;




private:
   RadiationBaseFacet& operator = (const RadiationBaseFacet& rhs);
   RadiationBaseFacet (const RadiationBaseFacet& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
