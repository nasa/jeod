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
 * @addtogroup Experimental
 * @{
 * @addtogroup ExpMessage
 * @{
 *
 * @file
 * Define various versions fail_simulation, which causes the simulation to
 * terminate, and utility macros that use this.
 */


/*
Purpose: ()
*/


#ifndef JEOD_FAIL_SIMULATION_HH
#define JEOD_FAIL_SIMULATION_HH


#include "utils/message/include/message_handler.hh"

#include <cstdlib>
#include <string>
#include <sstream>
#include <utility>


//! Namespace jeod
namespace jeod {

// This file defines:
// - Four non-template overloads of fail_simulation, which serve as base cases.
// - Three overloads of fail_simulation_helper_, which should be treated as private.
// - A variadic version of fail_simulation.
// The basic idea is to support everything from fail_simulation(__FILE__, __LINE__)
// to fail_simulation(__FILE__, __LINE__, just, about, anything, that, prints)


inline void
fail_simulation (
    const char * file,
    int lineno,
    const char* msg)
{
    MessageHandler::fail (
        file, lineno, "experimental/message/fail_simulation", msg);
    exit (-1);
}


inline void
fail_simulation (
    const char * file,
    int lineno)
{
    fail_simulation (file, lineno, "");
}


inline void
fail_simulation (
    const char * file,
    int lineno,
    const std::string& msg)
{
    fail_simulation (file, lineno, msg.c_str());
}


inline void
fail_simulation (
    const char * file,
    int lineno,
    std::stringstream& msgbuf)
{
    fail_simulation (file, lineno, msgbuf.str());
}


inline void
fail_simulation_helper_ (
    const char * file,
    int lineno,
    std::stringstream& msgbuf)
{
    fail_simulation (file, lineno, msgbuf);
}


template<typename T>
inline void
fail_simulation_helper_(
    const char * file,
    int lineno,
    std::stringstream& msgbuf,
    T&& arg)
{
    msgbuf << arg;
    fail_simulation (file, lineno, msgbuf);
}


template<typename T, typename... Args>
inline void
fail_simulation_helper_(
    const char * file,
    int lineno,
    std::stringstream& msgbuf,
    T&& arg,
    Args&&... rest)
{
    msgbuf << arg << ' ';
    fail_simulation_helper_ (file, lineno, msgbuf, std::forward<Args>(rest)...);
}


template<typename... Args>
inline void
fail_simulation (
    const char * file,
    int lineno,
    Args&&... args)
{
    std::stringstream msgbuf;
    fail_simulation_helper_ (file, lineno, msgbuf, std::forward<Args>(args)...);
}


#define FAIL_SIMULATION(...) fail_simulation (__FILE__, __LINE__, __VA_ARGS__)

#define SIM_ASSERT(expr) \
    ((expr) ? static_cast<void>(0) : FAIL_SIMULATION(#expr))


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
