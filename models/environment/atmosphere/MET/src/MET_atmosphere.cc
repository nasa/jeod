/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/MET/src/MET_atmosphere.cc
 * Implementation of MET atmosphere model
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (Provides the specific implementation of the MET atmosphere.)
REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
       Exosphere with Empirical Temperature Profiles) (Smithsonian
       Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
     ((Jacchia, L.G.) (Revised Static Models of the Termosphere
       and Exosphere with Emperical Temperature Profiles)
       (Smithsonian Astrophysical Observatory Report No. 332) (--)
       (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))

LIBRARY DEPENDENCY:
  ((environment/atmosphere/base_atmos/src/atmosphere_messages.cc)
   (utils/message/src/message_handler.cc))


*****************************************************************************/

// System includes
#include <cstddef> // provides NULL
#include <string.h> // memset
#include <algorithm> // std::min
#define _USE_MATH_DEFINES_ // for M_PI and the likes
#include <cmath>           // for M_PI and math functions

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "environment/time/include/time_utc.hh"

// Model includes
#include "../include/MET_atmosphere.hh"
#include "environment/atmosphere/base_atmos/include/atmosphere_messages.hh"


//! Namespace jeod
namespace jeod {

/*****************************************************************************
Default static data
*****************************************************************************/
// These are the coefficients from Jacchia(1970), p4, eqn(1):
const double METAtmosphere::mol_wt_coeffs[7] =
       { 28.15204, -0.085586,  1.284E-4, -1.0056E-5,
         -1.021E-5, 1.5044E-6, 9.9826E-8 };
// TODO 1970/71 inconsistency:
//     the coefficients according to Jacchia(1971) are:
//     { 28.82678, -7.40066E-2, -1.19407E-2, 4.51103E-4,
//        -8.21895E-6, 1.07561E-5, -6.97444E-7};

const double METAtmosphere::gauss_altitudes[9] = {
     90.0, 105.0, 125.0, 160.0, 200.0, 300.0, 500.0, 1500.0, 2500.0 };
//  TODO 1970/71 inconsistency
//        -- the barometric ceiling is 100km in Jacchia(1971), so edges become:
//    90.0, 100.0, 125.0, 160.0, 200.0, 300.0, 500.0, 1500.0, 2500.0 };

const int METAtmosphere::gauss_n[8] = { 4, 5, 6, 6, 6, 6, 6, 6 };


/****************************************************************************
Constructors
****************************************************************************/
METAtmosphereChemical::METAtmosphereChemical()
   :
   nominal_mol_weight(28.96)
{
   memset( num_density, 0, num_species * sizeof(double));

   // These numbers for N2, O2, Ar, and He  are taken from Jacchia(1971), p5
   frac[0] = 0.78110;  // N2
   frac[1] = 0.20955;  // O2
   frac[2] = 0.0;      // Ox
   frac[3] = 0.0093432;// Ar
   frac[4] = 1.289E-05;// He // TODO 1970/71 inconsistency
                             //      1970: 1.289E-5
                             //      1971: 6.1471E-6
   frac[5] = 0.0;      // H

   // These numbers for N2, O2, Ar, and He  are taken from Jacchia(1971), p5
   mol_weight[0] = 28.0134;// N2
   mol_weight[1] = 31.9988;// O2
   mol_weight[2] = 15.9994;// Ox
   mol_weight[3] = 39.948; // Ar
   mol_weight[4] = 4.0026; // He
   mol_weight[5] = 1.00797;// H
}
//****************************************************************************
METAtmosphereThermal::METAtmosphereThermal(
    const double & exo_temp,
    const double & altitude)
   :
   T_out(0.0),
   k_1( 0.054285714),          // see compute_temperature for discussion of
   k_3( -3.96501457725948E-5), // how theswe values are used in generating
   k_4( -5.3311120366514E-7),  // variables c1, c3, c4.
                               // NOTE - these values are only know to 2 sig
                               //        figs.  Rest is provided for
                               //        regression matching.
   T_90(183.0),
   T_125(0.0),
   T_exosphere( exo_temp),
   altitude_km( altitude)
{ }
//****************************************************************************
METAtmosphere::METAtmosphere ()
   :
   geo_index_type(ATMOS_MET_GI_AP),
   geo_index(0.0),
   F10(0.0),
   F10B(0.0),
   altitude_km(0.0),
   latitude(0.0),
   longitude(0.0),
   barometric_equation_ceiling(105.0), //  TODO 1970/71 inconsistency
                                       //       1970: 105.0
                                       //       1971: 100.0
   trunc_julian_time(11544.0),
   tjt_year_start(11544.0), // on 2000/01/01::00:0:00.0
   fraction_of_year(0.0),
   day_of_year(1),
   max_days_this_year(366),
   year(2000),
   solar_declination_angle(0.0),
   solar_hour_angle(0.0),
   state(),
   thermal( state.exo_temp, altitude_km),
   R_gas_constant(8.31432),// Note: This is not an accurate value for R
                           //       But it is the value used by Jacchia
                           //       See Jacchia(1971), p9, eq 5.

   days_per_year(365.2422),
   Avogadro(6.02257E23),// Note: This is not Avogadro's number!
                        //       But it is the number used by Jacchia.
                        //       See Jacchia(1971) p 7, eq(4)
   two_pi(6.28318531),
   three_pi_two(4.71238898),
   deg_to_rad(0.017453293),
   days_per_century(36525),
   minutes_per_day(1440),

   mol_weight_barometric_ceiling(27.72594278125), // see Jacchia(1970) eqn(1).
                                           // Jacchia(1971): 27.6398
   base_fairing_height(440.0),
   fairing_k (M_PI_2 / (500.0 - base_fairing_height))
{}



/*****************************************************************************
update
Purpose:(Main executable for METAtmosphereThermal, returns the temperature
         at the current altitude.)
Notes: (The temperature at 125 km is a function of the exospheric
        temperature and is empirically derived)
*****************************************************************************/
void
METAtmosphereThermal::update()
{
   // T_125 represents T_x in the Jacchia papers.
   // See eq(9) in either Jacchia paper.
   //  TODO 1970/71 inconsistency
   //       1970:
   T_125    = 444.3807 + (0.02385 * T_exosphere) -
               (392.8292 * exp (-0.0021357 * T_exosphere));
   //       1971:
   // T_125    = 371.6678 + (0.0518806 * T_exosphere) -
   //             (294.3505 * exp (-0.00216222 * T_exosphere));




   T_out = compute_temperature(altitude_km);
}

/*****************************************************************************
compute_temperature

PURPOSE:
   (Calculates the temperature at a specified altitude
    using equation (10) for altitudes between 90 and 125 km and equation (13)
    for altitudes greater than 125 km, from Smithsonian Astrophysical
    Observatory Report 313, 1970.)

NOTES: (This method is used to generate the temperature at the current
        altitude AND at other reference altitudes in support of other aspects
        of the model.  Therefore, it is necessary to have the altitude as a
        passed argument.  The argument should not be assumed to be the vehicle
        altitude because it often is not.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

Assumptions:(( All temperature profiles:
               - start at height 90km with value 183K and gradient 0.
               - rise to an inflection point at 125 km
               - become asymptotic to the exospheric temperature above 125 km.)
             ( The temperature T_125 and gradient T_grad_125 at the
               inflection point are functions of the exospheric temperature.)
            )


*****************************************************************************/
double
METAtmosphereThermal::compute_temperature(
   double altitude_km_in)
{
   // temperature-offset is the difference between temperatures at the 125km and
   // 90km altitude reference points:

   // For altitude in the range 90 km to 125 km, temperature is computed as
   // a polynomial function of altitude difference from 125 km
   // (see Jacchia(1971), p11, eq(10) or Jacchia(1970), p 9, eq(10)):
   //   T = T_125 + sum {i=1 to 4} (c_i * (z-125)^i)
   // Satisfaction of the boundary conditions (see Jacchia(1971), p 12, eq(11))
   //  implies that:
   //    c_1 = (1.9 / 35) * (T_125 - T_90)               see eq(11c)
   //    c_2 = 0                                             eq(11d)
   //    c_3 = ((4 - 3(1.9)) / (35)^3) * (T_125 - T_90)      eq(11a,b)
   //    c_4 = ((3 - 2(1.9)) / (35)^4) * (T_125 - T_90)      eq(11a,b)

   // Notes:
   //    - c_1 is the temperature gradient at 125km
   //    - the constants at the front of each c_i, e.g. (1.9 / 35) are stored
   //      in the class as values k_i:
   //        c_i = k_i * (T_125 - T_90)
   //    - the values c_i are not actually computed, the common factor
   //      (T_125 - T_90) is pulled out of the summation so that the
   //      values k_i can be used instead.

   // For altitudes > 125km, a different algorithm is used,
   // (see Jacchia(1971), p 12, eq(13))

   // In both computations, the values (dz), (dz)^2, and dT are needed. so
   // generate those first:
   // base-altitude is delta-altitude above 125 km.
   double dz   = altitude_km_in - 125.0;
   double dz_2 = dz * dz;
   double dT   = T_125 - T_90;

   if (dz <= 0.0) {
     // obtain the temperature as a polynomial in z:
     double dz_3 = dz   * dz_2;
     double dz_4 = dz_2 * dz_2;
     return T_125 + dT * ((k_1 * dz) +
                          (k_3 * dz_3) +
                          (k_4 * dz_4));
   }
   // else use equation 13, which is identical both Jacchia papers:
   double dz_2_5 = dz_2 * sqrt (dz);
   double coeff_A = 2 * (T_exosphere - T_125) / M_PI;
   //   magic number 4.5E-6 is empirical, identified as "B" in eq(13).
   return T_125 + coeff_A *
               std::atan2 ( k_1 * dT * dz * (1.0 + (4.5E-6 * dz_2_5)),
                            coeff_A);
}

/****************************************************************************
update_atmosphere:
Purpose:
   Front-end to the computation of the METAtmosphere at the current time
   Inserts the results into the AtmosphereState pointed to by
   state. This function is for a general AtmosphereState.
   \param[in] pfix_pos Geodetic altitude, latitude and longitude.
   \param[out] state Where the state results will be stored.
****************************************************************************/
void
METAtmosphere::update_atmosphere (
   const PlanetFixedPosition * pfix_pos,
   AtmosphereState           * ext_state)
{
   if (ext_state == nullptr) {
     MessageHandler::error(
       __FILE__,__LINE__, AtmosphereMessages::framework_error,
       "state pointer is NULL.  Cannot populate state.\n");
     return;
   }
   update_atmosphere( pfix_pos);
   // NOTE - this method looks remarkably similar to the method that takes a
   // METAtmosphereState pointer, BUT THEY ARE NOT THE SAME because of this
   // line, which slices the "METAtmosphereState state" variable, taking only
   // the AtmosphereState components.
   // If this method were the only one provided, and used the more generic
   // AtmosphereState pointer basis as representative of a
   // METAtmosphereStateVars pointer, the data slicing would
   // still occur and the external METAtmosphereStateVars would get its
   // AtmosphereState components updated, but not its extension components.
   // HENCE, IT IS NECESSARY TO HAVE BOTH METHODS EVEN THOUGH THEY ARE
   // IDENTICALLY STRUCTURED!!
   *ext_state = state;
}

//****************************************************************************
// update_atmosphere:
/**
 * Front-end to the computation of the METAtmosphere at the current time
 * Inserts the results into the METAtmosphereStateVars pointed to by
 * ext_state. This function is for a METAtmosphereStateVars.
 * \param[in] pfix_pos Geodetic altitude, latitude and longitude.
 * \param[out] ext_state Where the state results will be sent.
 */
//****************************************************************************
void
METAtmosphere::update_atmosphere (
   const PlanetFixedPosition * pfix_pos,
   METAtmosphereStateVars    * ext_state)
{
   if (ext_state == nullptr) {
     MessageHandler::error(
       __FILE__,__LINE__, AtmosphereMessages::framework_error,
       "state pointer is NULL.  Cannot populate state.\n");
     return;
   }
   update_atmosphere( pfix_pos);
   // See comments above regarding data slicing at this point.
   *ext_state = state;
}

//****************************************************************************
// update_atmosphere:
/**
 * Calculates the METAtmosphere, at the current time.
 * \param[in] pfix_pos Geodetic altitude, latitude and longitude.
 */
//****************************************************************************
void
METAtmosphere::update_atmosphere(
   const PlanetFixedPosition * pfix_pos)
{
   if (pfix_pos == nullptr) {
     MessageHandler::error(
       __FILE__,__LINE__, AtmosphereMessages::framework_error,
       "Position pointer is NULL.\n"
       "Cannot update atmosphere at an unknown locations.\n");
     return;
   }
   altitude_km =  pfix_pos->ellip_coords.altitude / 1000.0;
   latitude    =  pfix_pos->ellip_coords.latitude;
   longitude   =  pfix_pos->ellip_coords.longitude;

   // Compute the solar data.
   compute_solar_angles();
   // Compute exospheric temperature.
   compute_exospheric_temperature();
   // Call the main Jacchia atmosphere routine.
   jacchia();
   // Apply density modifications:
   modify_densities();

   /* Finish computations of density. */
   state.log10_dens = log10( state.density);
   state.N2  = species.num_density[0];
   state.Ox2 = species.num_density[1];
   state.Ox  = species.num_density[2];
   state.A   = species.num_density[3];
   state.He  = species.num_density[4];
   state.Hyd = species.num_density[5];

   // Compute the atmospheric pressure at altitude.
   // NOTE - the 1000.0 here is to change the molecular weight (molar mass)
   //        from grams to kg to cancel with the density.
   state.pressure =   (state.density * 1000.0 / state.mol_weight) *
                      R_gas_constant * state.temperature;
}

/*****************************************************************************
modify_densities
Purpose:(
    Modifies the densities:
    - mass density below 170km
    - mass and Helium densities above base-fairing-height)
*****************************************************************************/
void
METAtmosphere::modify_densities()
{
   if (altitude_km <= 170.0) {
      compute_seasonal_latitude_variation();
   }
   // 'Fair' helium number density between base fairing height ( BFH )
   // and 500 km
   else if (altitude_km >= 500.0) {
      compute_seasonal_lat_variation_He ();
   }
   else if (altitude_km > base_fairing_height) { // i.e. between 440km and 500km
      atmos_MET_FAIR5 (); // includes call to compute_seasonal_lat_variation_He
   }
}

/*****************************************************************************
Function: compute_solar_angles, formerly atmos_MET_TME

PURPOSE:
   (Subroutine 'atmos_MET_TME' performs the calculations of the solar
    declination angle and solar hour angle.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


******************************************************************************/
void
METAtmosphere::compute_solar_angles()
{
  //****************************************************************************
  // PART A - compute the necessary time representations
  //****************************************************************************

   // Compute the fraction of day, year, and century:
   // Truncated Julian Time (tjt, TJT) has epoch at 1957/05/24::00:00:00
   // It is a principal clock representation in JEOD.
   double tjt_prev_midnight = std::floor( trunc_julian_time);
   double fraction_of_day = trunc_julian_time - tjt_prev_midnight;

   // day of year has value 1 throughout Jan01, etc
   day_of_year = tjt_prev_midnight - tjt_year_start + 1.0;
   // Note - this is going to churn on the first pass, so would be better
   //        done as part of initialization -- except this model does not have
   //        an initialize method.
   //        Class defaults kick this off at Jan01, 2000
   while (day_of_year > max_days_this_year) {
      day_of_year    -= max_days_this_year;
      tjt_year_start += max_days_this_year;
      year++;
      if (year % 4 == 0) {
        max_days_this_year = 366;
      }
      else {
        max_days_this_year = 365;
      }
   }
   while (day_of_year < 0) {
      year--;
      if (year % 4 == 0) {
        max_days_this_year = 366;
      }
      else {
        max_days_this_year = 365;
      }
      day_of_year    += max_days_this_year;
      tjt_year_start -= max_days_this_year;
   }
   fraction_of_year = day_of_year / days_per_year;

   // Compute number of whole days since 1900/01/01::00:00:00.0
   // trunc_julian_time gives the number of days since the TJT epoch
   // (1957/05/24::00:00:00).  Add 24980 to this value;
   // 24980 is the number of days from 1900/01/01::00:00:00.0 to TJT epoch
   double century_days = tjt_prev_midnight + 24980.0;
   // What we really want, for today as a fraction of a century, is the number
   // of days to noon today, so we need to add 0.5 to this value.
   // (Formerly XJ)
   double century_frac = (century_days + 0.5) / days_per_century;

   // Computation of celestial longitude requires something previously
   // labeled FMJD (f? modified-julian-date?)
   // FMJD = (XMJD - 2435839.0) + (GMT / 1440.0)
   // XMJD is the modified Julian date at noon today
   // 2435839.0 is the modified Julian date at 1956/12/31::12:00:00.0
   // So the first part of that calculation is the number of days since that
   // epoch to noon today.  But then we add the fraction of this day to that
   // value, so the first part has to represent a time ending at midnight.
   // So it finishes up being the number of days since 1956/12/31::00:00:00.0
   // to now.  This may be an error.
   // TODO Turner 10/2017
   //      Investigate whether the original implementation is correct.
   //      This looks suspicious.
   //        Findings:  this is in neither of the Jacchia papers; the Jacchia
   //        papers just use the solar-hour-angle and fraction-of-year without
   //        specifying how to compute these angles.

   // 1956/12/31::00:00:00.0 epoch has a TJT of -4,162.0
   // Thus fmjd represents days since 1956/12/31::0:0:0.0.
   // It is not apparent why that date is significant.
   double fmjd = 4162.0 + tjt_prev_midnight + fraction_of_day;

  //****************************************************************************
  // PART B - compute the solar declination angle in radians:
  //****************************************************************************

   // Solar declination angle requires celestial longitude.
   // Compute celestial longitude in radians (formerly XLS).
   // The rest of the celestial longitude computation requires empirically
   // derived values:
   const double B1 = 0.0172028;  // i.e. 2pi / (365.25 days)
   const double B2 = 0.0335;
   const double B3 = 1.407;

   double celestial_longitude = std::fmod( (B1 * fmjd +
                                            B2 * sin(0.017202 * (fmjd - 3.0)) -
                                            B3),
                                           two_pi);

   // Compute solar declination angle in radians, range [-pi/2, pi/2]:
   const double dec_angle_const = (23.4523 - 0.013*century_frac) * deg_to_rad;
   solar_declination_angle = std::asin( std::sin(celestial_longitude) *
                                        std::sin(dec_angle_const));

  //****************************************************************************
  // PART C - compute the solar hour angle in radians, range (-pi, pi]:
  //****************************************************************************
  // PART C-1 - compute the right ascension of Sun in radians, range [0, 2pi]:
  //****************************************************************************

   // Note - this expression:
   //    ra = asin((tan(asin(sin(celestial_long)*sin(dec_angle_const)))) /
   //              (tan(dec_angle_const)))
   //  can be simplified but still requires computing
   //    - cos(dec_angle_const)
   //    - sqrt(1 - sin^2(cel_long)sin^2(dec_angle_const))
   //  which is 2 computations -- versus the 2 that are necessary to get the
   //  tan(*) values.  So just go with the original.
   double scratch1 = std::tan( solar_declination_angle);
   double scratch2 = std::tan( dec_angle_const);
   // protect against out-of-bounds on the next arcsin operation:
   // If the ratio is out-of-bounds, assign to pi/2.
   // The RA has to be put into the same quadrant as the celestial longitude,
   // so for now generate RA in the first quadrant.
   double solar_right_ascension = M_PI_2;
   if ( std::abs(scratch1) < std::abs(scratch2)) {
      solar_right_ascension = std::abs( asin( scratch1 / scratch2));
   }

   // Put Right-ascension in the same quadrant as celestial longitude via
   // reflection operations.  Right-ascension is currently in 1st quadrant.
   // celestial_longitude is >= 0.0, resulting from the fmod calculation.
   if (celestial_longitude > three_pi_two) {
     solar_right_ascension = two_pi - solar_right_ascension;
   }
   else if (celestial_longitude > M_PI) {
     solar_right_ascension = M_PI + solar_right_ascension;
   }
   else if (celestial_longitude > M_PI_2) {
     solar_right_ascension = M_PI - solar_right_ascension;
   }

  //****************************************************************************
  // PART C-2 - compute the solar hour angle in radians, range (-pi, pi]:
  //****************************************************************************

   // Compute greenwich mean position in degrees.
   // This algorithm requires some empirically derived constants and the
   // number of minutes passed since the most recent midnight (00h).
   double minutes_of_day  = fraction_of_day * minutes_per_day;
   const double A1 = 99.6909833;
   const double A2 = 36000.76892;
   const double A3 = 0.00038708;
   const double A4 = 0.250684477;
   double greenwich_mean_position =
        std::fmod (( A1 +
                    (A2 * century_frac) +
                    (A3 * century_frac * century_frac) +
                    (A4 * minutes_of_day)),
                   360.0);

   // previous algorithm's application of constraints on right ascension
   // point (RAP) was unnecessary because it is local, and constraint gets
   // applied in the computation of solar-hour-angle anyway.
   double right_ascension_point =  greenwich_mean_position * deg_to_rad +
                                   longitude;
   solar_hour_angle = right_ascension_point - solar_right_ascension;
   while (solar_hour_angle > M_PI) {
     solar_hour_angle -= two_pi;
   }
   while (solar_hour_angle < -M_PI) {
     solar_hour_angle += two_pi;
   }
}


/*****************************************************************************
compute_exospheric_temperature, formerly atmos_MET_TINF

PURPOSE:
   (Calculates the exospheric temperature
    according to L. Jacchia, Smithsonian Astrophysical Observatory 313,
    1970.  Subroutine 'atmos_MET_TME' performs the calculations of the solar
    declination angle and solar hour angle.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


******************************************************************************/
void
METAtmosphere::compute_exospheric_temperature()
{
   // Examine for bad latitude that wil generate floating point errors
   // Latitude should be in range [-pi/2, pi/2].  If it strays outside that
   // range, something has gone badly wrong.  Apparently, there can be
   // floating point issues if it gets beyond pi.
   if (std::abs(latitude) > M_PI) {
      MessageHandler::warn (
         __FILE__, __LINE__, AtmosphereMessages::numerical_warning,
         "The latitude in function compute_exospheric_temperature was\n"
         "greater than 180 degrees. There will likely be floating point\n"
         "errors in the results of this function.\n");
   }

  //****************************************************************************
  // PART A - compute the solar-activity variation
  //****************************************************************************
   // Ci are solar activity variables
   //  TODO 1970/71 inconsistency
   //       1970:
   const double C1 = 383.0, C2 = 3.32, C3 = 1.80;
   //       1971:
   //const double C1 = 379.0, C2 = 3.24, C3 = 1.30;

   //       See equation 14.
   double solar_activity_variation = C1 + C2 * F10B + C3 * (F10 - F10B);

  //****************************************************************************
  // PART B - compute the diurnal variation see Jacchia(1971) p 28
  //****************************************************************************a
   // angles beta  = - 37 degrees
   //        gamma =   43 degrees
   //        p     =    6 degrees
   const double beta = -0.6457718, gamma = 0.7504916, p = 0.1047198;
   // scaling factor RE veries from 0.27 to 0.4; 0.31 is a good average
   // according to Jacchia(1970)
   double RE = 0.31;
   // Compute the working variables.
   double theta = 0.5 * std::abs (latitude + solar_declination_angle);
   double   eta = 0.5 * std::abs (latitude - solar_declination_angle);
   double   tau = solar_hour_angle + beta + p *
                  std::sin (solar_hour_angle + gamma);

   if (tau > M_PI) {
      tau -= two_pi;
   }
   else if (tau < -M_PI) {
      tau += two_pi;
   }

   double sin_theta = std::sin (theta);
   double cos_eta   = std::cos (eta);
   double cos_tau_2 = std::cos (tau / 2.0);

   //  TODO 1970/71 inconsistency
   //       1970: the power of sin-theta and cos-eta is 2.5
   //       1971: the power of sin-theta and cos-eta is 2.2
   double A1 = sin_theta * sin_theta * sqrt (sin_theta);
   double A2 = cos_eta * cos_eta * sqrt (cos_eta);
   double A3 = cos_tau_2 * cos_tau_2 * cos_tau_2;

   // A simpler way of writing equation 17:
   double diurnal_variation = 1.0 + RE * (A1 + A3 * (A2 - A1));
  //****************************************************************************
  // PART C - compute the geomagnetic variation
  //****************************************************************************
   const double D1 = 28.0, D2 = 0.03, D3 = 1.0, D4 = 100.0, D5 = -0.08;
   double geomagnetic_variation = 0.0;
   //  TODO 1970/71 inconsistency
   //       1970: as implemented here
   //       1971: completely new formulations.  Not implemented at all.
   if (geo_index_type == ATMOS_MET_GI_KP) {  // geo_index represents K_p
      geomagnetic_variation = D1 * geo_index + D2 * std::exp (geo_index);
   }
   else {  // geo_index represents a_p
      geomagnetic_variation = D3 * geo_index + D4 *
                              (1.0 - std::exp (D5*geo_index));
   }

  //****************************************************************************
  // PART D - compute the semiannual variation.  See eqn(23)
  //  TODO 1970/71 inconsistency
  //       1970: as implemented here
  //       1971: completely new formulations.  Not implemented at all.
  //****************************************************************************
   double E1  = 2.41;      // Kelvin-degrees
   double E2  = 0.349;     // Kelvin-degrees per solar-flux-unit
   double E3  = 0.206;     // Kelvin-degrees per solar-flux-unit;
                           //   amplitude of periodic oscillation
   double E4  = 3.9531708; // 226.5 degrees of arc in radians
                           // NOTE - last 4 sig figs are provided for regression
                           //        they are not actually known.
   double E5  = 4.3214352; // 247.6 degrees of arc in radians
                           // NOTE - last 4 sig figs are provided for regression
                           //        they are not actually known.

   // constants for computing tau1
   double E6 = 0.1145;   // dimensionless constant
   double E7 = 5.974262; // 342.3 degrees of arc in radians
                         // NOTE - last 3 sig figs are provided for regression
                         //        they are not actually known.
   double E8 = 2.16;     // dimensionless; exponent for computation of tau1

   double tau0 = std::pow( (1.0 + sin (M_PI * 2 * fraction_of_year + E7)) / 2,
                            E8);
   double tau1 = fraction_of_year + E6 * (tau0 - 0.5);

   double sav_a = E2 + E3 * (std::sin (2*M_PI * tau1 + E4));
   double sav_b = std::sin (4*M_PI * tau1 + E5);
   // equation 23:
   double semiannual_variation = E1 + F10B * sav_a * sav_b;

   // Exospheric temperature (method output)
   state.exo_temp = solar_activity_variation * diurnal_variation +
                    geomagnetic_variation    + semiannual_variation;
}


/*****************************************************************************
jacchia, formerly atmos_MET_JAC

PURPOSE:
   (Calculates the temperature, the total
    density DENS and its logarithm DL, the mean molecular weight EM, the
    individual species number densities for N, O2, O, A, HE and H [each
    preceded with an 'A'] at altitude Z given the exospheric temperature T.
    This subroutine uses the subroutine 'atmos_MET_GAUSS', 'atmos_MET_TEMP'
    and 'atmos_MET_MOL_WT'.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


*****************************************************************************/
void
METAtmosphere::jacchia()
{
   thermal.update();
   state.temperature = thermal.T_out;

  //****************************************************************************
  // PART A - Use the barometric equation to compute the values for
  //          altitude < barometric ceiling, or at barometric-ceiling for use
  //          as a baseline for higher altitudes.
  //****************************************************************************
   // Equation is taken from Jacchia(1971), eqn(5)
   // d( ln(rho)) = d( ln( M/T)) - (Mg/RT) * dz
   // which integrates to:
   //  rho_1 / rho_0 = ((M_1 / T_1) / (M_0 / T_0)) * exp(int{ -(Mg/RT) dz})

   // where the index "1" is at the integration ceiling height, and index "0"
   // is at the 90km reference point.  This simplifies as:
   //  rho_1 = rho_0 * (M_1 * T_0) / (M_0 * T_1) * exp(int{ -(Mg/RT) dz})
   //  rho_1 = (rho_0 * T_0 / M_0) * (M_1 / T_1) * exp(int{ -(Mg/RT) dz})

   // Note - The constants (rho_0 * T_0 / M_0) evaluate toi
   //    2.1926E-8 (g K

   // Integration ceiling is at the lower of the current altitude or the
   // barometric-ceiling.  Start with current-altitude and adjust if the
   // current altitude is too high.
   double integration_ceiling_A = altitude_km;
   double temperature_ceiling_A = state.temperature;

   if (altitude_km < barometric_equation_ceiling) {
     // integration ceiling is the current altitude
     // generate M_1, mean molar mass (or mean molecular weight) at
     // specified altitude:
     state.mol_weight = compute_mol_wt (altitude_km);
     // temperature at the ceiling is the temperature at this altitude
     // (already assigned)
   }
   else {
     integration_ceiling_A = barometric_equation_ceiling;
     // mol-weight is default value.
     state.mol_weight = mol_weight_barometric_ceiling;
     // Have to recompute the temperature at the barometric ceiling
     temperature_ceiling_A =
             thermal.compute_temperature( barometric_equation_ceiling);
   }

   // evaluate the integral expression.  Note that the temperature profile is
   // not constant, so this does not have a constant value at 105km, so even
   // if the integration ceiling is 105km, it still has to be integrated.
   double integral_Mg_RT = apply_gauss_quadrature ( 0,
                                                    integration_ceiling_A) /
                           R_gas_constant;

   // Now put it all together:

   // Magic number:
   // FIXME Turner 2018-03
   //       This magic number cannot be traced.
   // 2.1926E-5 was previously identified as being the product of:
   //    state.density x temperature / mean molecular weight at 90 km.
   // However, while values are provided for density and temperature, no value
   // was found for mean molecular weight at 90km to verify this result.
   // The closest value in either paper appears to be a value at sea level,
   // with value 28.96.  But this is too high.  It needs to be 28.88 to obtain
   // this magic number.
   // Equation (1) provides an expression for M for altitude > 90km.  Assuming
   // this actually extends to altitude = 90km, M is calculated as:
   //    29.03    (1970)
   //    28.82678 (1971)
   // Note - the product of the numerical values in the papers produce O(10^-8)
   // in g/(cm^3).  This is modified to O(10^-5) kg/m^-3.
   state.density = 2.1926E-5 * (state.mol_weight / temperature_ceiling_A) *
                               std::exp (-integral_Mg_RT);

   // particle-density = Avogadro * molar-density =
   //                    Avogadro * mass-density / molar-mass
   // Note - adjust density to g/m^-3 to cancel with molar-mass in g.
   double scratch = Avogadro * (state.density) * 1000.0;
   double nominal_particle_density = scratch / species.nominal_mol_weight;
   double actual_particle_density  = scratch / state.mol_weight;

   // Reminder:
   // [0] - N2;    [1] - O2;    [2] - Ox;    [3] - AA;    [4] - He;    [5] - H
   //  - See Jacchia(1971), p7
   //  - The number density used in eq(3), N, is the actual-particle-density,
   //    but note that the actual and nominal densities are related to the mass
   //    density by:
   //      mass-density ~ particle-density * molecular-weight
   //    so the product particle-density * molecular-weight is conserved
   //    between nominal and actual.  Hence:
   //      nominal_particle_density = actual_particle_density * (
   //                                    (actual_mol_weight / nominal_mol_weight)
   //    the RHS of this equation is the product in eq(3), so
   //    nominal_particle_density is used as a convenient substitute.
   species.num_density[0] = species.frac[0] * nominal_particle_density;
   species.num_density[1] = actual_particle_density *
                            ( ((state.mol_weight / species.nominal_mol_weight) *
                                (1.0 + species.frac[1])) -
                              1.0);
   species.num_density[2] = 2.0 * actual_particle_density *
                                  (1.0 - state.mol_weight /
                                         species.nominal_mol_weight);
   species.num_density[3] = species.frac[3] * nominal_particle_density;
   species.num_density[4] = species.frac[4] * nominal_particle_density;
   species.num_density[5]  = 1.0;  // placehoder, not valid for altitude<500km.

  //****************************************************************************
  // PART B - Use the diffusion equation to compute the values for
  //          altitudes higher than the barometric ceiling, using the values at
  //          the barometric ceiling (just calculated) as a basis.
  //****************************************************************************
   if (altitude_km > barometric_equation_ceiling) {

     /* Integrate (g/T) from barometric_ceiling (index 1) to alt km :- */
     double integral_g_RT = apply_gauss_quadrature ( 1, altitude_km) /
                            R_gas_constant;
     // compute the ratio between the temperature at the barometric-ceiling
     // (already computed) and the current temperature.
     double temp_ratio = temperature_ceiling_A / state.temperature;

     // Adjust the non-Hydrogen number densities.  Leave Hydrogen alone because
     // it is just a placeholder until reaching 500km.  If altitude is > 500km,
     // Hydrogen will be computed independently in part C.
     // Using the diffusion equation to do this integration.
     for (unsigned int ii = 0; ii < 5; ++ii) {
        species.num_density[ii] *= temp_ratio *
                                   std::exp( - species.mol_weight[ii] *
                                               integral_g_RT);
     }
     // Helium gets an extra "ALPHA_i = -0.38" temperature-dependent term
     // based on the ratio of the temperature at the barometric ceiling
     // to the local temperature.
     species.num_density[4] *= std::pow( temp_ratio, -0.38);
  }
  //****************************************************************************
  // PART C - This section calculates atomic hydrogen densities above
  //          500 km altitude. Below this altitude , H densities are
  //          already set to 1.0
  //****************************************************************************
   if (altitude_km > 500.0) {
      double temperature_500 = thermal.compute_temperature(500.0);
      double log_temperature_500 = log10( temperature_500);

      double integral_g_RT = apply_gauss_quadrature ( 6, altitude_km) /
                             R_gas_constant;


      // Magic numbers:
      // 79.13 = documented 73.13 + 6 to go from cm^-3 to m^-3
      // 39.40  as documented
      //  5.5   as documented
      //  documented values from Kockarts and Nicolet (1962, 1963)
      species.num_density[5] =
              std::pow (10.0,
                   (79.13 - (39.4 * log_temperature_500) +
                    (5.5 * log_temperature_500 * log_temperature_500 ))) *
              (temperature_500 / state.temperature) *
              std::exp (-species.mol_weight[5] * integral_g_RT);
   }

   for (unsigned int ii = 0; ii < 6; ++ii) {
     if (species.num_density[ii] < 1.0) {
        species.num_density[ii] = 1.0;
     }
   }

  //****************************************************************************
  // PART D - Wrap up
  //****************************************************************************
   // For altitudes greater than barometric ceiling, calculate
   // total state.density and mean molecular weight from individual
   // species number densities.
   if (altitude_km > barometric_equation_ceiling) {
     double weighted_num_density = 0;
     double total_num_density = 0.0;
     for (unsigned int ii = 0; ii < 6; ++ii) {
       // temporary -- weighted_num_density in (g particle)/(mol m^3)
       //              total_num_density in (particle)/m^3
       weighted_num_density += species.mol_weight[ii] * species.num_density[ii];
       total_num_density += species.num_density[ii];
     }
     state.mol_weight = weighted_num_density / total_num_density;
     // convert density to kg / m^3
     state.density = weighted_num_density / (1000.0 * Avogadro);
   }
}

/*****************************************************************************

Function: compute_seasonal_latitude_variation

PURPOSE:
   (Computes the seasonal-latitudinal variation
    of density in the lower thermosphere in accordance with L. Jacchia,
    Smithsonian Astrophysical Observatory 332, 1971. This affects the
    densities between 90 and 170 km. This subroutine need not be called for
    densities above 170 km, because no effect is observed.  The variation
    should be computed after the calculation of density due to temperature
    variations and the density ( DEN ) must be in the form of a base 10 log.
    No adjustments are made to the temperature or constituent number
    densities in the region affected by this variation.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


*****************************************************************************/
void
METAtmosphere::compute_seasonal_latitude_variation()
{
   // This is taken from Jacchia(1971), eq(24).
   //  TODO 1970/71 inconsistency
   //       1970: not implemented
   //       1971: as implemented here
   //  FIXME Turner 2018-03:
   //       Now that's a bit troubling because the rest of this package has
   //       been based out of Jacchia(1970), and the 1971 coefficients have
   //       been ignored.  But here the 1971 coefficients are preferred over
   //       the 1970 version for an unexplained reason.

   // See eq(24)
   // Compute density change in lower thermosphere
   double Z       = altitude_km - 90.0;
   double exp_arg = -0.0013 * Z * Z;
   double P       = std::sin (2 * M_PI* fraction_of_year + 1.72);
   double sin_lat = std::sin (latitude);
   double s_lat_2  = sin_lat * sin_lat;
   double S       = 0.014 * Z * exp (exp_arg);

   double d_log_rho  = S * P * s_lat_2;

   // Check to compute absolute value of 'latitude'
   if (latitude < 0.0) {
      d_log_rho *= -1.0;
   }

   // density = 10^ (log(nominal-density) + delta-log-density))
   //         = 10^ (log(nominal-density)) * 10^(delta-log-density)
   //         = nominal_density * 10^(delta-log-density)
   state.density *= std::pow( 10.0, d_log_rho);
}


/*****************************************************************************

Function: compute_seasonal_lat_variation_He (was atmos_MET_SLVH)

PURPOSE:
   (Computes the seasonal-latitudinal variation
    of the helium number density according to L. Jacchia, Smithsonian
    Astrophysical Observatory 332, 1971. This correction is not important
    below about 500 km.)

REFERENCE:
   (((Jacchia, L.G.) (Revised Static Models of the Thermosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


*****************************************************************************/
void
METAtmosphere::compute_seasonal_lat_variation_He ()
{
   // This is taken from Jacchia(1971), eqn(25):

   // Compute delta to log10(helium density)
   // Equation from Jacchia (1971) p47 equation 25
   // Magic number: 0.4091:
   //     obliquity of the equator, 23.44 degrees = 0.4091 radians
   double A  = std::abs (0.65 * (solar_declination_angle / 0.4091));
   double B  = 0.5 * latitude;
   if (solar_declination_angle < 0.0) {
      B = -B;
   }
   double sin_x                = std::sin (M_PI_4 - B);
   double sin_x_3              = sin_x * sin_x * sin_x;
   // magic number: 0.35355 = sin^3(pi/4)
   double delta_log_He_density = A * (sin_x_3 - 0.35355);

   // Compute helium number density change.  Need this as an intermediate step
   // because we need to apply that change to the overall density and to the
   // Helium number density.
   double delta_He_num_density = species.num_density[4] *
                                  (std::pow (10.0, delta_log_He_density) - 1.0);
   // apply changes
   species.num_density[4] += delta_He_num_density;

   // delta-mass-density = delta-mol-density * molar-mass
   //                    = (delta-number-density / Avogadro) * mol_weight[4]
   //                    = (mol_weight[4] / Avogadro) * delta-number-density
   //                    = 6.646E-24 * delta-num-density.
   // Magic number 6.646E-27:  (mol-weight[4] / Avogadro) * (1kg/1000g).
   state.density += 6.646E-27 * delta_He_num_density;
}


/*****************************************************************************

Function: atmos_MET_FAIR5

PURPOSE:
   (Subroutine 'atmos_MET_FAIR5' fairs between the region above 500 km -- which
    invokes the seasonal-latitudinal variation of the helium number density --
    and the region below, which does not invoke any
    seasonal-latitudinal variation at all.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


*****************************************************************************/
void
METAtmosphere::atmos_MET_FAIR5 ()
{
   // store off the pre-slvh (seasonal-latitude-variations of Helium) values
   double He_num_density_pre_slvh = species.num_density[4];
   double mass_density_pre_slvh = state.density;

   // Now compute the seasonal-latitude-variation of Helium;
   // this will modify the two densities.
   compute_seasonal_lat_variation_He();

   // Generate the value of the exponent used:
   double A = std::cos( fairing_k * (altitude_km - base_fairing_height));
   double CZI = A * A;

   // Old method modified log-densities, ala:
   //    log(new_density) = CZI*(log-pre-slvh) + SZI*(log-post-slvh)
   // with SZI = 1-CZI.
   // Which can be written as:
   //    log(new_density) = (log-post-slvh) + CZI*(log-pre-slvh - log-post-slvh)
   // Which converts to:
   //    new_density = 10 ^ (...)
   //                = 10 ^ (log-post-slvh) * 10 ^ (CZI * log-pre-slvh) /
   //                                         10 ^ (CZI * log-post-slvh)
   //                = post-slvh * (pre-slvh / post-slvh) ^ (CZI)
   state.density *= std::pow( (mass_density_pre_slvh / state.density), CZI);
   species.num_density[4] *=
                  std::pow( (He_num_density_pre_slvh / species.num_density[4]),
                            CZI);
}



/*****************************************************************************
compute_mol_wt, formerly atmos_MET_MOL_WT

PURPOSE:
   (Subroutine 'atmos_MET_MOL_WT' calculates the molecular weight for
    altitudes between 90 and 105 km according to equation (1) of Smithsonian
    Astrophysical Observatory Report 313, 1970.
    Otherwise, mean molar amss is set to the constant as defined in this class.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


*****************************************************************************/
double
METAtmosphere::compute_mol_wt (
   double altitude_km_in)
{
   // Polynomial coefficients.
   // Molecular weight = sigma (mol_weight_coeffs_n * h^n)
   //    where h is the difference:
   //     TODO 1970/71 inconsistency
   //          1970: (altitude - 100km)
   //          1971: (altitude - 90km)
   //          Changing the reference-altitude would mean changing the
   //          polynomial coefficients.
   double reference_altitude = 100.0;
   //double reference_altitude = 90.0;

   if (altitude_km_in > barometric_equation_ceiling) {
      // return mol_weight of 1.0.  Algorithm is invalid.
      MessageHandler::error(
        __FILE__,__LINE__,"Invalid call.\n",
        "Molecular weight algorithm is only valid for altitudes between\n"
        "90 km and %E km.  Current altitude = %E.\n",
        barometric_equation_ceiling, altitude_km_in);
      return mol_weight_barometric_ceiling;
   }
   if (altitude_km_in < 90.0) {
      MessageHandler::error(
        __FILE__,__LINE__,"Invalid call.\n",
        "Molecular weight algorithm is only valid for altitudes between\n"
        "90 km and %E km.  Current altitude = %E.\n"
        "Continuing as though altitude was 90km.\n",
        barometric_equation_ceiling, altitude_km_in);
      altitude_km_in = 90;
   }

   // else:
   // Limit to valid range for polynomial:
   // TODO 1970/71 inconsistency
   //      1970:  (90-105) km maps to (-10, +5)
   //      1971:  (90-100) km maps to (0, 10)
   double offset = std::max( altitude_km_in - reference_altitude,
                             90.0 - reference_altitude);
   // Start with c_0 term.  (z-reference_altitude)^0 = 1.
   double mol_weight_ = mol_wt_coeffs[0];
   double offset_to_n = 1.0;

   // Accumulate the remaining terms in the molecular weight polynomial.
   for (unsigned int ii = 1; ii < 7; ++ii) {
      offset_to_n *= offset;
      mol_weight_ += (mol_wt_coeffs[ii] * offset_to_n);
   }
   return mol_weight_;
}


/*****************************************************************************
apply_gauss_quadrature

PURPOSE:
   (Subdivides total integration-altitude range
    into intervals suitable for applying Gaussian Quadrature, sets the number
    of points for integration for each sub-interval, and then perform
    Gaussian Quadrature.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
      Exosphere with Empirical Temperature Profiles) (Smithsonian
      Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
    ((Jacchia, L.G.) (Revised Static Models of the Termosphere
      and Exosphere with Emperical Temperature Profiles)
      (Smithsonian Astrophysical Observatory Report No. 332) (--)
      (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))


*****************************************************************************/
double
METAtmosphere::apply_gauss_quadrature(
   int altitude_index_start,
   double altitude_end)
{
   // Gauss quadrature is used to approximate the integration over height of
   // either (mu g / T) or (g / T)

   // Between 90km and the barometric ceiling, (i.e. cell 0), the integration
   // uses the barometric equation, which requires the mean molecular weight
   // of the entire atmosphere.  This is a variable, so it must be included
   // in the integrand.
   // Above the barometric-ceiling (i.e. cells 1 and higher), the integration
   // uses the diffusion equation for each species.  In this case, each species
   // uses its own molecular weight and multiplies that by the integral of (g /
   // RT)

   // Cannot integrate both equations in a single call.
   if (altitude_index_start == 0 &&
       altitude_end > barometric_equation_ceiling * 1.0001){
      MessageHandler::error(
       __FILE__,__LINE__,"Invalid call.\n",
       "apply_gauss_quadrature was called for an altitude range starting in\n"
       "the barometric-equation range and ending in the diffusion-equation\n"
       "range.  This requires two separate calls.\n");
   }
   bool include_mu = (altitude_index_start == 0) ? true : false;
   double integral = 0.0;

   // Divide the integration down into individual cells, compute the integral in
   // each cell, and add them together at the end to get the integral over the
   // whole range.
   for (unsigned int ii = altitude_index_start;
         ii < 8 && gauss_altitudes[ii] < altitude_end;
         ++ii) {
      unsigned int gauss_order = gauss_n[ii];
      double alt_lo            = gauss_altitudes[ii];
      double alt_hi            = std::min( altitude_end, gauss_altitudes[ii+1]);
      double half_cell_height  = 0.5 * (alt_hi - alt_lo);

      // - Now within each cell, apply a Gauss Quadrature integration of the
      // integrand (mu * g / (R * T)).
      // - mu, g, and T are functions of altitude, whereas R is constant.  So we
      // really only need to integrate (mu * g / T) and then divide that by R.
      // - Gauss quadrature integration involves
      // computing the value of the function at several locations and summing
      // those.
      double integral_cell = 0.0;
      for (unsigned int kk = 0; kk < gauss_order; ++kk) {
         // The integrand is a function of altitude, so generate the altitude
         // for each of the Gauss Quadrature reference points in the interval.
         // The gauss reference-points are specified in a range [-1,1] where -1
         // corresponds to the lower bound and +1 to the upper bound of the
         // integration range.  For this reference point, convert the "location"
         // value to a real altitude:
         double alt_eval_point = alt_lo + half_cell_height * (1.0 +
                            GaussQuadrature::gauss_xvalues[gauss_order][kk]);
         // express this altitude as a radius as a multiple of a standard
         // earth-radius
         //    r_G  =  r / r_E   =   (r_E + alt) / r_E  =   1 + alt / r_E
         // Then, letting g_E be the gravitational accel at earth's surface:
         //    g = GM/(r^2) = (g_E * r_E ^2) / r^2  =  g_E / r_G^2
         //
         // Magic numbers 9.80665 is local g when r = 6.357666E3 km.
         double rad_eval_point = 1.0 + (alt_eval_point / 6.356766E3);
         double grav = 9.80665 / (rad_eval_point * rad_eval_point);

         // Now evaluate the function
         double value_eval_point =
                    grav / thermal.compute_temperature(alt_eval_point);
         if (include_mu) {
            value_eval_point *= compute_mol_wt(alt_eval_point);
         }
         // weight this value and add it to the integral over the cell.
         integral_cell += GaussQuadrature::gauss_weights[gauss_order][kk] *
                           value_eval_point;
      }
      // We now have a scaled version of the integration over the cell.  The
      // Gauss quadrature assumes integration over [-1,1] and we need an
      // integration from alt_lo to alt_hi.  So scale by
      // (real distance) / (theoretical distance), i.e. by
      // (alt_hi - alt_lo) /2, which we already have computed.
      integral_cell *= half_cell_height;

      // Now sum that to the total integral
      integral += integral_cell;

      // Continue onto the next cell.
   }
   return integral;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
