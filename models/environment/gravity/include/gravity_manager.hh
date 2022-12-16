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
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/include/gravity_manager.hh
 * Define the Gravity Manager.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((gravity_manager.o))


*******************************************************************************/


#ifndef JEOD_GRAVITY_MODEL_HH
#define JEOD_GRAVITY_MODEL_HH


// System includes

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"


//! Namespace jeod
namespace jeod {

class BaseDynManager;
class RefFrame;


/**
 * The master gravitational model for a simulation.
 */
class GravityManager {

 JEOD_MAKE_SIM_INTERFACES (GravityManager)

 public:

 // No public data


 private:

   /**
    * The gravitational bodies
    */
   JeodPointerVector<GravitySource>::type sources;  //!< trick_io(**)


 // Member functions

 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies
 private:
   GravityManager (const GravityManager &);
   GravityManager & operator= (const GravityManager &);

 public:

   GravityManager ();

   ~GravityManager ();

   GravitySource * find_grav_source (std::string source_name) const;

   void add_grav_source (GravitySource & source);

   void initialize_model (BaseDynManager & manager);

   void initialize_state (BaseDynManager & manager);

   /**
    * Compute the gravitational attraction of gravitational bodies on the
    * provided dynamic body.
    *
    * \par Assumptions and Limitations
    *   - Only the gravitational bodies specified in the
    *     dynamic body's gravity controls have a bearing on the dynamic
    *     body's state.
    * \warning This overload is deprecated.
    * \param[in] integ_pos Dyn body location (integ frm)\n Units: M
    * \param[in,out] grav Gravity interaction
    */
   void gravitation (const double integ_pos[3], GravityInteraction & grav);

   /**
    * Compute the gravitational attraction of gravitational bodies on the
    * provided dynamic body.
    *
    * \par Assumptions and Limitations
    *   - Only the gravitational bodies specified in the
    *     dynamic body's gravity controls have a bearing on the dynamic
    *     body's state.
    *   - The supplied reference frame is assumed to be a direct child
    *     of the dynamic body's integration frame.
    * \param[in] point  Point of interest, as a reference frame.
    * \param[in,out] grav Gravity interaction
    */
   void gravitation (const RefFrame& point, GravityInteraction& grav);

   /**
    * Get the vector of gravitational bodies.
    * \warning Do not modify the vector, or elements of it.
    */
   const std::vector<GravitySource*>& get_bodies () const
   {
      return sources;
   }


};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
