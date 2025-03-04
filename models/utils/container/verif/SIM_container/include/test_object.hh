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
/*******************************************************************************
Purpose:
  (Test container model checkpoint and restart capabilities.)
Library dependencies:
  ((../src/test_object.cc))
*******************************************************************************/

#ifndef CONTAINER_VERIF_TEST_HH
#define CONTAINER_VERIF_TEST_HH

#ifdef SWIG
%rename(streamOut) operator<<; // cppcheck-suppress syntaxError
%rename(lessThan) operator<; // cppcheck-suppress syntaxError
#endif

#include "utils/container/include/object_list.hh"
#include "utils/container/include/object_set.hh"
#include "utils/container/include/object_vector.hh"
#include "utils/container/include/pointer_list.hh"
#include "utils/container/include/pointer_set.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/container/include/primitive_list.hh"
#include "utils/container/include/primitive_set.hh"
#include "utils/container/include/primitive_vector.hh"
#include "utils/container/include/simple_checkpointable.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include <iostream>
#include <string>

//! Namespace jeod
namespace jeod
{

/**
 * A simple object used to test JeodObjectContainer capabilities.
 */
class TestObject
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TestObject)
    friend std::ostream & operator<<(std::ostream & out, const TestObject & obj);
    friend bool operator<(const TestObject & x, const TestObject & y);

public:
    TestObject() = default;

    TestObject(int in)
        : ival(in),
          dval(in)
    {
    }

private:
    int ival{}; /* trick_units(--) @n
    Integer value */

    double dval{}; /* trick_units(--) @n
       Double value */
};

/**
 * A simple object used to test SimpleCheckpointable capabilities.
 */
class TestSimple : public SimpleCheckpointable
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TestSimple)

public:
    TestSimple() = default;
    ~TestSimple() override = default;

    void assign(int val)
    {
        value = val;
        hidden_value = value + 42;
    }

    void simple_restore() override
    {
        if(hidden_value != value + 42)
        {
            std::cout << "Restoring in TestSimple::simple_restore()\n\n";
            hidden_value = value + 42;
        }
    }

    void print_contents()
    {
        std::cout << "Test_simple contents:"
                  << " value=" << value << " hidden_value=" << hidden_value << "\n\n";
    }

    TestSimple(const TestSimple &) = delete;
    TestSimple & operator=(const TestSimple &) = delete;

private:
    int value{}; /* trick_units(--) @n
       Value exposed to Trick. */

    int hidden_value{42}; /* trick_io(**) @n
       Value hidden from Trick. */
};

/**
Test container
This object tests that each of the publicly-defined STL replacements
can be checkpointed and restarted.
*/
class TestContainer
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TestContainer)
public:
    TestContainer();
    ~TestContainer();
    TestContainer(const TestContainer &) = delete;
    TestContainer & operator=(const TestContainer &) = delete;
    void shutdown();
    void test1();

private:
    TestObject obj1; /* trick_units(--) @n
     Object #1 */

    TestObject obj2; /* trick_units(--) @n
     Object #2 */

    TestObject obj3; /* trick_units(--) @n
     Object #3 */

    TestSimple simple; /* trick_units(--) @n
       Simple restorable object */

    JeodObjectList<TestObject>::type object_list; /* trick_io(**) @n
      Object list */

    JeodObjectVector<TestObject>::type object_vec; /* trick_io(**) @n
       Object vector */

    JeodObjectSet<TestObject>::type object_set; /* trick_io(**) @n
    Object set */

    JeodPointerList<TestObject>::type pointer_list; /* trick_io(**) @n
      Pointer list */

    JeodPointerVector<TestObject>::type pointer_vec; /* trick_io(**) @n
       Pointer vector */

    JeodPointerSet<TestObject>::type pointer_set; /* trick_io(**) @n
    Pointer set */

    JeodPrimitiveList<double>::type double_list; /* trick_io(**) @n
      Double list */

    JeodPrimitiveVector<double>::type double_vec; /* trick_io(**) @n
       Double vector */

    JeodPrimitiveSet<double>::type double_set; /* trick_io(**) @n
    Double set */

    JeodPrimitiveList<float>::type float_list; /* trick_io(**) @n
      Float list */

    JeodPrimitiveVector<float>::type float_vec; /* trick_io(**) @n
       Float vector */

    JeodPrimitiveSet<float>::type float_set; /* trick_io(**) @n
    Float set */

    JeodPrimitiveList<int>::type int_list; /* trick_io(**) @n
      Int list */

    JeodPrimitiveVector<int>::type int_vec; /* trick_io(**) @n
       Int vector */

    JeodPrimitiveSet<int>::type int_set; /* trick_io(**) @n
    Int set */

    JeodPrimitiveList<bool>::type bool_list; /* trick_io(**) @n
      Boolean list */

    JeodPrimitiveVector<bool>::type bool_vec; /* trick_io(**) @n
       Boolean vector */

    JeodPrimitiveSet<bool>::type bool_set; /* trick_io(**) @n
    Boolean set */

    JeodPrimitiveList<std::string>::type string_list; /* trick_io(**) @n
      String list */

    JeodPrimitiveVector<std::string>::type string_vec; /* trick_io(**) @n
       String vector */

    JeodPrimitiveSet<std::string>::type string_set; /* trick_io(**) @n
    String set */
};

} // namespace jeod

#endif
