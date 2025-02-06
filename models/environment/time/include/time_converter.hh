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
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/include/time_converter.hh
 * The Time Converter is an abstract class that defines the basic structure of
 * all the methods used by the converter objects; converters are the
 * objects that specify the conversion algorithms between time-types.
 */

/******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((Used to convert between two and only two clocks.))



Library dependencies:
  ((../src/time_converter.cc))
*******************************************************************************/

#ifndef JEOD_TIME_CONVERTER_HH
#define JEOD_TIME_CONVERTER_HH

// System includes
#include <string>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes

//! Namespace jeod
namespace jeod
{

class JeodBaseTime;

/**
 * The Time Converter is an abstract class that defines the basic structure of
 * all the methods used by the converter objects; converters are the
 * objects that specify the conversion algorithms between time-types.
 */
class TimeConverter
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeConverter)

    friend class JeodBaseTime;
    // Member Data
public:
    /**
     * Possible conversion directions
     */
    enum Direction
    {
        NO_DIRECTION = 0x0000,
        A_TO_B_INIT = 0x0001,
        B_TO_A_INIT = 0x0010,
        A_TO_B_UPDATE = 0x0100,
        B_TO_A_UPDATE = 0x1000,
        A_TO_B = 0x0101,
        B_TO_A = 0x1010,
        ANY_DIRECTION = 0x1111
    };

public:
    /**
     * name of time-type "a".
     */
    std::string a_name{""}; //!< trick_units(--)

    /**
     * name of time-type "b".
     */
    std::string b_name{""}; //!< trick_units(--)

protected:
    /**
     * whether converter has been initialized.
     */
    bool initialized{}; //!< trick_units(--)

    /**
     * Difference between the two time-types
     */
    double a_to_b_offset{}; //!< trick_units(--)

    /**
     * Bit packed flag specifying whether how a converter can be used
     */
    Direction valid_directions{NO_DIRECTION};

    // Member functions
public:
    virtual ~TimeConverter() = default;
    TimeConverter(const TimeConverter &) = delete;
    TimeConverter & operator=(const TimeConverter &) = delete;

    /**
     * Initialize the converter
     * \param[in] parent parent-type
     * \param[in] child child-type
     * \param[in] direction L-R, or R-L
     */
    virtual void initialize(JeodBaseTime * parent, JeodBaseTime * child, const int direction) = 0;

    // Return initialized bool
    virtual bool is_initialized();

    // Return initialized bool
    void override_initialized(bool init)
    {
        initialized = init;
    }

    // Return whether supplied conversion is possible
    bool can_convert(Direction query);

    // convert_a_to_b: Apply the converter in the forward direction
    virtual void convert_a_to_b();

    // convert_b_to_a: Apply the converter in the reverse direction
    virtual void convert_b_to_a();

    // resets the offset value mid-sim.
    virtual void reset_a_to_b_offset();

    // used at time reversals to verify the ends of the lookup table
    virtual void verify_table_lookup_ends();

    /**
     * Return the offset from the parent time object to this object.
     * @return a_to_b_offset member.
     */
    inline double get_a_to_b_offset()
    {
        return a_to_b_offset;
    }

protected:
    // Constructor
    TimeConverter() = default;

    // verify_setup: Verifies that the initializer function can run
    void verify_setup(const JeodBaseTime * parent, const JeodBaseTime * child, const int direction);
};

/*----------------------------------------------------------------------------*/

/**
 * Bitwise or operator for combining multiple converter direction flags
 */
inline TimeConverter::Direction operator|(TimeConverter::Direction a, TimeConverter::Direction b)
{
    return static_cast<TimeConverter::Direction>(static_cast<int>(a) | static_cast<int>(b));
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
