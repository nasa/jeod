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
 * @addtogroup LvlhFrame
 * @{
 *
 * @file models/utils/lvlh_frame/include/lvlh_type.hh
 * Define the class LvlhType, which identifies the type of LVLH desired to
 * be calculated.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_LVLH_TYPE_HH
#define JEOD_LVLH_TYPE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes

//! Namespace jeod
namespace jeod
{

/**
 * The class used to identify the type of LVLH desired.
 */
class LvlhType
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LvlhType)

    // Enumerations
public:
    /**
     * An enumeration to specify the type of LVLH coordinates to use,
     * whether rectilinear, circular curvilinear, or elliptical
     * curvilinear. As of March 2015, elliptical is not implemented.
     */
    enum Type
    {
        Rectilinear = 0, /*
           Compute relative LVLH state using rectilinear coordinates; default. */

        CircularCurvilinear = 1, /*
           Compute relative LVLH state using circular curvilinear coordinates. */

        EllipticalCurvilinear = 2 /*
           Compute relative LVLH state using elliptical curvilinear coordinates;
           option not yet implemented as of March 2015. */
    };

    // Member data
public:
    /**
     * Indicates type of LVLH coordinates desired. Default is rectilinear.
     */
    Type value; //!< trick_units(--)

    // Methods
public:
    /**
     * Default constructor.
     */
    LvlhType()
    {
        value = LvlhType::Rectilinear;
    }
};

} // namespace jeod

#ifdef TRICK_VER
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
