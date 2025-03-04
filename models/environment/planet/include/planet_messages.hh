//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @addtogroup Planet
 * @{
 *
 * @file models/environment/planet/include/planet_messages.hh
 * Define the class PlanetMessages, the class that specifies the message IDs
 * used in the planet model.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the planet model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/planet_messages.cc))



*******************************************************************************/

#ifndef JEOD_PLANET_MESSAGES_HH
#define JEOD_PLANET_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Specifies the message IDs used in the planet model.
 */
class PlanetMessages
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PlanetMessages)

    // Static member data
public:
    /**
     * Issued when the name is invalid.
     */
    static const char * name_error; //!< trick_units(--)

    /**
     * Issued when the model has not been properly registered/initialized.
     */
    static const char * registration_error; //!< trick_units(--)

    /**
     * Issued when some value is invalid.
     */
    static const char * domain_error; //!< trick_units(--)

    // Member functions
    // This class is not instantiable.
    // The constructors and assignment operator for this class are deleted.
    PlanetMessages() = delete;
    PlanetMessages(const PlanetMessages &) = delete;
    PlanetMessages & operator=(const PlanetMessages &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
