/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_properties_init.cc
 * Define methods for the MassPropertiesInit class.
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
    ((mass_properties_init.o)
     (mass_messages.o)
     (mass_point_mass_inertia.o)
     (mass_point_init.o)
     (mass_point_state.o)
     (utils/message/message_handler.o)
     (utils/orientation/orientation.o))



*******************************************************************************/


// Model includes
#include "../include/mass_properties_init.hh"
#include "../include/mass.hh"
#include "../include/mass_messages.hh"

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/message/include/message_handler.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor; constructs a MassPropertiesInit object.
 */
MassPropertiesInit::MassPropertiesInit (
   void)
{

   // Initialize top-level member data.
   mass = 0.0;
   cm = position;
   Matrix3x3::initialize (inertia);
   Vector3::initialize (inertia_offset);
   inertia_spec = Body;

   return;
}


/**
 * Initialize the subject MassProperties.
 * \param[out] properties Properties to initialize
 */
void
MassPropertiesInit::initialize_mass_properties (
   MassProperties & properties)
const
{
   double offset_inertia[3][3];

   // Initialize the center of mass and structure-to-body transform
   initialize_mass_point (properties);

   // Copy the mass to the properties.
   properties.mass = mass;

   // Initialize the inertia tensor per the inertia_spec.
   // Inertia_spec == NoSpec means the inertia tensor in the target MassProperties
   // object has (presumably) been set by some other means (e.g., direct user
   // input); the inertia tensor in this MassPropertiesInit object is not
   // used.
   // For all other cases, the inertia tensor in the target MassProperties is
   // to be based on this MassPropertiesInit object, with inertia_spec
   // specifying in the frame and origin in which / about which this
   // object's inertia tensor is specified.
   switch (inertia_spec) {

   // NoSpec: Ignore the inertia tensor in this MassPropertiesInit object.
   case NoSpec:
      break;


   // Body: This inertia tensor is in the target body frame and about the
   // target body CoM; exactly what is needed.
   // Load this inertia directly into the mass properties.
   case Body:

      Matrix3x3::copy (inertia, properties.inertia);
      break;


   // StructCG: This inertia tensor is represented in target structural
   // coordinates and is about the target body CoM.
   // Transform this inertia tensor from structural frame to body.
   case StructCG:

      Matrix3x3::transform_matrix (properties.T_parent_this, inertia,
                                   properties.inertia);
      break;


   // Struct: This inertia tensor is represented in target structural
   // coordinates and is about the target structural origin.
   // Transform and translate this inertia tensor from structure to body.
   case Struct:

      // Compute the inertia tensor for a point mass located at the CoM
      // in structural coordinates.
      MassBody::compute_point_mass_inertia (properties.mass,
                                            properties.position,
                                            offset_inertia);

      // Translate the input inertia to the CoM.
      Matrix3x3::subtract (inertia, offset_inertia, offset_inertia);

      // Transform to body.
      Matrix3x3::transform_matrix (properties.T_parent_this, offset_inertia,
                                   properties.inertia);
      break;


   // SpecCG: This inertia tensor is represented in a user-specified frame
   // and is about the target body CoM.
   // Transform this inertia tensor from the inertia frame to body.
   case SpecCG: {

      // Make a copy of the orientation to preserve constness.
      Orientation orientation(inertia_orientation);

      // Compute the transformation matrix from user inputs.
      orientation.compute_transform();

      // Transform the inertia tensor.
      Matrix3x3::transform_matrix (orientation.trans, inertia,
                                   properties.inertia);
      break;
   }


   // Spec: This inertia tensor is represented in a user-specified frame
   // and is about a user-specified origin.
   // Transform and translate this inertia tensor from the user frame to body.
   case Spec: {

      // Make a copy of the orientation to preserve constness.
      Orientation orientation(inertia_orientation);

      // Compute the transformation matrix from user inputs.
      orientation.compute_transform();

      // Compute the inertia tensor for a point mass located at the CoM
      // in user frame coordinates.
      MassBody::compute_point_mass_inertia (properties.mass,
                                            inertia_offset,
                                            offset_inertia);

      // Translate the input inertia to the CoM.
      Matrix3x3::subtract (inertia, offset_inertia, offset_inertia);

      // Transform to body.
      Matrix3x3::transform_matrix (orientation.trans, offset_inertia,
                                   properties.inertia);
      break;
   }


   default:
      MessageHandler::fail (
         __FILE__, __LINE__, MassBodyMessages::invalid_enum,
         "Illegal enum value (%d) for MassPropertiesInit.inertia_spec",
         static_cast <int> (inertia_spec));

      // Not reached
      return;
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
