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
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPMars
 * @{
 *
 * @file models/environment/RNP/RNPMars/include/precession_mars.hh
 * Represent the precession portion of the JPL-developed "Pathfinder" Mars
 * orientation model.
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
((../src/precession_mars.cc))



*******************************************************************************/

#ifndef PRECESSION_MARS_HH
#define PRECESSION_MARS_HH

// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "nutation_mars.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Implements the axial rotation portion of the "Pathfinder" Mars RNP model.
 */
class PrecessionMars : public PlanetRotation
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PrecessionMars)

    // Public data members
public:
    /**
     * Pointer to the Mars nutation object, used to access nutation in longitude
     * information. Will be NULL (automatically) if anything but Full_Term_RNP
     * is set in the RNPMars object.
     */
    NutationMars * nutation{}; //!< trick_units(--)

    /**
     * The (constant) precession angle of Mars at the J2000 epoch.
     */
    double psi_at_j2000{}; //!< trick_units(rad)

    /**
     * The (constant) simple precession rate of Mars.
     */
    double psi_dot{}; //!< trick_units(rad/s)

    /**
     * The most recent calculated value of the precession angle for Mars,
     * measured since J2000 epoch.
     */
    double psi_precess{}; //!< trick_units(rad)

    /**
     * The (constant) angle from the J2000 vernal equinox to the node of the
     * Mars mean orbit and ICRF x-y plane.
     */
    double N{}; //!< trick_units(rad)

    /**
     * The (constant) inclination of the Mars mean orbit relative to
     * the ICRF x-y plane.
     */
    double J{}; //!< trick_units(rad)

    // Private data members
private:
    /**
     * The (constant) rotation matrix calculated from N and J
     */
    double NJ_matrix[3][3]{}; //!< trick_units(--)

    // Public member functions
public:
    PrecessionMars() = default;
    ~PrecessionMars() override = default;
    PrecessionMars & operator=(const PrecessionMars &) = delete;
    PrecessionMars(const PrecessionMars &) = delete;

    // PrecessionMars specific implementation of update_rotation, inherited
    // from PlanetRotation. Before this is called, the current_time
    // parameter must be set to Julian days since standard epoch J2000, per
    // Konopliv references.
    void update_rotation() override;

    // Initialize the constant rotation matrices associated with data
    // members N and J, so they don't have to be calculated repeatedly.
    void compute_fixed_matrices();
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
