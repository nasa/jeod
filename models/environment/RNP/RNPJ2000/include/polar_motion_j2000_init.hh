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
 * @addtogroup RNPJ2000
 * @{
 *
 * @file models/environment/RNP/RNPJ2000/include/polar_motion_j2000_init.hh
 * Used to load data and initialize the PolarMotionJ2000 class through
 * PolarMotionJ2000::initialize
 */

/*******************************************************************************

Purpose:
  ()

Reference:
    (((G.H. Kaplan)
      (U.S. Naval Observatory Vector Astrometry Subroutines)
      (NOVAS FORTRAN vers F2.0) (1 Nov 98)))

Assumptions and limitations:
  ((Implementation of PolarMotion is only limited to the data that can be
    put into the initialization file))

Library dependencies:
  ((../src/polar_motion_j2000_init.cc))



*******************************************************************************/

#ifndef POLAR_MOTION_J2000_INIT_HH
#define POLAR_MOTION_J2000_INIT_HH

// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation_init.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initializes the PolarMotionJ2000 object.
 */
class PolarMotionJ2000Init : public PlanetRotationInit
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PolarMotionJ2000Init)

    // class for initialization (loading of the coefficients and related data)
    // of the PolarMotionJ2000 model
public: // public data members
    /**
     * Current X Polar coordinate
     */
    double xp{}; //!< trick_units(rad)

    /**
     * Current Y Polar coordinate
     */
    double yp{}; //!< trick_units(rad)

    /**
     * X Polar coordinate table
     */
    double * xp_tbl{}; //!< trick_units(rad)

    /**
     * Y Polar coordinate table
     */
    double * yp_tbl{}; //!< trick_units(rad)

    /**
     * Independent variable for the XY coordinate table
     */
    double * polar_mjd{}; //!< trick_units(--)

    /**
     * If true, do no table lookup and use the
     * currently set xp and yp
     */
    bool override_table{}; //!< trick_units(--)

    /**
     * Size - 1 of xp_tbl, yp_tbl and polar_mjd (last index)
     */
    unsigned int last_table_index{}; //!< trick_units(count)

public: // public member functions
    PolarMotionJ2000Init() = default;
    ~PolarMotionJ2000Init() override;
    PolarMotionJ2000Init & operator=(const PolarMotionJ2000Init &) = delete;
    PolarMotionJ2000Init(const PolarMotionJ2000Init &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
