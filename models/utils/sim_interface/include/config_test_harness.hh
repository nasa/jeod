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
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/config_test_harness.hh
 * Configure JEOD for use in standalone test mode.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and Limitations:
  (((Inclusion of this file and use of the JEOD_MAKE_SIM_INTERFACES macro
     in each defined mandatory for all JEOD class definition files.)))

 

*******************************************************************************/

#ifndef JEOD_CONFIG_TEST_HARNESS_HH
#define JEOD_CONFIG_TEST_HARNESS_HH


// The following are used by jeod_class.hh.
// Define JEOD_CLASS_ primitives, but only if the JEOD installation
// is not providing an alternate implementation.
#ifndef JEOD_CLASS_IMPLEMENTATION_HEADER

#endif


// The following are used by memory_attributes.hh.
// Define JEOD_ATTRIBUTES_ primitives, but only if the JEOD installation
// is not providing an alternate implementation.
#ifndef JEOD_ATTRIBUTES_IMPLEMENTATION_HEADER

#define JEOD_ATTRIBUTES_TYPE int
#define JEOD_ATTRIBUTES_POINTER_TYPE void *

#endif


// The following are used by integrator_interface.hh
// Define JEOD_SIM_INTEGRATOR_ primitives, but only if the JEOD installation
// is not providing an alternate implementation.
#ifndef JEOD_SIM_INTEGRATOR_IMPLEMENTATION_HEADER

#define JEOD_SIM_INTEGRATOR_POINTER_TYPE void *

#endif


// Various classes nominally want to make their data members private,
// but this can get in the way of Trick.
#ifndef JEOD_NOMINALLY_PRIVATE

#define JEOD_NOMINALLY_PRIVATE private

#endif


#endif

/**
 * @}
 * @}
 * @}
 */
