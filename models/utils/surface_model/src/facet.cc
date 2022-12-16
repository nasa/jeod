/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/facet.cc
 * Individual facets for use in the surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((facet.o)
     (surface_model_messages.o)
     (dynamics/mass/mass.o)
     (dynamics/mass/mass_point_state.o))

 
*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "dynamics/mass/include/mass.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/math/include/vector3.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/surface_model_messages.hh"
#include "../include/facet.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

Facet::Facet (
   void)
: // Return: -- void
   param_name(NULL),
   name(),
   mass_body_name(NULL),
   mass_body_ptr(NULL),
   connections_initialized(false)
{
   Vector3::initialize (position);
   Vector3::initialize (local_position);
   Vector3::initialize (int_pos);
   temperature  = 0.0;
   area = 0.0;

}

/**
 * Destructor
 */

Facet::~Facet (
   void)
{

   // empty for now

}

/*******************************************************************************
  function: initialize_mass_connection
  purpose: (Connects the facet, through
            a user set name in the facet, to a mass body in
            the supplied tree of mass bodies)
*******************************************************************************/

void
Facet::initialize_mass_connection (
   BaseDynManager& manager)

{

   NamedItem::validate_name(__FILE__, __LINE__, mass_body_name,
                            "Name of the associated mass body",
                            "mass_body_name");

   mass_body_ptr = manager.find_mass_body(mass_body_name);

   if(mass_body_ptr == NULL){

      MessageHandler::fail (
         __FILE__, __LINE__, SurfaceModelMessages::initialization_error,
         "In Facet::initialize_mass_connection, no mass body was found, "
         "in the given DynManager, with the name %s. "
         "Initialization "
         "has failed.\n", mass_body_name);
      return;
   }

   connections_initialized = true;

   return;

}

/*******************************************************************************
  function: update_articulation
  purpose: (The per-facet function to update articulation. Calls the virtual
            and protected method update_articulation_internal on each facet,
            which is the function that should be overridden in inheritence
            situations.)
*******************************************************************************/

void
Facet::update_articulation (
   void)
{

   if(!connections_initialized) {

      MessageHandler::fail (
         __FILE__, __LINE__, SurfaceModelMessages::initialization_error,
         "In Facet::update_articulation, the invoking Facet has not been "
         "properly initialized using initialize_mass_connection. "
         "Either the surface model was not initialized or the appropriate "
         "mass body "
         "was not found. This method has failed\n");

      return;

   }


   // called through this 'this' pointer to insure overriden
   // virtual function is used. It may not be necessary, but it
   // also points out very well that what we are calling is
   // expected to be an overridden virtual function

   this->update_articulation_internal();

   return;

}

/*******************************************************************************
  function: get_mass_body_ptr
  purpose: (Returns the mass_body_ptr associated with this facet,
            if it has been set using the initialize_mass_connection function.
            If initialize_connections has not been called, then
            this function will simply return NULL.)
*******************************************************************************/

MassBody*
Facet::get_mass_body_ptr(
   void)
{

   return mass_body_ptr;

}

/*******************************************************************************
  function: update_articulation_internal
  purpose: (A version of update_articulation_internal specific to
            FlatPlate)
*******************************************************************************/

void
Facet::update_articulation_internal (
   void)
{

   // mass_rel_struct should now contain the position and orientation
   // of mass_body's structural, with respect to struct_body's
   // structural, in struct body's structural. If it doesn't, then
   // this function shouldn't have been called yet (since it's
   // protected we'll make that assumption)

   // convert the local position from the local mass body struct
   // frame to the integrating mass body struct frame (the frame
   // associated with the mass body named in struct_body_name
   // first rotate it into that frame

   Vector3::transform_transpose(mass_rel_struct->T_parent_this,
                                local_position,
                                int_pos);

   // now add the position of the mass frame w.r.t. the struct
   // frame, in the struct frame, to the local_position in the
   // struct frame (contained in int_pos since we just calculated
   // it)

   Vector3::sum(mass_rel_struct->position,
                int_pos,
                position);


   return;

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
