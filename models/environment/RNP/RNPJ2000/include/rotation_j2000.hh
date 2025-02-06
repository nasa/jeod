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
 * @addtogroup RNPJ2000
 * @{
 *
 * @file models/environment/RNP/RNPJ2000/include/rotation_j2000.hh
 * Model the axial rotation portion of the RNP routine for the Standard Epoch
 * J2000. This is a form of the IAU-76/FK5 RNP model, as implemented in Jeod
 * 1.52
 */

/*******************************************************************************

Purpose:
  ()

Reference:
      (((Mulcihy, David D. and Bond, Victor R.)
        (The RNP Routine for the STandard Epoch J2000)
        (NASA:JSC-24574) (NASA Engineering Directorate, Navigation, Control,
         and Aeronautics Division) (September 1990) (--)))

Assumptions and limitations:
  ((Earth specific))

Library dependencies:
  ((../src/rotation_j2000.cc))


*******************************************************************************/
#ifndef ROTATION_J2000_HH
#define ROTATION_J2000_HH

// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "nutation_j2000.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Implements the axial rotation portion of J2000 RNP.
 */
class RotationJ2000 : public PlanetRotation
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RotationJ2000)

public: // public data members
    /**
     * The nominal axial rotational velocity of the earth
     */
    double planet_rotational_velocity{}; //!< trick_units(rad/s)

    /**
     * Pointer to the J2000 nutation object, used
     * for get obliquity information out. Will be
     * NULL (automatically) if anything but
     * Full_Term_RNP is set in the RNPJ2000 object
     */
    NutationJ2000 * nutation{}; //!< trick_units(--)

    /**
     * Tells the rotation object if it should use a full
     * blown rotation formulation, or just use the time
     * passed multiplied by the rotational velocity.
     * Used with the different intialization options
     * for the main RNP class
     */
    bool use_full_rnp{true}; //!< trick_units(--)

    /**
     * The last theta_gast (angle the earth had axially
     * rotated) calculated.
     */
    double theta_gast{}; //!< trick_units(rad)

    /**
     * GMST, currently saved for logging purposes
     */
    double GMST{}; //!< trick_units(--)

public: // public member functions
    RotationJ2000() = default;
    ~RotationJ2000() override = default;
    RotationJ2000 & operator=(const RotationJ2000 &) = delete;
    RotationJ2000(const RotationJ2000 &) = delete;

    // RotationJ2000 specific implementation of update rotation
    // Before this is called, the
    // current_time parameter must be set to seconds since standard epoch
    // J2000, in the GMST time standard.

    void update_rotation() override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
