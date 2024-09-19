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
 * @file models/dynamics/mass/include/mass.hh
 * Define the class MassBody. A MassBody represents both some atomic chunk of
 * mass and an interconnected set of masses, and contains mass properties to
 * represent both this atomic chunk of mass and the composite chunks of mass
 * connected to this MassBody.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ((Rigid Bodies)
   (Tree attachment Structure))

LIBRARY DEPENDENCIES:
  ((../src/mass.cc))



*******************************************************************************/

#ifndef JEOD_MASS_HH
#define JEOD_MASS_HH

// system includes
#include <list>

// Model includes
#include "class_declarations.hh"
#include "mass_body_links.hh"
#include "mass_point.hh"
#include "mass_point_init.hh"
#include "mass_properties.hh"

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "utils/container/include/pointer_list.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>

//! Namespace jeod
namespace jeod
{

class BaseDynManager;

/**
 * Represent both an atomic chunk of mass and an interconnected set of masses.
 * Multiple MassBody objects can be attached to one another in a tree structure.
 * This connected sets of bodies has a composite set of mass properties.
 * Sans these connections, a body will have some core mass proprties.
 *
 * The MassBody class is one of the key classes in JEOD 2.0. In addition to
 * representing masses, it is also the parent of the DynBody class, which is
 * used in JEOD 2.0 to represent vehicles.
 *
 * \par Assumptions and Limitations
 *  - Rigid Bodies
 *  - Tree attachment Structure
 */
class MassBody
{
    JEOD_MAKE_SIM_INTERFACES(jeod, MassBody)

    friend class MassBodyLinks;
    friend class DynBody;

public:
    // Static methods

    // Compute the inertia due to a point mass at some distance from the origin.
    static void compute_point_mass_inertia(double mass, const double r_pt[3], double inertia[3][3]);

    // Member functions
    MassBody();
    virtual ~MassBody();
    MassBody(const MassBody &) = delete;
    void operator=(const MassBody &) = delete;

    // Initialization methods

    // Initialize a body
    void initialize_mass(const MassPropertiesInit & properties, const std::vector<MassPointInit *> & points);

    bool get_mass_properties_initialized()
    {
        return mass_properties_initialized;
    }

    // Set the name
    void set_name(std::string name_in)
    {
        name = std::move(name_in);
    }

    // Attachment methods

    // Attach this body to parent body at specified mass points
    virtual bool attach_to(const std::string & this_point_name,
                           const std::string & parent_point_name,
                           MassBody & parent);

    // Attach this body to parent body at geometric location relative to parent structure frame
    virtual bool attach_to(double offset_pstr_cstr_pstr[3], double T_pstr_cstr[3][3], MassBody & parent);

    // Attach child body to this body at specified mass points
    virtual bool attach_child(const std::string & this_point_name,
                              const std::string & child_point_name,
                              MassBody & child);

    // Attach child body to this body at geometric location relative to this body's structure frame
    virtual bool attach_child(double offset_pstr_cstr_pstr[3], double T_pstr_cstr[3][3], MassBody & child);

    // Test whether attachment is valid
    virtual bool attach_validate(const MassBody & parent, bool generate_message) const;

    // Detachment methods

    // Detach bodies at specified connectivity point
    virtual bool detach(MassBody & from_body);

    // Detach body from its parent
    virtual bool detach(); // cppcheck-suppress virtualCallInConstructor

    // Test whether detachment is valid
    virtual bool detach_validate(const MassBody * parent, bool generate_message) const;

    // Reorientation methods

    virtual bool reattach(double offset[3], double T_pstr_cstr[3][3]);

    // Mass points methods

    const MassPoint * find_mass_point(const std::string & pt_name) const;

    virtual void add_mass_point(const MassPointInit & mass_point_init);

    std::size_t mass_points_size() const;

    // Linkage queries

    // Is this body a child of (direct or indirect) the test body?
    bool is_progeny_of(const MassBody & test_body) const;

    // Find this body's parent and root bodies.
    // Note that the const methods are public. The modifiable method is not.
    virtual const MassBody * get_parent_body() const;
    virtual const MassBody * get_root_body() const;

    // Mass update methods

    void set_update_flag();
    virtual void update_mass_properties();

    // Print methods

    void print_body(FILE * file_ptr, int levels) const;
    void print_tree(const char * file_name, int levels) const;

    // Member data
public:
    /**
     * Body name
     */
    NamedItem name; //!< trick_units(--)

    /**
     * The mass properties of this body alone, excluding child bodies.
     * The core_properties object is a child of the structure_point.
     */
    MassProperties core_properties; //!< trick_units(--)

    /**
     * The mass properties of this body, including child bodies.
     * The composite_properties are core_properties are synonymous for atomic
     * bodies. The composite_properties is a child of the structure_point.
     */
    MassProperties composite_properties; //!< trick_units(--)

    /**
     * When clear, the inverse of the inertia tensor is not calculated.
     * The inverse of the inertia tensor is only calculated if this flag
     * is set and the composite mass is positive.
     */
    bool compute_inverse_inertia{}; //!< trick_units(--)

    /**
     * The location and orientation of the structural frame with respect
     * to the parent body's structural frame.
     * Attaching one MassBody to some other MassBody makes the attaching
     * MassBody's structure_point a child of the attachee's structure_point.
     */
    MassPoint structure_point; //!< trick_units(--)

    /**
     * Indicates this body belongs to a DynBody. Many JEOD functions manipulate
     * MassBody properties with the implementation depending/varying upon
     * whether the MassBody refers to a dynamic body.
     */
    DynBody * const dyn_owner{}; //!< trick_units(--)

protected:
    // Member functions

    /**
     * Create a MassBody, marking is_dynamic as true. Utilized by DynBody
     * via friendship.
     */
    explicit MassBody(DynBody & owner);

    // Attachment methods

    // Attach a root body to some other body (sent to child)
    virtual bool attach_root_body(double offset_pstr_cstr_pstr[3], double T_pstr_cstr[3][3], MassBody & parent);

    // Test validity from connectivity point of view (sent to child)
    virtual bool attach_validate_parent(const MassBody & parent, bool generate_message) const;

    // Test validity from properties point of view (sent to parent)
    virtual bool attach_validate_child(const MassBody & child, bool generate_message) const;

    // Establish logical connectivity between parent and child (sent to child)
    virtual void attach_establish_links(MassBody & parent);

    // Establish physical connectivity between parent and child (sent to parent)
    virtual void attach_update_properties(const double offset_pstr_cstr_pstr[3],
                                          const double T_pstr_cstr[3][3],
                                          MassBody & child);

    // Detachment methods

    // Test validity from connectivity point of view (sent to child)
    virtual bool detach_validate_parent(const MassBody * parent, bool generate_message) const;

    // Test validity from properties point of view (sent to parent)
    virtual bool detach_validate_child(const MassBody & child, bool generate_message) const;

    // Sever logical connectivity between parent and child (sent to child)
    virtual void detach_sever_links(MassBody & parent);

    // Update physical properties of separated parent and child (sent to parent)
    virtual void detach_update_properties(MassBody & child);

    // Linkage queries

    virtual MassBody * get_parent_body_internal();
    virtual MassBody * get_root_body_internal();

    // Mass update methods

    void calc_composite_cm();
    void calc_composite_inertia();

    // Member data
protected:
    /**
     * The dynamics manager for the simulation.
     */
    BaseDynManager * dyn_manager{}; //!< trick_units(--)

    /**
     * Initialized flag. Indicates whether the initialize_mass method has
     * been executed, specifically the initialize_mass_properties method from
     * the initialize_mass method.
     */
    bool mass_properties_initialized{}; //!< trick_units(--)

    /**
     * Linkage to rest of mass tree.
     * Programmatic interfaces:
     * - MassBodyLinks provides accessors to the parent and root and
     * provides methods to attach, detach links (and hence bodies).
     * - This class provides accessors to the same.
     * - Various iterators provide the ability to iterate over child bodies
     * and up the parent chain.
     */
    MassBodyLinks links; //!< trick_units(--)

    /**
     * When set, indicates that an update to the composite mass is needed.
     */
    bool needs_update{}; //!< trick_units(--)

    /**
     * List of points associated with this mass body. @n
     * NOTE WELL: The MassBody manages the memory associated with the contents
     * of this list.
     */
    std::list<MassPoint *> mass_points;

    /**
     * The location and orientation of the composite CoM and body frame with
     * respect to the parent body's composite CoM and body frame.
     */
    MassPoint core_wrt_composite; //!< trick_units(--)

    /**
     * The location and orientation of the composite CoM and body frame with
     * respect to the parent body's structural frame.
     */
    MassPoint composite_wrt_pstr; //!< trick_units(--)

    /**
     * The location and orientation of the composite CoM and body frame with
     * respect to the parent body's composite CoM and body frame.
     */
    MassPoint composite_wrt_pbdy; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
