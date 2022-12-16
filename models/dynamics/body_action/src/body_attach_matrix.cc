/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/mass_body_attach_matrix.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_attach_matrix.o)
   (body_action.o)
   (body_attach.o)
   (dynamics/mass/mass_point_state.o)
   (dynamics/dyn_manager/dyn_manager.o)
   (dynamics/dyn_body/dyn_body_attach.o)
   (dynamics/mass/mass_attach.o)
   (utils/orientation/orientation.o))



*******************************************************************************/


// System includes

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/body_attach_matrix.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a MassBodyAttachMatrix.
 */
BodyAttachMatrix::BodyAttachMatrix (
   void)
:
   pstr_cstr()
{
   Vector3::initialize (offset_pstr_cstr_pstr);
}


/**
 * Initialize the core mass properties of the subject MassBody.
 * \param[in,out] dyn_manager Jeod manager
 */
void
BodyAttachMatrix::apply (
   DynManager & dyn_manager)
{
    // Convert user-specified orientation to transformation matrix.
    pstr_cstr.compute_transform ();

    // DynBody to DynBody
    if( dyn_subject != nullptr && dyn_parent != nullptr )
    {
        succeeded = dyn_parent->attach( offset_pstr_cstr_pstr,
                                        pstr_cstr.trans,
                                        *dyn_subject);
    }
    // MassBody subbody/subassembly to DynBody
    else if( dyn_subject == nullptr && dyn_parent != nullptr )
    {
        succeeded = dyn_parent->add_mass_body( offset_pstr_cstr_pstr,
                                               pstr_cstr.trans,
                                               *subject);
    }
    // MassBody/MassBody assembly to MassBody
    else if( dyn_subject == nullptr && dyn_parent == nullptr )
    {
        succeeded = subject->attach_to (offset_pstr_cstr_pstr, pstr_cstr.trans, *parent);

    }
    // DynBody to MassBody -- ILLEGAL
    else // if( dyn_subject != nullptr && dyn_parent == nullptr )
    {
        succeeded = false;
    }

    // Forward the action up the chain. The parent class deals with status.
    BodyAttach::apply (dyn_manager);

    return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
