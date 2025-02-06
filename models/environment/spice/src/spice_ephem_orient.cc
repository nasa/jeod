/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Spice
 * @{
 *
 * @file models/environment/spice/src/spice_ephem_orient.cc
 * Define the methods for the SPICE-specific ephemeris orientation class.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependencies:
  ((environment/ephemerides/ephem_item/src/ephem_orient.cc))


*******************************************************************************/

// System includes

// SPICE includes
#include "SpiceUsr.h"

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/spice_ephem_orient.hh"

//! Namespace jeod
namespace jeod
{

/**
 * SpiceEphemOrientation default constructor.
 */
SpiceEphemOrientation::SpiceEphemOrientation()
{
    JEOD_REGISTER_CLASS(SpiceEphemOrientation);
}

/**
 * Update the rotational state of the target frame
 * \param[in] time_tdb Ephemeris time (TDB)\n Units: s
 * \param[in] time_dyn dyn time for timestamp\n Units: s
 */
void SpiceEphemOrientation::update(double time_tdb, double time_dyn)
{
    // Call the wrapper function around sxform_c(), the SPICE function
    // that attempts to populate the 6 x 6 matrix that will be used to
    // update the target frame's state.  get_spice_transformation() contains
    // the safety check which confirms whether the frame is covered by the
    // currently loaded set of SPICE kernels.
    double trans6x6[6][6];
    get_spice_transformation(time_tdb, trans6x6);

    // Extract the upper left and lower left 3 x 3 submatrices of the 6 x 6
    // matrix returned by get_spice_transformation(). In this case, these are
    // respectively the transformation from J2000 to planet-fixed, and its
    // time derivative.
    double t_dot[3][3]; // derivative of T_parent_this

    for(unsigned ii = 0; ii < 3; ++ii)
    {
        for(unsigned jj = 0; jj < 3; ++jj)
        {
            target_frame->state.rot.T_parent_this[ii][jj] = trans6x6[ii][jj];
            t_dot[ii][jj] = trans6x6[3 + ii][jj];
        }
    }

    // Calculate and store the target frame's angular velocity
    double omega_b_wrt_a_in_b[3][3]; // skew-symmetric ang-vel matrix

    Matrix3x3::product_right_transpose(target_frame->state.rot.T_parent_this, t_dot, omega_b_wrt_a_in_b);

    target_frame->state.rot.ang_vel_this[0] = -omega_b_wrt_a_in_b[1][2];
    target_frame->state.rot.ang_vel_this[1] = omega_b_wrt_a_in_b[0][2];
    target_frame->state.rot.ang_vel_this[2] = -omega_b_wrt_a_in_b[0][1];

    target_frame->state.rot.compute_quaternion();

    // Update timestamps for both this model and the target frame
    update_time = time_dyn;
    target_frame->set_timestamp(time_dyn);
}

/**
 * Confirm that the target frame exists in the loaded SPICE kernels.
 * \param[in] time_tdb Ephemeris time (TDB)
 */
void SpiceEphemOrientation::validate(double time_tdb)
{
    // Call the wrapper function around sxform_c(), the SPICE function
    // that attempts to populate the 6 x 6 matrix that will be used to
    // update the target frame's state.  get_spice_transformation() contains
    // the safety check which confirms whether the frame is covered by the
    // currently loaded set of SPICE kernels.
    double trans6x6[6][6];
    get_spice_transformation(time_tdb, trans6x6);
}

/**
 * Populate the SPICE 6 x 6 matrix via sxform_c().
 * \param[in] time_tdb Ephem time (TDB)
 * \param[out] trans6x6 Spice matrix
 */
void SpiceEphemOrientation::get_spice_transformation(double time_tdb, double trans6x6[6][6])
{
    // sxform_c() is a SPICE library function that populates a 6 x 6 matrix.
    // The matrix is populated as follows: [[R, 0]; [dR/dt, R]], where R
    // is the 3 x 3 transformation matrix from parent to child, and
    // dR/dt is its time derivative. Note that only the upper left (R) and lower
    // left (dR/dt) submatrices will be needed to perform the target frame state
    // update.
    sxform_c("J2000", spice_frame_name.c_str(), time_tdb, trans6x6);

    // Check for successful call of sxform_c()
    if(failed_c())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::item_not_in_file,
                             "The reference frame %s was not in the loaded kernels.",
                             spice_frame_name.c_str());

        // Not reached
        return;
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
