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
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/include/mass_point_state.hh
 * Define the class MassPointState, which defines the state -- position and
 * orientation -- of a MassPoint.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Library Dependencies:
 ((../src/mass_point_state.cc))



*******************************************************************************/


#ifndef JEOD_MASS_POINT_STATE_HH
#define JEOD_MASS_POINT_STATE_HH


// Model includes
#include "class_declarations.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/quaternion/include/quat.hh"


//! Namespace jeod
namespace jeod {

/**
 * Defines the state -- position and orientation -- of a MassPoint.
 */
class MassPointState {

   JEOD_MAKE_SIM_INTERFACES(MassPointState)

 // Member data
 public:

   /**
    * Mass point location with respect to the origin of some parent frame and
    * expressed in the parent frame's coordinates.
    */
   double position[3]; //!< trick_units(m)

   /**
    * Left transformation quaternion from the parent frame to the frame
    * associated with the mass point.
    */
   Quaternion Q_parent_this; //!< trick_units(--)

   /**
    * transformation matrix from the parent frame to the frame
    * associated with the mass point.
    * NOTE: The MassPointState infrastructure ensures that the quaternion
    * and matrix representations are equivalent.
    */
   double T_parent_this[3][3]; //!< trick_units(--)


 // Member functions
 public:

   // Constructor.
   MassPointState ();

   /**
    * Destructor.
    */
   virtual ~MassPointState () = default;

   // Initialize a MassPointState.
   virtual void initialize_mass_point ();

   // Update point location, including derived items.
   void update_point (const double pt_location[3]);

   // Update point frame orientation, including derived items.
   void update_orientation (const double transformation[3][3]);

   void update_orientation (const Quaternion & left_quat);

   // Compute quaternion, assuming someone changed the matrix under our noses.
   void compute_quaternion ();

   // Compute matrix, assuming someone changed the quaternion under our noses.
   void compute_transformation ();

   // Copy reference state.
   void copy_state (const MassPointState & source);

   // Copy and negate reference state.
   void negate (const MassPointState & source);

   // 'Add' another frame, left operand
   void incr_left (const MassPointState & s_ab);

   // 'Add' another frame, right operand
   void incr_right (const MassPointState & s_bc);

   // 'Subtract' another frame, left operand
   void decr_left (const MassPointState & s_ab);

   // 'Subtract' another frame, right operand
   void decr_right (const MassPointState & s_bc);

};



/**
 * Update the mass point's structure-to-point vector.
 * \param[in] pt_location New location, struct coords.\n Units: M
 */
inline void
MassPointState::update_point (
   const double pt_location[3])
{

   // Save the new point location.
   Vector3::copy (pt_location, position);
}


/**
 * Update the mass point's structure-to-point frame transformation
 * matrix and items derived from it.
 * \param[in] transformation New structure-to-point xform
 */
inline void
MassPointState::update_orientation (
   const double transformation[3][3])
{

   // Save the new structure-to-point transformation matrix.
   Matrix3x3::copy (transformation, T_parent_this);
   Q_parent_this.left_quat_from_transformation (T_parent_this);
}


/**
 * Update the mass point's structure-to-point frame transformation
 * matrix and items derived from it.
 * \param[in] left_quat New structure-to-point quat
 */
inline void
MassPointState::update_orientation (
   const Quaternion & left_quat)
{

   // Save the new structure-to-point transformation quaternion.
   Q_parent_this = left_quat;
   Q_parent_this.left_quat_to_transformation (T_parent_this);
}


/**
 * Compute quaternion. Someone changed the matrix under our noses.
 */
inline void
MassPointState::compute_quaternion ()
{

   Q_parent_this.left_quat_from_transformation (T_parent_this);
}


/**
 * Compute matrix. Someone changed the quaternion under our noses.
 */
inline void
MassPointState::compute_transformation ()
{

   // Save the new structure-to-point transformation quaternion.
   Q_parent_this.left_quat_to_transformation (T_parent_this);
}


/**
 * Copy the provided state to this.
 * \param[in] source Source state
 */
inline void
MassPointState::copy_state (
   const MassPointState & source)
{
   Q_parent_this = source.Q_parent_this;
   Matrix3x3::copy (source.T_parent_this, T_parent_this);
   Vector3::copy (source.position, position);
}

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
