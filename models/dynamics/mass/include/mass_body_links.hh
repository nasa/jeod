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
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/include/mass_body_links.hh
 * Define the class MassBodyLinks, the class that encapsulates the links between
 * mass bodies.
 */

/*******************************************************************************

Purpose:
 ()



*******************************************************************************/

#ifndef JEOD_MASS_BODY_LINKS_HH
#define JEOD_MASS_BODY_LINKS_HH

// System includes

// JEOD includes
#include "utils/ref_frames/include/tree_links.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "mass_messages.hh"

//! Namespace jeod
namespace jeod
{

class MassBodyLinks;

/**
 * Encapsulates the links between mass bodies.
 */
class MassBodyLinks : public TreeLinks<MassBodyLinks, MassBody, MassBodyMessages>
{
    JEOD_MAKE_SIM_INTERFACES(jeod, MassBodyLinks)

    // Member functions
public:
    /**
     * Non-default constructor.
     * @param container_in The MassBody object that contains this object.
     */
    explicit MassBodyLinks(MassBody & container_in)
        : TreeLinks<MassBodyLinks, MassBody, MassBodyMessages>(container_in, default_path_size)
    {
    }

    // The default constructor, copy constructor, and copy assignment
    // operator don't make sense and are deleted.
    MassBodyLinks() = delete;
    MassBodyLinks(const MassBodyLinks &) = delete;
    void operator=(const MassBodyLinks &) = delete;

    /**
     * Destructor.
     */
    ~MassBodyLinks() override = default;

private:
    const static unsigned int default_path_size = 8; /* trick_units(--) @n
       The default length of a MassBodyLinks object's path_to_node_ array. */
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
