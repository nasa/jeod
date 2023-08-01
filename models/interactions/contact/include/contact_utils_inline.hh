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
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/include/contact_utils_inline.hh
 * Define ContactUtils::create_relstate_name,
 * ContactUtils::copy_const_char_to_char
 */

/* ======================================================================== -
PURPOSE:
   ()

REFERENCES:
   (((dist_line_segments - derived from
     Copyright 2001, softSurfer "www.softsurfer.com"
     This code may be freely used and modified for any purpose
     providing that this copyright notice is included with it.
     SoftSurfer makes no warranty for this code, and cannot be held
     liable for any real or imagined damage resulting from its use.
     Users of this code must verify correctness for their application.)))

ASSUMPTIONS AND LIMITATIONS:
   ((none))



 - ======================================================================== */
#ifndef CONTACT_UTILS_INLINE_HH
#define CONTACT_UTILS_INLINE_HH

/* System includes */
#include <cstring>

// JEOD includes
#include "utils/math/include/vector3.hh"

// Model includes
#include "contact_utils.hh"
#include "contact_messages.hh"

//! Namespace jeod
namespace jeod {

/**
 * create a name for a relstate out of two facet names
 * @return char**
 * \param[in] name1 name of first contact facet
 * \param[in] name2 name of second contact facet
 * \param[in,out] out_str output name for the relstate
 */
inline int
ContactUtils::create_relstate_name (
   char * name1,

   char * name2,

   char ** out_str)
{
   size_t len;
   len = strlen (name1) + strlen (name2) + 2;

   (*out_str) = new char[len + 1];

   strcpy (*out_str, name1);
   strcat (*out_str, ".");
   strcat (*out_str, name2);
   (*out_str)[len] = '\0';

   return 1;
}


/**
 * create a name for a relstate out of two facet names
 * @return int
 * \param[in] in_str const char input
 * \param[in,out] out_str char output
 */
inline int
ContactUtils::copy_const_char_to_char (
   const char * in_str,

   char ** out_str)
{
/*   if (in_str == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, ContactMessages::initialization_error,
         "A NULL pointer was given to the copy_const_char_to_char function. "
         "This is not allowed, and a failure has happened.\n");
      return 0;
   }
   else { */
      const size_t len = strlen (in_str);
      char * new_str = new char[len + 1];
      (*out_str) = new_str;
      strncpy (new_str, in_str, len);
      new_str[len] = '\0';

      return 1;
//   }
}


/**
 * calculate the closest points between two line segments
 * @return success
 * \param[in] p1 vector to one point of the first line seg\n Units: M
 * \param[in] p2 vector to one point of the first line seg\n Units: M
 * \param[in] p3 vector to one point of the second line seg\n Units: M
 * \param[in] p4 vector to one point of the second line seg\n Units: M
 * \param[out] pa vector to close_point on line 1\n Units: M
 * \param[out] pb vector to close_point on line 2\n Units: M
 */
inline int
ContactUtils::dist_line_segments (
   double p1[3],
   double p2[3],
   double p3[3],
   double p4[3],
   double *pa,
   double *pb)
{
   double p13[3],p43[3],p21[3];
   double d1343,d4321,d1321,d4343,d2121;
   double numer,denom;
   double vpa[3], vpb[3];
   double EPS = 1e-10;


   Vector3::diff(p1, p3, p13);
   Vector3::diff(p4, p3, p43);
   Vector3::diff(p2, p1, p21);

   d1343 = Vector3::dot(p13, p43);
   d4321 = Vector3::dot(p43, p21);
   d1321 = Vector3::dot(p13, p21);
   d4343 = Vector3::dot(p43, p43);
   d2121 = Vector3::dot(p21, p21);

   denom = d2121 * d4343 - d4321 * d4321;

   if (d4343 < EPS && d2121 < EPS) {
      Vector3::copy(p1, pa);
      Vector3::copy(p3, pb);
   }
   else if (d4343 < EPS) {
      //u = d3121 / d2121
      //vpa = p1 + u(p21)
      double p31[3];
      double d3121;
      double u;
      Vector3::diff(p3, p1, p31);
      d3121 = Vector3::dot(p31, p21);
      u = d3121 / d2121;
      if (u > 1) {
         u = 1;
      }
      else if (u < 0) {
         u = 0;
      }
      Vector3::scale(p21, u, vpa);
      Vector3::sum(p1, vpa, pa);

      Vector3::copy(p3, pb);
   }
   else if(d2121 < EPS) {
      //u = d1343 / d4343
      //vpb = p3 + u(p43)
      double u;
      u = d1343 / d4343;
      if (u > 1) {
         u = 1;
      }
      else if (u < 0) {
         u = 0;
      }
      Vector3::scale(p43, u, vpb);
      Vector3::sum(p3, vpb, pb);
      Vector3::copy(p1, pa);
   }
   else if (fabs(denom) < EPS) {  // Test of parallel lines.
      // Lines are parallel look for the ends that are closest to each other
      double p14[3], p23[3], p24[3];
      double d13, d14, d23, d24;

      Vector3::diff(p1, p4, p14);
      Vector3::diff(p2, p3, p23);
      Vector3::diff(p2, p4, p24);

      d13 = Vector3::vmag(p13);
      d14 = Vector3::vmag(p14);
      d23 = Vector3::vmag(p23);
      d24 = Vector3::vmag(p24);

      if(d13 < d14) {
         if(d13 < d23) {
            if(d13 < d24) {
               Vector3::copy(p1, vpa);
               Vector3::copy(p3, vpb);
            }
            else {
               Vector3::copy(p2, vpa);
               Vector3::copy(p4, vpb);
            }
         }
         else {
            if(d23 < d24) {
               Vector3::copy(p2, vpa);
               Vector3::copy(p3, vpb);
            }
            else {
               Vector3::copy(p2, vpa);
               Vector3::copy(p4, vpb);
            }
         }
      }
      else {
         if(d14 < d23) {
            if(d14 < d24) {
               Vector3::copy(p1, vpa);
               Vector3::copy(p4, vpb);
            }
            else {
               Vector3::copy(p2, vpa);
               Vector3::copy(p4, vpb);
            }
         }
         else {
            if(d23 < d24) {
               Vector3::copy(p2, vpa);
               Vector3::copy(p3, vpb);
            }
            else {
               Vector3::copy(p2, vpa);
               Vector3::copy(p4, vpb);
            }
         }
      }

      Vector3::copy(vpa, pa);
      Vector3::copy(vpb, pb);

   }
   else {
      numer = d1343 * d4321 - d1321 * d4343;

      double ma = numer / denom;
      double mb = (d1343 + d4321 * ma) / d4343;

      if (ma <= 0.0) {
         Vector3::copy(p1, vpa);
      }
      else if (ma >= 1.0) {
         Vector3::copy(p2, vpa);
      }
      else {
         vpa[0] = p1[0] + ma * p21[0];
         vpa[1] = p1[1] + ma * p21[1];
         vpa[2] = p1[2] + ma * p21[2];
      }

      if (mb <= 0.0) {
         Vector3::copy(p3, vpb);
      }
      else if (mb >= 1.0) {
         Vector3::copy(p4, vpb);
      }
      else {
         vpb[0] = p3[0] + mb * p43[0];
         vpb[1] = p3[1] + mb * p43[1];
         vpb[2] = p3[2] + mb * p43[2];
      }

      Vector3::copy(vpa, pa);
      Vector3::copy(vpb, pb);


   }
   return true;
}

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
