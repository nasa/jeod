/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/contact_facet.cc
 * Define ContactFacet::create_vehicle_point
 */

/*******************************************************************************
Purpose:
  ()

Library Dependencies:
  ((contact_facet.o)
  (contact_messages.o)
  (dynamics/mass/mass_point_init.o)
  (dynamics/dyn_body/dyn_body.o))


*******************************************************************************/

/* System includes */
#include <cstddef>

/* JEOD includes */
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/mass/include/mass_point_init.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/math/include/numerical.hh"

/* Model includes */
#include "../include/contact_facet.hh"
#include "../include/contact_messages.hh"
#include "../include/contact_utils.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor
 */
ContactFacet::ContactFacet (
   void)
   : // Return: -- None
   active (true),
   surface_type (NULL),
   vehicle_body (NULL),
   max_dimension (0.0)
{
   Vector3::initialize (position);
   Vector3::initialize (normal);
}


/**
 * Destructor
 */
ContactFacet::~ContactFacet (
   void)
{

   // empty for now

}

/**
 * Create a vehicle point from the base facet position and orientation
 * and store the created vehicle point in the contact facet
 */
void
ContactFacet::create_vehicle_point ()
{
   MassPointInit mass_point_init;

   double X[3];
   double Y[3] = { 0.0, 1.0, 0.0 };
   double Z[3];

   // set all the names to the same value.
   mass_point_init.name = get_name ();
   mass_point_init.pt_frame_spec = MassPointInit::StructToPoint;

   Vector3::copy (position, mass_point_init.position);


   if (Numerical::compare_exact(Y[1],fabs(normal[1]))) {
      Y[0] = normal[1];
      Y[1] = 0.0;
   }
   Vector3::copy(normal, X);

   Vector3::cross (X, Y, normal);
   Vector3::normalize(normal,Z);
   Vector3::cross (Z, X, Y);

   mass_point_init.pt_orientation.data_source = Orientation::InputMatrix;

   mass_point_init.pt_orientation.trans[0][0] = X[0];
   mass_point_init.pt_orientation.trans[0][1] = Y[0];
   mass_point_init.pt_orientation.trans[0][2] = Z[0];
   mass_point_init.pt_orientation.trans[1][0] = X[1];
   mass_point_init.pt_orientation.trans[1][1] = Y[1];
   mass_point_init.pt_orientation.trans[1][2] = Z[1];
   mass_point_init.pt_orientation.trans[2][0] = X[2];
   mass_point_init.pt_orientation.trans[2][1] = Y[2];
   mass_point_init.pt_orientation.trans[2][2] = Z[2];

   vehicle_body->add_mass_point (mass_point_init);

   vehicle_point = vehicle_body->find_vehicle_point (mass_point_init.name.c_str());

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
