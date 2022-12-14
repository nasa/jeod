//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @addtogroup EphemInterface
 * @{
 *
 * @file models/environment/ephemerides/ephem_interface/include/simple_ephemerides.hh
 * Define classes that define simple ephemeris models.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((simple_ephemerides.o))



*******************************************************************************/


#ifndef JEOD_SIMPLE_EPHEMERIDES_HH
#define JEOD_SIMPLE_EPHEMERIDES_HH

// System includes

// JEOD includes
#include "environment/ephemerides/ephem_item/include/ephem_point.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "ephem_interface.hh"
#include "ephem_ref_frame.hh"



//! Namespace jeod
namespace jeod {

/**
 * A SinglePointEphemeris has one ephemeris point.
 */
class SinglePointEphemeris : public EphemerisInterface {

  JEOD_MAKE_SIM_INTERFACES(SinglePointEphemeris)

public:

   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Constructor and destructor
   SinglePointEphemeris (void);
   virtual ~SinglePointEphemeris (void);


   // Set the names of this object and of the central frame.
   virtual void set_name (const char * new_name);


   // Implemented EphemerisInterface methods

   // Activate the model. (Null implementation.)
   virtual void activate (void);

   // Dectivate the model. (Null implementation.)
   virtual void deactivate (void);


   // Return the last update time.
   virtual double timestamp (void) const;

   // Return the identifier.
   virtual const char * get_name (void) const;

   // Update the ephemeris model; nothing to do with a single point ephemeris.
   virtual void ephem_update (void);


   // Unimplemented SinglePointEphemeris methods

      /**
    * Register the model and its ephemeris points.
    * \param[in,out] manager Ephemerides manager
    */

   /*
    Purpose: (Register the model and its ephemeris points.)
    */
   virtual void initialize_model (
      EphemeridesManager & manager)
   = 0;


   // Unimplemented EphemerisInterface methods

      /**
    * Initialize the ephemerides.
    * \param[in,out] manager Ephemerides manager
    */

   /*
    Purpose: (Initialize the ephemerides.)
    */
   virtual void ephem_initialize (
      EphemeridesManager & manager)
   = 0;


      /**
    * Activate the model.
    * \param[in,out] manager Ephemerides manager
    */
   virtual void ephem_activate (
      EphemeridesManager & manager)
   = 0;


      /**
    * Build the model's contribution to the reference frame tree.
    * \param[in,out] manager Ephemerides manager
    */
   virtual void ephem_build_tree (
      EphemeridesManager & manager)
   = 0;


protected:

   // Member data

   /**
    * Identifier for this model.
    */
   char * identifier; //!< trick_units(--)

   /**
    * Time of last update, dynamic time seconds
    */
   double update_time; //!< trick_units(s)

   /**
    * Is the model active?
    */
   bool active; //!< trick_units(--)


private:
   ///
   /// Not implemented.
   SinglePointEphemeris (const SinglePointEphemeris &);
   ///
   /// Not implemented.
   SinglePointEphemeris & operator = (const SinglePointEphemeris &);
};


/**
 * Empty space has one ephemeris point.
 */
class EmptySpaceEphemeris : public SinglePointEphemeris {

  JEOD_MAKE_SIM_INTERFACES(EmptySpaceEphemeris)

public:

   // Member functions

   // Constructor and destructor
   EmptySpaceEphemeris (void);
   virtual ~EmptySpaceEphemeris (void);

   // Set the names of this object and of the central frame.
   virtual void set_name (const char * frame_name);

   // Initialize the model.
   virtual void initialize_model (EphemeridesManager & ephem_manager);

   // EphemerisInterface methods.
   virtual void ephem_initialize (EphemeridesManager & ephem_manager);
   virtual void ephem_activate (EphemeridesManager & ephem_manager);
   virtual void ephem_build_tree (EphemeridesManager & ephem_manager);


protected:

   // Member data

   /**
    * The EphemerisPoint that represents the center of an empty universe.
    */
   EphemerisPoint central_point; //!< trick_units(--)

   /**
    * The sole ephemeris frame for this model.
    */
   EphemerisRefFrame central_frame; //!< trick_units(--)


private:
   ///
   /// Not implemented.
   EmptySpaceEphemeris (const EmptySpaceEphemeris &);
   ///
   /// Not implemented.
   EmptySpaceEphemeris & operator = (const EmptySpaceEphemeris &);
};


/**
 * A space with one gravitation body has one ephemeris point.
 * Note well: A SinglePlanetEphemeris does not contain a Planet object.
 * The planet must be specified elsewhere.
 */
class SinglePlanetEphemeris : public SinglePointEphemeris {

  JEOD_MAKE_SIM_INTERFACES(SinglePlanetEphemeris)

public:

   // Member functions

   // Constructor and destructor
   SinglePlanetEphemeris (void);
   virtual ~SinglePlanetEphemeris (void);

   // Set the names of this object and of the central planet.
   virtual void set_name (const char * frame_name);

   // Initialize the model.
   virtual void initialize_model (EphemeridesManager & ephem_manager);

   // EphemerisInterface methods.
   virtual void ephem_initialize (EphemeridesManager & ephem_manager);
   virtual void ephem_activate (EphemeridesManager & ephem_manager);
   virtual void ephem_build_tree (EphemeridesManager & ephem_manager);


 protected:

   // Member data

   /**
    * The EphemerisPoint that represents the center of a simple universe.
    */
   EphemerisPoint central_point; //!< trick_units(--)


private:
   ///
   /// Not implemented.
   SinglePlanetEphemeris (const SinglePlanetEphemeris &);
   ///
   /// Not implemented.
   SinglePlanetEphemeris & operator = (const SinglePlanetEphemeris &);
};


/**
 * Retrieve the timestamp.
 * @return Timestamp\n Units: s
 */
inline double
SinglePointEphemeris::timestamp (
   void)
const
{
   return update_time;
}


/**
 * Retrieve the identifier.
 * @return Identifier
 */
inline const char *
SinglePointEphemeris::get_name (
   void)
const
{
   return identifier;
}


/**
 * Update the ephemerides, which in this case is a no-op.
 */
inline void
SinglePointEphemeris::ephem_update (
   void)
{
   return;
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
