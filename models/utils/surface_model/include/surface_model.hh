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
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/include/surface_model.hh
 * Vehicle surface model for general environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/surface_model.cc))


*******************************************************************************/

#ifndef JEOD_SURFACE_MODEL_HH
#define JEOD_SURFACE_MODEL_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/container/include/object_vector.hh"
#include "utils/container/include/object_list.hh"
#include "dynamics/mass/include/mass_point_state.hh"

// Model includes


//! Namespace jeod
namespace jeod {

class Facet;
class MassBody;
class BaseDynManager;


/**
 * This is a structure used only in the surface model to aid in relative
 * state calculations for articulation. For each mass body that needs a
 * relative state calculated w.r.t. the mass body named in struct_body_name,
 * one of these objects will be instantiated. That way, the relative state
 * information must only be calculated once per mass body.
 */
struct FacetStateInfo {

   JEOD_MAKE_SIM_INTERFACES(FacetStateInfo)
   /**
    * The resulting relative mass point state between the
    * structural body named in struct_body_name and the
    * MassBody pointed to in this structure's mass_body
    */
   MassPointState mass_state; //!< trick_io(**)
   /**
    * The MassBody object whose state, relative and w.r.t.
    * the MassBody named in struct_body_name, is
    * being calculated
    */
   MassBody* mass_body; //!< trick_io(**)

   /**
    * Default constructor to keep the memory manager happy.
    */
   FacetStateInfo(){}

   /**
    * FacetStateInfo non-default constructor.
    * @param new_mass_body The mass body to which this object will refer.
    */
   explicit FacetStateInfo(MassBody* new_mass_body)
      :
      mass_body(new_mass_body)
      {}

   /**
    * FacetStateInfo non-default constructor.
    * @param new_mass_body The mass body to which this object will refer.
    */
   explicit FacetStateInfo(MassBody& new_mass_body)
      :
      mass_body(&new_mass_body)
      {}

   /**
    * Compare this FacetStateInfo object to another.
    * The two are 'equal' if they refer to the same mass body.
    * @param rhs Object to be compared with this object.
    */
   bool operator == (const FacetStateInfo& rhs) const {
      return mass_body == rhs.mass_body;
   }
};


/**
 * A general, non-interaction specific surface that can be
 * used to create surfaces suitable for specific interactions.
 */
class SurfaceModel {

   JEOD_MAKE_SIM_INTERFACES(SurfaceModel)

public:
   // constructor
   SurfaceModel ();

   // destructor
   ~SurfaceModel ();


   /**
    * Is the articulation active? If yes, facet information will be
    * updated from the previously supplied mass tree. If not, nothing
    * will update. This defaults to false
    */
   bool articulation_active; //!< trick_units(--)

   void add_facets (Facet** new_facets, unsigned int num_new_facets);

   void add_facet (Facet* new_facet);

   void initialize_mass_connections (BaseDynManager& manager);

   void update_articulation ();

   /**
    * The name of the MassBody representing the overall structural
    * frame of the vehicle associated with this surface model.
    * All states of all contained facets will be relative to
    * the structural frame of this MassBody. This name is
    * only required for specific applications, such as contact and
    * articulation
    */
   char* struct_body_name; //!< trick_units(--)

   /**
    * The facets that make up the surface
    */
   JeodPointerVector<Facet>::type facets; //!< trick_io(**)


protected:

   /**
    * A pointer to the MassBody named by struct_body_name. This pointer
    * will be set, using struct_body_name, by searching the DynManager
    * object supplied to the initialize_mass_connections function.
    * This pointer is only used for specific applications, such as contact
    * and articulation
    */
   MassBody* struct_body_ptr; //!< trick_units(--)

   /**
    * The set of states used to update the articulation of
    * each facet
    */
    std::list<FacetStateInfo *> articulation_states;


private:

   // operator = and copy constructor locked from use because they
   // are declared private

   SurfaceModel& operator = (const SurfaceModel& rhs);
   SurfaceModel (const SurfaceModel& rhs);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
