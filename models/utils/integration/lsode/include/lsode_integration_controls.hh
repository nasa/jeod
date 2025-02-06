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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup Lsode
 * @{
 *
 * @file models/utils/integration/lsode/include/lsode_integration_controls.hh
 * Define the class LsodeIntegrationControls.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Library dependencies:
  ((lsode_integration_controls.o))

 

*******************************************************************************/


#ifndef JEOD_LSODE_INTEGRATION_CONTROLS_HH
#define JEOD_LSODE_INTEGRATION_CONTROLS_HH

// System includes

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


// Integration includes
#include "er7_utils/integration/core/include/standard_integration_controls.hh"
#include "er7_utils/integration/core/include/generalized_position_derivative.hh"

namespace jeod
{

/**
 * Contains controls for an LSODE integrator.
 */
class LsodeIntegrationControls : public er7_utils::StandardIntegrationControls
{
// JEOD_MAKE_SIM_INTERFACES(LsodeIntegrationControls)
JEOD_MAKE_SIM_INTERFACES(LsodeIntegrationControls)


// Methods:
public:

   /**
    * LsodeIntegrationControls default constructor.
    */
   LsodeIntegrationControls(void);
   LsodeIntegrationControls(unsigned int num_stages);

   /**
    * LsodeIntegrationControls destructor.
    */
   virtual ~LsodeIntegrationControls() {};


   // Member functions.
   virtual unsigned int integrate ( double start_time,
                                    double sim_dt,
                                    er7_utils::TimeInterface & time_interface,
                                    er7_utils::IntegratorInterface & integ_interface,
                                    er7_utils::BaseIntegrationGroup & integ_group);

   /**
    * Create a copy of 'this' StandardIntegrationControls object.
    * @return Clone of 'this'.
    */
   virtual LsodeIntegrationControls * create_copy () const ;


private:
   /**
    * LsodeIntegrationControls assignment operator.
    * @param src  Item to be copied.
    */
   LsodeIntegrationControls & operator=(const LsodeIntegrationControls & src);
   /**
    * LsodeIntegrationControls copy constructor.
    * @param[in] src  Item to be copied.
    */
   LsodeIntegrationControls(const LsodeIntegrationControls & src);

};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
