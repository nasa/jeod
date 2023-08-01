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
 * @file models/environment/time/include/time_dyn.hh
 * Represents the Dynamic Time in the simulation.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((Dynamic Time always starts at 0.0 for every simulation.))



Library dependencies:
  ((../src/time_dyn.cc))
******************************************************************************/

#ifndef JEOD_TIME_DYN_HH
#define JEOD_TIME_DYN_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "time.hh"


//! Namespace jeod
namespace jeod {

class TimeManagerInit;


/**
 * Represents the Dynamic Time in the simulation.
 */
class TimeDyn : public JeodBaseTime {

  JEOD_MAKE_SIM_INTERFACES(TimeDyn)

// Member Data
public:
   /**
    * Multiplicative difference between sim-time and dyn-time.  This is the
    * value that is changed externally.
    */
  double scale_factor;//!< trick_units(--)

private:
   /**
    * Private copy of scale_factor.  This value should not be changed
    * externally; it is used for comparison purposes to identify when
    * "scale_factor" has changed.
    */
  double ref_scale;   //!< trick_units(--)
   /**
    * Extrapolated difference between sim-time and dyn-time at the sim-start
    * (0 if there are no changes to direction or scale)
    */
  double offset;      //!< trick_units(--)

// Member functions:
public:
  //Constructor
   TimeDyn ();
  // Destructor
   ~TimeDyn () override;
   bool update_offset (void);

private:
   void initialize_initializer_time (TimeManagerInit * tm_init) override;
   void update (void) override;

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:
   TimeDyn (const TimeDyn&);
   TimeDyn & operator = (const TimeDyn&);
};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
