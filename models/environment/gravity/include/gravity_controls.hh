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
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/include/gravity_controls.hh
 * Define the gravity controls.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((../src/gravity_controls.cc))



*******************************************************************************/

#ifndef JEOD_GRAVITY_CONTROLS_HH
#define JEOD_GRAVITY_CONTROLS_HH

// System includes
#include <string>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

class BaseDynManager;
class RefFrame;

/**
 * Specifies whether and how a GravitySource affects a vehicle.
 */
class GravityControls
{
    JEOD_MAKE_SIM_INTERFACES(jeod, GravityControls)

    // Member data
public:
    /**
     * Planet name
     */
    std::string source_name; //!< trick_units(--)

    /**
     * Gravity for this body active?
     */
    bool active{}; //!< trick_units(--)

    /**
     * Ignore non-spherical effects?
     */
    bool spherical{}; //!< trick_units(--)

    /**
     * Compute gravity gradient matrix?
     */
    bool gradient{}; //!< trick_units(--)

    /**
     * Compute only the perturbing gravity?
     */
    bool perturbing_only{}; //!< trick_units(--)

    /**
     * Compute third body gravity using Battin's method
     * (Battin, Mathematics and Methods of Astrodynamics)?
     */
    bool battin_method{}; //!< trick_units(--)

    /**
     * Indicates that the relativistic correction to Newtonian gravitation
     * is to be computed. The public methods enable_relativistic_correction
     * and disable_relativistic_correction set / clear this flag.
     */
    bool relativistic{}; //!< trick_units(--)

    /**
     * Pointer to the GravitySource object named by planet_name.
     * @note Users should not set this data member in the input file.
     */
    GravitySource * body{}; //!< trick_units(--)

    /**
     * Gravitational acceleration toward the GravitySource at the location
     * of the DynBody, including third body effects.
     */
    double grav_accel[3]{}; //!< trick_units(m/s2)

    /**
     * Gradient of the gravitational acceleration.
     */
    double grav_grad[3][3]{}; //!< trick_units(1/s2)

    /**
     * Gravitational potential.
     */
    double grav_pot{}; //!< trick_units(m2/s2)

    /**
     * Square of the magnitude of grav_accel.
     */
    double grav_accel_magsq{}; //!< trick_units(m2/s4)

protected:
    /**
     * Pointer to the simulation-wide GravityManager object.
     * @note Users should not set this data member in the input file.
     */
    GravityManager * grav_manager{}; //!< trick_units(--)

    /**
     * Indicates that a subscription to the planet-centered inertial frame of the
     * planet associated with the gravity_source been issued.
     * @note Users should not set this data member in the input file.
     */
    bool subscribed_to_inertial{}; //!< trick_units(--)

    /**
     * Indicates that a subscription to the planet-centered, planet-fixed frame
     * of the planet associated with the gravity_source been issued.
     * @note Users should not set this data member in the input file.
     */
    bool subscribed_to_pfix{}; //!< trick_units(--)

    /**
     * Some derived classes' calc_nonspherical method computes all contributions
     * to gravitation, including spherical. Those classes need to set this flag.
     * The flag is clear in the base class and in the
     * SphericalHarmonicsGravityControls derived class.
     */
    bool skip_spherical{}; //!< trick_units(--)

public:
    GravityControls() = default;
    virtual ~GravityControls() = default;
    GravityControls(const GravityControls &) = delete;
    GravityControls & operator=(const GravityControls &) = delete;

    // Virtual function to perform basic setup of this control
    virtual void initialize_control( // Return: -- Void
        GravityManager & grav_man);  // In:     -- Reference to Gravity Manager

    // Virtual function to perform basic reset of this control
    virtual void reset_control(        // Return: -- Void
        BaseDynManager & dyn_manager); // In:     -- Reference to dynamics manager

    // Compute the acceleration due to the gravitional body associated with
    // this control at the specified point of interest. This version of
    // gravitation must not be used if relativistic gravity is enabled.
    virtual void gravitation(         // Return: --   Void
        const double integ_pos[3],    // In:     m    Pt. of interest, integ coords
        unsigned int integ_frame_idx, // In:     --   Integ frame index
        double body_grav_accel[3],    // Out:    m/s2 Accel for given grav body
        double dgdx[3][3],            // Out:    1/s2 Gradient for given grav body
        double Pot[1]);               // Out:    --   Potential

    // Compute the acceleration due to the gravitional body associated with
    // this control at the specified point of interest. This version of
    // gravitation is needed if relativistic gravity is enabled.
    virtual void gravitation(const RefFrame & point_of_interest,
                             // In:     --    Pt. of interest, as a frame
                             unsigned int integ_frame_idx, // In:     --    Integ frame index
                             double body_grav_accel[3],    // Out:    m/s2  Accel for given grav body
                             double dgdx[3][3],            // Out:    1/s2  Gradient for given grav body
                             double & pot);                // Out:    m2/s2 Specific potential

    /**
     * Compares the magnitude of the two input gravity controls, returning true
     * if a->grav_accel_magsq is less than b->grav_accel_magsq, false otherwise.
     * \return Result of comparison
     * \param a  First control to be compared.
     * \param b  Second control to be compared.
     */
    static bool accel_mag_less_ptr(const GravityControls * a, const GravityControls * b)
    {
        return a->grav_accel_magsq < b->grav_accel_magsq;
    }

protected:
    /**
     * Nominally, compute the non-spherical contribution to gravity at a given
     * position. Derived classes whose override of this function computes the
     * full gravitation model, including the spherical contribution, should set
     * the skip_spherical flag.
     * \param[in] posn  Inertial position of the point of interest relative
     *                  to the gravitional body.
     * \param[out] body_grav_accel  Acceleration at the point of interest
     *                              due to the gravitional body.
     * \param[out] dgdx  Gravity gradient at the point of interest.
     * \param[out] Pot  Specific gravitational potential energy.
     */

    virtual void calc_nonspherical(const double integ_pos[3],
                                   const double posn[3],
                                   const GravityIntegFrame & grav_source_frame,
                                   double body_grav_accel[3],
                                   double dgdx[3][3],
                                   double & pot) = 0;

    /**
     * Calculates the relativistic correction to gravitational acceleration.
     * @param point_of_interest  The point of interest, as a reference frame.
     * @param rel_pos  Displacement vector from the grav body to the POI.
     * @param rel_vel  Time derivative of rel_pos.
     * \param perturbing_accel Output relativistic accel, sans the Newtonian term.
     */
    void calc_relativistic(const RefFrame & point_of_interest,
                           const double rel_pos[3],
                           const double rel_vel[3],
                           double perturbing_accel[3]);

    /**
     * Calculate the spherical gravitational acceleration, either directly
     * or as a third body acceleration.
     * @param integ_pos  Point of interest location, integ frame coordinates
     * @param posn  Vector from gravitational body to point of interest
     * @param grav_source_frame  Frame corresponding to the gravitational body
     * @param body_grav_accel  Acceleration at integ_pos due to the grav body
     * @param dgdx  Gravity gradient at integ_pos due to the grav body
     * @param pot  Gravitational potential at integ_pos due to the grav body.
     */
    void calc_spherical(const double integ_pos[3],
                        const double posn[3],
                        const GravityIntegFrame & grav_source_frame,
                        double body_grav_accel[3],
                        double dgdx[3][3],
                        double & pot);
};

} // namespace jeod

#ifdef TRICK_VER
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "gravity_source.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
