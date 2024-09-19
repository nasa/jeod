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
 * @file models/environment/RNP/RNPJ2000/include/rnp_j2000.hh
 * A specific implementation of PlanetRNP, for Earth. This is a form of the
 * IAU-76/FK5 RNP model, as implemented in Jeod 1.52
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
  ((../src/rnp_j2000.cc))


*******************************************************************************/

#ifndef RNP_J2000_HH
#define RNP_J2000_HH

// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rnp.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "nutation_j2000.hh"
#include "polar_motion_j2000.hh"
#include "precession_j2000.hh"
#include "rotation_j2000.hh"

//! Namespace jeod
namespace jeod
{

class TimeTT;
class TimeUT1;
class TimeGMST;
class TimeDyn;

// The J2000 RNP implementation from Jeod 1.52
/**
 * Implements the J2000 RNP model using the generic RNP framework.
 */
class RNPJ2000 : public PlanetRNP
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RNPJ2000)

public: // public member variables
    // Model instances for rotation, nutation, precession and polar motion.
    /**
     * Earth J2000 rotation model.
     */
    RotationJ2000 RJ2000; //!< trick_units(--)

    /**
     * Earth J2000 nutation model.
     */
    NutationJ2000 NJ2000; //!< trick_units(--)

    /**
     * Earth J2000 precession model.
     */
    PrecessionJ2000 PJ2000; //!< trick_units(--)

    /**
     * Earth J2000 polar motion model.
     */
    PolarMotionJ2000 PMJ2000; //!< trick_units(--)

public: // public member functions
    RNPJ2000();
    ~RNPJ2000() override = default;
    RNPJ2000 & operator=(const RNPJ2000 &) = delete;
    RNPJ2000(const RNPJ2000 &) = delete;

    // function to initialize the RNP. Deletes modules and sets their
    // pointers to null based on the options set in enable_polar and
    // rnp_type. This then calls the base class initialize
    void initialize(DynManager & manager) override;

    // updates the entire RNP, including both setting the time for each
    // module then updating the RNP and sending it to the ref_frame found
    // in the dyn manager at initialization
    void update_rnp(const TimeTT & time_tt, TimeGMST & time_gmst, const TimeUT1 & time_ut1);

    // does the same thing as update_rnp but only updates the rotation (z axis
    // axial rotation) of the RNP.
    void update_axial_rotation(TimeGMST & time_gmst);

    // implementations to make the functions inherited from
    // EphemInterface non-pure virtual

    // Indicates when class was last updated
    double timestamp() const override;

    // Identify the model
    std::string get_name() const override;

    // update the model. This calls the update_axial_rotation function.
    void ephem_update() override;

    /**
     * The hard coded internal name to be returned on calling
     * the overridden EphemerisInterface function "get_name"
     */
    std::string internal_name{"RNPJ2000"}; //!< trick_units(--)

private: // private member functions
    // accesses the TimeManager pointed to by the given TimeGMST,
    // and then uses it to get the simulations DynTime. If
    // a DynTime is not found, this is a fatal error.
    void get_dyn_time_ptr(TimeGMST & gmst);

    /**
     * Pointer to the TimeGMST used to update
     * this object when ephem_update is invoked
     */
    TimeGMST * gmst_ptr{}; //!< trick_units(--)

    /**
     * Pointer to the TimeDyn object, used to time stamp the reference frame
     * when it is being updated
     */
    TimeDyn * time_dyn_ptr{}; //!< trick_units(--)

    /**
     * The last update time, when updated through update_rnp,
     * for the RNP, referencing TimeDyn.seconds .
     * If the time from time_dyn_ptr is the same as this update time, then
     * the RNP will not be updated. This is to prevent unnecessary
     * updating.
     */
    double last_updated_time_full{}; //!< trick_units(s)

    /**
     * Indicates that last_updated_time_full has never been populated, and
     * that the update must be done regardless of given time.
     */
    bool never_updated_full{true}; //!< trick_units(--)

    /**
     * The last update time, when updated through update_axial_rotation,
     * referencing TimeDyn.seconds .
     * If the time from time_dyn_ptr is the same as this update time, then
     * the R component of RNP will not be updated. This is to prevent
     * unnecessary updating.
     */
    double last_updated_time_rotational{}; //!< trick_units(s)

    /**
     * Indicates that last_updated_time_rotational has never been
     * populated, and that the update must be done regardless of given time.
     */
    bool never_updated_rotational{true}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
