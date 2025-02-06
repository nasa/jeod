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
 * @file models/interactions/radiation_pressure/include/radiation_third_body.hh
 * Define the class RadiationThirdBody.
 */

/*******************************************************************************
Purpose:
    ()

Library dependencies:
     ((radiation_third_body.o))



*****************************************************************************/

#ifndef JEOD_RADIATION_THIRD_BODY_HH
#define JEOD_RADIATION_THIRD_BODY_HH

// System includes
#include <string>
#include <utility>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

class RadiationSource;
class RadiationBaseFacet;
class RefFrame;
class DynManager;
class Planet;

/**
 * Provide information on bodies that may cause shadowing or reflected
 * illumination.
 */
class RadiationThirdBody  {

   JEOD_MAKE_SIM_INTERFACES(RadiationThirdBody)

public:
   /**
    * distinguishes between conical and cylindrical shadowing
    */
  enum ShadowGeometry {
      Cylindrical = 0 , /**< planet casts a cylindrical shadow */
      Cyl = 1 ,             /**< planet casts a cylindrical shadow */
      Conical = 2 ,         /**< planet casts a conical shadow */
      Con = 3               /**< planet casts a conical shadow */
  };

   /**
    * Pointer to the primary illumination source
    */
  RadiationSource * primary_source_ptr; //!< trick_units(--)

   /**
    * Flag indicating cylindrical / conical shadow geometry
    */
  ShadowGeometry  shadow_geometry ; //!< trick_units(--)

   /**
    * Flag to indicate whether to enforce an update to the third-body body-state.
    * Default: true
    * Self-resets to false after each use.
    */
   bool force_state_update; //!< trick_units(--)

   /**
    * Name of planet
    */
  std::string name ; //!< trick_units(--)

   /**
    * Model on/off flag
    */
  bool active;   //!< trick_units(--)

   /**
    * Time interval between third body state updates.
    */
  double update_interval;  //!< trick_units(s)

   /**
    * Radius of RadiationThirdBody. Note - this is set automatically if RTB
    * is a planet, but must be set manually if RTB is a DynBody.
    */
  double radius;  //!< trick_units(m)

   /**
    * Calculated value.  Set only in derived class
    * RadiationReflectingThirdBody.
    */
  double output_flux[3];  //!< trick_units(--)

   /**
    * Pointer to the local reference frame, for determining position of
    * this object.
    */
  RefFrame * local_frame_ptr; //!< trick_units(--)


protected:

 // Reference data:
   /**
    * Flag to indicate whether body has already been added to a model
    */
   bool added_to_model; //!< trick_units(--)

   /**
    * Flag to indicate that initialization has been complete.
    */
   bool initialized; //!< trick_units(--)

   /**
    * Value between 0 and 1 representing the fraction of possible flux that is
    * actually incident after shadowing effects.
    */
   double illum_factor;   //!< trick_units(--)

   /**
    * Time at which the third body state was last updated.
    */
   double last_update_time;  //!< trick_units(s)

 // Specification data for RadiationThirdBody:
   /**
    * Pointer to the associated Planet object.
    */
   Planet * planet_link; //!< trick_units(--)

   /**
    * The value (radius + R_source)
    */
   double r_plus; //!< trick_units(--)

   /**
    * The value (radius - R_source)
    */
   double r_minus; //!< trick_units(--)

   /**
    * The ratio (radius / R_source)
    */
   double r_ratio; //!< trick_units(--)


 //Relative position data for vehicle with respect to RTB:
   /**
    * Component of vector from RadiationThirdBody to vehicle that is parallel to
    * vector from primary source to RadiationThirdBody.
    */
   double r_par;  //!< trick_units(m)

   /**
    * Component of vector from RadiationThirdBody to vehicle that is
    * perpendicular to vector from primary source to RadiationThirdBody.
    */
   double r_perp; //!< trick_units(--)

   /**
    * Vector from RadiationThirdBody to vehicle center of gravity,
    * in inertial RF.
    */
   double third_to_cg_inrtl[3];  //!< trick_units(m)



 // Specification of RTB position:
   /**
    * Vector from the primary source to the RadiationThirdBody in inertial RF.
    */
   double source_to_third_inrtl[3]; //!< trick_units(m)

   /**
    * Magnitude of source_to_third.
    */
   double d_source_to_third; //!< trick_units(m)

   /**
    * Unit vector of source_to_third.
    */
   double source_to_third_hat_inrtl[3]; //!< trick_units(--)



// Member functions
public:
   RadiationThirdBody ();
   virtual ~RadiationThirdBody (){};


   virtual void initialize (DynManager * dyn_mgr_ptr);


   void calculate_shadow (void);

   void convert_shadow_from_int (int geometry);

   virtual double process_third_body( double real_time,
                                      RefFrame & veh_struc_frame );

   /**
    * Setter for the name.
    */
   void set_name (std::string name_in)
   {
      name = std::move(name_in);
   }

   /**
    * To provide base class null implementation
    * @param veh_surf_elem The facet of the vehicle.
    * @param calculate_forces pass-through flag.
    */
   virtual void accumulate_refl_flux( RadiationBaseFacet * veh_surf_elem JEOD_UNUSED ,
                                      bool calculate_forces JEOD_UNUSED ) {};

   /**
    * To provide base class null implementation
    * @param veh_surf_elem The facet of the vehicle.
    * @param calculate_forces pass-through flag.
    */
   virtual void accumulate_rad_flux( RadiationBaseFacet * veh_surf_elem JEOD_UNUSED ,
                                     bool calculate_forces JEOD_UNUSED ) {};

   /**
    * Identifies this class as one that does not produce a radiaiton field
    * @return false
    */
   virtual bool is_interactive() {return false;}

   /**
    * Returns information on whether the body has already been used in one
    * RadiationPressure model.  Protects against multiple use of one
    * ThirdBody instance.
    * @return added_to_model
    */
   bool get_added_to_model() {return added_to_model;}

   /**
    * Sets the added_to_model value
    * @param value New value
    */
   void set_added_to_model( bool value) { added_to_model = value;}


protected:
   double generate_alpha( double rho_adj, double delta);
   bool test_for_state_update( double time );
   virtual bool update_third_body_state( void );




 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:
   RadiationThirdBody (const RadiationThirdBody&);
   RadiationThirdBody & operator = (const RadiationThirdBody&);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
