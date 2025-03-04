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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup Lsode
 * @{
 *
 * @file models/utils/integration/lsode/include/lsode_control_data_interface.hh
 * Define the class LsodeControlDataInterface.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Library dependencies:
  ((../src/lsode_control_data_interface.cc))



*******************************************************************************/

#ifndef JEOD_LSODE_CONTROL_DATA_INTERFACE_HH
#define JEOD_LSODE_CONTROL_DATA_INTERFACE_HH

// System includes
#include <vector>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Integration includes

namespace jeod
{
/**
 * Specifies controls for an LSODE integrator.
 */
class LsodeControlDataInterface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LsodeControlDataInterface)

public:
    /**
     *
     */
    enum IntegrationMethod
    {
        ImplicitAdamsNonStiff = 1, ///< Variable-step, variable-order, implicit Adams
                                   //    method.  Non-stiff.  Order = 1 to 12.
        ImplicitBackDiffStiff = 2  ///< Variable-step, variable-order, implicit
                                   //    backward differentiation for stiff systems
                                   //    Order = 1 to 5.
    };

    /**
     *
     */
    enum CorrectorMethod
    {
        FunctionalIteration = 0,      ///< Functional iteration.
        NewtonIterUserJac = 1,        ///< Modified Newton iteration with
                                      //    user-supplied analytical Jacobian
        NewtonIterInternalJac = 2,    ///< Modified Newton iteration with internally
                                      //   generated numerical Jacobian
        JacobiNewtonInternalJac = 3,  ///< Modified Jacobi-Newton iteration with
                                      //   internally generated numerical Jacobian
        NewtonIterUserBandJac = 4,    ///< Modified Newton iteration with
                                      //   user-supplied banded Jacobian
                                      //   NOT SUPPORTED
        NewtonIterInternalBandJac = 5 ///< Modified Newton iteration with internally
                                      //   generated banded Jacobian.
                                      //   NOT SUPPORTED.
    };

    /**
     *
     */
    enum ErrorControlIndicator
    {
        CommonAbsCommonRel = 1,    ///< Use the same absolute and relative values
                                   //   for all terms
        SpecificAbsCommonRel = 2,  ///< Use separate absolute values for each
                                   //   term, and a common relative value
        CommonAbsSpecificRel = 3,  ///< Use a common absolute values and separate
                                   //   relative value for each term.
        SpecificAbsSpecificRel = 4 ///< Use separate absolute and relative values
                                   //    for each term.
    };

    LsodeControlDataInterface();

    virtual ~LsodeControlDataInterface()
    {
        destroy_allocated_arrays();
    }

    LsodeControlDataInterface(const LsodeControlDataInterface &) = default;
    LsodeControlDataInterface & operator=(const LsodeControlDataInterface &) = delete;

    void check_interface_data();
    void set_rel_tol(int index, double value);
    void set_abs_tol(int index, double value);
    void allocate_arrays();
    void destroy_allocated_arrays();

    /**
     * Tests whether corrector is functional iteration.
     */
    bool is_corrector_method_functional_iteration()
    {
        return (corrector_method == FunctionalIteration);
    }

    // default constructor, destructor, etc defaults are acceptable.
    /**
     * Was ITOL
     */
    ErrorControlIndicator error_control_indicator{CommonAbsCommonRel}; //!< trick_units(--)

    /**
     * Temporary pre-initialized place to store loaded error values.
     */
    std::vector<double> abs_tolerance_error_control_vec; //!< trick_units(--)

    /**
     * Temporary pre-initialized place to store loaded error values.
     */
    std::vector<double> rel_tolerance_error_control_vec; //!< trick_units(--)

    bool error_control_vector_copied_over{};
    unsigned int num_odes_at_alloc{};
    /**
     * Was ATOL.  Vector of the absolute error tolerances.
     */
    double * abs_tolerance_error_control{}; //!< trick_units(--)
    /**
     * Was RTOL.  Vector of the relative error tolerances.
     */
    double * rel_tolerance_error_control{}; //!< trick_units(--)

    /**
     * Was N, in DLS001 common block.
     * Number of ODEs to be solved at next step.
     * In this implementation, num_odes = num_equations.
     * In original implementation, num_odes (N) was set to NEQ at the start,
     * the some subset could be identified, NYH and solved for.
     */
    unsigned int num_odes{3}; //!< trick_units(--)

    /**
     * Was METH, in DLS001 common block.
     */
    IntegrationMethod integration_method{ImplicitAdamsNonStiff}; //!< trick_units(--)

    /**
     * Was MITER, in DLS001 common block.
     */
    CorrectorMethod corrector_method{FunctionalIteration}; //!< trick_units(--)

    /**
     * was HMIN, in DLS001 common block.
     * Minimum absolute value of step size allowable.
     * Default to 0.0, user-specified otherwise.
     */
    double min_step_size{}; //!< trick_units(--)
    /**
     * was HMAX. RWORK[6]
     * Maximum absolute value of step size allowable.
     * Default to 0.0, interpreted as infinity. user-specified otherwise.
     */
    double max_step_size{}; //!< trick_units(--)
    /**
     * Was H0.
     * Initial guess at the step size.  May be input, will be calculated if not.
     * Note - this is the actual step, not the magnitude of the step.
     * whereas max_step_size and min_step_size are magnitudes.
     */
    double initial_step_size{}; //!< trick_units(--)

    /**
     * Was MAXORD, in DLS001 common block.  Populated from IWORK[5]
     * Maximum order allowable.
     */
    unsigned int max_order{12}; //!< trick_units(--)
    /**
     * Was MXHNILI, in DLS001 common block. Populated from IWORK[7[
     * Maximum number of small-step warnings that may be printed.
     */
    unsigned int max_num_small_step_warnings{10}; //!< trick_units(--)
    /**
     * Was MAXCOR, in DLS001 common block
     * Maximum number of corrector-iterations to attempt on any step.
     */
    unsigned int max_correction_iters{3}; //!< trick_units(--)
    /**
     * Was MSBP, in DLS001 common block. Populated from IWORK[6]
     * Maximum number of steps for which the same Jacobian can be used.
     */
    unsigned int max_num_steps_jacobian{20}; //!< trick_units(--)
    /**
     * Was MXNCF, in DLS001 common block.
     * Maximum number of convergence failures on one step.
     */
    unsigned int max_num_conv_failure{10}; //!< trick_units(--)
    /**
     * Was MXSTEP, in DLS001 common block.
     * Maximum number of steps that the integrator may take. Default = 500.
     */
    unsigned int max_num_steps{500}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
