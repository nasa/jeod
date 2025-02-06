/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_point_init.cc
 * Define methods for the MassPointInit class.
 */

/*******************************************************************************
  Purpose:
    ()

  Reference:
    (((TBS)))

  Assumptions and limitations:
    ((N/A))

  Class:
    (N/A)

  LIBRARY DEPENDENCY:
    ((mass_point_init.o)
     (mass_messages.o)
     (mass_point_state.o)
     (utils/message/message_handler.o)
     (utils/orientation/orientation.o))



*******************************************************************************/


// Model includes
#include "../include/mass_point_init.hh"
#include "../include/mass_point.hh"
#include "../include/mass_messages.hh"

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"



//! Namespace jeod
namespace jeod {

/**
 * Default constructor; constructs a MassPointInit object.
 */
MassPointInit::MassPointInit ()
:
   pt_orientation(),
   pt_frame_spec(StructToPoint),
   name()
{

   Vector3::initialize (position);
}


/**
 * Default constructor; constructs a MassPointInit object.
 * \param[out] mass_point Point to initialize
 */
void
MassPointInit::initialize_mass_point (
   MassPoint & mass_point)
const
{
   // Make a copy of the orientation to preserve constness.
   Orientation orientation(pt_orientation);

   // Compute the transformation matrix and quaternion from the user-specified
   // values, in the user-specified frame.
   orientation.compute_transformation_and_quaternion();


   // Convert to the parent-to-child form required for a mass point.
   switch (pt_frame_spec) {
   case StructToPoint:
      Matrix3x3::copy (orientation.trans, mass_point.T_parent_this);
      mass_point.Q_parent_this = orientation.quat;
      break;

   case PointToStruct:
      Matrix3x3::transpose (orientation.trans, mass_point.T_parent_this);
      orientation.quat.conjugate (mass_point.Q_parent_this);
      break;

   default:
      MessageHandler::fail (
         __FILE__, __LINE__, MassBodyMessages::invalid_enum,
         "Illegal enum value (%d) for MassPointInit.pt_frame_spec",
         static_cast <int> (pt_frame_spec));

      // Not reached
      return;
   }


   // The input point is always the location of the point in structural
   // coordinates, which is exactly what the mass point needs.
   mass_point.update_point (position);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
