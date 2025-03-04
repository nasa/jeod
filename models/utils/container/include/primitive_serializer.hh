//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @addtogroup Container
 * @{
 *
 * @file models/utils/container/include/primitive_serializer.hh
 * Define class template JeodPrimitiveSerializer.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_PRIMITIVE_SERIALIZER_H
#define JEOD_MEMORY_PRIMITIVE_SERIALIZER_H

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <cmath>
#include <limits>
#include <sstream>
#include <string>

//! Namespace jeod
namespace jeod
{

/**
 * Base class for serializing / deserializing primitive data.
 */
class JeodPrimitiveSerializerBase
{
public:
    /**
     * Construct a JeodPrimitiveSerializerBase.
     */
    JeodPrimitiveSerializerBase() = default;

    /**
     * Destruct a JeodPrimitiveSerializerBase.
     */
    virtual ~JeodPrimitiveSerializerBase() = default;

protected:
    static const std::string serialize_string(const std::string & val);
    static const std::string deserialize_string(const std::string & val);
    static const std::string serialize_float(const float & val);
    static float deserialize_float(const std::string & val);
    static const std::string serialize_double(const double & val);
    static double deserialize_double(const std::string & val);
    static const std::string serialize_long_double(const long double & val);
    static long double deserialize_long_double(const std::string & val);
};

/**
 * Serializer / deserializer for primitive data.
 */
template<typename Type> class JeodPrimitiveSerializer : public JeodPrimitiveSerializerBase
{
public:
    /**
     * Construct a JeodPrimitiveSerializer.
     */
    JeodPrimitiveSerializer() = default;

    /**
     * Destruct a JeodPrimitiveSerializer.
     */
    ~JeodPrimitiveSerializer() override = default;

    /**
     * Convert a primitive value to its string-equivalent.
     */
    const std::string to_string(const Type & val)
    {
        std::ostringstream obuf;
        obuf << val;
        return obuf.str();
    }

    /**
     * Convert a string to its corresponding primitive value.
     */
    Type from_string(const std::string & val)
    {
        Type result;
        std::istringstream ibuf(val);
        ibuf >> result;
        return result;
    }

    JeodPrimitiveSerializer(const JeodPrimitiveSerializer &) = delete;
    JeodPrimitiveSerializer & operator=(const JeodPrimitiveSerializer &) = delete;
};

/**
 * Convert a string to a string suitable for output:
 * Backslash-escape backslashes and double quotes.
 */
template<> inline const std::string JeodPrimitiveSerializer<std::string>::to_string(const std::string & val)
{
    return serialize_string(val);
}

/**
 * Convert a string as recorded in the checkpoint file to its original form.
 */
template<> inline std::string JeodPrimitiveSerializer<std::string>::from_string(const std::string & val)
{
    return deserialize_string(val);
}

/**
 * Convert a float to a string.
 */
template<> inline const std::string JeodPrimitiveSerializer<float>::to_string(const float & val)
{
    return serialize_float(val);
}

/**
 * Convert a string to a float.
 */
template<> inline float JeodPrimitiveSerializer<float>::from_string(const std::string & val)
{
    return deserialize_float(val);
}

/**
 * Convert a double to a string.
 */
template<> inline const std::string JeodPrimitiveSerializer<double>::to_string(const double & val)
{
    return serialize_double(val);
}

/**
 * Convert a string to a double.
 */
template<> inline double JeodPrimitiveSerializer<double>::from_string(const std::string & val)
{
    return deserialize_double(val);
}

/**
 * Convert a long double to a string.
 */
template<> inline const std::string JeodPrimitiveSerializer<long double>::to_string(const long double & val)
{
    return serialize_long_double(val);
}

/**
 * Convert a string to a long double.
 */
template<> inline long double JeodPrimitiveSerializer<long double>::from_string(const std::string & val)
{
    return deserialize_long_double(val);
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
