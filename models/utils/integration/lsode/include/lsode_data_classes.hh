//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @file models/utils/integration/lsode/include/lsode_data_classes.hh
 * Define LSODE classes that contain just data members
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (())

Library dependencies:
  ((lsode_data_classes.o))

 

*******************************************************************************/

#ifndef JEOD_LSODE_DATA_CLASSES_HH
#define JEOD_LSODE_DATA_CLASSES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Integration includes
#include "lsode_control_data_interface.hh"

namespace jeod
{

/**
 * Data associated with the method DPREPJ
 */
class LsodeDataJacobianPrep
{
JEOD_MAKE_SIM_INTERFACES(LsodeDataJacobianPrep)

public:

  /**
   * Destructor.
   */
  virtual ~LsodeDataJacobianPrep(void){};
  LsodeDataJacobianPrep(void);

// variables used in DPREPJ - these used to be local, but have to be recorded with DPRERPJ getting severed
// by the CALL F.
   double fac;
   double hl0;
   int index;
   int index_max;
   double r0;
   double yj;

private:
   LsodeDataJacobianPrep & operator=(const LsodeDataJacobianPrep & src);
   LsodeDataJacobianPrep(const LsodeDataJacobianPrep & src);
};


/**
 * The data arrays
 */
class LsodeDataArrays
{
JEOD_MAKE_SIM_INTERFACES(LsodeDataArrays)

public:

  /**
   * Destructor.
   */
  virtual ~LsodeDataArrays(void)
    {destroy_allocated_arrays();};
  LsodeDataArrays(void);

  void allocate_arrays( unsigned int num_odes,
                        LsodeControlDataInterface::CorrectorMethod corrector_method);
  void destroy_allocated_arrays();

   /**
    * Was IWM(21) or IPVT.
    * Pivot vector generated in dgefa, and used in dgesl.
    */
   int * pivots; //!< trick_units(--)

// Breakdown of RWORK:
// NOTE:   LYH, LEWT, LACOR, LSAVF, LWM all provide base address in a larger
//         array (RWORK) for he vectors YH, EWT, ACOR, SAVF, and WM.
//         I am dviding the RWORK array into these individual arrays, so do not
//         need the indices any more.*/

// RWORK[1-21] is divided into multiple variables now.


   /**
    * was RWORK[LYH:LYH+NYH*(MAXORD+1)-1].  LYH = 21
    * First index is to "i" in y_i, second index is to history order.
    * history[i,j] = rwork[LYH + j*nyh + i], with lyh = 21 typically.
    */
   double ** history; //!< trick_units(--)
   double lin_alg_1;
   double lin_alg_2;
   /**
    * was RWORK[LWM:LWM+LENWM-1].  LWM = LYH + (NYH*(MAXORD+1))
    * lin_alg_1 = rwork[lwm]
    * lin_alg_2 = rwork[lwm + 1]
    * lin_alg[i,j] = rwork[lwm+ j*n + i + 2].
    * The first two elements are treated differently, then it goes to an
    * array that is sized based on the correction_method.
    * The array sizes are as follows, ordered by value of correction_method:
    * 0:     0
    * 1,2:   n x n
    * 3:     1 x n
    * 4,5:   (2*ml+mu+1) x n.
    */
   double ** lin_alg; //!< trick_units(--)
   /**
    * was RWORK[LEWT:LEWT+N-1].  LEWT = LWM + LENWM
    * error_weight[i] = rwork[lewt+i].
    */
   double * error_weight; //!< trick_units(--)
   /**
    * was RWORK[LSAVF:LSAVF+N-1]. LSAVF = LEWT + N
    * save[i] = rwork[lsavf+i].
    */
   double * save; //!< trick_units(--)
   /**
    * was RWORK[LACOR:LACOR+N-1]. LACOR = LSAVF + N
    * acum_correction[i] = rwork[lacor+i].
    */
   double * accum_correction; //!< trick_units(--)


   /**
    * Number of record, this is the value used for data allocation.
    */
   unsigned int lin_alg_index1; //!< trick_units(--)
   /**
    * Number of record, this is the value used for data allocation.
    */
   unsigned int num_odes;  //!< trick_units(--)

   /**
    * Indicator of whether the arrays have been allocated.
    */
   bool allocated; //!< trick_units(--)

private:
   LsodeDataArrays & operator=(const LsodeDataArrays & src);
   LsodeDataArrays(            const LsodeDataArrays & src);
};


/**
 * The data associated with method Dstode
 */
class LsodeDataStode
{
JEOD_MAKE_SIM_INTERFACES(LsodeDataStode)

public:

  /**
   * Destructor.
   */
  virtual ~LsodeDataStode(void){};
  LsodeDataStode(void);

// Variables used within the DSTODE method, which has been dividied into
// multiple sub-methods.  Adding these to the class to avoid having to
// track them across multiple methods.  They are internal.
   double step_ratio;
   double step_ratio_order_inc;
   double told;
   double dsm;
   int iredo;
   int iret;
   unsigned int ncf;
   unsigned int new_method_order;

private:
   LsodeDataStode & operator=(const LsodeDataStode & src);
   LsodeDataStode(            const LsodeDataStode & src);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
