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
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/ref_frame_items.hh
 * Define the class RefFrameItems, which identifies the aspects of a
 * reference frame's state that have been set.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Library dependencies:
  ((../src/ref_frame_items.cc))



******************************************************************************/

#ifndef JEOD_REF_FRAME_ITEMS_HH
#define JEOD_REF_FRAME_ITEMS_HH

// System includes
#include <string>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Identify which aspects of a reference frame's state have been set.
 * The aspects that are managed are the position, velocity, attitude,
 * and attitude rate.
 */
class RefFrameItems
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RefFrameItems)

    // Enumerations
public:
    /**
     * The Items enumeration identifies the major items that can be
     * set in a RefFrameState structure -- position, velocity, attitude, and
     * attitude rate. The enumeration values are implemented as bit flags.
     * The four basic items, position, velocity, attitude, and rate, have
     * values of 1, 2, 4, and 8, respectively. Combinations thereof have values
     * corresponding to the bitwise or of the basic components.
     */
    enum Items
    {
        // Values.
        No_Items = 0,         ///< Nothing set
        Pos = 1,              ///< Position
        Vel = 2,              ///< Velocity
        Pos_Vel = 3,          ///< Position + velocity
        Att = 4,              ///< Attitude
        Pos_Att = 5,          ///< Position + attitude
        Vel_Att = 6,          ///< Velocity + attitude
        Pos_Vel_Att = 7,      ///< Position + velocity + attitude
        Rate = 8,             ///< Attitude rate
        Pos_Rate = 9,         ///< Position + rate
        Vel_Rate = 10,        ///< Velocity + rate
        Pos_Vel_Rate = 11,    ///< Position + velocity + rate
        Att_Rate = 12,        ///< Attitude + Rate
        Pos_Att_Rate = 13,    ///< Position + attitude + Rate
        Vel_Att_Rate = 14,    ///< Velocity + attitude + Rate
        Pos_Vel_Att_Rate = 15 ///< Position + velocity + attitude + Rate
    };

    // Static member functions
public:
    // Return a string naming the set items.
    static std::string to_string(Items test_items);

    // Member data
public:
    /**
     * Indicates which aspects of a RefFrameState have been set.
     */
    Items value; //!< trick_units(--)

    // Member functions
public:
    // Constructors
    RefFrameItems();

    explicit RefFrameItems(Items new_value);

    // Get the value of a RefFrameItems object.
    Items get() const;

    // Determine whether specified aspects of a RefFrameItems are set.
    bool contains(Items test_items) const;

    // Determine whether a RefFrameItems equals the specified aspects.
    bool equals(Items test_items) const;

    // Determine whether a RefFrameItems has nothing set.
    bool is_empty() const;

    // Determine whether a RefFrameItems has everything set.
    bool is_full() const;

    // Set the value of a RefFrameItems object.
    Items set(Items new_value);

    // Mark aspects of a RefFrameItems as set.
    Items add(Items new_items);

    // Mark aspects of a RefFrameItems as not set.
    Items remove(Items old_items);

    // Return a string naming the set items.
    std::string to_string() const;
};

} // namespace jeod

#include "ref_frame_items_inline.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
