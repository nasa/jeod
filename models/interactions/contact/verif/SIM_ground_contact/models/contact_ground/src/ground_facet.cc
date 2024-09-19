/*******************************************************************************
Purpose:
  (Define GroundFacet class constructor and destructor)

Library Dependencies:
  ((ground_facet.cc)
  (interactions/contact/src/line_contact_facet.cc)
  (interactions/contact/src/point_contact_facet.cc)
  (line_ground_interaction.cc)
  (point_ground_interaction.cc))


*******************************************************************************/

/* JEOD Includes */
#include "../include/line_ground_interaction.hh"
#include "../include/point_ground_interaction.hh"
#include "interactions/contact/include/contact.hh"
#include "interactions/contact/include/contact_messages.hh"
#include "interactions/contact/include/contact_params.hh"
#include "interactions/contact/include/contact_utils.hh"
#include "interactions/contact/include/line_contact_facet.hh"
#include "interactions/contact/include/point_contact_facet.hh"
#include "utils/memory/include/jeod_alloc.hh"

/* Model includes */
#include "../include/ground_facet.hh"

//! Namespace jeod
namespace jeod
{

/*******************************************************************************
  Function: GroundFacet
  Purpose: (Default constructor)
*******************************************************************************/
GroundFacet::GroundFacet()
{
    JEOD_REGISTER_CLASS(LineGroundInteraction);
    JEOD_REGISTER_CLASS(PointGroundInteraction);
}

/*******************************************************************************
 Function: create_interaction
 Purpose: (Overloaded function to create a GroundInteraction of the appropriate type and return
 a pointer to the Contact class.)
 Purpose: (initialization)
 *******************************************************************************/
GroundInteraction * GroundFacet::create_interaction( // Return: -- void
    ContactFacet * subject,                          // In: -- Contact Facet
    Contact * contact)                               /* In: -- Contact object used to find the pair interaction */
{
    LineContactFacet * temp_line;
    PointContactFacet * temp_point;

    // attempt to cast the target to a LineContactFacet or PointContactFacet
    temp_line = dynamic_cast<LineContactFacet *>(subject);
    temp_point = dynamic_cast<PointContactFacet *>(subject);
    if(temp_line == nullptr && temp_point == nullptr)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             ContactMessages::initialization_warns,
                             "The subject ContactFacet passed into GroundFacet::create_interaction was "
                             "not of type LineContactFacet or PointContactFacet as required.");

        return nullptr;
    }
    if(temp_line == nullptr)
    {
        PointGroundInteraction * ground_interaction;
        // Allocate a ContactPair instance
        ground_interaction = JEOD_ALLOC_CLASS_OBJECT(PointGroundInteraction, ());

        ground_interaction->interaction = contact->find_interaction(surface_type, temp_point->surface_type);
        if(ground_interaction->interaction == nullptr)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 ContactMessages::initialization_warns,
                                 "No interaction type could be found for the surface types of these ContactFacets.");

            JEOD_DELETE_OBJECT(ground_interaction);
            return nullptr;
        }

        // set the subject and target of the new pair
        ground_interaction->ground = this;
        ground_interaction->subject = temp_point;
        ground_interaction->subject_point = temp_point;

        ground_interaction->interaction_distance = temp_point->max_dimension * contact->contact_limit_factor * 2.0;

        return ground_interaction;
    }
    else
    {
        LineGroundInteraction * ground_interaction;
        // Allocate a ContactPair instance
        ground_interaction = JEOD_ALLOC_CLASS_OBJECT(LineGroundInteraction, ());

        ground_interaction->interaction = contact->find_interaction(surface_type, temp_line->surface_type);
        if(ground_interaction->interaction == nullptr)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 ContactMessages::initialization_warns,
                                 "No interaction type could be found for the surface types of these ContactFacets.");

            JEOD_DELETE_OBJECT(ground_interaction);
            return nullptr;
        }

        // set the subject and target of the new pair
        ground_interaction->ground = this;
        ground_interaction->subject = temp_line;
        ground_interaction->subject_line = temp_line;

        ground_interaction->interaction_distance = temp_line->max_dimension * contact->contact_limit_factor * 2.0;

        return ground_interaction;
    }
}
} // namespace jeod
