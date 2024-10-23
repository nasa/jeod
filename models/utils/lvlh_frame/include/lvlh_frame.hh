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
 * @file models/utils/lvlh_frame/include/lvlh_frame.hh
 * Define the class LvlhFrame, the class used to represent a local-vertical,
 * local-horizontal reference frame associated with a subject DynBody.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/lvlh_frame.cc))



*******************************************************************************/

#ifndef JEOD_LVLH_FRAME_HH
#define JEOD_LVLH_FRAME_HH

// System includes
#include <string>

// JEOD includes
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/ref_frames/include/ref_frame.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The class used to represent an LVLH reference frame associated
 * with a subject DynBody.
 */
class LvlhFrame
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LvlhFrame)

    // Member data
public:
    /**
     * The LVLH frame defined by the subject frame's motion with respect to
     * the reference planet.
     */
    RefFrame frame; //!< trick_units(--)

    /**
     * The frame whose motion defines LVLH. Can be on a vehicle or not.
     */
    std::string subject_name{""}; //!< trick_units(--)

    /**
     * The planet used as reference for the LVLH frame.
     */
    std::string planet_name{""}; //!< trick_units(--)

protected:
    /**
     * The (moving) frame specified with subject_name.
     */
    RefFrame * subject_frame{}; //!< trick_units(--)

    /**
     * The inertial frame with origin at the center of the specified planet.
     */
    RefFrame * planet_centered_inertial{}; //!< trick_units(--)

private:
    /**
     * A local pointer to the dynamics manager needed for clean-up
     */
    DynManager * local_dm{}; //!< trick_units(--)

    /*
     * Indicates whether frame has already been initialized.
     */
    bool initialized{}; //!< trick_units(--)

    // Methods
public:
    // Default constructor and destructor
    LvlhFrame() = default;
    ~LvlhFrame();

    LvlhFrame(const LvlhFrame &) = delete;
    LvlhFrame & operator=(const LvlhFrame &) = delete;

    // Initialize the model (but not necessarily the state).
    void initialize(DynManager & dyn_manager);

    // Update the state of the LVLH frame relative to its parent
    // frame, which is the planet-centered inertial.
    void update();

    // Specify the defining frame's name
    void set_subject_name(const std::string & new_name);

    // Specify the reference planet's name
    void set_planet_name(const std::string & new_name);

    // Specify the defining frame
    void set_subject_frame(RefFrame & new_frame);

    // Specify the reference planet whose PCI frame defines LVLH
    void set_planet(BasePlanet & new_planet);

protected:
    // Calculate the current LVLH frame orientation based on given state.
    void compute_lvlh_frame(const RefFrameTrans & rel_trans);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
