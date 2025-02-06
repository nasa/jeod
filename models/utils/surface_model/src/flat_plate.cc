/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/flat_plate.cc
 * Flat plates for use in the surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((flat_plate.cc)
     (facet.cc))


*******************************************************************************/

#include "../include/flat_plate.hh"
#include "utils/math/include/vector3.hh"

//! Namespace jeod
namespace jeod
{

/*******************************************************************************
  function: update_articulation_internal
  purpose: (A FlatPlate specific version of update_articulation_internal.
            Updates the direction of the normal from the local struct
            frame to the global, user set one)
*******************************************************************************/

void FlatPlate::update_articulation_internal()
{
    // mass_rel_struct should now contain the position and orientation
    // of mass_body's structural, with respect to struct_body's
    // structural, in struct body's structural. If it doesn't, then
    // this function shouldn't have been called yet (since it's
    // protected we'll make that assumption

    // Facet's version of this function will get the position (inherited
    // from facet). So call it to knock out the position.

    Facet::update_articulation_internal();

    // Transform the normal from the local struct frame to the
    // user set vehicle struct frame

    Vector3::transform_transpose(mass_rel_struct->T_parent_this, local_normal, normal);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
