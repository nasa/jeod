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
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/MET/include/MET_atmosphere.hh
 * Implement the MET atmosphere using the atmosphere framework
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (Provides the specific implementation of the MET atmosphere.)
ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.)
    (Only an METAtmosphere pointer should be given to an METAtmosphereState
     object for the update_state call. Otherwise there can be unexpected
     behavior))
LIBRARY DEPENDENCIES:
   (../src/MET_atmosphere.cc)

*******************************************************************************/

#ifndef JEOD_MET_ATMOSPHERE_HH
#define JEOD_MET_ATMOSPHERE_HH

// System includes

// JEOD includes
#include "environment/time/include/time_utc.hh"
#include "utils/math/include/gauss_quadrature.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "MET_atmosphere_state_vars.hh"
#include "environment/atmosphere/base_atmos/include/atmosphere.hh"

//! Namespace jeod
namespace jeod
{

/*****************************************************************************
METAtmosphereChemical
Purpose:(The chemical composition of the MET Atmosphere.)
*****************************************************************************/
/**
 * The chemical composition of the MET Atmosphere.
 */
class METAtmosphereChemical
{
    JEOD_MAKE_SIM_INTERFACES(jeod, METAtmosphereChemical)

public:
    static const int num_species = 6; /* !<trick_units(--) Number of chemical species to track.*/

    double num_density[num_species]{}; /* !<trick_units(1/m3) Particle density of the particular species */

    // These numbers for N2, O2, Ar, and He  are taken from Jacchia(1971), p5
    double frac[num_species]{
        0.78110,   // N2
        0.20955,   // O2
        0.0,       // Ox
        0.0093432, // Ar
        1.289E-05, // He // TODO 1970/71 inconsistency
                   //      1970: 1.289E-5
                   //      1971: 6.1471E-6
        0.0        // H
    };             /* !<trick_units(--) The fraction of the atmosphere by volume represented by each species.*/

    double mol_weight[num_species]{
        28.0134, // N2
        31.9988, // O2
        15.9994, // Ox
        39.948,  // Ar
        4.0026,  // He
        1.00797  // H
    };           /* !<trick_units(g/mol) Nominal molecular weight, or molar mass for each species.*/

    const double nominal_mol_weight{28.96}; /* !<trick_units(g/mol) Nominal molecular weight,
                                               or molar mass for each species.*/

    METAtmosphereChemical() = default;
    virtual ~METAtmosphereChemical() = default;
    METAtmosphereChemical & operator=(const METAtmosphereChemical &) = delete;
    METAtmosphereChemical(const METAtmosphereChemical &) = delete;
};

/*****************************************************************************
METAtmosphereThermal
Purpose:(The Thermal aspect of the computation.)
*****************************************************************************/
/**
 * The Thermal aspect of the computation.
 */
class METAtmosphereThermal
{
    JEOD_MAKE_SIM_INTERFACES(jeod, METAtmosphereThermal)
public:
    double T_out{}; /* !<trick_units(K)
                     The computed tempeerature at the current altitude.*/
    void update();
    double compute_temperature(double altitude_km);
    METAtmosphereThermal(const double & T_exosphere, const double & altitude_km);
    virtual ~METAtmosphereThermal() = default;
    METAtmosphereThermal & operator=(const METAtmosphereThermal &) = delete;
    METAtmosphereThermal(const METAtmosphereThermal &) = delete;

private:
    /**
     * Temperature coefficients.
     */
    const double k_1{0.054285714}; /*!< trick_units(1/m)
                      parameter used to obtain the first coefficient of the
                      temperature polynomial, which is also the temperature
                      gradient at 125km. */

    const double k_3{-3.96501457725948E-5}; /*!< trick_units(1/m3)
                       parameter used to obtain the 3rd coefficient of the
                       temperature polynomial. */

    const double k_4{-5.3311120366514E-7}; /*!< trick_units(1/m4)
                       parameter used to obtain the 4th coefficient of the
                       temperature polynomial. */

    const double T_90{183.0}; /*!< trick_units(K)
                         Temperature at 90km reference point.*/

    double T_125{}; /*!< trick_units(K)
                    Temperature at 125km reference point.*/

    const double & T_exosphere; /* !< trick_units(K)
                                   reference to the current exospheric temperature.*/

    const double & altitude_km; /* !< trick_units(km)
                                   reference to the current altitude in km.*/

    void generate_base_temperature();
};

/*****************************************************************************
METAtmosphere
Purpose:(The MET specific implementation of Atmosphere.)
*****************************************************************************/
class METAtmosphere : public Atmosphere
{
    JEOD_MAKE_SIM_INTERFACES(jeod, METAtmosphere)

public: // public member variables
    enum AtmosMETGeoIndexType
    {                        /* The type of geomagnetic index to be used.*/
        ATMOS_MET_GI_AP = 0, /* Calculate the geomagnetic variations in
                                temperature (tg) using the Ap formulation. */
        ATMOS_MET_GI_KP = 1  /* Calculate the geomagnetic variations in
                                temperature (tg) using the Kp formulation. */
    };

    AtmosMETGeoIndexType geo_index_type{ATMOS_MET_GI_AP}; /* !< trick_units(--)
        Calculation type for the geomagnetic variations in temperature (tg).*/

    double geo_index{}; /*!< trick_units(--) Geomagnetic variations index (Ap or Kp). */

    double F10{}; /*!< trick_units(--) Solar radio noise flux. */

    double F10B{}; /*!< trick_units(--) 90 day average of solar radio noise flux. */

    METAtmosphereChemical species; /*!< trick_units(--) The chemical composition of the atmosphere. */

private:                  // private member variables
    double altitude_km{}; /*!< trick_units(km) Copy of vehicle altitude */
    double latitude{};    /*!< trick_units(rad) Copy of vehicle latitude */
    double longitude{};   /*!< trick_units(rad) Copy of vehicle longitude */

    double barometric_equation_ceiling{105.0}; /*!< trick_units(km)
        the ceiling for integration using the barometric equation.
        Above this value, the integration switches to the diffusion equation.
        Value is 105km in the 1970 paper and 100km in the 1971 paper.*/
    //  TODO 1970/71 inconsistency
    //       1970: 105.0
    //       1971: 100.0

    const double & trunc_julian_time; /*!< trick_units(day) Current time */
    double tjt_year_start{11544.0};   /*!< trick_units(day)
    value of trunc_julian_time at the start of the current year. */
                                      // on 2000/01/01::00:0:00.0

    double fraction_of_year{}; /*!< trick_units(--) fraction of this year that has passed. */

    int day_of_year{1}; /*!< trick_units(count) day number since start of year. */

    int max_days_this_year{366}; /*!< trick_units(count) number of days this year (365 or 366) */

    int year{2000}; /*!< trick_units(count) current year identifier */

    double solar_declination_angle{}; /*!< trick_units(rad) declination angle */

    double solar_hour_angle{}; /*!< trick_units(rad) solar hour angle */

    METAtmosphereStateVars state; /*!< trick_units(--)
       A scratch set of state variables, used for populating state
       variables internally before being copied onto the real state. */

    METAtmosphereThermal thermal; /*!< trick_units(--) Thermal aspect of the model */

    // Physical Constants.
    const double R_gas_constant{8.31432};  /*!< trick_units(J/(mol*K)) R */
                                           // Note: This is not an accurate value for R
                                           //       But it is the value used by Jacchia
                                           //       See Jacchia(1971), p9, eq 5.
    const double days_per_year{365.2422};  /*!< trick_units(day) days per year */
    const double Avogadro{6.02257E23};     /*!< trick_units(--)  Avogadros number */
                                           // Note: This is not Avogadro's number!
                                           //       But it is the number used by Jacchia.
                                           //       See Jacchia(1971) p 7, eq(4)
    const double two_pi{6.28318531};       /*!< trick_units(--)  2 pi */
    const double three_pi_two{4.71238898}; /*!< trick_units(--)  1.5 pi */
    const double deg_to_rad{0.017453293};  /*!< trick_units(degree/rad) degree-to-radian conversion */
    const int days_per_century{36525};     /*!< trick_units(count) days per century */
    const int minutes_per_day{1440};       /*!< trick_units(count) minutes per day */

    // Model constants
    const double mol_weight_barometric_ceiling{27.72594278125}; /*!< trick_units(g/mol) mean molar mass at
                                                     barometric-ceiling and higher.*/
                                                                // see Jacchia(1970) eqn(1).
                                                                // Jacchia(1971): 27.6398
    const double base_fairing_height{440.0};                    /*!< trick_units(km)
                                        Altitude at which to start fairing between the lower altitude which has
                                        no seasonal-latitude Helium density variation, and the upper atmosphere
                                        -- starting at 500km -- which does. */
    const double fairing_k;                                     /*!< trick_units(rad/km)
                                                                  Factor which, when multiplied by the altitude delta above the
                                                                  base-fairing-height provides an angle.  The square of the cosine of
                                                                  that angle indicates how much of the seasonal-variation in Helium
                                                                  density to apply.
                                                                  density = corrected-density *
                                                                              (non-corrected-density / corrected-density) ^
                                                                                                  (cos^2 ( fairing_k * delta-altitude))
                                                                  At base-fairing-height, none gets applied.  By 500km, it all gets
                                                                  applied. */

    // Molecular weight coefficients
    static const int num_mol_wt_coeffs = 7; /*!< trick_units(count) the number of polynomial coefficients.*/
    static const double mol_wt_coeffs[num_mol_wt_coeffs]; /*!< trick_units(--)
        polynomial coefficients for computing the molecular weights in the
        region where the barometric equation is used.*/

    // Gauss Quadrature (integration) values:
    static const int num_integ_divisions = 8;                     /*!< trick_units(count)
                                               the number of altitude bins used for dividing the atmosphere into
                                               manageable pieces.*/
    static const double gauss_altitudes[num_integ_divisions + 1]; /*!< trick_units(--)
           The boundaries of the cells that are used to break down the
           integration over the atmosphere into more manaegable pieces.
           NOTE - gauss_altitudes[1] must mark the upper limit of the
           altitude over which the barometric equation is valid, this is
           either 100km or 105km, depending on which paper is used;
           gauss-altitude[6] must be equal to 500km.*/
    static const int gauss_n[num_integ_divisions];                /*!< trick_units(--)
                                      The number of data-points to be used for the gauss-quadrature
                                      integration for each interval defined in the gauss_altitudes array.
                                      AKA the order of the gauss-quadrature.*/

public: // public member functions
    // constructor
    explicit METAtmosphere(const double & trunc_julian_time_in);
    ~METAtmosphere() override = default;
    METAtmosphere & operator=(const METAtmosphere &) = delete;
    METAtmosphere(const METAtmosphere &) = delete;

    void update_atmosphere(const PlanetFixedPosition * pfix_pos, AtmosphereState * state) override;
    void update_atmosphere(const PlanetFixedPosition * pfix_pos, METAtmosphereStateVars * state);

private: // private member functions
    void update_atmosphere(const PlanetFixedPosition * pfix_pos);
    void modify_densities();
    void compute_solar_angles();
    void compute_exospheric_temperature();
    void jacchia();
    void compute_seasonal_latitude_variation();
    void compute_seasonal_lat_variation_He();
    void atmos_MET_FAIR5();
    double compute_mol_wt(double altitude);
    double apply_gauss_quadrature(int altitude_index_start, double ceiling);
}; // end of MET atmosphere class

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
