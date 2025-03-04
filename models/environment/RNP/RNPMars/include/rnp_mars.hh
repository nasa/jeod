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
 * @addtogroup RNPMars
 * @{
 *
 * @file models/environment/RNP/RNPMars/include/rnp_mars.hh
 * A specific implementation of PlanetRNP, for Mars. This is a form of the
 * JPL-developed "Pathfinder" Mars orientation model.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((Konopliv, Alex S., et al)
    (A global solution for the Mars static and seasonal gravity, Mars
     orientation, Phobos and Deimos masses, and Mars ephemeris)
    (Icarus, Volume 182, Issue 1, Pages 23-50) (May 2006))
   ((Konopliv, Alex S., et al)
    (Mars high resolution gravity fields from MRO, Mars season gravity, and
     other dynamical parameters)
    (Icarus, Volume 211, Issue 1, Pages 401-428) (January 2011)))

Assumptions and limitations:
  ((Mars specific))

Library dependencies:
  ((../src/rnp_mars.cc))



*******************************************************************************/

#ifndef JEOD_RNP_MARS_HH
#define JEOD_RNP_MARS_HH

// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rnp.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "nutation_mars.hh"
#include "precession_mars.hh"
#include "rotation_mars.hh"

//! Namespace jeod
namespace jeod
{

class TimeTT;
class TimeDyn;

/**
 * Implements the "Pathfinder" Mars RNP model using the generic RNP framework.
 */
class RNPMars : public PlanetRNP
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RNPMars)

    // Public data members
public:
    /**
     * "Pathfinder" Mars rotation model.
     */
    RotationMars RMars; //!< trick_units(--)

    /**
     * "Pathfinder" Mars nutation model.
     */
    NutationMars NMars; //!< trick_units(--)

    /**
     * "Pathfinder" Mars precession model.
     */
    PrecessionMars PMars; //!< trick_units(--)

    /**
     * The hard coded internal name to be returned on calling
     * the overridden EphemerisInterface function "get_name".
     */
    std::string internal_name{"RNPMars"}; //!< trick_units(--)

    // Private data members
private:
    /**
     * Pointer to the TimeTT used to update this object when ephem_update is invoked.
     */
    TimeTT * tt_ptr{}; //!< trick_units(--)

    /**
     * Pointer to the TimeDyn object, used to time stamp the reference frame
     * when it is being updated.
     */
    TimeDyn * time_dyn_ptr{}; //!< trick_units(--)

    /**
     * The last update time for the RNP, when updated through update_rnp.
     * References TimeDyn.seconds. If the time from time_dyn_ptr is the same
     * as this update time, then the RNP will not be updated; prevents
     * unnecessary updating.
     */
    double last_updated_time_full{}; //!< trick_units(s)

    /**
     * Indicates that last_updated_time_full has never been populated, and
     * that the update must be done regardless of given time.
     */
    bool never_updated_full{true}; //!< trick_units(--)

    /**
     * The last rotational update time, when updated through
     * update_axial_rotation, referencing TimeDyn.seconds. If the time from
     * time_dyn_ptr matches this update time, then the R component of the RNP
     * will not be updated; prevents unnecessary updating.
     */
    double last_updated_time_rotational{}; //!< trick_units(s)

    /**
     * Indicates that last_updated_time_rotational has never been populated,
     * and that the update must be done regardless of given time.
     */
    bool never_updated_rotational{true}; //!< trick_units(--)

    // Public member functions
public:
    RNPMars();
    ~RNPMars() override = default;
    RNPMars & operator=(const RNPMars &) = delete;
    RNPMars(const RNPMars &) = delete;

    // Function to initialize the RNP. Deletes modules and sets their
    // pointers to NULL based on the options set in rnp_type. This then
    // calls the base class initialize.
    void initialize(DynManager & manager) override;

    // Updates the entire RNP, including both setting the time for each
    // module then updating the RNP and sending it to the ref_frame found
    // in the dyn manager at initialization.
    void update_rnp(TimeTT & time_tt);

    // Does the same thing as update_rnp, except it only updates the z-axis
    // axial rotation of the RNP.
    void update_axial_rotation(TimeTT & time_tt);

    // Implementations to make the functions inherited from
    // EphemInterface non-pure virtual

    // Indicates when class was last updated
    double timestamp() const override;

    // Identify the model
    std::string get_name() const override;

    // Update the model. This calls the update_axial_rotation function.
    void ephem_update() override;

    // Private member functions
private:
    // Accesses the TimeManager pointed to by the given TimeTT,
    // and then uses it to get the simulation's DynTime. If
    // a DynTime is not found, this is a fatal error.
    void get_dyn_time_ptr(TimeTT & time_tt);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
