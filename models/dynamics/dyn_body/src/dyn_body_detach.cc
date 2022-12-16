/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_detach.cc
 * Define DynBody detachment methods.
 */

/*******************************************************************************

Purpose:
   ()

Library dependencies:
  ((dyn_body_detach.o)
   (dynamics/dyn_manager/dyn_manager.o)
   (dyn_body.o))



*******************************************************************************/


// System includes
#include <cstddef>
#include <algorithm>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/tree_links_iterator.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"


//! Namespace jeod
namespace jeod {

// Break the logical connectivity between a parent and child DynBody.
// Returns false if unable to disconnect.
bool
DynBody::detach (
        DynBody & other_body)
{
    DynBody* parent = nullptr;
    DynBody* detacher = nullptr;

    bool success = true;


    // Find the body that is to be detached from its immediate parent.
    // First search from 'this' up for the input mass_body, and if that fails
    // search from the input mass_body up for 'this'. Both searches failing is
    // an error.
    // 1) search from this up to other_body
    for (auto* link : TreeLinksAscendRange<MassBodyLinks>(mass.links)) {
       MassBody* iter_body = &(link->container());
       if (iter_body == &other_body.mass) {
          parent = &other_body;
          break;
       }
       detacher = iter_body->dyn_owner;
    }

    // 2) parent_body is null => First search failed.
    if (parent == nullptr) {
       // Search from other_body up to this
       for(auto* link :
               TreeLinksAscendRange<MassBodyLinks>(other_body.mass.links))
       {
          MassBody* iter_body = &(link->container());
          if (iter_body == &mass) {
             parent = this;
             break;
          }
          detacher = iter_body->dyn_owner;
       }
    }
    // Else: bodies or not attached, or detach point is arbitrary
    //       In either case, unable to proceed. Return false.
    if( parent == nullptr )
    {
        success = false;
        MessageHandler::inform (
                __FILE__, __LINE__, DynBodyMessages::invalid_attachment,
                "\nWarning: Unable to find detach point between\n"
                "bodies '%s' and '%s'. No detachment was performed.",
                name.c_str(), other_body.name.c_str());
        return success;
    }

    // Delete child from parent
    std::list<DynBody *>::iterator it;
    for( it  = parent->dyn_children.begin();
         it != parent->dyn_children.end();
         it++)
    {
        if (*it == detacher) {
            parent->dyn_children.erase(it);
            break;
        }
    }
    if( it == parent->dyn_children.end() )
    {
        success = false;
        MessageHandler::fail (
                __FILE__, __LINE__, DynBodyMessages::invalid_attachment,
                "\nDuring detach process:\n"
                "%s recognizes %s as its dyn_parent, "
                "but %s does not recognize it as a child.\n"
                "An invalid attachment hierarchy has been detected.",
                detacher->name.c_str(),
                parent->name.c_str(),
                parent->name.c_str() );

        // Unreached
        return success;
    }

    // Delete parent from child
    detacher->dyn_parent = nullptr;

    // Delete mass links
    parent->detach_mass_internal(detacher->mass);
    return success;
}

// Detach this DynBody from its parent
bool
DynBody::detach ( void )
{
    bool success = false;
    if(    dyn_parent != nullptr
        && dyn_parent != this    )
    {
        success = detach(*dyn_parent);
    }
    else
    {
        success = false;
        MessageHandler::inform (
                __FILE__, __LINE__, DynBodyMessages::invalid_technique,
                "\nDuring detach process:\n"
                "%s ordered to detach from parent, but the body \n"
                "has no parent. No detachment performed.\n",
                name.c_str() );
    }

    return success;
}

// Break the logical connectivity between parent DynBody and child submass.
bool
DynBody::remove_mass_body (
   MassBody & child)
{
    // Look up references
    MassBody* child_root; // the root MassBody of the detaching subassembly
    MassBody* bdy; // MassBody iteration pointer
    DynBody* child_body; // look up if MassBody belongs to a DynBody

    // Status variables
    bool success = true;


    // First, find the root body that is to be detached from this parent.
    // This is performed because the user is able to call attach for a
    // subassembly of mass bodies, in which case the root of the subassembly
    // is attached if the user specified a subbody in the subassembly. In the
    // same manner, this process conveniently allows the user to detach the
    // root of the subassembly by requesting a detach of the subbody.
    // This way, the user essentially doesn't have to worry about the root of
    // the subassembly.
    child_root = nullptr;
    bdy = nullptr;
    for( auto* link : TreeLinksAscendRange<MassBodyLinks>(child.links) )
    {
        bdy = &(link->container());
        if( bdy->get_parent_body() == &mass )
        {
            child_root = bdy;
            break;
        }
    }
    if( child_root == nullptr )
    {
        success = false;
        MessageHandler::inform (
                __FILE__, __LINE__, DynBodyMessages::invalid_technique,
                "\nDuring detach process:\n"
                "%s ordered to detach sub-body MassBody %s, \n"
                "But no corresponding subassembly was found. "
                "No detachment performed.\n",
                name.c_str(),
                child.name.c_str() );
        return success;
    }

    // Check if mass is actually belonging to DynBody
    // If a DynBody, warn user and reroute detach method.
    child_body = child_root->dyn_owner;
    if( child_body != nullptr )
    {
        MessageHandler::inform (
                __FILE__, __LINE__, DynBodyMessages::invalid_technique,
                "\nDuring detach process:\n"
                "%s ordered to detach child MassBody %s, which refers\n"
                "to a DynBody attachment. Attempting to detach child "
                "DynBody %s instead.\n",
                name.c_str(),
                child.name.c_str(),
                child_body->name.c_str() );
        success = detach(*child_body);
    }
    else
    {
        detach_mass_body_frames(*child_root);
        detach_mass_internal(*child_root);
        mass_children.remove(child_root);
    }


    return success;
}

// Remove BodyRefFrame for each MassBody point from vehicle_points recursively
void DynBody::detach_mass_body_frames( MassBody &subbody )
{
    // Remove points for current subbody
    BodyRefFrame* pt_frame = nullptr;
    std::list<BodyRefFrame*>::iterator veh_pt;
    for(std::list<MassPoint*>::iterator pt = subbody.mass_points.begin();
        pt != subbody.mass_points.end();
        pt++)
    {
        pt_frame = find_body_frame( (*pt)->get_name() );
        if( pt_frame == nullptr )
        {
            continue;
        }
        veh_pt = std::find( vehicle_points.begin(),
                            vehicle_points.end(),
                            pt_frame );
        vehicle_points.erase(veh_pt);
        dyn_manager->remove_ref_frame(*pt_frame);
        JEOD_DELETE_OBJECT( pt_frame );
    }

    // Call for each of subbody's children
    if( subbody.links.has_children() )
    {
        MassBodyLinks* it = subbody.links.child_head();
        while( true )
        {
            detach_mass_body_frames(it->container());
            if( it == subbody.links.child_tail() )
            {
                break;
            }
            else
            {
                it++;
            }
        }
    }

    return;
}


// Update parent and child properties to reflect that they are detached.
void
DynBody::detach_mass_internal (   // Return: -- Void
   MassBody & child)                  /* Inout: -- The newly detached child */
{
   DynBody * root_body = get_root_body_internal();

   // Update mass properties.
   child.detach_sever_links(this->mass);
   mass.detach_update_properties (child);

   // Reset the dynamic state from the root body's core state.
   root_body->set_state_source_internal (RefFrameItems::Pos_Vel_Att_Rate,
                                         root_body->core_body);
   root_body->propagate_state ();
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
