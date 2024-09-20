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
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemInterface
 * @{
 *
 * @file models/environment/ephemerides/ephem_interface/include/ephem_interface.hh
 * Define base class for all ephemeris interface models
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_EPHEMERIS_INTERFACE_HH
#define JEOD_EPHEMERIS_INTERFACE_HH

// System includes
#include <string>

// JEOD includes
#include "utils/ref_frames/include/subscription.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class EphemeridesManager;

/**
 * Interface class that specifies minimal functionality of an ephemeris model.
 */
class EphemerisInterface : virtual public ActivateInterface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, EphemerisInterface)

public:
    // Member functions
    // Note: This class does not define a constructor.
    // The au-gratis constructor does nothing, which is what is desired.
    // The freebie copy constructor and assignment operator are OK as well.

    // Destructor - does nothing other than the virtual designation.
    ~EphemerisInterface() override = default;

    // Pure virtual methods.

    /**
     * Indicates when class was last updated.
     * @return Time of last update\n Units: s
     */

    /*
     Purpose: (Indicates when class was last updated.)
     */
    virtual double timestamp() const = 0;

    /**
     * Identify the model.
     * @return Model name
     */
    virtual std::string get_name() const = 0;

    /**
     * Initialize the model.
     * \param[in,out] manager Ephemerides manager
     */
    virtual void ephem_initialize(EphemeridesManager & manager) = 0;

    /**
     * Activate the model.
     * \param[in,out] manager Ephemerides manager
     */
    virtual void ephem_activate(EphemeridesManager & manager) = 0;

    /**
     * Build the model's contribution to the reference frame tree.
     * \param[in,out] manager Ephemerides manager
     */
    virtual void ephem_build_tree(EphemeridesManager & manager) = 0;

    /**
     * Update the model.
     */
    virtual void ephem_update() = 0;

    // Member data: None. This is an interface class.
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
