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
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/include/time_converter_dyn_tdb.hh
 * Define class TimeConverter_Dyn_TDB, which converts from simulation dynamic
 * time to Barycentric Dynamic Time.
 */

/******************************************************************************
 Purpose:
 ()

 Reference:
 (((None)))

 Assumptions and limitations:
 ((This class converts from simulation dynamic time to TDB only. There is
 no reverse conversion.)
 (There is only one instance of this converter.))

 

 Library dependencies:
 ((time_converter_dyn_tdb.o))
 ******************************************************************************/

#ifndef time_converter_dyn_tdb_hh
#define time_converter_dyn_tdb_hh

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "time_converter.hh"


//! Namespace jeod
namespace jeod {

class TimeDyn;
class TimeTDB;
class JeodBaseTime;


/**
 * Define class TimeConverter_Dyn_TDB, which converts from simulation dynamic
 * time to Barycentric Dynamic Time.
 */
class TimeConverter_Dyn_TDB: public TimeConverter {

    JEOD_MAKE_SIM_INTERFACES(TimeConverter_Dyn_TDB)
    // Member Variables
private:
    /**
     * Converter parent time, always a TimeDyn for this converter
     **/
    TimeDyn * dyn_ptr; //!< trick_units(--)

    /**
     * Converter child time, always a TimeTDB for this converter.
     */
    TimeTDB * tdb_ptr; //!< trick_units(--)

    // Member Functions
public:
    // Constructor
    TimeConverter_Dyn_TDB ();
    // Destructor
    ~TimeConverter_Dyn_TDB ();

    // Initialize the converter
    void initialize (JeodBaseTime * parent,
                     JeodBaseTime * child,
                     const int direction);

    // convert_a_to_b: Apply the converter in the direction
    void convert_a_to_b (void);

    // Copy constructor and assignment operator for this class are
    // declared private and not implemented to prevent usage of C++ defaults
private:
    TimeConverter_Dyn_TDB (const TimeConverter_Dyn_TDB&);
    TimeConverter_Dyn_TDB & operator = (const TimeConverter_Dyn_TDB&);
};
/*----------------------------------------------------------------------------*/

} // End JEOD namespace

#endif /* time_converter_dyn_tdb_h */

/**
 * @}
 * @}
 * @}
 */
