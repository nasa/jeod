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
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/include/planet_rnp.hh
 * Establish a framework for rotation-nutatation-precession style planet
 * attitude models.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((David A. Vallado)
    (Fundamentals of Astrodynamics and Applications, Third Edition)
    (Microcosm Press and Springer)
    (2007)))

Assumptions and limitations:
  ((This framework is limited to models which are of the form:
    celestial frame = Precession * Nutation *
                      Rotation * Polar Motion * terestrial frame
    When the entire model is updated, it will be updated:
    Precession - Nutation - Rotation - Polar Motion
    This is important when individual models are inter-related
    This can be overriden by an inheriting class, if so needed
    ))

Library dependencies:
  ((planet_rnp.o))

 

*******************************************************************************/


#ifndef PLANET_RNP_HH
#define PLANET_RNP_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "planet_orientation.hh"

//! Namespace jeod
namespace jeod {

class RefFrameRot;
class Planet;
class DynManager;
class PlanetRotation;

// The over arching RNP framework, including the possibility of polar motion
/**
 * The generic framework for orientation models based on the RNP paradigm.
 */
class PlanetRNP : public PlanetOrientation {

   JEOD_MAKE_SIM_INTERFACES(PlanetRNP)

public: //public member variables

// An enumeration that dictates the complexity of the RNP.

   /**
    * Specifies the initialization fidelity of the RNP model.
    */
   enum RNPFidelity {

      FullRNP = 0, ///< Full fidelity RNP matrix. Formerly Full_Term_RNP
      RotationOnly = 1, ///< Identity NP matrix, then rotation calculates linearly
                        // in time. Formerly Identity_RNP
      ConstantNP = 2 ///< Once calculated NP matrix (at the start), then rotation
                     // calculates linearly in time. Formerly Identity_NP
   };

   // These are pointers to the models used in this particular RNP for
   // nutation, precession, polar_motion and the rotation model for the planet.
   // Any of these can be left as NULL, and will then be ignored
   // in the total calculation

   /**
    * Pointer to the nutation model
    */
   PlanetRotation* nutation; //!< trick_units(--)
   /**
    * Pointer to the precession model
    */
   PlanetRotation* precession; //!< trick_units(--)
   /**
    * Pointer to the polar_motion model
    */
   PlanetRotation* polar_motion; //!< trick_units(--)
   /**
    * Pointer to the rotation model
    */
   PlanetRotation* rotation; //!< trick_units(--)

   /**
    * The fidelity of the RNP model
    */
   RNPFidelity rnp_type; //!< trick_units(--)
   /**
    * Gives the option of turning on or off polar motion
    */
   bool enable_polar; //!< trick_units(--)

   /**
    * Rotation Matrix representing:
    * transpose(nutation->rotation) * transpose(precession->rotation)
    */
   double NP_matrix[3][3]; //!< trick_units(--)

protected: // private member variables

   /**
    * A transformation matrix used for intermediate math steps
    */
   double scratch_matrix[3][3]; //!< trick_units(--)

public: // public member functions

   PlanetRNP ();

   virtual ~PlanetRNP ();

   // Invokes the calculation for all rotation models contained in the RNP,
   // based on the last time set in each model through
   // PlanetRotation::set_time . Then multiplies out and updates the attitude
   // of the planet found in the given dyn manager.
   void update_rnp ();

   // Invokes the calculation for the axial rotation model (the largest
   // contributor to rotation, the axial Z-rotation that causes days)
   // and then multiples out the RNP with the most recent calculations done
   // for nutation, precession and polar motion
   void update_axial_rotation ();

   // Multiples out the (up to) four planet rotation models (nutation,
   // precession, polar motion and rotation, based on
   // initialization options) and feeds it to the planet attitude found
   // in the dyn manager the RNP was initialized from
   void propagate_rnp ();

   /**
    * A re-declaration of the pure virtual function in order
    * to convince trick that yes, this is a pure virtual class.
    * @return Planet name.
    */
   virtual const char* get_name() const = 0;


private: // private member functions

   // operator = and copy constructor locked from use by being private

   /**
    * Not implemented.
    */
   PlanetRNP& operator = (const PlanetRNP& rhs);

   /**
    * Not implemented.
    */
   PlanetRNP (const PlanetRNP& rhs);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
