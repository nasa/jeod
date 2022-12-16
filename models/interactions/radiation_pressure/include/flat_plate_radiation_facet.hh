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
 * @file models/interactions/radiation_pressure/include/flat_plate_radiation_facet.hh
 * Individual facets for use with rad environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((flat_plate_radiation_facet.o))


*******************************************************************************/

#ifndef JEOD_FLAT_PLATE_RADIATION_FACET_HH
#define JEOD_FLAT_PLATE_RADIATION_FACET_HH


// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "radiation_facet.hh"
//#include "radiation_base_facet.hh"


//! Namespace jeod
namespace jeod {

class RadiationThirdBody;
class FlatPlate;

/**
 * A flat plate facet to be used for radiation interaction
 */
class FlatPlateRadiationFacet : public RadiationFacet {

   JEOD_MAKE_SIM_INTERFACES(FlatPlateRadiationFacet)


   // Member data

public:
   /**
    * Unit vector normal to the plate surface, pointing outward
    * (structural frame). once the radiation surface is initialized,
    * it points to the normal found in FlatPlate
    */
   double* normal; //!< trick_units(--)

   /**
    * Temporary value.
    */
   double incident_flux_hat[3]; //!< trick_units(--)

private:
   /**
    * Theta is the angle between the plate and the radiation vector
    */
   double sin_theta; //!< trick_units(--)


   //Member methods
public:
   // constructor
   FlatPlateRadiationFacet ();

   // destructor
   virtual ~FlatPlateRadiationFacet ();

   void incident_radiation (
      const double flux_mag,
      const double flux_struct_hat[3],
      const bool calculate_forces);

   void initialize_geom (double center_grav[3]);

   void define_facet (FlatPlate * flat_plate);

   void radiation_pressure (void);

protected:

private:

   FlatPlateRadiationFacet& operator = (const FlatPlateRadiationFacet& rhs);
   FlatPlateRadiationFacet (const FlatPlateRadiationFacet& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
