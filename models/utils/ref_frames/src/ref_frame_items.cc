/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/ref_frame_items.cc
 * Define basic methods for the RefFrameState class.
 */

/*******************************************************************************
  Purpose:
    ()

  Library dependency:
    ((ref_frame_items.o))

   
*******************************************************************************/


// System includes

// JEOD includes
#include "../include/ref_frame_items.hh"


//! Namespace jeod
namespace jeod {

/**
 * Return a string indicating the set items.
 * @return Set items, by name
 * \param[in] test_items Items enum value
 */
const char *
RefFrameItems::to_string (
   Items test_items)
{
   const char * name;

   switch (test_items) {
   case No_Items:         name = "no items";                            break;
   case Pos:              name = "position";                            break;
   case Vel:              name = "velocity";                            break;
   case Pos_Vel:          name = "position, velocity";                  break;
   case Att:              name = "attitude";                            break;
   case Pos_Att:          name = "position, attitude";                  break;
   case Vel_Att:          name = "velocity, attitude";                  break;
   case Pos_Vel_Att:      name = "position, velocity, attitude";        break;
   case Rate:             name = "rate";                                break;
   case Pos_Rate:         name = "position, rate";                      break;
   case Vel_Rate:         name = "velocity, rate";                      break;
   case Pos_Vel_Rate:     name = "position, velocity, rate";            break;
   case Att_Rate:         name = "attitude, rate";                      break;
   case Pos_Att_Rate:     name = "position, attitude, rate";            break;
   case Vel_Att_Rate:     name = "velocity, attitude, rate";            break;
   case Pos_Vel_Att_Rate: name = "position, velocity, attitude, rate";  break;
   default:               name = "unknown items";                       break;
   };
   return name;
}


/**
 * Construct a RefFrameItems object.
 */
RefFrameItems::RefFrameItems (
   void)
{
   value = RefFrameItems::No_Items;
}


/**
 * Construct a RefFrameItems object.
 * \param[in] new_value Initial value
 */
RefFrameItems::RefFrameItems (
   Items new_value)
{
   value = new_value;
}


/**
 * Return a string indicating the set items.
 * @return Set items, by name
 */
const char *
RefFrameItems::to_string (
   void)
const
{
   return RefFrameItems::to_string (value);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
