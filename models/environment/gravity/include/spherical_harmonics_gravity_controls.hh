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
 * @file models/environment/gravity/include/spherical_harmonics_gravity_controls.hh
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
  ((../src/spherical_harmonics_gravity_controls.cc))



*******************************************************************************/

#ifndef JEOD_SPHERICAL_HARMONICS_GRAVITY_CONTROLS_HH
#define JEOD_SPHERICAL_HARMONICS_GRAVITY_CONTROLS_HH

// System includes

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "gravity_controls.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Specifies whether and how a SphericalHarmonicsGravitySource affects a vehicle.
 */
class SphericalHarmonicsGravityControls : public GravityControls
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SphericalHarmonicsGravityControls)

    // Member data

protected:
    /**
     * Indicates that the minimum radius threshold has been crossed and that a
     * warning has been issued for such. This prevents a spew of messages
     * regarding such under-threshold conditions.
     * @note Users should not set this data member in the input file unless you
     * wish to disable all such messages.
     */
    bool min_radius_warn{}; //!< trick_units(--)

public:
    /**
     * The GravitySource pointer from the base class, recast.
     * @note Users should not set this data member in the input file.
     */
    SphericalHarmonicsGravitySource * harmonics_source{}; //!< trick_units(--)

    /**
     * LeGendre polynomials used to calculate non-spherical attraction
     */
    double ** Pnm{}; //!< trick_units(--)

    /**
     * Coefficient degree to be used for totaling up all active delta_coeffs.
     */
    unsigned int delta_degree{}; //!< trick_units(--)

    /**
     * Coefficient order to be used for totaling up all active delta_coeffs.
     */
    unsigned int delta_order{}; //!< trick_units(--)

    /**
     * Array for collecting all active normalized real (cosine) variational
     * spherical harmonic coefficients.
     */
    double ** delta_Cnm{}; //!< trick_units(--)

    /**
     * Array for collecting all active normalized real (sine) variational
     * spherical harmonic coefficients.
     */
    double ** delta_Snm{}; //!< trick_units(--)

    /**
     * delta C20 coefficient for collecting first order effects of all
     * active delta_coeffs.
     */
    double total_dC20{}; //!< trick_units(--)

    /**
     * Non-spherical degree to be used.  NOTE: this data being public is
     * deprecated and should not be relied upon in future releases. Use the
     * provided accessor methods instead.
     */
    unsigned int degree{}; //!< trick_units(--)

    /**
     * Non-spherical order to be used.  NOTE: this data being public is
     * deprecated and should not be relied upon in future releases. Use the
     * provided accessor methods instead.
     */
    unsigned int order{}; //!< trick_units(--)

    /**
     * Non-spherical degree to be used for computing gradient.  NOTE: this data
     * being public is deprecated and should not be relied upon in future
     * releases. Use the provided accessor methods instead.
     */
    unsigned int gradient_degree{}; //!< trick_units(--)

    /**
     * Non-spherical order to be used for computing gradient.  NOTE: this data
     * being public is deprecated and should not be relied upon in future
     * releases. Use the provided accessor methods instead.
     */
    unsigned int gradient_order{}; //!< trick_units(--)

    /**
     * List of controls for variational gravity effects like solid-body tides
     */
    JeodPointerVector<SphericalHarmonicsDeltaControls>::type var_effects; //!< trick_io(**)

public:
    SphericalHarmonicsGravityControls();
    ~SphericalHarmonicsGravityControls() override;
    SphericalHarmonicsGravityControls(const SphericalHarmonicsGravityControls &) = delete;
    SphericalHarmonicsGravityControls & operator=(const SphericalHarmonicsGravityControls &) = delete;

    // Perform derived-class specific setup of this control
    void initialize_control(                     // Return: -- Void
        GravityManager & grav_manager) override; // In:     -- Reference to Gravity Manager

    // Add a new delta-control to var_effects list
    virtual void add_deltacontrol(SphericalHarmonicsDeltaControls * delta_control);

    // Accessors to modify/obtain desired degree and order to use
    unsigned int get_degree(); // Return: --  Current degree

    unsigned int get_order(); // Return: --  Current order

    void get_degree_order(             // Return: --  Void
        unsigned int & current_degree, // Out:    --  Current degree
        unsigned int & current_order); // Out:    --  Current order

    unsigned int get_grad_degree(); // Return: --  Current gradient degree

    unsigned int get_grad_order(); // Return: --  Current gradient order

    void get_grad_degree_order(          // Return: --  Void
        unsigned int & curr_grad_degree, // Out:    --  Current degree
        unsigned int & curr_grad_order); // Out:    --  Current order

    void set_degree(              // Return: --  Void
        unsigned int new_degree); // In:     --  New desired degree

    void set_order(              // Return: --  Void
        unsigned int new_order); // In:     --  New desired order

    void set_degree_order(       // Return: --  Void
        unsigned int new_degree, // In:     --  New desired degree
        unsigned int new_order); // In:     --  New desired order

    void set_grad_degree(              // Return: --  Void
        unsigned int new_grad_degree); // In:     --  New desired degree

    void set_grad_order(              // Return: --  Void
        unsigned int new_grad_order); // In:     --  New desired order

    void set_grad_degree_order(       // Return: --  Void
        unsigned int new_grad_degree, // In:     --  New desired degree
        unsigned int new_grad_order); // In:     --  New desired order

    /**
     * Disable minimum radius warnings for this spherical harmonics gravity control.
     */
    void disable_min_radius_warnings()
    {
        min_radius_warn = true;
    }

protected:
    // Compute non-spherical gravity acceleration, potential at a point
    void calc_nonspherical( // Return: --  Void
        const double integ_pos[3],
        const double posn[3],
        const GravityIntegFrame & grav_source_frame,
        double body_grav_accel[3],
        double dgdx[3][3],
        double & pot) override; // Out:   --   Potential

    // Check the validity of this control
    virtual void check_validity(); // Return: --  Void

    // Update all of the active variational gravity effects
    virtual void update_deltacoeffs(); // Return: --  Void

    // Add up (via superposition) all active variational gravity effects
    virtual void sum_deltacoeffs(); // Return: --  Void
};

} // namespace jeod

#ifdef TRICK_VER
#include "spherical_harmonics_gravity_source.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
