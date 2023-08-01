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
 * @addtogroup Atmosphere
 * @{
 * @addtogroup BaseAtmosphere
 * @{
 * @file models/environment/atmosphere/base_atmos/include/wind_velocity.hh
 * A wind velocity model based on winds caused by rotation of the planet
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (A simple table-lookup winds model.  Wind magnitude is a tabulated
    function of altitude; wind direction is a simple analytic function of
    position (inertial-z cross r).)
ASSUMPTIONS AND LIMITATIONS:
   ((The inertial frame of the planet causing the wind velocity must have it's
     Z-axis nominally aligned with the major rotation of the planet))
LIBRARY DEPENDENCIES:
   ((../src/wind_velocity.cc))

*******************************************************************************/

#ifndef JEOD_WIND_VELOCITY_HH
#define JEOD_WIND_VELOCITY_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * A generic wind velocity implementation.
 */
class WindVelocity {

   JEOD_MAKE_SIM_INTERFACES(WindVelocity)


public:
   // Constructor
   WindVelocity ();

   // Destructor
   virtual ~WindVelocity ();

   virtual void update_wind (
      double inertial_pos[3], double altitude, double wind_inertial[3]);

   unsigned int get_num_layers();

   void set_omega_scale_table(double altitude, double factor);

   void set_omega_scale_table(unsigned int num_layers, const double* altitude, const double* factor);

   /*
    * model is active flag
    */
   bool active; //!< trick_units(--)

   /**
    * The rotational velocity of the planet
    */
   double omega; //!< trick_units(rad/s)

// Data type

   /**
    * An entry in an omega scale table.
    */
   struct OmegaTableEntry {

      /**
       * Altitude at which omega is multiplied by the corresponding factor.
       */
      double altitude; //!< trick_units(m)

      /**
       * Factor by which omega is multiplied depending on altitude.
       */
      double scale_factor; //!< trick_units(--)
   };

   OmegaTableEntry* get_omega_scale_table();

protected:
   /**
    * Number of altitude layers.
    */
   unsigned int num_layers; //!< trick_units(count)

   /**
     * Table of factors to scale omega based on altitude
     */
   OmegaTableEntry* omega_scale_table;

private:
   /**
    * last known index into the arrays
    */
   unsigned int array_index;

   /**
    * Altitude direction check flag
    */
   bool first_pass; //!< trick_units(--)

   /**
    * Altitude increasing or decreasing flag
    */
   bool increasing_altitude; //!< trick_units(--)

   // operator = and copy constructor locked from use by being private
   WindVelocity (const WindVelocity& rhs);
   WindVelocity& operator = (const WindVelocity& rhs);

} ;

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
