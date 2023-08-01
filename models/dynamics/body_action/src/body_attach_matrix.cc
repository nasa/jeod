/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_attach_matrix.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_attach_matrix.cc)
   (body_action.cc)
   (body_attach.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (dynamics/dyn_manager/src/dyn_manager.cc)
   (dynamics/dyn_body/src/dyn_body_attach.cc)
   (dynamics/mass/src/mass_attach.cc)
   (utils/orientation/src/orientation.cc))



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

    if (dyn_parent != nullptr)
    {
        if (dyn_subject != nullptr) // DynBody to DynBody
        {
            succeeded = dyn_parent->attach_child(offset_pstr_cstr_pstr, pstr_cstr.trans, *dyn_subject);
        }
        else // MassBody subbody/subassembly to DynBody
        {
            succeeded = dyn_parent->add_mass_body(offset_pstr_cstr_pstr, pstr_cstr.trans, *mass_subject);
        }
    }
    else if (mass_parent != nullptr)
    {
        if (dyn_subject != nullptr) // DynBody to MassBody -- ILLEGAL
        {
            succeeded = false;
        }
        else // MassBody/MassBody assembly to MassBody
        {
            succeeded = mass_subject->attach_to (offset_pstr_cstr_pstr, pstr_cstr.trans, *mass_parent);
        }
    }
    else if (ref_parent != nullptr)
    {
        if (dyn_subject != nullptr) // DynBody to RefFrame
        {
            succeeded = dyn_subject->attach_to_frame(offset_pstr_cstr_pstr, pstr_cstr.trans, *ref_parent);
        }
        else // MassBody to RefFrame -- ILLEGAL
        {
            succeeded = false;
        }
    }
    else // Should be unreachable
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
