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
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/include/time_manager.hh
 * To manage the various time representations and the converters
 * between them throughout the simulation.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))


Library dependencies:
  ((../src/time_manager.cc)
   (../src/time_manager__initialize.cc))

******************************************************************************/

#ifndef JEOD_TIME_MANAGER_HH
#define JEOD_TIME_MANAGER_HH

// System includes
#include <string>
#include <vector>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/integration/include/jeod_integration_time.hh"

// Model includes
#include "time_dyn.hh"


//! Namespace jeod
namespace jeod {

class TimeManagerInit;
class JeodBaseTime;
class TimeConverter;


/**
 * To manage the various time representations and the converters
 * between them throughout the simulation.
 */
class TimeManager : private JeodIntegrationTime {

  JEOD_MAKE_SIM_INTERFACES(TimeManager)

  friend class TimeManagerInit;


  // Member data
public:

   /**
    * Simulation time (sys.exec.out.time).
    */
  double simtime;                 //!< trick_units(--)

   /**
    * The instance of TimeDyn, the dynamic time that is used as the integration
    * time.
    */
  TimeDyn dyn_time;    //!< trick_units(--)

   /**
    * Size of time_types_ptrs vector.
    */
  int num_types; //!< trick_units(--)


private:

   /**
    * Indicates that the dynamic scale factor changed.
    */
  bool time_change_flag; //!< trick_units(--)

   /**
    * List of pointers to time-types
    */
  std::vector<JeodBaseTime*> time_vector;

   /**
    * List of pointers to time-converters
    */
  std::vector<TimeConverter*> converter_vector;

// Member functions:
public:
  //Constructor
   TimeManager ();
  // Destructor
   ~TimeManager () override;

   void initialize (TimeManagerInit * time_manager_init);

   int time_lookup ( const std::string& name ) const;

   bool get_time_change_flag () const;
   JeodBaseTime * get_time_ptr (const std::string& name) const;
   JeodBaseTime * get_time_ptr (const int index) const;
   TimeConverter * get_converter_ptr (const int index) const;

   bool time_standards_exist (void);

   virtual void update (double time);
   void verify_table_lookup_ends (void);

   void register_time (JeodBaseTime & time_ref);
   void register_time_named (JeodBaseTime & time_ref, const std::string& name);

   void register_converter ( TimeConverter & converter_ref,
                                     const std::string & name_a = "",
                                     const std::string & name_b = "");

   // Expose the private inheritance.
   // This is implemented as a function rather than as a conversion operator
   // so as to make finding the use of this function searchable.
   JeodIntegrationTime& get_jeod_integration_time ();

   // Implement the pure virtual getters inherited from JeodIntegrationTime.
   // Note that this class exposes these functions as public methods.
   double get_time_scale_factor () const override;
   double get_timestamp_time () const override;

   // Make the already-implemented time change subscriber methods inherited
   // from JeodIntegrationTime public.
   using JeodIntegrationTime::add_time_change_subscriber;
   using JeodIntegrationTime::remove_time_change_subscriber;


private:

   // Implement the pure virtual update_time inherited from JeodIntegrationTime.
   void update_time (double time) override;


 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:
   TimeManager (const TimeManager&);
   TimeManager & operator = (const TimeManager&);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
