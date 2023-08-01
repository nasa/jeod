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
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/config_trick10.hh
 * Configure JEOD for use in a Trick10 environment.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and Limitations:
  (((Inclusion of this file and use of the JEOD_MAKE_SIM_INTERFACES macro
     in each defined mandatory for all JEOD class definition files.)))

 

*******************************************************************************/

#ifndef JEOD_CONFIG_TRICK10_HH
#define JEOD_CONFIG_TRICK10_HH

// Define standard integral types
#define JEOD_SIZE_T size_t
#define JEOD_PTRDIFF_T long int
#define JEOD_INTPTR_T long int
#define JEOD_UINTPTR_T unsigned long int


// The following are used by jeod_class.hh.
// Define JEOD_CLASS_ primitives, but only if the JEOD installation
// is not providing an alternate implementation.
#ifndef JEOD_CLASS_IMPLEMENTATION_HEADER

#define JEOD_CLASS_ESTABLISH_FRIENDS(class_name) \
   friend class InputProcessor; \
   friend void init_attrjeod__ ## class_name();

// FUTURE: Add JEOD_CLASS_DECLARE_FRIENDS
#endif


// The following are used by memory_attributes.hh.
// Define JEOD_ATTRIBUTES_ primitives, but only if the JEOD installation
// is not providing an alternate implementation.
#ifndef JEOD_ATTRIBUTES_IMPLEMENTATION_HEADER

#define JEOD_ATTRIBUTES_SIM_ENGINE_HEADER \
   "sim_services/MemoryManager/include/attributes.h"
#define JEOD_ATTRIBUTES_TYPE struct ATTRIBUTES_tag
#define JEOD_ATTRIBUTES_POINTER_TYPE JEOD_ATTRIBUTES_TYPE *

#endif


// The following are used by jeod_integrator_interface.hh
// Define JEOD_SIM_INTEGRATOR_ primitives, but only if the JEOD installation
// is not providing an alternate implementation.
#ifndef JEOD_SIM_INTEGRATOR_IMPLEMENTATION_HEADER

#define JEOD_SIM_INTEGRATOR_SIM_ENGINE_HEADER \
   "sim_services/Integrator/include/Integrator.hh"
#define JEOD_SIM_INTEGRATOR_FORWARD \
   namespace Trick { class Integrator; }
#define JEOD_SIM_INTEGRATOR_POINTER_TYPE Trick::Integrator *
#define JEOD_SIM_INTEGRATOR_ENUM Integrator_type

#endif


// Various classes nominally want to make their data members private.
// This can be problematic for Trick checkpoint/restart for classes that
// inherit from template classes.
#ifndef JEOD_NOMINALLY_PRIVATE

#define JEOD_NOMINALLY_PRIVATE protected

#endif


#endif

/**
 * @}
 * @}
 * @}
 */
