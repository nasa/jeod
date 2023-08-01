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
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup De4xxEphem
 * @{
 *
 * @file models/environment/ephemerides/de4xx_ephem/include/de4xx_ephem.hh
 * Define class for the De4xx ephemeris model
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/de4xx_ephem.cc))



*******************************************************************************/

#ifndef JEOD_DE4xx_EPHEM_HH
#define JEOD_DE4xx_EPHEM_HH


// System includes
#include <string>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/ephemerides/ephem_item/include/ephem_item.hh"
#include "environment/ephemerides/ephem_item/include/ephem_orient_zxz.hh"
#include "environment/ephemerides/ephem_item/include/ephem_point.hh"
#include "environment/time/include/class_declarations.hh"
#include "utils/ref_frames/include/ref_frame_interface.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "de4xx_base.hh"
#include "de4xx_file.hh"


//! Namespace jeod
namespace jeod {

class EphemeridesManager;
class DynManager;

/**
 * Describes a point modeled in a DE4xx ephemeris file.
 * This class is only used inside the De4xxEphemeris class as the type
 * of the protected item_data data member.
 */
class De4xxEphemItem {
JEOD_MAKE_SIM_INTERFACES(De4xxEphemItem)

   friend class De4xxEphemeris;

public:

   // Data types

   /**
    * Enumerates the status values of a De4xEphemItem.
    */
   enum Status {
      Deselected = 0,  /**< The item is marked as deselected or the corresponding
                          reference frame is not present in the simulation.
                          The corresponding ephemeris item is permanently
                          disabled under such circumstances. */
      Inactive   = 1,  /**< The item has not been deselected and the corresponding
                          reference frame is present but is inactive. */
      IsRoot     = 2,  /**< The item has not been deselected and the corresponding
                          reference frame is present and active. However, the
                          item in question is the root of the reference frame
                          tree and hence its state is the trivial state. */
      InTree     = 3,  /**< The item has not been deselected and the corresponding
                          reference frame is present and active. However, the
                          ephemeris item that updates this frame is not a
                          part of this model. */
      Active     = 4,  /**< The item has not been deselected, the corresponding
                          reference frame is present and active, and the
                          ephemeris item that updates this frame belongs to
                          this model. */
   };


   // Member functions

   // Default constructor
   De4xxEphemItem (void);

   // Destructor
   ~De4xxEphemItem (void);


protected:

   // Member data

   /**
    * The ephemeris item for this item from this model.
    */
   EphemerisItem * item;  //!< trick_units(--)

   /**
    * The enabled ephemeris item for this item,
    * not necessarily from this model.
    */
   EphemerisItem * enabled_item;  //!< trick_units(--)

   /**
    * The reference frame whose state is set by this item.
    */
   EphemerisRefFrame * frame; //!< trick_units(--)

   /**
    * Item name; used for reporting errors.
    */
   const char * name; //!< trick_units(--)

   /**
    * The status for this item.
    */
   Status status; //!< trick_units(--)

   /**
    * The node index number, per the De4xxEphemBodies numbering scheme.
    */
   unsigned int index; //!< trick_units(--)


 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies
 private:
   /**
    * Not implemented.
    */
   De4xxEphemItem (const De4xxEphemItem &);

   /**
    * Not implemented.
    */
   De4xxEphemItem & operator= (const De4xxEphemItem &);
};


/**
 * The S_define-level class that provides planetary ephemerides.
 * The De4xxEphemeris class constructs the ephemeris reference frame tree and
 * updates the states of the planets based on data from a DE4xx ephemeris model.
 */
class De4xxEphemeris :
   public EphemerisInterface, public RefFrameOwner {
JEOD_MAKE_SIM_INTERFACES(De4xxEphemeris)

public:

   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Default constructor
   De4xxEphemeris (void);

   // Destructor
   ~De4xxEphemeris (void) override;


   // S_define-level interfaces

   // Initialize the model
   void initialize_model (
      const TimeManager & time_manager, DynManager & dyn_manager,
      const std::string & time_type="TT");
   void initialize_model (
      const TimeManager & time_manager, EphemeridesManager & ephem_manager,
      const std::string & time_type="TT");

   // Propagate the lunar orientation
   void propagate_lunar_rnp (void);

   // Close the model (free resources)
   void shutdown (void);

   // EphemInterface methods

   void activate (void) override;
   void deactivate (void) override;

   // EphemInterface accessors
   double timestamp (void) const override;

   const char * get_name (void) const override;

   // EphemeridesManager interface methods
   void ephem_initialize (EphemeridesManager & ephem_manager) override;

   void ephem_activate (EphemeridesManager & ephem_manager) override;

   void ephem_build_tree (EphemeridesManager & ephem_manager) override;

   void ephem_update (void) override;

   // Check whether the time is represented in the ephemeris file
   bool time_is_in_range (void) const;

   /**
    * Set ephemeris model number.
    * This number is used to specify the de file to use
    * the pathname is of the form PWD/build/de4xx_lib/libde\<denumIn\>.so
    */
   void set_model_number(int denum_in){
       file.file_spec.set_model_number(denum_in);
   }

   /**
    * Get Ephemeris model number.
    * This number is used to specify the de file to use
    * the pathname is of the form PWD/build/de4xx_lib/libde\<denumIn\>.so
    */
   uint32_t get_model_number() {
       return file.file_spec.get_model_number();
   }

   const De4xxFileHeader & get_header_data() {
       return file.header;
   }


   // Member data

   /**
    * Is the model active?
    * This is set to true by the constructor.
    * Setting this flag to false prior to initialization time will result in the
    * model never doing anything. Setting this flag to false after the model has
    * been active for some time is not supported.
    */
   bool active; //!< trick_units(--)

   /**
    * Used at initialization time only to selectively enable/disable portions of
    * the model. The constructor initializes all elements of this array to true.
    * Users can set selected elements to false to disable the corresponding
    * ephemeris items.
    * The intent is to enable the use of multiple ephemeris models. Typical
    * users of the model can leave this member as-is.
    * NOTE: while the container is of length 16, Nutations and lunar librations
    * are not currently supported by JEOD
    */
   bool* selected_items; //!< trick_units(--)


protected:

   // Member data
   // Trick 07 users:
   // Do not set any of the protected members from the input file.

   /**
    * The ephemeris file model.
    * The items of interest to the typical user are the data members
    * file.file_spec.ephem_file_name and file.file_spec.denum. The former
    * specifies the name of the file while the latter serves as a sanity check
    * that the right file is being read.
    */
   De4xxFile file; //!< trick_units(--)

   /**
    * Is an update needed even if the time hasn't changed?
    */
   bool force_update; //!< trick_units(--)

   /**
    * Number of items that are currently active.
    */
   unsigned int nactive_items; //!< trick_units(--)

   /**
    * Data pertaining to the points for which translational states
    * are calculated.
    */
   De4xxEphemItem * item_data; //!< trick_units(--)

   /**
    * Identifier for this model, computed from the supplied file.
    */
   char * ident; //!< trick_units(--)

   /**
    * Time of last update, dynamic time seconds
    */
   double update_time; //!< trick_units(s)

   /**
    * The planets and barycenter points, in De4xxEphemBodies FileBodies order
    */
   EphemerisPoint* points; //!< trick_units(--)

   /**
    * Lunar orientation
    */
   EphemerisZXZOrientation lunar_orientation; //!< trick_units(--)

   /**
    * Earth-Moon barycenter reference frame.
    */
   EphemerisRefFrame earth_moon_barycenter_frame; //!< trick_units(--)

   /**
    * Solar system barycenter reference frame.
    */
   EphemerisRefFrame solar_system_barycenter_frame; //!< trick_units(--)

   /**
    * The root point in the reference frame tree
    */
   De4xxEphemItem * root_item; //!< trick_units(--)

   /**
    * The source of ephemeris time information
    */
   const TimeStandard * time_tt; //!< trick_units(--)

   /**
    * The source of dynamic time information
    */
   const TimeDyn * time_dyn; //!< trick_units(--)

   /**
    * Mapping from De4xxEphemBodies numbers to De4xxFileBodies numbers.
    */
   int* body_to_file_idx; //!< trick_units(--)


private:

   // Member functions

   // Initialize timing
   void initialize_time (const TimeManager & time_manager, const std::string & time_type);

   // Initialize the DE4xx file object
   void initialize_file (void);

   // Initialize the DE4xx ephemeris items
   void initialize_items (EphemeridesManager & ephem_manager);

   // Activate nodes per frame activity
   unsigned int activate_nodes (void);

   // Adjust Earth, Moon, and Earth-Moon barycenter activity
   unsigned int activate_em_nodes (unsigned int tot_active);

   // Determine which node should be the root of the ref frame tree
   void determine_root_node (void);


   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   De4xxEphemeris (const De4xxEphemeris &);

   /**
    * Not implemented.
    */
   De4xxEphemeris & operator= (const De4xxEphemeris &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
