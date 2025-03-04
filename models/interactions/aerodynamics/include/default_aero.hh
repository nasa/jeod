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
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/include/default_aero.hh
 * An implementation of ballistic coefficent and coefficient of drag
 * for use in the AerodynamicDrag object. This class can be inherited from
 * and overriden for use with the AerodynamicDrag object.
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

Library dependencies:
    ((../src/default_aero.cc))


*******************************************************************************/

#ifndef DEFAULT_AERO_HH
#define DEFAULT_AERO_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class AeroDragParameters;

/*
  Purpose:
    (Gives different options for calculating basic drag in the default, simple aero model. These include coefficient of
  drag, ballistic coefficient, etc.)
 */

/**
 * The simple, default, aerodynamic drag model, including
 * coefficient of drag, ballistic coefficient, etc. This can be
 * overriden with a user defined model in the AerodynamicDrag class.
 */
class DefaultAero
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DefaultAero)

public:
    /**
     * Specifies how drag is to be computed.
     */
    enum DragOption
    {
        DRAG_OPT_CD = 0,   /**< Use Coefficient of drag for drag computations. */
        DRAG_OPT_BC = 1,   /**< Use Ballistic Coefficient for drag computations. */
        DRAG_OPT_CONST = 2 /**< Use specified constant drag. */
    };

    DefaultAero() = default;
    virtual ~DefaultAero() = default;
    DefaultAero & operator=(const DefaultAero &) = delete;
    DefaultAero(const DefaultAero &) = delete;

    // Calculates the aerodynamic drag force and torque on a vehicle, given
    // the necessary parameters
    virtual void aerodrag_force(const double velocity_mag,
                                const double rel_vel_hat[3],
                                AeroDragParameters * aero_drag_param_ptr,
                                double mass,
                                double force[3],
                                double torque[3]);

    /**
     * Coefficient of drag
     */
    double Cd{}; //!< trick_units(--)
    /**
     * Ballistic Coefficient
     */
    double BC{}; //!< trick_units(kg/m2)
    /**
     * Vehicle aerodynamic area.
     */
    double area{}; //!< trick_units(m2)

    /**
     * Drag calculated during use. Can be set by user and
     * will then never be changed with a DRAG_OPT_CONST
     */
    double drag{}; //!< trick_units(N)

    /**
     * The type of simple drag to use
     */
    DragOption option{DRAG_OPT_CONST}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
