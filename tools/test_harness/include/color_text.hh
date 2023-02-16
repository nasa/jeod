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
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/include/color_text.hh
 * Define class ColorText.
 */

/**
Purpose:
  ()
*/
#ifndef JEOD_COLOR_TEXT_HH
#define JEOD_COLOR_TEXT_HH

#include <cstring>
#include <cstdio>


//! Namespace jeod 
namespace jeod {

/**
 * Defines methods that highlight key outputs from a unit test run.
 */
class ColorText {

 public:

   /**
    * Enumerates the standard ASCII text colors, sans black and white.
    */
   enum TextColor {
      Red     = 31,   ///< Red text
      Green   = 32,   ///< Green text
      Yellow  = 33,   ///< Yellow text
      Blue    = 34,   ///< Blue text
      Magenta = 35,   ///< Magenta text
      Cyan    = 36    ///< Cyan text
   };

   ColorText ();

   std::string color (const std::string & text, TextColor color);

   std::string pass_fail (bool passed);
   std::string pass_fail (bool passed, const std::string & text);
   std::string pass_fail (bool passed, const char * format, int number);
   std::string pass_fail (bool passed, const char * format, double number);
   std::string pass_fail (
      const bool passed[3], const char * format,
      unsigned int nsp1st, unsigned int nspmid, double number[3]);
   std::string pass_fail (
      const bool passed[3][3], const char * format,
      unsigned int nsp1st, unsigned int nspmid, double number[3][3]);

 private:
   /**
    * Array of ascii colors.
    */
   char ascii_color[7][6]; //!< trick_units(--)
   /**
    * Buffer for internal use.
    */
   char buf[64]; //!< trick_units(--)
};


/**
 * Default constructor.
 */
inline
ColorText::ColorText (
   void)
{
   std::strcpy (&ascii_color[0][0], "\033[00m");
   std::strcpy (&ascii_color[1][0], "\033[31m");
   std::strcpy (&ascii_color[2][0], "\033[32m");
   std::strcpy (&ascii_color[3][0], "\033[33m");
   std::strcpy (&ascii_color[4][0], "\033[34m");
   std::strcpy (&ascii_color[5][0], "\033[35m");
   std::strcpy (&ascii_color[6][0], "\033[36m");
}


/**
 * Base method used by the other ColorText methods to wrap a chunk of text
 * between an ASCII start color and ASCII restore pair of markers.
 * \return  A std::string whose c_str() value is the input text sandwiched
 *          between an ASCII start color escape sequence and an ASCII restore
 *          color escape sequence
 * \param[in] text       The text to be processed
 * \param[in] text_color The color to be applied
 */
inline std::string
ColorText::color (
   const std::string & text,
   TextColor text_color)
{
   unsigned int color_idx = static_cast<unsigned int>(text_color) - 30;
   std::string result = ascii_color[color_idx] + text + ascii_color[0];
   return result;
}


/**
 * Generate a string whose c_str() value is "Passed" or "Failed" sandwiched
 * betweeen an ASCII start color / restore monitor pair. The text is "Passed"
 * and the color green if the value of the input flag is true, "Failed" colored
 * red if the input flag is false.
 * \return  An ASCII-colored Passed/Failed string
 * \param[in] passed Pass/fail flag
 */
inline std::string
ColorText::pass_fail (
   bool passed)
{
   if (passed) {
      return color ("Passed", Green);
   }
   else {
      return color ("Failed", Red);
   }
}


/**
 * Generate a string whose c_str() value is that of the provided @a text but
 * sandwiched betweeen an ASCII color text/restore monitor pair. The text is
 * colored green if the flag @a passed is true, red otherwise.
 * \return  An ASCII-colored string
 * \param[in] passed Pass/fail flag
 * \param[in] text   String
 */
inline std::string
ColorText::pass_fail (
   bool passed,
   const std::string & text)
{
   return color (text, passed ? Green : Red);
}


/**
 * Generate a string whose c_str() value is the number as formatted per the
 * format string but sandwiched betweeen an ASCII color text/restore monitor
 * pair. The text is colored green if the passed flag is true, red otherwise.
 * \return  An ASCII-colored, c-formatted string
 * \param[in] passed Pass/fail flag
 * \param[in] format C-language sprintf format (e.g. "%d")
 * \param[in] number The number to be formatted and colored
 */
inline std::string
ColorText::pass_fail (
   bool passed,
   const char * format,
   int number)
{
   std::snprintf (buf, sizeof(buf), format, number);
   return color (buf, passed ? Green : Red);
}


/**
 * Generate a string whose c_str() value is the number as formatted per the
 * format string but sandwiched betweeen an ASCII color text/restore monitor
 * pair. The text is colored green if the passed flag is true, red otherwise.
 * \return  An ASCII-colored, c-formatted string
 * \param[in] passed Pass/fail flag
 * \param[in] format C-language sprintf format (e.g. "%g" or "%.7f")
 * \param[in] number The number to be formatted and colored
 */
inline std::string
ColorText::pass_fail (
   bool passed,
   const char * format,
   double number)
{
   std::snprintf (buf, sizeof(buf), format, number);
   return color (buf, passed ? Green : Red);
}


/**
 * Generate a string whose c_str() value is the number as formatted per the
 * format string but sandwiched betweeen an ASCII color text/restore monitor
 * pair. The text is colored green if the passed flag is true, red otherwise.
 * \return  An ASCII-colored, c-formatted string
 * \param[in] passed Pass/fail flags
 * \param[in] format C-language sprintf format (e.g. "%g" or "%.7f")
 * \param[in] nsp1st Number of spaces before first item
 * \param[in] nspmid Number of spaces between items
 * \param[in] number The vector to be formatted and colored
 */
inline std::string
ColorText::pass_fail (
   const bool passed[3],
   const char * format,
   unsigned int nsp1st,
   unsigned int nspmid,
   double number[3])
{
   std::snprintf (buf, sizeof(buf), "%*s", nsp1st, "");
   std::string result (buf);

   std::snprintf (buf, sizeof(buf), "%*s", nspmid, "");
   std::string sep (buf);

   for (unsigned int ii = 0; ii < 3; ++ii) {
      std::snprintf (buf, sizeof(buf), format, number[ii]);
      result += color (buf, passed[ii] ? Green : Red);
      if (ii < 2) {
         result += sep;
      }
   }

   return result;
}


/**
 * Generate a string whose c_str() value is the number as formatted per the
 * format string but sandwiched betweeen an ASCII color text/restore monitor
 * pair. The text is colored green if the passed flag is true, red otherwise.
 * \return  An ASCII-colored, c-formatted string
 * \param[in] passed Pass/fail flags
 * \param[in] format C-language sprintf format (e.g. "%g" or "%.7f")
 * \param[in] nsp1st Number of spaces before first item
 * \param[in] nspmid Number of spaces between items
 * \param[in] number The matrix to be formatted and colored
 */
inline std::string
ColorText::pass_fail (
   const bool passed[3][3],
   const char * format,
   unsigned int nsp1st,
   unsigned int nspmid,
   double number[3][3])
{
   std::string result("");

   for (unsigned int ii = 0; ii < 3; ++ii) {
      result += pass_fail (passed[ii], format, nsp1st, nspmid, number[ii]);
      result += "\n";
   }

   return result;
}

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 */
