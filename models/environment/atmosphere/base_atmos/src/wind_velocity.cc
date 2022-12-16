/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/base_atmos/src/wind_velocity.cc
 * General base class for wind velocity models
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (A simple table-lookup winds model.  Wind magnitude is a tabulated
    function of altitude; wind direction is a simple analytic function of
    position (inertial-z cross r).)
LIBRARY DEPENDENCY:
  ((atmosphere_messages.o)
   (utils/message/src/message_handler.o))

*******************************************************************************/


// System includes
#include <cstddef> // provides NULL

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/wind_velocity.hh"
#include "../include/atmosphere_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */
WindVelocity::WindVelocity (
   void)
: // Return: -- void
   active(true),
   omega(0.0),
   num_layers(0),
   omega_scale_table(NULL),
   array_index(0),
   first_pass(true),
   increasing_altitude(true)
{
   // empty for now
}

/**
 * Destructor
 */
WindVelocity::~WindVelocity (
   void)
{
   if (JEOD_IS_ALLOCATED (omega_scale_table)) {
      JEOD_DELETE_ARRAY (omega_scale_table);
   }
}


/**
 * Updates the wind velocity from the parameters given.
 * \param[in] inertial_pos The inertial position of the vehicle\n Units: M
 * \param[in] altitude The altitude of the vehicle\n Units: M
 * \param[out] wind_inertial The wind, in the inertial frame, applied to the vehicle\n Units: M/s
 */
void
WindVelocity::update_wind (
   double inertial_pos[3],
   double altitude,
   double wind_inertial[3])
{
   if (!active) {
      return;
   }

   if (omega_scale_table == NULL ||
       inertial_pos      == NULL ||
       wind_inertial     == NULL) {
      MessageHandler::error(
         __FILE__,__LINE__, AtmosphereMessages::framework_error,
         "One of the required pointers is NULL.\n"
         "Wind cannot be computed.\n"
         "Deactivating model to prevent this message repeating.\n");
      active = false;
   }

   /* Value by which omega is scaled depending on alt. */
   double omega_scale = 0;

   if (num_layers == 1) {
      omega_scale = omega_scale_table[0].scale_factor;
   }
   else {
      //Check if increasing, decreasing, or invalid the first time this is called
      if (first_pass) {
         bool hasIncreased = false;
         bool hasDecreased = false;
         for (unsigned int i=0; i<num_layers-1; i++) {
            if (omega_scale_table[i+1].altitude > omega_scale_table[i].altitude) {
               hasIncreased = true;
               if(hasDecreased) break;
            }
            else {
               hasDecreased = true;
               if (hasIncreased) break;
            }
         }

         if (hasIncreased && hasDecreased) {
            MessageHandler::error(
               __FILE__,__LINE__, AtmosphereMessages::framework_error,
               "The provided table has neither strictly increasing nor strictly decreasing altitude.\n"
               "Wind cannot be computed.\n"
               "Deactivating model to prevent this message repeating.\n");
            active = false;
         }
         else if (hasIncreased) {
            increasing_altitude = true;
         }
         else if (hasDecreased) {
            increasing_altitude = false;
         }
         else {
            MessageHandler::error(
               __FILE__,__LINE__, AtmosphereMessages::framework_error,
               "The provided table somehow has no increases or decreases in altitude.\n"
               "Wind cannot be computed.\n"
               "Deactivating model to prevent this message repeating.\n");
            active = false;
         }
         
         first_pass = false;
      }

      //Check if current altitude is outside the bounds of the table and assign the top/bottom value as necessary
      if ((increasing_altitude && altitude < omega_scale_table[0].altitude) || (!increasing_altitude && altitude > omega_scale_table[0].altitude)) {
         omega_scale = omega_scale_table[0].scale_factor;
         array_index = 0;
      }
      else if ((increasing_altitude && altitude > omega_scale_table[num_layers - 1].altitude) || (!increasing_altitude && altitude < omega_scale_table[num_layers - 1].altitude)) {
         omega_scale = omega_scale_table[num_layers - 1].scale_factor;
         array_index = num_layers - 2; //Note that the array_index is the lower of the two indices used for interpolation. It can never be greater than num_layers-2.
      } 
      //If the current altitude lies within the bounds of the table, determine the correct indices to interpolate between
      //Start with most recent index to save time. It's reasonable to assume it's close to where it was before.
      else {
         if (increasing_altitude) {
            if (altitude < omega_scale_table[array_index].altitude) {
               while (altitude < omega_scale_table[array_index].altitude) {
                  array_index--;
               }
            }
            else {
               while (altitude > omega_scale_table[array_index + 1].altitude) {
                  array_index++;
               }
            }
         }
         else {
            if (altitude < omega_scale_table[array_index + 1].altitude) {
               while (altitude < omega_scale_table[array_index + 1].altitude) {
                  array_index++;
               }
            }
            else {
               while (altitude > omega_scale_table[array_index].altitude) {
                  array_index--;
               }
            }
         }

         //Interpolate between the two indices 
         double cell_fraction = (altitude - omega_scale_table[array_index].altitude) / (omega_scale_table[array_index + 1].altitude - omega_scale_table[array_index].altitude);
         omega_scale = omega_scale_table[array_index].scale_factor + cell_fraction * (omega_scale_table[array_index + 1].scale_factor - omega_scale_table[array_index].scale_factor);
      }
   }

   // Scale omega by the proper scaling factor.
   double omega_scaled = omega * omega_scale;

   // Cross product of the omega vector and the target position vector
   wind_inertial[0] = -omega_scaled * inertial_pos[1];
   wind_inertial[1] =  omega_scaled * inertial_pos[0];
   wind_inertial[2] =  0.0;
}

unsigned int WindVelocity::get_num_layers()
{
   return num_layers;
}

void WindVelocity::set_omega_scale_table(double altitude, double factor)
{
   if (JEOD_IS_ALLOCATED (omega_scale_table)) {
      JEOD_DELETE_ARRAY (omega_scale_table);
   }
   omega_scale_table = JEOD_ALLOC_CLASS_ARRAY(1, OmegaTableEntry);
   omega_scale_table[0].altitude = altitude;
   omega_scale_table[0].scale_factor = factor;
   num_layers = 1;
}

void WindVelocity::set_omega_scale_table(unsigned int num_layers, double* altitude, double* factor)
{
   if(num_layers < 1) {
      MessageHandler::error(
               __FILE__,__LINE__, AtmosphereMessages::framework_error,
               "The number of layers cannot be less than 1.\n");
   }
   else {
      if (JEOD_IS_ALLOCATED (omega_scale_table)) {
         JEOD_DELETE_ARRAY (omega_scale_table);
      }
      omega_scale_table = JEOD_ALLOC_CLASS_ARRAY(num_layers, OmegaTableEntry);
      for(unsigned int i=0; i<num_layers; i++) {
         omega_scale_table[i].altitude = altitude[i];
         omega_scale_table[i].scale_factor = factor[i];
      }
      this->num_layers = num_layers;
   }
}

WindVelocity::OmegaTableEntry* WindVelocity::get_omega_scale_table()
{
   return omega_scale_table;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
