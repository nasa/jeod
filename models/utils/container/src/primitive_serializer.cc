/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Container
 * @{
 *
 * @file models/utils/container/src/primitive_serializer.cc
 * Define class JeodPrimitiveSerializerBase static methods.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/


// System includes
#define __USE_ISOC99
#include <cmath>
#include <cstddef>
#include <limits>
#include <sstream>
#include <string>

// JEOD includes

// Model includes
#include "../include/primitive_serializer.hh"



//! Namespace jeod
namespace jeod {

/**
 * Convert a string to a string suitable for output.
 * Special characters are backslash-escaped.
 * @return Serialized string
 * \param[in] val String to serialize
 */
const std::string
JeodPrimitiveSerializerBase::serialize_string (
   const std::string & val)
{
   std::string result("\"");
   std::string escape_these("\\\"\n");

   std::size_t prev_pos = 0;
   std::size_t next_pos = 0;

   for (next_pos = val.find_first_of (escape_these, prev_pos);
        next_pos != val.npos;
        next_pos = val.find_first_of (escape_these, prev_pos)) {

      if (next_pos > prev_pos) {
         result.append (val.substr (prev_pos, next_pos - prev_pos));
      }

      result.append ("\\");
      switch (val[next_pos]) {
      case '\n':
         result.append (1, 'n');
         break;
      default:
         result.append (1, val[next_pos]);
         break;
      }
      prev_pos = next_pos + 1;
   }

   if (prev_pos < val.length()) {
      result.append (val.substr (prev_pos));
   }
   result.append("\"");

   return result;
}


/**
 * Convert a serialized string to its internal representation.
 * Backslash-escaped characters are converted to special characters.
 * @return Deserialized string
 * \param[in] val Serialized string
 */
const std::string
JeodPrimitiveSerializerBase::deserialize_string (
   const std::string & val)
{
   std::string result;

   std::size_t prev_pos = 1;
   std::size_t next_pos = 1;

   for (next_pos = val.find_first_of ('\\', prev_pos);
        next_pos != val.npos;
        next_pos = val.find_first_of ('\\', prev_pos)) {

      if (next_pos > prev_pos) {
         result.append (val.substr (prev_pos, next_pos - prev_pos));
      }
      switch (val[next_pos+1]) {
      case 'n':
         result.append (1, '\n');
         break;
      default:
         result.append (1, val[next_pos+1]);
         break;
      }
      prev_pos = next_pos + 2;
   }

   if (prev_pos < val.length()) {
      result.append (val.substr (prev_pos));
   }
   result.erase (result.length() - 1);

   return result;
}


/**
 * Convert a float to a string suitable for output.
 * NaNs and Infs get special treatment.
 * Everything is serialized via c++ I/O.
 * @return Serialized number
 * \param[in] val Number to serialize
 */
const std::string
JeodPrimitiveSerializerBase::serialize_float (
   const float & val)
{
   switch (std::fpclassify (val)) {
   case FP_NAN:
      return "NaN";

   case FP_INFINITE:
      if (std::signbit(val) == 0) {
         return "Inf";
      }
      else {
         return "-Inf";
      }

   default: {
      std::ostringstream obuf;
      obuf.precision (std::numeric_limits<float>::digits10 + 2);
      obuf << val;
      return obuf.str();
   }
   }
}


/**
 * Convert a serialized float to its internal representation.
 * @return Deserialized float
 * \param[in] val Serialized string
 */
float
JeodPrimitiveSerializerBase::deserialize_float (
   const std::string & val)
{
   float result;

   if (val.compare("NaN") == 0) {
      result = std::numeric_limits<float>::signaling_NaN();
   }
   else if (val.compare("Inf") == 0) {
      result = std::numeric_limits<float>::infinity();
   }
   else if (val.compare("-Inf") == 0) {
      result = -std::numeric_limits<float>::infinity();
   }
   else {
      std::istringstream ibuf(val);
      ibuf >> result;
   }

   return result;
}


/**
 * Convert a double to a string suitable for output.
 * NaNs and Infs get special treatment.
 * Everything is serialized via c++ I/O.
 * @return Serialized number
 * \param[in] val Number to serialize
 */
const std::string
JeodPrimitiveSerializerBase::serialize_double (
   const double & val)
{
   switch (std::fpclassify (val)) {
   case FP_NAN:
      return "NaN";

   case FP_INFINITE:
      if (std::signbit(val) == 0) {
         return "Inf";
      }
      else {
         return "-Inf";
      }

   default: {
      std::ostringstream obuf;
      obuf.precision (std::numeric_limits<double>::digits10 + 2);
      obuf << val;
      return obuf.str();
   }
   }
}


/**
 * Convert a serialized double to its internal representation.
 * @return Deserialized double
 * \param[in] val Serialized string
 */
double
JeodPrimitiveSerializerBase::deserialize_double (
   const std::string & val)
{
   double result;

   if (val.compare("NaN") == 0) {
      result = std::numeric_limits<double>::signaling_NaN();
   }
   else if (val.compare("Inf") == 0) {
      result = std::numeric_limits<double>::infinity();
   }
   else if (val.compare("-Inf") == 0) {
      result = -std::numeric_limits<double>::infinity();
   }
   else {
      std::istringstream ibuf(val);
      ibuf >> result;
   }

   return result;
}


/**
 * Convert a long double to a string suitable for output.
 * NaNs and Infs get special treatment.
 * Everything is serialized via c++ I/O.
 * @return Serialized number
 * \param[in] val Number to serialize
 */
const std::string
JeodPrimitiveSerializerBase::serialize_long_double (
   const long double & val)
{
   switch (std::fpclassify (val)) {
   case FP_NAN:
      return "NaN";

   case FP_INFINITE:
      if (std::signbit(val) == 0) {
         return "Inf";
      }
      else {
         return "-Inf";
      }

   default: {
      std::ostringstream obuf;
      obuf.precision (std::numeric_limits<long double>::digits10 + 2);
      obuf << val;
      return obuf.str();
   }
   }
}


/**
 * Convert a serialized double to its internal representation.
 * @return Deserialized long double
 * \param[in] val Serialized string
 */
long double
JeodPrimitiveSerializerBase::deserialize_long_double (
   const std::string & val)
{
   long double result;

   if (val.compare("NaN") == 0) {
      result = std::numeric_limits<long double>::signaling_NaN();
   }
   else if (val.compare("Inf") == 0) {
      result = std::numeric_limits<long double>::infinity();
   }
   else if (val.compare("-Inf") == 0) {
      result = -std::numeric_limits<long double>::infinity();
   }
   else {
      std::istringstream ibuf(val);
      ibuf >> result;
   }

   return result;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
