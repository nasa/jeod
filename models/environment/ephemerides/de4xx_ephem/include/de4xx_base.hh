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
 * @addtogroup Ephemerides
 * @{
 * @addtogroup De4xxEphem
 * @{
 *
 * @file models/environment/ephemerides/de4xx_ephem/include/de4xx_base.hh
 * Define data types for JPL ephemeris model
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((JPL ephemeris model reference needed)))

Assumptions and limitations:
  ((Assumption 1.
    Host machine is big-endian or little-endian.)
   (Assumption 2.
    32-bit integers occupy 4 chars and doubles, 8 chars)
   (Assumption 3.
    IEEE-standard doubles)
   (Limitation 1.
    Librations and nutations are not represented.))


*******************************************************************************/


#ifndef JEOD_DE4xx_BASE_HH
#define JEOD_DE4xx_BASE_HH

// JEOD includes
#include <cstdint>

#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * Defines enumerations used in the DE4xx ephemeris model.
 */
namespace De4xxBase {

   /**
    * Defines names for planetary body descriptors in the ephemeris file.
    * This enum defines names for the bodies as they are represented in the
    * ephemeris file.
    */
   enum De4xxFileEntries {
      // These items are present in ALL de4xx models
      De4xx_File_Mercury    =  0,   ///< Mercury                  XYZ [km]
      De4xx_File_Venus      =  1,   ///< Venus                    XYZ
      De4xx_File_EMbary     =  2,   ///< Earth-moon barycenter    XYZ
      De4xx_File_Mars       =  3,   ///< Mars                     XYZ
      De4xx_File_Jupiter    =  4,   ///< Jupiter                  XYZ
      De4xx_File_Saturn     =  5,   ///< Saturn                   XYZ
      De4xx_File_Uranus     =  6,   ///< Uranus                   XYZ
      De4xx_File_Neptune    =  7,   ///< Neptune                  XYZ
      De4xx_File_Pluto      =  8,   ///< Pluto                    XYZ
      De4xx_File_Moon       =  9,   ///< Moon, geocentric coords. XYZ
      De4xx_File_Sun        = 10,   ///< Sun                      XYZ
      De4xx_File_ENutation  = 11,   ///< Earth Nutations  d(psi), d(eps) [rad]
      De4xx_File_LLibration = 12,   ///< Lun mantle Libratns phi,tht,psi [rad]

      // These items are only reported in de4xx models beginning in DE430t & up
      De4xx_File_LAngVel    = 13,   ///< Lun mantle ang vel omg_{xyz} [rad/day]
      De4xx_File_tt_tdb     = 14,   ///< TDB to TT offset @ geocenter [s]
      De4xx_File_MaxEntries
   };

   /**
    * Index aliases for the constants listed in the DE header that are used by
    * JEOD. This is an incomplete subset of the constants provided by DE, but
    * all of these constants must be present for JEOD ephemeris to perform.
    */
   enum De4xxEphemConsts {
      // These items *should* be present in all DE models
      De4xx_Const_DENUM = 0,
      De4xx_Const_LENUM,
      De4xx_Const_AU,
      De4xx_Const_EMRAT,
      De4xx_Const_CLIGHT,
      De4xx_Const_GM1,
      De4xx_Const_GM2,
      De4xx_Const_GMB,
      De4xx_Const_GM4,
      De4xx_Const_GM5,
      De4xx_Const_GM6,
      De4xx_Const_GM7,
      De4xx_Const_GM8,
      De4xx_Const_GM9,
      De4xx_Const_GMS,
      De4xx_Const_MaxConsts,
   };


   /**
    * Defines names for ephemeris items as represented in the JEOD DE4xx model.
    * NOTA BENE: The Earth-moon barycenter follows the massive bodies as the
    * barycenter is not itself a massive body.
    */
   enum De4xxEphemBodies {

       // Items associated with TRANS states IN basic DE model
      De4xx_Ephem_Sun       =  0,   ///< Sun
      De4xx_Ephem_Mercury   =  1,   ///< Mercury
      De4xx_Ephem_Venus     =  2,   ///< Venus
      De4xx_Ephem_Earth     =  3,   ///< Earth
      De4xx_Ephem_Mars      =  4,   ///< Mars
      De4xx_Ephem_Jupiter   =  5,   ///< Jupiter
      De4xx_Ephem_Saturn    =  6,   ///< Saturn
      De4xx_Ephem_Uranus    =  7,   ///< Uranus
      De4xx_Ephem_Neptune   =  8,   ///< Neptune
      De4xx_Ephem_Pluto     =  9,   ///< Pluto
      De4xx_Ephem_Moon      = 10,   ///< Moon
      De4xx_Ephem_EMbary    = 11,   ///< Earth-moon barycenter

      // Implied and DERIVED TRANS states
      De4xx_Ephem_SSbary    = 12,   ///< Solar system barycenter
      De4xx_Ephem_EML1      = 13,   ///< Earth-moon L1 point (disabled)

      // ORIENTATIONS in the basic DE model
      De4xx_Ephem_ENutation  = 14,   ///< Nutations (disabled)
      De4xx_Ephem_LLibration = 15,   ///< Librations
      De4xx_Ephem_MaxBodies
   };

   static const char* point_names[32] __attribute__ ((unused)) =
   {
      "Sun",
      "Mercury",
      "Venus",
      "Earth",
      "Mars",
      "Jupiter",
      "Saturn",
      "Uranus",
      "Neptune",
      "Pluto",
      "Moon",

      "EMBary",
      "SSBary",
   };

   /**
    *  Total number of items in the JEOD ephemeris. Refer to De4xxEphemBodies
    *  for identities of each item. Currently only one possibility, but
    *  written for extensibility
    */
   static inline uint32_t number_jeod_items(int de_version_num __attribute__ ((unused)) )
   {
       return 16;
   }

   /**
    * Total number of translational states in the JEOD ephemeris.
    * (Sun, Mercury to Pluto, EMBary, SSbary) Currently excludes EML1
    * Currently only one possibility, but written for extensibility
    */
   static inline uint32_t number_trans_points(int de_version_num __attribute__ ((unused)) )
   {
       return 13;
   }

   /**
    * Number of gravity models in the JEOD ephemeris
    * (Mercury to Sun + implied Earth)
    * Currently only one possibility, but written for extensibility
    */
   static inline uint32_t number_grav_models(int de_version_num __attribute__ ((unused)) )
   {
       return 12;
   }

   /**
    * Number of bodies in the JEOD ephemeris (Planets + Pluto + Moon + Sun)
    * Currently only one possibility, but written for extensibility
    */
   static inline uint32_t number_physical_bodies(int de_version_num __attribute__ ((unused)) )
   {
       return 11;
   }

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
