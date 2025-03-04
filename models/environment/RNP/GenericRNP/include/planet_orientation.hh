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
 * @addtogroup RNP
 * @{
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/include/planet_orientation.hh
 * Establish a pure virtual framework for interfacing with the DynManager to set a planet
 * orientation, meaning the transformation information from a planet's
 * inertial frame to it's planet fixed frame.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  ((none))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((../src/planet_orientation.cc))



*******************************************************************************/

#ifndef PLANET_ORIENTATION_HH
#define PLANET_ORIENTATION_HH

// System includes
#include <string>
#include <utility>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_item/include/ephem_orient.hh"
#include "utils/ref_frames/include/ref_frame_interface.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class DynManager;
class Planet;
class TimeManager;

// An interface for any object intending to interact with a DynManager for
// the purpose of controlling a planet's orientation
/**
 * The generic framework for orientation models to interact with a DynManager object
 */
class PlanetOrientation : public EphemerisInterface,
                          public RefFrameOwner
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PlanetOrientation)

public:
    PlanetOrientation() = default;
    ~PlanetOrientation() override = default;
    PlanetOrientation & operator=(const PlanetOrientation & rhs) = delete;
    PlanetOrientation(const PlanetOrientation & rhs) = delete;

    //  initializer, goes to the dyn manager given and searches for the the
    //  planet indicated by the user inputted name, in the given dyn manager.
    //  Will exec_terminate if the named planet is not found.
    virtual void initialize(DynManager & dyn_manager);

    // implementing the virtual functions from ActivateInterface
    void activate() override;
    void deactivate() override;

    /**
     * Is the orientation model actively updating?  Defaults to true.
     */
    bool active{true}; //!< trick_units(--)

    /**
     * The planet the attitude model will be working on
     */
    Planet * planet{}; //!< trick_units(--)

    /**
     * Name of the planet the attitude model will be working on. Planet must be
     * found in the DynManager sent in at initialization
     */
    std::string name{""}; //!< trick_units(--)

    /**
     * The current rotational state of the planet
     */
    RefFrameRot * planet_rot_state{}; //!< trick_units(--)

    /**
     * Nominal axial velocity of the earth
     */
    double planet_omega{}; //!< trick_units(rad/s)

    // PlanetOrientation specific instantiations of EphemerisInterface virtuals

    // timestamp is left to inheriting classes
    // name is left to inheriting classes

    // Initialize the ephemeris aspect of the class
    void ephem_initialize(EphemeridesManager & manager) override;

    // Activate the model
    void ephem_activate(EphemeridesManager & manager) override;

    // Build this model's contribution to the reference frame tree.
    // Nominally does nothing for PlanetOrientation and its inheriters.
    // This can, obviously, be overridden.
    void ephem_build_tree(EphemeridesManager & manager) override;

    // ephem_update is left to the inheriting class.

    /**
     * The ephemeris interface to the in question orientation
     */
    EphemerisOrientation orient_interface; //!< trick_units(--)

    /**
     * Setter for the name.
     */
    void set_name(std::string name_in)
    {
        name = std::move(name_in);
    }

    /**
     * A re-declaration of the pure virtual function in order
     * to convince trick that yes, this is a pure virtual class.
     * @return Planet name.
     */
    std::string get_name() const override = 0;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
