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
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/include/mass_point_init.hh
 * Define the class MassPointInit, which initializes a MassPoint
 * object.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Library Dependencies:
  ((../src/mass_point_init.cc))



*******************************************************************************/

#ifndef JEOD_MASS_POINT_INIT_HH
#define JEOD_MASS_POINT_INIT_HH

// Model includes
#include "class_declarations.hh"

// JEOD includes
#include "utils/orientation/include/orientation.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <string>
#include <utility>

#ifdef SWIG
%include "std_string.i"
#endif

//! Namespace jeod
namespace jeod
{

/**
 * Contains data used to initialize a MassPoint.
 */
class MassPointInit
{
    JEOD_MAKE_SIM_INTERFACES(jeod, MassPointInit)

public:
    /**
     * Specifies sense of the input point orientation data.
     * The overloading of names is intentional.
     */
    enum FrameSpec
    {
        StructToBody = 0,  ///< Orientation specifies structure-to-body transform
        StructToCase = 0,  ///< Orientation specifies structure-to-case transform
        StructToPoint = 0, ///< Orientation specifies structure-to-point transform
        StructToChild = 0, ///< Orientation specifies structure-to-child transform
        BodyToStruct = 1,  ///< Orientation specifies body-to-structure transform
        CaseToStruct = 1,  ///< Orientation specifies case-to-structure transform
        PointToStruct = 1, ///< Orientation specifies point-to-structure transform
        ChildToStruct = 1  ///< Orientation specifies child-to-structure transform
    };

    // Member data

    /**
     * Mass point location expressed in mass element structural coordinates.
     */
    double position[3]{}; //!< trick_units(m)

    /**
     * Mass point frame orientation specification.
     */
    Orientation pt_orientation; //!< trick_units(--)

    /**
     * Indicates whether user orientation input defines the structure-to-body
     * or body-to-structure transformation matrix.
     */
    FrameSpec pt_frame_spec{StructToPoint}; //!< trick_units(--)

    /**
     * Item name, with the following semantics for items that have a name:
     * - The mass point name will always be of the form "mass_name.point_name".
     * - If the supplied name does not begin with "mass_name.", this prefix
     *   will be applied to the supplied name in naming the mass point.
     *
     * This can be left as the empty string for items that don't have a name.
     */
    std::string name{""}; //!< trick_units(--)

    // Member functions
    MassPointInit();
    virtual ~MassPointInit() = default;

    // Initialize a mass point.
    void initialize_mass_point(MassPoint & mass_point) const;

    /**
     * Set the name.
     */
    void set_name(std::string name_in)
    {
        name = std::move(name_in);
    }
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
